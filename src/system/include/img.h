#pragma once

#include <stdbool.h>

static bool CreateImage(const char *imagePath, const char *bootSectorPath);
static bool WriteFile(const char *path, unsigned char *data, unsigned int len);
static bool ReadFile(const char *path, unsigned char **outData, unsigned int *outLen);