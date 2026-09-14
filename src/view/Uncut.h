#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Uncut.h"
#include "library/LibraryView.h"
#include "preview/PreviewView.h"

class Uncut : public QMainWindow
{
	Q_OBJECT

public:
	Uncut(QWidget* parent = nullptr);
	~Uncut() override;

	QPushButton* getPlayBtn() const;

	LibraryView* getLibView() const;
	QAction* getImportAction() const;
	PreviewView* getPreviewView() const;
	TlView* getTlView() const;

public slots:
	void onLoad();

private:
	Ui::UncutClass ui;
};
