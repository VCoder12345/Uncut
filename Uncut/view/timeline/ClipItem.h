#pragma once

#include <QGraphicsItem>
#include <model/timeline/ClipData.h>
#include <memory>
#include <QPainter>


class ClipItem  : public QGraphicsItem
{

public:
	ClipData* data;

	ClipItem(ClipData* data, int wps, int height, double opacity = 1.0);
	~ClipItem();

	ClipItem* shallowCopy() {
		ClipItem* item = new ClipItem(data, wps, height, opacity);
		item->setPos(pos());
		return item;
	}

	void setOpacity(double opacity) {
		this->opacity = opacity;
	}

protected:
	QRectF boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
	int wps;
	int height;
	double opacity;
};
