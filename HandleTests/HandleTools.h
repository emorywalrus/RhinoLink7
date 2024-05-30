#pragma once
#include <Windows.h>
#include <vector>
#include <string>

using namespace std;

BOOL CALLBACK EnumWindowsProc(HWND handle, LPARAM lparam) {
	((vector<HWND>*)lparam)->push_back(handle);
	return TRUE;
}

vector<HWND> GetAllWindows() {
	vector<HWND> ret_vec = {};
	EnumWindows(EnumWindowsProc, (LPARAM)&ret_vec);
	return ret_vec;
}

string get_text(HWND window) {
	char ret_str[1024];
	GetWindowTextA(window, ret_str, GetWindowTextLengthA(window));
	return (string)ret_str;
}