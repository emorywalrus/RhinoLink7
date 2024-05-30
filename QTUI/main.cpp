#include "qtui.h"
#include <QtWidgets/QApplication>
#include <iostream>

__declspec(dllexport) void initialize_rhino();

int main(int argc, char *argv[])
{
    initialize_rhino();
    QApplication a(argc, argv);
    QTUI w;

    w.show();
    
    return a.exec();
}