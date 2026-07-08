#pragma once
#include <stdlib.h>
#include <stdio.h>

void SaveToFile(void* item, size_t size, const char* name_of_file);

void Recover(void* item, size_t size, const char* name_of_file);