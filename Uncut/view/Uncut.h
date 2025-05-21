#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Uncut.h"
#include "model/library/LibModel.h"
#include "controller/library/LibController.h"
#include "controller/preview/PreviewController.h"

class Uncut : public QMainWindow
{
    Q_OBJECT

public:
    Uncut(QWidget *parent = nullptr);
    ~Uncut();

public slots:
    void onLoad();

private:
    Ui::UncutClass ui;
    LibModel* libModel;
    LibController* libController;
    PreviewController* previewController;

    
};
