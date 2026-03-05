#include <resizable_array.h>
#include <stdlib.h>
#include <stdio.h>

//============================================================================
// INT
//============================================================================

resizable_array_int create_array_int(void)
{
  return (resizable_array_int) {
    .data = NULL,
    .capacity = 0,
    .length = 0
  };
}


void free_array_int(resizable_array_int* arr)
{
  free(arr->data);
  arr->data = NULL;
  arr->capacity = 0;
  arr->length = 0;
}

void reserve_int(resizable_array_int* arr, int amount)
{
  arr->data = realloc(arr->data, amount * sizeof(int));
  arr->capacity = amount;
}

void push_int(resizable_array_int* arr, int val)
{
  if (arr->length >= arr->capacity)
  {
    int new_capacity = arr->capacity * 2;
    if (new_capacity <= 0)
      new_capacity = 1;

    reserve_int(arr, new_capacity);
  }

  arr->data[arr->length] = val;
  arr->length++;
}

void pop_int(resizable_array_int* arr)
{
  if (arr->length > 0) 
    arr->length--;
}

void insert_int(resizable_array_int* arr, int at, int val)
{
  if (at < 0 || at > arr->length - 1)
    return;

  int curr = arr->data[at];
  for (int i = at; i < arr->length; i++)
  {
    if (i + 1 >= arr->capacity)
      reserve_int(arr, arr->capacity * 2);

    int temp = arr->data[i + 1];
    arr->data[i + 1] = curr;
    curr = temp;
  }

  arr->length++;
  arr->data[at] = val;
}

void delete_int(resizable_array_int* arr, int at)
{
  if (at < 0 || at > arr->length - 1)
    return;

  for (int i = at; i < arr->length - 1; i++)
  {
    arr->data[i] = arr->data[i + 1];
  }
  arr->length--;
}

int* at_int(resizable_array_int* arr, int pos)
{
  if (pos < 0 || pos > arr->length - 1)
    return NULL;

  return arr->data + pos;
}


// debug
void print_array_int(resizable_array_int* arr)
{
  for (int i = 0; i < arr->length; i++)
  {
    printf("%d, ", *at_int(arr, i));
  }
  printf("\n");
}


//============================================================================
// CHAR
//============================================================================

resizable_array_char create_array_char(void)
{
  return (resizable_array_char) {
    .data = NULL,
    .capacity = 0,
    .length = 0
  };
}

resizable_array_char* allocate_array_char(int reserve)
{
  resizable_array_char* arr = malloc(sizeof(resizable_array_char));
  arr->data = NULL;
  arr->length = 0;
  arr->capacity = 0;

  reserve_char(arr, reserve);

  return arr;
}

void free_array_char(resizable_array_char* arr)
{
  free(arr->data);
  arr->data = NULL;
  arr->capacity = 0;
  arr->length = 0;
  free(arr);
}

void reserve_char(resizable_array_char* arr, int amount)
{
  arr->data = realloc(arr->data, amount * sizeof(char));
  arr->capacity = amount;
}

void push_char(resizable_array_char* arr, char val)
{
  if (arr->length >= arr->capacity)
  {
    int new_capacity = arr->capacity * 2;
    if (new_capacity <= 0)
      new_capacity = 1;

    reserve_char(arr, new_capacity);
  }

  arr->data[arr->length] = val;
  arr->length++;
}

void pop_char(resizable_array_char* arr)
{
  if (arr->length > 0) 
    arr->length--;
}

void insert_char(resizable_array_char* arr, int at, char val)
{
  if (at < 0 || at > arr->length - 1)
    return;

  int curr = arr->data[at];
  for (int i = at; i < arr->length; i++)
  {
    if (i + 1 >= arr->capacity)
      reserve_char(arr, arr->capacity * 2);

    int temp = arr->data[i + 1];
    arr->data[i + 1] = curr;
    curr = temp;
  }

  arr->length++;
  arr->data[at] = val;
}

void delete_char(resizable_array_char* arr, int at)
{
  if (at < 0 || at > arr->length - 1)
    return;

  for (int i = at; i < arr->length - 1; i++)
  {
    arr->data[i] = arr->data[i + 1];
  }
  arr->length--;
}

char* at_char(resizable_array_char* arr, int pos)
{
  if (pos < 0 || pos > arr->length - 1)
    return NULL;

  return arr->data + pos;
}


// debug
void print_array_char(resizable_array_char* arr)
{
  for (int i = 0; i < arr->length; i++)
  {
    printf("%c", *at_char(arr, i));
  }
  printf("\n");
}
