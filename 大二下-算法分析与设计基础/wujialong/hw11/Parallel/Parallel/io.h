#pragma once
#include "sorts.h"
#include "parallel_sorts.h"

int get_n();
int get_f();
int get_count();

void printOmpInfo();
void validate(int n, value_t* a, value_t* b, value_t* tmp);