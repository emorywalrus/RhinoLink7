#include <iostream>
#include "HandleTools.h"
#include <iostream>


int main()
{
    vector<HWND> windows = GetAllWindows();
    for (int i = 0; i < windows.size(); i++) {
        std::cout << get_text(windows[i]) << "\n";
    }
}