#ifndef ARR_DEF
#define ARR_DEF 1
// global necessities
#include <stdio.h>
#include <time.h>
#include "raylib.h"
#include "raymath.h"
// #include "arrs.c"


// Array sizes
#define SPRITE_SIZE 16
#define WORK_MEM_SIZE 128
#define AST_ARR_SIZE 64

typedef struct{
    // Vector2 sprite primary array
    Vector2 arr[WORK_MEM_SIZE][SPRITE_SIZE];
    unsigned char arr_s[WORK_MEM_SIZE];
    unsigned char count;
} sprite_mem_arr;
sprite_mem_arr work_mem;
#define WORKMEM &work_mem.arr[work_mem.count], &work_mem.arr_s[work_mem.count]
#define PLAYER_SPEED 400

// Window sizes
#define RENDER_SIZE_X 1080
#define RENDER_SIZE_Y 720

// Texti guess?
#define TEXT_S 12

#endif