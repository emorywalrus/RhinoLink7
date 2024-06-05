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

QTUI::~QTUI()
{
    
}

// import from CLI
__declspec(dllexport) void* launch_rhino(void* window_handle, void* ui_ptr);
void QTUI::on_testButton_clicked() {
    //start process of opening rhino
    HWND window_handle = (HWND)QTUI::window()->winId();
    caller_ptr = launch_rhino((void*)window_handle, (void*)this);
}

__declspec(dllexport) void run_script(std::string script);
void QTUI::on_boxButton_clicked() {
    run_script("_Box");
}

void QTUI::on_sphereButton_clicked() {
    run_script("_Sphere");
}

void QTUI::lock_rhino(void* rhino_handle) {
    // may well break if the layout is different
    HWND command_handle = GetWindowFromText(COMMAND_WINDOW);
    HWND viewport_handle = GetWindowFromText(VIEWPORT_WINDOW);

    QWidget* command_window = QWidget::createWindowContainer(QWindow::fromWinId((WId)command_handle));
    command_window->setMaximumHeight(150);
    ui.verticalLayout->insertWidget(0, command_window);
    command_window->show();

    QWidget* viewport_window = QWidget::createWindowContainer(QWindow::fromWinId((WId)viewport_handle));
    ui.verticalLayout->insertWidget(0, viewport_window);
    viewport_window->show();

    // start timer to always show info in below textbox
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_text()));
    timer->start(500);
}

string ret;
string old_ret;

string get_window_name_string() {
    vector<HWND> handles = GetAllWindows();
    ret = "";
    for (int i = 0; i < handles.size(); i++) {
        string newText = MyGetWindowText(handles[i]);
        if (newText != "") {
            ret += newText + "\n";
        }
    }
    if (ret == old_ret) {
        old_ret = ret;
        return "unchanged";
    }
    else {
        old_ret = ret;
        return ret;
    }
}

__declspec(dllexport) std::string get_data();

void QTUI::update_text() {
    //ui.retBox->setText(QString::fromStdString(get_object_string()));
    string disp = get_window_name_string();
    if (disp != "unchanged") {
        ui.retBox->setText(QString::fromStdString(disp));
    }
}

extern "C" {
    __declspec(dllexport) void lock_rhino_time(void* qtui, void* rhino_handle) {
        QMetaObject::invokeMethod((QTUI*)qtui, "lock_rhino", Qt::QueuedConnection, Q_ARG(void*, rhino_handle));
    }
}
