#include "qtui.h"
#include <QtGui>
#include <QEvent>
#include <Windows.h>
#include <vector>
#include "HandleTools.h"
#include "qsizepolicy.h"

using namespace std;

QTUI::QTUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // call button on startup, save time
    this->on_runButton_clicked();
}

QTUI::~QTUI()
{

}

// import from CLI
__declspec(dllexport) void launch_rhino(void* window_handle, void* ui_ptr);
void QTUI::on_runButton_clicked() {
    //start process of opening rhino
    HWND window_handle = (HWND)QTUI::window()->winId();
    launch_rhino((void*)window_handle, (void*)this);
}

__declspec(dllexport) void destroy_rhino();

void QTUI::on_closeButton_clicked() {
    destroy_rhino();
}

void QTUI::on_Button2_clicked() {

}

void QTUI::lock_rhino(void* rhino_handle) {
    // may well break if the layout is different
    HWND command_handle = GetWindowFromText(COMMAND_WINDOW);

    QWidget* command_window = QWidget::createWindowContainer(QWindow::fromWinId((WId)command_handle));
    command_window->setMaximumHeight(150);
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
