#pragma once
#include <Windows.h>
#include <vector>
#include <string>

#define RHINO_WINDOW "Untitled - Rhino 8 Not For Resale Lab"
#define COMMAND_WINDOW "UNMANAGED TabPanelDockBarFloatingForm -> Command"
#define VIEWPORT_WINDOW "Rhino Viewport"

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

string MyGetWindowText(HWND window) {
	char ret_str[1024];
	GetWindowTextA(window, ret_str, GetWindowTextLengthA(window)+1);
	return (string)ret_str;
}

HWND GetWindowFromText(string title) {
	vector<HWND> handles = GetAllWindows();
	for (int i = 0; i < handles.size(); i++) {
		string window_text = MyGetWindowText(handles[i]);
		if (window_text == title) {
			return handles[i];
		}
	}
	return nullptr;
}
