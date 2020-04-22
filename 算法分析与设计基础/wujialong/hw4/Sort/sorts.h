#pragma once

typedef unsigned int value_t;

value_t rand32();

void gen_data(value_t *a, int n);

void print_data(value_t *a, int n);

void InsertionSort(value_t *a, int n);

void ShellInsert(value_t *a, int n, int d);

void ShellSort(value_t *a, int n);

int Partition(value_t *a, int l, int r);

void QuickSort(value_t *a, int l, int r);

void MergeSort(value_t *a, int l, int r, value_t *tmp);

void RadixSort(value_t *A, int n, int w, int r, value_t *tmp, int *c);

void validate(int n);