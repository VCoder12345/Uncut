#pragma once

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QPixmap>

#include "ui_PreviewView.h"
#include "services/decoder/VideoFrame.h"

class PreviewView : public QWidget
{
	Q_OBJECT

public:
	PreviewView(QWidget *parent = nullptr);
	~PreviewView();

	

public slots:
	void drawFrame(std::shared_ptr<VideoFrame> frame);

private:
	Ui::PreviewViewClass ui;
	std::shared_ptr<VideoFrame> frame;
	QImage img;

protected:
	void paintEvent(QPaintEvent* e);
	void drawWidget(QPainter& qp);
};
