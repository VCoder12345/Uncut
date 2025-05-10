#pragma once

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QPixmap>

#include "ui_PreviewView.h"

class PreviewView : public QWidget
{
	Q_OBJECT

public:
	PreviewView(QWidget *parent = nullptr);
	~PreviewView();


private:
	Ui::PreviewViewClass ui;
	QImage img;

protected:
	void paintEvent(QPaintEvent* e);
	void drawWidget(QPainter& qp);
};
