#include "num_def.h"

enum ast_size
{
    AST_SMALL = 7,
    AST_MEDIUM = 10,
    AST_LARGE = 13,
    AST_SUPER = 16
} ast_size;

typedef struct
{
    Vector2 sprite[SPRITE_SIZE];
    Vector2 velocity;
    Vector2 pos;
    float rotation;
    float size;
    int points;
} object;

typedef struct
{
    object object;
    float rotational_velocity;
    int integrity;
} asteroid;
// Creates an array of vector2 points of varying length and angle
// To be used by by the renderer as a a "sprite", vector graphic, for the asteroids
void asteroid_sprite_gen(Vector2 sprite_arr[], int size)
{
    // computer indexing starts at 0, math starts counting at 1
    size--;
    float angle_offset = (360.0 / (float)size) * DEG2RAD;
    // Creates a vector at the angle_offset, with a length of 1
    // Scales it to a random value between 0.4 and 1.2
    for (char i = 0; i < size; i++)
    {
        float rand_angle_offset = (GetRandomValue(0, angle_offset) - (angle_offset * 2)) * DEG2RAD;
        sprite_arr[i] = Vector2Rotate((Vector2){0, -1}, (angle_offset * i) + rand_angle_offset);
        sprite_arr[i] = Vector2Scale(sprite_arr[i], GetRandomValue(700, 1000) / 1000.0);
    }
    // last point is the same as the first so we get one closed polygon
    sprite_arr[size] = sprite_arr[0];
}

asteroid asteroid_gen(int points, Vector2 position)
{
    asteroid product;
    // object setup
    {
        product.object.points = points;
        product.object.size = points * 3;
        product.object.rotation = 0;
        product.object.pos = position;
        // velocity
        float direction = GetRandomValue(0, 360);
        Vector2 temp_velocity = {sin(direction), cos(direction)};
        product.object.velocity = Vector2Scale(temp_velocity, GetRandomValue(1, 20 - points) / 6.0);
    }
    // asteroid specific
    asteroid_sprite_gen(product.object.sprite, points);
    product.rotational_velocity = GetRandomValue(0, 720 - (points * 20)) * DEG2RAD;
    product.integrity = (points - 3) / 3;
    return product;
}

int asteroid_destroyed(asteroid arr[], char ast_arr_size, bool arr_spots[], int pos_in_arr)
{
    char ast_dif = -1;
    asteroid removed_asteroid = arr[pos_in_arr];
    arr_spots[pos_in_arr] = 0;
    if (removed_asteroid.object.points == AST_SMALL)
        return ast_dif;

    char new_asteroid_count = GetRandomValue(1, 6);
    if (new_asteroid_count > 3)
        new_asteroid_count = new_asteroid_count / 2;
    char arr_index = 0;
    while (arr_index < ast_arr_size)
    {
        arr_index++;
        if (arr_spots[arr_index])
            continue;

        arr[arr_index] = asteroid_gen(removed_asteroid.object.points - 3, removed_asteroid.object.pos);
        arr[arr_index].object.velocity = Vector2Rotate(removed_asteroid.object.velocity, (GetRandomValue(0, 120) - 60) * DEG2RAD);
        arr_spots[arr_index] = 1;
        new_asteroid_count--;
        ast_dif++;
        if (new_asteroid_count == 0)
        {
            arr[arr_index].object.velocity = removed_asteroid.object.velocity;
            break;
        }
    }
    return ast_dif;
}

void asteroid_creation(asteroid arr[], int ast_arr_size, bool arr_spots[], int amount)
{
    for (char i = 0; i < ast_arr_size; i++)
    {
        if (arr_spots[i]) // existance check
            continue;

        // Size Ranoomizer, skewed to 1/6 for small, 2/6 for large and 3/6 for medium
        // With a 1/20 for a super. Ratio Not exact as the Super sized ones don't care about the rest
        char ast_size = GetRandomValue(1, 6);
        switch (ast_size)
        {
        case (1):
            ast_size = AST_SMALL;
            break;
        case (2):
        case (3):
            ast_size = AST_LARGE;
            break;
        default:
            ast_size = AST_MEDIUM;
            break;
        }
        if (GetRandomValue(1, 20) == 20)
            ast_size = AST_SUPER;

        // Assigning start pos at the edge of the screen
        char ast_width = ast_size * 3;
        Vector2 ast_pos = (Vector2){GetRandomValue(0, RENDER_SIZE_X), GetRandomValue(0, RENDER_SIZE_Y)};
        switch (GetRandomValue(0, 3))
        {
        case (0):
            ast_pos.y = -ast_width;
            break;
        case (1):
            ast_pos.x = RENDER_SIZE_X + ast_width;
            break;
        case (2):
            ast_pos.y = RENDER_SIZE_Y + ast_width;
            break;
        case (3):
            ast_pos.x = -ast_width;
            break;
        }
        // Generating the steroids
        arr[i] = asteroid_gen(ast_size, ast_pos);
        arr_spots[i] = 1;
        amount--;
        if (amount == 0)
            break;
    }
}