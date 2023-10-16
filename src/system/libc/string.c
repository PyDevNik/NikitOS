#include <str.h>
#include <types.h>

char* int_to_ascii(int value, char* buffer, int base)
{
    int n = abs(value);
    int index = 0;
    while (n)
    {
        int digit = n % base;
        if (digit >= 10)
        {buffer[index++] = 0x41 + digit - 10;}
        else {buffer[index++] = 0x30 + digit;}
        n /= base;
    }
    if (value == 0) {buffer[index++] = '0';}
    if (value < 0 && base == 10) {buffer[index++] = '-';}
    buffer[index] = '\0';
    reverse(buffer);
    return buffer;
}

int abs(int n) {int nums[2] = {-n, n}; return nums[n > 0];}

char *strrchr(const char *str, int c) {
    const char *last_occurrence = NULL;
    while (*str) {if (*str == c) {last_occurrence = str;}; str++;}
    if (c == '\0') {return (char *)str;}
    return (char *)last_occurrence;
}

char *strchr(const char *str, int c) {
    while (*str != '\0') {if (*str == c) {return (char *)str;}; str++;}
    return NULL;
}

int toupper(int c) {
    if (c >= 'a' && c <= 'z') {return c - 'a' + 'A';}
    return c;
}
void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

int strlen(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

void append(char s[], char n) {
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}

void backspace(char s[]) {
    int len = strlen(s);
    s[len-1] = '\0';
}

int strcmp(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}