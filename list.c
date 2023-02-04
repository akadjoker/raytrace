#include "types.h"



List* create_list(void)
{
  List* array = malloc(sizeof(List));
  array->data = malloc(sizeof(Model) * 10);
  array->size = 0;
  array->capacity = 10;
  return array;
}

void clear_list(List* array)
{
  size_t i = 0;
  while (i < array->size)
{
    printf(" Free model %d %f \n",array->data[i]->index,array->data[i]->dist);
    free(array->data[i]->mesh);
    free(array->data[i]);
    i++;
  }
  array->size = 0;
}

void destroy_list(List* array)
{
  clear_list(array);
  free(array->data);
  free(array);
}

void list_add(List* array, const void* data)
{
  if (array->size == array->capacity)
  {
    array->capacity *= 2;
    array->data = realloc(array->data, sizeof(void*) * array->capacity);
  }
  array->data[array->size] = data;
  array->size++;
}

void list_remove(List* array, size_t index)
 {
  if (index >= array->size)
  {
    return;
  }
  free(array->data[index]);
  size_t i = index;
  while (i < array->size - 1)
  {
    array->data[i] = array->data[i + 1];
    i++;
  }
  array->size--;
}

void list_sort(List* array,int (*cmp)(void*, void*))
{
  size_t i = 0;
  while (i < array->size - 1)
{
    size_t j = 0;
    while (j < array->size - i - 1)
 {

      if (cmp(array->data[j], array->data[j + 1]) == 0)
      {
        void* temp = array->data[j];
        array->data[j] = array->data[j + 1];
        array->data[j + 1] = temp;
      }


      j++;
    }
    i++;
  }
}



