#include <stdbool.h>
#include <stdint.h>

void swap_int(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void swap_char(char* xp, char* yp)
{
    char temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void swap_short(uint16_t* xp, uint16_t* yp)
{
    uint16_t temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void int_swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubble_sort(int array[], int n) // syntax bubble_sort(array[], n=size of array)
{
    bool swapped;
    int i, j;
    for (i = 0; i < n - 1; i++)
    {
        swapped = false;
        for (j = 0; j < n - i - 1; j++)
        {
            if (array[j] > array[j + 1])
            {
                int_swap(&array[j], &array[j + 1]);
                swapped = true;
            }
        }
        if (swapped == false)
            break;
    }
}

void merge(int* array, int first, int middle, int last)
{
    int i, j, k;
    int n1 = middle - first + 1; // sizeof
    int n2 = last - middle;      // sizeof
    int L[n1], R[n2];

    for (i = 0; i < n1; i++) // copy array into L
        L[i] = array[first + i];
    for (j = 0; j < n2; j++)
        R[j] = array[middle + 1 + j];

    i = 0;
    j = 0;
    k = first;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            array[k] = L[i];
            i++;
        }

        else
        {
            array[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        array[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        array[k] = R[j];
        j++;
        k++;
    }
}

void merge_sort(int array[], int first, int last)
{
    if (first < last)
    {
        int middle = first + (last - first) / 2;

        merge_sort(array, first, middle);
        merge_sort(array, middle + 1, last);

        merge(array, first, middle, last);
    }
}
