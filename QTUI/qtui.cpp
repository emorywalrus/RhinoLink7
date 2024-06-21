#include "qtui.h"
#include <QtGui>
#include <QEvent>
#include <Windows.h>
#include <vector>
#include "HandleTools.h"
#include "qsizepolicy.h"
#include <cstdlib>
#include <qscrollbar.h>

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
    connect(timer, &QTimer::timeout, this, &QTUI::update);
}





// lock rhino to window, export so C# can call it, invoke slot from C# thread
void QTUI::lock_rhino(void* rhino_handle) {
    // may well break if the layout is different
    this->rhino_window = QWidget::createWindowContainer(QWindow::fromWinId(reinterpret_cast<WId>(rhino_handle)));
    this->rhino_window->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->rhino_window->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    ui.rhinoLayout->addWidget(rhino_window);
    this->rhino_window->show();
    timer->start(100);
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

}

__declspec(dllexport) void run_command(std::string);
__declspec(dllexport) std::string get_history();
__declspec(dllexport) std::string get_prompt();


string old_prompt = "";
string prompt = "";

void QTUI::update() {
    int old_pos = ui.textBrowser->verticalScrollBar()->value();
    int old_max = ui.textBrowser->verticalScrollBar()->maximum();

    ui.textBrowser->setText(QString::fromStdString(get_history()));

    if (old_pos == old_max) {
        int new_max = ui.textBrowser->verticalScrollBar()->maximum();
        ui.textBrowser->verticalScrollBar()->setValue(new_max);
    }
    else {
        ui.textBrowser->verticalScrollBar()->setValue(old_pos);
    }

    prompt = get_prompt() + ": ";
    if (prompt != old_prompt) {
        ui.lineEdit->setText(QString::fromStdString(prompt));
        old_prompt = prompt;
    }

    SetFocus((HWND)QTUI::winId());
}

void QTUI::write_to_cmd_line_slot() {
    run_command(ui.lineEdit->text().toStdString().substr(prompt.length()));
}

void QTUI::on_boxButton_clicked() {
    run_command("Box");
}

void QTUI::on_sphereButton_clicked() {
    run_command("Sphere");
}

void QTUI::on_addButton_clicked() {
    run_command("BooleanUnion");
}

void QTUI::on_subtractButton_clicked() {
    run_command("BooleanDifference");
}

void QTUI::closeEvent(QCloseEvent* event) {
    destroy_rhino();
    event->accept();
}
