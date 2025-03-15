#pragma once

#ifdef __cplusplus
extern "C" {
#endif

  void int_swap(int* xp, int* yp);
  void bubble_sort(int array[], int n); // syntax bubble_sort(array[], n=size of array)
  void merge(int* array, int first, int middle, int last);
  void merge_sort(int array[], int first, int last);

#ifdef __cplusplus
}
#endif