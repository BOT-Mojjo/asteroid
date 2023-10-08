// self files
#include "num_def.h"
#include "asteroid.c"
#include "textout.c"

enum GAME_STATE
{
    MENU,
    MAIN_LOOP,
    GAME_OVER,
    COUNT
} GAME_STATE;

// Reformats a sprites vectors from memory so raylib can display it properly
void sprite_processing(object object, Vector2 *work_mem_out)
{
    for (char i = 0; i < object.points; i++)
    {
        work_mem_out[i] = Vector2Rotate(object.sprite[i], object.rotation);
        work_mem_out[i] = Vector2Scale(work_mem_out[i], object.size);
        work_mem_out[i] = Vector2Add(work_mem_out[i], object.pos);
    }
}

void sprite_processing_dif(Vector2 pos, float size, int points, Vector2 *arr_src, Vector2 *work_mem_out)
{
    for (char i = 0; i < points; i++)
    {
        work_mem_out[i] = Vector2Scale(arr_src[i], size);
        work_mem_out[i] = Vector2Add(work_mem_out[i], pos);
    }
}

void arena_boundry_check(object *obj)
{
    { // y-axis boundry check
        if (obj->pos.y > RENDER_SIZE_Y + obj->size)
            obj->pos.y = -obj->size;
        if (obj->pos.y < -obj->size)
            obj->pos.y = RENDER_SIZE_Y + obj->size;

        // x-axis boundry check
        if (obj->pos.x > RENDER_SIZE_X + obj->size)
            obj->pos.x = -obj->size;
        if (obj->pos.x < -obj->size)
            obj->pos.x = RENDER_SIZE_X + obj->size;
    }
}

// Player Sprite and player Data
Vector2 player_sprite[5] = {{0, -1}, {-0.70710678, 0.70710678}, {0, 0.2}, {0.70710678, 0.70710678}, {0, -1}};
object player_obj;
float inv_cooldown = 0;
Vector2 acceleration = {0, -1};
char integrity = 3;
int int_count = 0; // when reaches > 1000 give extra life
int score = 0;
float score_multiplier = 1;
float death_time = 0;

// Laser data
int laser_length;
bool fired;
float laser_cooldown = 0;
int in_row_fire = 0;

char laser_queue = 0;
char las_assist_count = 30;
Vector2 laser[2];
// 'expression must have a constant value'
// keep the length of the laser array the same as las_assist_count

// ast data
asteroid ast_arr[AST_ARR_SIZE];
bool ast_arr_spot[AST_ARR_SIZE];
char ast_count;

void game_start(int ast_amount)
{
    // player setup
    for (int i = 0; i < 5; i++)
    {
        player_obj.sprite[i] = player_sprite[i];
    }

    player_obj.rotation = 0;
    player_obj.points = 5;
    player_obj.size = 8;
    player_obj.velocity = (Vector2){0};
    player_obj.pos = (Vector2){RENDER_SIZE_X / 2, RENDER_SIZE_Y / 2};
    for (int i = 0; i < AST_ARR_SIZE; i++)
    {
        ast_arr_spot[i] = 0;
    }
    asteroid_creation(ast_arr, AST_ARR_SIZE, ast_arr_spot, ast_amount);
    ast_count = ast_amount;
}

int main()
{
    // TODO: RANDOM SEED IS HERE YOU DOOFUS
    SetRandomSeed((unsigned long)time(NULL));
    char STATE_MACHINE = MENU;
    game_start(24);
    InitWindow(RENDER_SIZE_X, RENDER_SIZE_Y, "ASTEROIDS");
    SetTargetFPS(256);
    SetWindowPosition(50, 50);

    // Sets the length of an unubtructed laser vector to the hypothenuse of the scren + 50px
    // So i don't have to worry the laser end-point is on screen
    laser_length = Vector2Length((Vector2){RENDER_SIZE_X, RENDER_SIZE_Y}) + 50;

    // ----------------------------------------------
    // PRIMARY GAME LOOP
    // ----------------------------------------------

    while (!WindowShouldClose())
    {
        // Necessary Input
        work_mem.count = 0;
        float delta_time = GetFrameTime();
        switch (STATE_MACHINE)
        {
        case (MENU):
            goto MAIN_MENU;
        case (MAIN_LOOP):
            goto GAME_LOOP;
        case (GAME_OVER):
            goto DEATH_SCREEN;

        default:
            CloseWindow();
            puts("Gamestate set to invalid value");
            return 0;
        }

    MAIN_MENU:
    {
        float x_center = (RENDER_SIZE_X / 2) + 8.1f;
        text_formatter("ASTEROIDS", (Vector2){x_center - 121.5, 300}, WORKMEM);
        num_formatter(2023, (Vector2){x_center + 40.5, 300}, WORKMEM);
        text_formatter("PRESS SPACE TO START", (Vector2){x_center - 170.1, 350}, WORKMEM);
        for (char i = 0; i < AST_ARR_SIZE; i++)
        {
            if (!ast_arr_spot[i])
                continue;

            if (GetRandomValue(0, 9999) == 0)
                asteroid_destroyed(ast_arr, AST_ARR_SIZE, ast_arr_spot, i);

            if (ast_count < 12)
                asteroid_creation(ast_arr, AST_ARR_SIZE, ast_arr_spot, GetRandomValue(6, 18));
            ast_arr[i].object.pos = Vector2Add(ast_arr[i].object.pos, ast_arr[i].object.velocity);
            ast_arr[i].object.rotation += ast_arr[i].rotational_velocity * delta_time;
            arena_boundry_check(&ast_arr[i].object);
            sprite_processing(ast_arr[i].object, work_mem.arr[work_mem.count]);
            work_mem.arr_s[work_mem.count] = ast_arr[i].object.points;
            work_mem.count++;
        }
        BeginDrawing();
        ClearBackground(BLACK);
        for (char i = 0; i < work_mem.count; i++)
        {
            DrawLineStrip(work_mem.arr[i], work_mem.arr_s[i], LIGHTGRAY);
        }
        EndDrawing();
        if (IsKeyReleased(KEY_SPACE))
        {
            game_start(16);
            STATE_MACHINE = MAIN_LOOP;
        }
    }
        continue;

    GAME_LOOP:
    {
        if (IsKeyDown(KEY_SPACE) || laser_queue)
        {
            if (laser_cooldown <= 0)
            {
                // laser firing preperations, i.e. adding the cooldown for the next shot to limit spam
                // Although a .25 second firing delay isn't too much
                fired = 1;
                laser_cooldown = 0.25;
                laser_queue = 0;
            }
            else
            {
                // So it won't feel like it's dropping inputs
                laser_queue = 1;
            }
        }

        // if right key is pressed
        if (IsKeyDown(262) || IsKeyDown(KEY_D))
            player_obj.rotation += 270 * DEG2RAD * delta_time;
        // if left key is pressed
        if (IsKeyDown(263) || IsKeyDown(KEY_A))
            player_obj.rotation -= 270 * DEG2RAD * delta_time;
        // if up key is pressed
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        {
            Vector2 temp_vector = Vector2Rotate(acceleration, player_obj.rotation);
            temp_vector = Vector2Scale(temp_vector, delta_time * (delta_time * PLAYER_SPEED));
            player_obj.velocity = Vector2Add(temp_vector, player_obj.velocity);
        }
        else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        {
            Vector2 temp_vector = Vector2Rotate(acceleration, (90 * DEG2RAD) + atan2(player_obj.velocity.y, player_obj.velocity.x));
            temp_vector = Vector2Scale(temp_vector, delta_time * (delta_time * PLAYER_SPEED / 2));
            player_obj.velocity = Vector2Subtract(player_obj.velocity, temp_vector);
        }

        if (IsKeyPressed(KEY_K))
            integrity = -1;

        // -----------------------------------------
        // Update

        // Speed Limiter
        player_obj.velocity = Vector2ClampValue(player_obj.velocity, 0, delta_time * PLAYER_SPEED);
        if (Vector2Length(player_obj.velocity) < 0.3 * delta_time)
            player_obj.velocity = Vector2Zero();

        player_obj.pos = Vector2Add(player_obj.pos, player_obj.velocity);
        arena_boundry_check(&player_obj);
        sprite_processing(player_obj, work_mem.arr[0]);
        work_mem.arr_s[0] = player_obj.points;
        work_mem.count++;

        // STEROIDS --------------
        if (ast_count < 8)
        {
            char new_ast = GetRandomValue(3, 12);
            asteroid_creation(ast_arr, AST_ARR_SIZE, ast_arr_spot, new_ast);
            ast_count += new_ast;
        }
        for (char i = 0; i < AST_ARR_SIZE; i++)
        {
            if (!ast_arr_spot[i])
                continue;
            ast_arr[i].object.pos = Vector2Add(ast_arr[i].object.pos, ast_arr[i].object.velocity);
            ast_arr[i].object.rotation += ast_arr[i].rotational_velocity * delta_time;
            arena_boundry_check(&ast_arr[i].object);

            if (ast_arr[i].integrity <= 0)
            {
                ast_count += asteroid_destroyed(ast_arr, AST_ARR_SIZE, ast_arr_spot, i);
                score += 10 * score_multiplier;
                int_count += 10 * score_multiplier;
                score_multiplier += 0.1;
            }

            sprite_processing(ast_arr[i].object, work_mem.arr[work_mem.count]);
            work_mem.arr_s[work_mem.count] = ast_arr[i].object.points;

            if (inv_cooldown <= 0 && Vector2Distance(player_obj.pos, ast_arr[i].object.pos) < player_obj.size + ast_arr[i].object.size)
            {
                char ii = 1;
                while (ii < 5)
                {
                    if (!CheckCollisionPointPoly(work_mem.arr[0][ii - 1], work_mem.arr[work_mem.count], work_mem.arr_s[work_mem.count]))
                    {
                        ii *= 2;
                        continue;
                    }
                    score_multiplier = 1;
                    integrity--;
                    inv_cooldown = 3;
                    break;
                }
            }
            work_mem.count++;
        }

        if (inv_cooldown > 0)
            inv_cooldown -= delta_time;
        // laser logic/creation
        // I put it here so that we can use the math already done for the ship to position
        // the tartpoint of the laser, without having to re-calculate it
        if (fired)
        {
            fired = 0;
            // laser startpoit is equal to the ships front-most point
            // Edit the shape of the ship/player changes or you update the way the rendering logic operates.
            laser[0] = work_mem.arr[0][0];
            laser[1] = Vector2Add(Vector2Rotate((Vector2){0, -laser_length}, player_obj.rotation), player_obj.pos);
            Vector2 temp_laser[2];
            temp_laser[0] = work_mem.arr[0][0];
            char ast_spot;
            char final_ast;
            bool weHit = 0;
            // Aim assist by shooting 5 or so times per shot
            for (char ii = 0; ii < las_assist_count; ii++)
            {
                // adds a rotated vector of a certain length to the player pos so it lines up in a straigh line
                float aim_offset = ((ii * 0.5) - (las_assist_count * 0.25f)) * DEG2RAD;
                temp_laser[1] = Vector2Add(Vector2Rotate((Vector2){0, -1}, player_obj.rotation + aim_offset), player_obj.pos);
                Vector2 laser_end_point = Vector2Add(Vector2Rotate((Vector2){0, -laser_length}, player_obj.rotation + aim_offset), player_obj.pos);

                while (Vector2Distance(temp_laser[0], temp_laser[1]) < laser_length)
                {
                    float march_dist = laser_length - 10;
                    // Finds the closest asteroid to the current end of the laser
                    for (char i = 0; i < AST_ARR_SIZE; i++)
                    {
                        if (!ast_arr_spot[i])
                            continue;
                        float temp_dist = Vector2Distance(temp_laser[1], ast_arr[i].object.pos);
                        if (temp_dist > march_dist)
                            continue;

                        march_dist = temp_dist;
                        ast_spot = i;
                    }
                    // Move laser forward march_dist
                    Vector2 march_vector = Vector2Rotate((Vector2){0, -march_dist}, player_obj.rotation + aim_offset);
                    temp_laser[1] = Vector2Add(temp_laser[1], march_vector);
                    // laser[1] = Vector2MoveTowards(laser[1], laser_end_point, march_dist);
                    // moving data to new place so i don't have to have 150 char long lines of code
                    object ast = ast_arr[ast_spot].object;
                    // if laser endpoint is within closest asteroid
                    if (!CheckCollisionCircles(temp_laser[1], 0, ast.pos, ast.size))
                        continue;

                    bool hit = 0;
                    sprite_processing(ast, ast.sprite);
                    for (char i = 0; i < ast.points - 1; i++)
                    {
                        hit = CheckCollisionLines(temp_laser[0], temp_laser[1], ast.sprite[i], ast.sprite[i + 1], &temp_laser[1]);
                        if (hit)
                            break;
                    }
                    if (!hit)
                        continue;
                    weHit = 1;
                    if (Vector2Distance(temp_laser[0], temp_laser[1]) < Vector2Distance(laser[0], laser[1]))
                    {
                        laser[0] = temp_laser[0];
                        laser[1] = temp_laser[1];
                        final_ast = ast_spot;
                    }
                    break;
                }
            }
            if (weHit)
                ast_arr[final_ast].integrity--;
        }

        // Score
        num_formatter(score, (Vector2){15, 20}, WORKMEM);
        if (int_count > 1000)
        {
            int_count -= 1000;
            integrity++;
        }

        for (char i = 0; i < integrity; i++)
        {
            int ii = work_mem.count;
            sprite_processing_dif((Vector2){15 + (20 * i), 50}, TEXT_S, 5, player_sprite, work_mem.arr[ii]);
            work_mem.arr_s[ii] = 5;
            work_mem.count++;
        }

        // Death
        if (integrity <= 0)
            STATE_MACHINE = GAME_OVER;

        // -----------------------------------------
        // Draw/Display

        // char text[128];
        // sprintf(text, "%f, %f, %f, %f", player_obj.rotation, Vector2Length(player_obj.velocity), player_obj.sprite[0].x, work_mem.arr[0][0].x);
        BeginDrawing();
        ClearBackground(BLACK);
        // DrawText(text, 10, 80, 14, GREEN);
        // DrawFPS(25, 60);
        bool playerdamaged = inv_cooldown > 0;
        // player renderer. needed 'cause i want to make it flash
        if (playerdamaged)
        {
            inv_cooldown -= delta_time;
            if ((int)(inv_cooldown * 4) % 2)
                DrawLineStrip(work_mem.arr[0], 5, (Color){200, 0, 0, 255});
            else
                playerdamaged = false;
        }

        // else renderer
        for (char i = playerdamaged; i < work_mem.count; i++)
        {
            DrawLineStrip(work_mem.arr[i], work_mem.arr_s[i], LIGHTGRAY);
        }

        if (laser_cooldown > 0)
        {
            laser_cooldown -= delta_time;
            if (laser_cooldown > 0.15)
            {
                DrawLineEx(laser[0], laser[1], 1, (Color){255, 255, 255, laser_cooldown * 1000});
            }
        }

        EndDrawing();
    }
        continue;

    DEATH_SCREEN:
    {
        death_time += delta_time;
        for (char i = 0; i < AST_ARR_SIZE; i++)
        {
            if (!ast_arr_spot[i])
                continue;
            ast_arr[i].object.pos = Vector2Add(ast_arr[i].object.pos, ast_arr[i].object.velocity);
            ast_arr[i].object.rotation += ast_arr[i].rotational_velocity * delta_time;
            arena_boundry_check(&ast_arr[i].object);
            sprite_processing(ast_arr[i].object, work_mem.arr[work_mem.count]);
            work_mem.arr_s[work_mem.count] = ast_arr[i].object.points;
            work_mem.count++;
        }
        char text[25];
        text_formatter("you scored", (Vector2){400, 400}, WORKMEM);
        num_formatter(score, (Vector2){578.2, 400}, WORKMEM);
        sprintf(text, "Press SPACE to try again");
        text_formatter(text, (Vector2){400, 450}, WORKMEM);
        BeginDrawing();
        ClearBackground(BLACK);
        for (char i = 0; i < work_mem.count; i++)
        {
            DrawLineStrip(work_mem.arr[i], work_mem.arr_s[i], LIGHTGRAY);
        }
        // player death animation
        if (death_time < 0.5)
        {
            float offset = 11.25f;
            for (char i = 0; i < 32; i++)
            {
                Vector2 death_anim[2];
                float beam_offset = ((i % 2) + 1) * player_obj.size * death_time * 2;
                death_anim[0] = Vector2Rotate((Vector2){0, 1}, offset * i * DEG2RAD);
                death_anim[0] = Vector2Scale(death_anim[0], beam_offset * 2 - (8 * death_time * 2));
                death_anim[0] = Vector2Add(death_anim[0], player_obj.pos);

                death_anim[1] = Vector2Rotate((Vector2){0, 1}, offset * i * DEG2RAD);
                death_anim[1] = Vector2Scale(death_anim[1], beam_offset * 4 - (i % 2 * 16));
                death_anim[1] = Vector2Add(death_anim[1], player_obj.pos);

                DrawLineStrip(death_anim, 2, LIGHTGRAY);
            }
        }

        EndDrawing();
        if (IsKeyReleased(KEY_SPACE))
        {
            death_time = 0;
            integrity = 3;
            inv_cooldown = 0;
            score = 0;
            int_count = 0;
            STATE_MACHINE = MAIN_LOOP;
            game_start(16);
        }
    }
        continue;
    }

    CloseWindow();

    puts("ASTEROIDS closed nominally");

    return 0;
}