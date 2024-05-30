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
}

__declspec(dllexport) void destroy_rhino(void*);
QTUI::~QTUI()
{
    if (caller_ptr != nullptr) {
        destroy_rhino(caller_ptr);
    }
}

// import from CLI
__declspec(dllexport) void* launch_rhino(void* window_handle, void* ui_ptr);

void QTUI::on_testButton_clicked() {
    //start process of opening rhino
    HWND window_handle = (HWND)QTUI::window()->winId();
    caller_ptr = launch_rhino((void*)window_handle, (void*)this);
}

void QTUI::lock_rhino(void* rhino_handle) {
    // may well break if the layout is different
    HWND command_handle = GetWindowFromText(COMMAND_WINDOW);

    QWidget* command_window = QWidget::createWindowContainer(QWindow::fromWinId((WId)command_handle));
    command_window->setMaximumHeight(150);
    ui.verticalLayout->insertWidget(1, command_window);
    command_window->show();

    QWidget* rhino_window = QWidget::createWindowContainer(QWindow::fromWinId((WId)rhino_handle));
    ui.verticalLayout->insertWidget(1, rhino_window);
    rhino_window->show();

    // start timer to always show info in below textbox
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_text()));
    timer->start(500);
}

__declspec(dllexport) std::string get_data();

void QTUI::update_text() {
    string text = get_data();
    ui.retBox->setText(QString::fromStdString(text));
}

extern "C" {
    __declspec(dllexport) void lock_rhino_time(void* qtui, void* rhino_handle) {
        QMetaObject::invokeMethod((QTUI*)qtui, "lock_rhino", Qt::QueuedConnection, Q_ARG(void*, rhino_handle));
    }
}
