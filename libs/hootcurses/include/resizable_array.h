#pragma once
#ifndef RESIZABLE_ARRAY
#define RESIZABLE_ARRAY

//============================================================================
// INT
//============================================================================

typedef struct resizable_array_int
{
  int* data;
  int capacity;
  int length;
} resizable_array_int;

resizable_array_int create_array_int(void);
void free_array_int(resizable_array_int* arr);
void reserve_int(resizable_array_int* arr, int amount);
void push_int(resizable_array_int* arr, int val);
void pop_int(resizable_array_int* arr);
void insert_int(resizable_array_int* arr, int at, int val);
void delete_int(resizable_array_int* arr, int at);
int* at_int(resizable_array_int* arr, int pos);

void print_array_int(resizable_array_int* arr);

//============================================================================
// CHAR
//============================================================================

typedef struct resizable_array_char
{
  char* data;
  int capacity;
  int length;
} resizable_array_char;

resizable_array_char create_array_char(void);
resizable_array_char* allocate_array_char(int reserve);
void free_array_char(resizable_array_char* arr);
void reserve_char(resizable_array_char* arr, int amount);
void push_char(resizable_array_char* arr, char val);
void pop_char(resizable_array_char* arr);
void insert_char(resizable_array_char* arr, int at, char val);
void delete_char(resizable_array_char* arr, int at);
char* at_char(resizable_array_char* arr, int pos);

void print_array_char(resizable_array_char* arr);
#endif
