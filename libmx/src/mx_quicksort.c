#include "../inc/libmx.h"


int mx_quicksort(char **arr, int left, int right);
static void swap(char **a, char **b);
/*
#include <stdio.h>
int main() {
    char **arr = malloc(sizeof(char *) * 6);
    arr[0] = mx_strnew(20);
    arr[1] = mx_strnew(20);
    arr[2] = mx_strnew(20);
    arr[3] = mx_strnew(20);
    arr[0] = "1234567 ";
    arr[1] = "123456 ";
    arr[2] = "12345 ";
    arr[3] = "1234 ";
    int swaps = mx_quicksort(arr, 0, 3);
    for (int i = 0; i < 4; i++) {
        mx_printstr(arr[i]);
        
    }
    
    mx_printint(swaps);
    
}
*/
int mx_quicksort(char **arr, int left, int right)
{   
    int swaps = 0;
    if(left >= right)
        return 0;

    int mid=left + (right - left) / 2;
    char *pivot=arr[mid];
    int i = left, j = right;
    //char *temp;
    while(i<j)
    {
        if(mx_strlen(arr[i]) >= mx_strlen(pivot) 
           && mx_strlen(arr[j]) <= mx_strlen(pivot))
        {
            swap(arr + i, arr + j);
            i++;
            j--;
            swaps++;
        }
        else
        {
            i++;
        }
    }
    swaps += mx_quicksort(arr, left, mid - 1);
    swaps += mx_quicksort(arr, mid+1, right);
    return swaps;
}
static void swap(char **a, char **b)
{
    char *tmp = *a;
    *a = *b;
    *b = tmp;
}
