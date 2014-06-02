#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<string.h>

#define MAX_LENGTH 20000
#define LIN_COMBO 200

void permute(int * array, int i, int width);
void swap( int * row, int *array );
typedef struct{
    int * row;
    int size;
    int id;
} Row;
void get_solutions( int b);
int * make_row(Row permutes[],int x, int y);
void swap_row( Row permute, Row array );
void fill_scoreboard(int b);
void count_karel(int d, Row counter, int c);
void new_permute_row(int * array, int i,int width);
