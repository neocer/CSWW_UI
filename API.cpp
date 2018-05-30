#include <windows.h>
#include <metahook.h>
#include <triangleapi.h>

char *UnicodeToANSI(const wchar_t *str)
{
	static char result[1024];
	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, str, -1, result, len, NULL, NULL);
	result[len] = '\0';
	return result;
}
wchar_t *UTF8ToUnicode(const char *str)
{
	static wchar_t result[1024];
	int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, str, -1, result, len);
	result[len] = L'\0';
	return result;
}
wchar_t *ANSIToUnicode(const char* str)
{
	int textlen;
	static wchar_t result[1024];
	textlen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	memset(result, 0, sizeof(char) * (textlen + 1));
	MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)result, textlen);
	return result;
}
char *UnicodeToUTF8(const wchar_t *str)
{
	static char result[1024];
	int len = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, str, -1, result, len, NULL, NULL);
	result[len] = '\0';
	return result;
}