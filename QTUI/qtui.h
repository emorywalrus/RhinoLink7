#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qtui.h"
#include <Windows.h>
#include <QtGui>
#include <QEvent>

using namespace std;


class QTUI : public QMainWindow
{
    Q_OBJECT
public:
    QTUI(QWidget *parent = nullptr);
    ~QTUI();

    Ui::QTUIClass ui;

    QTimer* timer = new QTimer(this);

    QWidget* rhino_window;
protected:
    void closeEvent(QCloseEvent* event) override;

public slots:
    void lock_rhino(void* rhino_handle);

    void write_to_cmd_line_slot();

    void on_boxButton_clicked();

    void on_sphereButton_clicked();

    void on_addButton_clicked();

    void on_subtractButton_clicked();

    void update();
};

