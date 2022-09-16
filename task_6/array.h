#ifndef __ARR__
#define __ARR__

#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>

typedef struct array {
	off_t* dat;
	size_t cap;
	size_t pos;
} array;

array* initArray(size_t initSize) {
	array* newArr = (array*)malloc(sizeof(array));
    if(!newArr)
        return NULL;

    newArr->dat = (off_t*)malloc(initSize * sizeof(off_t));
    if(!newArr->dat) {
        free(newArr);
        return NULL;
    }
    
    newArr->pos = 0;
    newArr->cap = initSize;

    return newArr;
}

bool put(array* arr, off_t data) {
    if(!arr)
        return false;

    if(arr->pos == arr->cap) {
        size_t newSize = arr->cap * 2;
        off_t* newDat = (off_t*)realloc(arr->dat, newSize * sizeof(off_t));
        if(!newDat)
            return false;

        arr->cap = newSize;
        arr->dat = newDat;
    }

    arr->dat[arr->pos++] = data;
    return true;
}

off_t get(array* arr, size_t pos) {
    if(!arr || pos >= arr->pos)
        return -1;   // Плохо, что-то придумать
    return arr->dat[pos];
}

off_t getLast(array* arr) {
    if(!arr)
        return -1;   // Плохо, что-то придумать
    return arr->dat[arr->pos - 1];
}

size_t getSize(array* arr) {
    if(!arr)
        return 0;
    return arr->pos;
}

void destroyArr(array* arr) {
    free(arr->dat);
    arr->cap = 0;
    arr->pos = 0;
    free(arr);
}

#endif