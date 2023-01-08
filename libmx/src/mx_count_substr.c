//#include<stdio.h>
#include "../inc/libmx.h"

int mx_count_substr(const char *str, const char *sub);

int mx_count_substr(const char *str, const char *sub) {
    int len = mx_strlen(sub);
    int count = 0;

    for (char *found = mx_strstr(str, sub);
        found != (void *)0;
        found = mx_strstr(found + len, sub)) {
        count++;
    }
    return count;
}
/*
int main() {
    printf("%i %i", mx_count_substr("yoyoyo neo", "yo"), mx_count_substr("Matrix", "yo"));
}
*/
