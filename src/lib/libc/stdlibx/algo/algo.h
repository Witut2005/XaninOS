#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

  void swap_int(int* xp, int* yp);
  void swap_char(char* xp, char* yp);
  void swap_short(uint16_t* xp, uint16_t* yp);
  void int_swap(int* xp, int* yp);
  void bubble_sort(int array[], int n); // syntax bubble_sort(array[], n=size of array)
  void merge(int* array, int first, int middle, int last);
  void merge_sort(int array[], int first, int last);

#ifdef __cplusplus
}
#endif