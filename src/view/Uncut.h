#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Uncut.h"
#include "library/LibraryView.h"
#include "preview/PreviewView.h"

class Uncut : public QMainWindow
{
    Q_OBJECT

public:
    Uncut(QWidget *parent = nullptr);
    ~Uncut();

    QPushButton* getPlayBtn();

    LibraryView* getLibView();
    QAction* getImportAction();
    PreviewView* getPreviewView();
    TlView* getTlView();

public slots:
    void onLoad();

private:
    Ui::UncutClass ui;
};
