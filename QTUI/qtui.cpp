#include "qtui.h"
#include <QtGui>
#include <QEvent>
#include <Windows.h>
#include <vector>
#include "HandleTools.h"
#include "qsizepolicy.h"
#include <cstdlib>

using namespace std;

QTUI::QTUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // call button on startup, save time
    this->on_runButton_clicked();
}

__declspec(dllexport) void destroy_rhino();

QTUI::~QTUI()
{
    destroy_rhino();
}

// import from CLI
__declspec(dllexport) void launch_rhino(void* window_handle, void* ui_ptr);
void QTUI::on_runButton_clicked() {
    //start process of opening rhino
    HWND window_handle = (HWND)QTUI::window()->winId();
    launch_rhino((void*)window_handle, (void*)this);
}

void QTUI::on_closeButton_clicked() {
    destroy_rhino();
}

void QTUI::on_installButton_clicked() {
    char buffer[1024];
    GetModuleFileNameA(NULL, buffer, 1024);
    string::size_type len = string(buffer).find_last_of("\\/");
    string folder = string(buffer).substr(0, len);

    const char* rhino_installer = folder.append("\\rhino_en-us_8.2.23346.13001.exe").c_str();

    system(rhino_installer);
}

void QTUI::lock_rhino(void* rhino_handle) {
    // may well break if the layout is different
    HWND command_handle = GetWindowFromText(COMMAND_WINDOW);

    QWidget* command_window = QWidget::createWindowContainer(QWindow::fromWinId((WId)command_handle));
    command_window->setMaximumHeight(100);
    ui.verticalLayout->insertWidget(0, command_window);
    command_window->show();

    QWidget* rhino_window = QWidget::createWindowContainer(QWindow::fromWinId((WId)rhino_handle));
    ui.verticalLayout->insertWidget(0, rhino_window);
    rhino_window->show();

    // start timer to always show info in below textbox
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_text()));
    timer->start(500);
}

__declspec(dllexport) std::string get_data();

void QTUI::update_text() {
    ui.retBox->setText(QString::fromStdString(get_data()));
}

extern "C" {
    __declspec(dllexport) void lock_rhino_time(void* qtui, void* rhino_handle) {
        QMetaObject::invokeMethod((QTUI*)qtui, "lock_rhino", Qt::QueuedConnection, Q_ARG(void*, rhino_handle));
    }
}
