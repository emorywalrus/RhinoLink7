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

    void* caller_ptr = nullptr;

    QTimer* timer;

    void closeEvent(QCloseEvent* event) override;

public slots:
    void on_testButton_clicked();

    void on_boxButton_clicked();

    void on_sphereButton_clicked();

    void lock_rhino(void* rhino_handle);

    void update_text();
};

