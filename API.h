#pragma once
wchar_t *ANSIToUnicode(const char* str);
char *UnicodeToANSI(const wchar_t *str);
wchar_t *UTF8ToUnicode(const char *str);
char *UnicodeToUTF8(const wchar_t *str);