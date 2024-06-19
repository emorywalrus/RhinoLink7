#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qtui.h"
#include <Windows.h>
#include <QtGui>
#include <QEvent>

using namespace std;

__declspec(dllexport) std::string get_history();
class QTUI : public QMainWindow
{
    Q_OBJECT
public:
    QTUI(QWidget *parent = nullptr);
    ~QTUI();

    Ui::QTUIClass ui;

    QTimer* timer;

public slots:
    void lock_rhino(void* rhino_handle);

    void write_to_cmd_line_slot();
};

