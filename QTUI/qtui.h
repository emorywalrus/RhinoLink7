#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qtui.h"
#include <Windows.h>
#include <QtGui>
#include <QEvent>

class QTUI : public QMainWindow
{
    Q_OBJECT

public:
    QTUI(QWidget *parent = nullptr);
    ~QTUI();

    Ui::QTUIClass ui;

    QTimer* timer;

public slots:
    void on_runButton_clicked();

    void on_closeButton_clicked();

    void on_Button2_clicked();

    void lock_rhino(void* rhino_handle);

    void update_text();
};

