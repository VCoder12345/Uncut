#include "PreviewView.h"

PreviewView::PreviewView(QWidget *parent)
	: QWidget(parent)
{
    this->img.load("res/einBild.jpg");
	ui.setupUi(this);

    
}

PreviewView::~PreviewView()
{}

void PreviewView::paintEvent(QPaintEvent* e) {


    QPainter qp(this);

    drawWidget(qp);

    QWidget::paintEvent(e);

}

void PreviewView::drawWidget(QPainter& qp) {
    int w = width();
    int h = height();
    qp.setBrush(QColor(0, 0, 0));
    qp.drawRect(0, 0, w, h);
    QImage simg;
    if (img.height() > img.width()) {
        simg = img.scaledToHeight(height(), Qt::SmoothTransformation);
    }
    else {
        simg = img.scaledToWidth(width(), Qt::SmoothTransformation);
    }

    qp.drawImage(w / 2 - simg.width() / 2, h / 2 - simg.height() / 2, simg);
}

