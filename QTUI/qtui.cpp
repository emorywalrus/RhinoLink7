#include "qtui.h"
#include <QtGui>
#include <QEvent>
#include <Windows.h>
#include <vector>
#include "HandleTools.h"
#include "qsizepolicy.h"
#include <cstdlib>

using namespace std;

// import from CLI
__declspec(dllexport) void launch_rhino(void* window_handle, void* ui_ptr);


QTUI::QTUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //start process of opening rhino
    launch_rhino(reinterpret_cast<void*>(QTUI::window()->winId()), this);

    connect(ui.lineEdit, &QLineEdit::returnPressed, this, &QTUI::write_to_cmd_line_slot);
}

// lock rhino to window, export so C# can call it, invoke slot from C# thread
void QTUI::lock_rhino(void* rhino_handle) {
    // may well break if the layout is different
    QWidget* rhino_window = QWidget::createWindowContainer(QWindow::fromWinId(reinterpret_cast<WId>(rhino_handle)));
    ui.verticalLayout->insertWidget(0, rhino_window);
    rhino_window->show();
}
// invokes lock rhino slot. connection is queued, so this can be invoked from whatever thread.
extern "C" {
    __declspec(dllexport) void lock_rhino_time(void* qtui, void* rhino_handle) {
        QMetaObject::invokeMethod(reinterpret_cast<QTUI*>(qtui), "lock_rhino", Qt::QueuedConnection, Q_ARG(void*, rhino_handle));
    }
}

__declspec(dllexport) void destroy_rhino();
QTUI::~QTUI()
{
    destroy_rhino();
}

__declspec(dllexport) void write_to_cmd_line(std::string line);
__declspec(dllexport) std::string get_history();
void QTUI::write_to_cmd_line_slot() {
    write_to_cmd_line(ui.lineEdit->text().toStdString());
    ui.lineEdit->clear();
    ui.retBox->setText(QString::fromStdString(get_history()));
    SetFocus(reinterpret_cast<HWND>(ui.lineEdit->winId()));
}
