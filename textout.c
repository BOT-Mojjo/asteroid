#include "num_def.h"

Vector2 char_nums[10][7] = { // the fucking numbers 0-9
    {{-0.5, -1}, {0.5, -1}, {0.5, 1}, {-0.5, 1}, {-0.5, -1}},
    {{0, -1}, {0, 1}},
    {{-0.5, -1}, {0.5, -1}, {0.5, 0}, {-0.5, 0}, {-0.5, 1}, {0.5, 1}},
    {{-0.5, -1}, {0.5, -1}, {0.5, 0}, {-0.5, 0}, {0.5, 0}, {0.5, 1}, {-0.5, 1}},
    {{-0.5, -1}, {-0.5, 0}, {0.5, 0}, {0.5, -1}, {0.5, 1}},
    {{0.5, -1}, {-0.5, -1}, {-0.5, 0}, {0.5, 0}, {0.5, 1}, {-0.5, 1}},
    {{0.5, -1}, {-0.5, -1}, {-0.5, 1}, {0.5, 1}, {0.5, 0}, {-0.5, 0}},
    {{-0.5, -1}, {0.5, -1}, {0.5, 1}},
    {{-0.5, 0}, {-0.5, -1}, {0.5, -1}, {0.5, 1}, {-0.5, 1}, {-0.5, 0}, {0.5, 0}},
    {{0.5, 0}, {-0.5, 0}, {-0.5, -1}, {0.5, -1}, {0.5, 1}}};

char char_nums_s[] = { // length of the number sprite array
    5, 2, 6, 7, 5, 6, 6, 3, 7, 5};

Vector2 char_chars[27][13] = { // the rest of the fucking alphabet
    {{-0.5, 1}, {-0.5, -0.33}, {0, -1}, {0.5, -0.33}, {0.5, 0.33}, {-0.5, 0.33}, {0.5, 0.33}, {0.5, 1}},
    {{-0.5, 1}, {-0.5, -1}, {0.25, -1}, {0.5, -0.66}, {0.5, -0.33}, {0.25, 0}, {-0.5, 0}, {0.25, 0}, {0.5, 0.33}, {0.5, 0.66}, {0.25, 1}, {-0.5, 1}},
    {{0.5, 1}, {-0.25, 1}, {-0.5, 0.66}, {-0.5, -0.66}, {-0.25, -1}, {0.5, -1}},
    {{-0.5, 1}, {-0.5, -1}, {0.25, -1}, {0.5, -0.66}, {0.5, 0.66}, {0.25, 1}, {-0.5, 1}},
    {{0.5, 1}, {-0.5, 1}, {-0.5, 0}, {0.5, 0}, {-0.5, 0}, {-0.5, -1}, {0.5, -1}},
    {{-0.5, 1}, {-0.5, 0}, {0.5, 0}, {-0.5, 0}, {-0.5, -1}, {0.5, -1}},
    {{0, 0.33}, {0.5, 0.33}, {0.5, 0.66}, {0.25, 1}, {-0.25, 1}, {-0.5, 0.66}, {-0.5, -0.66}, {-0.25, -1}, {0.25, -1}, {0.5, -0.66}},
    {{-0.5, -1}, {-0.5, 1}, {-0.5, 0}, {0.5, 0}, {0.5, 1}, {0.5, -1}},
    {{-0.5, -1}, {0.5, -1}, {0, -1}, {0, 1}, {0.5, 1}, {-0.5, 1}},
    {{-0.5, 0.33}, {0, 1}, {0.5, 1}, {0.5, -1}},
    {{-0.5, -1}, {-0.5, 0}, {0.5, -1}, {-0.5, 0}, {0.5, 1}, {-0.5, 0}, {-0.5, 1}},
    {{-0.5, -1}, {-0.5, 1}, {0.5, 1}},
    {{-0.5, 1}, {-0.5, -1}, {0, 0.33}, {0.5, -1}, {0.5, 1}},
    {{-0.5, 1}, {-0.5, -1}, {0.5, 1}, {0.5, -1}},
    {{-0.25, 1}, {-0.5, 0.66}, {-0.5, -0.66}, {-0.25, -1}, {0.25, -1}, {0.5, -0.66}, {0.5, 0.66}, {0.25, 1}, {-0.25, 1}},
    {{-0.5, 1}, {-0.5, -1}, {0.25, -1}, {0.5, -0.66}, {0.5, -0.33}, {0.25, 0}, {-0.5, 0}},
    {{-0.25, 1}, {-0.5, 0.66}, {-0.5, -0.66}, {-0.25, -1}, {0.25, -1}, {0.5, -0.66}, {0.5, 0.33}, {0.25, 0.66}, {0.5, 1}, {0, 0.33}, {0.25, 0.66}, {0, 1}, {-0.25, 1}},
    {{-0.5, 1}, {-0.5, -1}, {0.25, -1}, {0.5, -0.66}, {0.5, -0.33}, {0.25, 0}, {-0.5, 0}, {0.5, 1}},
    {{0.5, -0.66}, {0.25, -1}, {-0.25, -1}, {-0.5, -0.66}, {-0.5, -0.33}, {-0.25, 0}, {0.25, 0}, {0.5, 0.33}, {0.5, 0.66}, {0.25, 1}, {-0.25, 1}, {-0.5, 0.66}},
    {{-0.5, -1}, {0.5, -1}, {0, -1}, {0, 1}},
    {{-0.5, -1}, {-0.5, 0.66}, {-0.25, 1}, {0.25, 1}, {0.5, 0.66}, {0.5, -1}},
    {{-0.5, -1}, {0, 1}, {0.5, -1}},
    {{-0.5, -1}, {-0.5, 1}, {0, 0.66}, {0.5, 1}, {0.5, -1}},
    {{-0.5, -1}, {0, 0}, {0.5, -1}, {-0.5, 1}, {0, 0}, {0.5, 1}},
    {{-0.5, -1}, {0, -0.33}, {0.5, -1}, {0, -0.33}, {0, 1}},
    {{-0.5, -1}, {0.5, -1}, {-0.5, 1}, {0.5, 1}}};

char char_chars_s[] = { // length of the char sprite array
    8, 12, 6, 7, 7, 6, 10, 6, 6, 5, 7, 3, 5, 4, 9, 7, 13, 8, 12, 4, 6, 3, 5, 6, 5, 4};
//  A,  B, C, D, E, F, G, H, I, J, K, L, M, N, O, P,  Q, R,  S, T, U, V, W, X, Y, Z

void num_formatter(int num, Vector2 pos, Vector2 text_sprite_out_arr[WORK_MEM_SIZE][SPRITE_SIZE], unsigned char sprite_sizes_arr[])
{
  char i = 0;
  char text[10];
  sprintf(text, "%d", num);
  while (text[i] != 0)
  {
    char num = text[i] - 48;
    char num_s = char_nums_s[num];
    for (char ii = 0; ii < num_s; ii++)
    {
      text_sprite_out_arr[i][ii] = char_nums[num][ii];
      text_sprite_out_arr[i][ii] = Vector2Scale(text_sprite_out_arr[i][ii], TEXT_S);
      text_sprite_out_arr[i][ii] = Vector2Add(text_sprite_out_arr[i][ii], pos);
    }
    // textspacing is handled by pos
    pos.x += TEXT_S * 1.35f;
    sprite_sizes_arr[i] = num_s;
    work_mem.count++;
    i++;
  }
}

// Please just give it characters + whitespace. i begeth of thee
void text_formatter(char *text, Vector2 pos, Vector2 text_sprite_out_arr[WORK_MEM_SIZE][SPRITE_SIZE], unsigned char sprite_sizes_arr[])
{
  char i = 0;   // Index for the letters in the text
  char iii = 0; // Index for the work memory
  while (text[i] != 0)
  {
    if (text[i] != 32) // i forgot white space and is tired
    {
      char sym = text[i] - (65 + ((text[i] > 90) * 32)); // convert all characters into 0-26
      char sym_s = char_chars_s[sym];
      for (char ii = 0; ii < sym_s; ii++) // Index for the vector points in the sprite
      {
        text_sprite_out_arr[iii][ii] = char_chars[sym][ii];
        text_sprite_out_arr[iii][ii] = Vector2Scale(text_sprite_out_arr[iii][ii], TEXT_S);
        text_sprite_out_arr[iii][ii] = Vector2Add(text_sprite_out_arr[iii][ii], pos);
      }
      sprite_sizes_arr[iii] = sym_s;
      work_mem.count++;
      iii++;
    }
    // textspacing is handled by pos
    pos.x += TEXT_S * 1.35f;
    i++;
  }
}
