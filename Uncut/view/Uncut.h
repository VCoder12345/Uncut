#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Uncut.h"

class Uncut : public QMainWindow
{
    Q_OBJECT

public:
    Uncut(QWidget *parent = nullptr);
    ~Uncut();

private:
    Ui::UncutClass ui;
};
