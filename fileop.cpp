#include "fileop.h"

void SaveToFile(void *item, size_t size, const char *name_of_file) {
	FILE* p_file;
	fopen_s(&p_file, name_of_file, "wb");
	fwrite(item, size, 1, p_file);
}

void Recover(void* item, size_t size, const char* name_of_file) {
	FILE* p_file;
	fopen_s(&p_file, name_of_file, "rb");
	fread(item, size, 1, p_file);
}