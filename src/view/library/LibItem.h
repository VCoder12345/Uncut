#pragma once

#include <QGraphicsItem>
#include <QImage>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "model/library/LibModel.h"

class LibItem  : public QGraphicsItem
{
public:
	int index;
	const LibItemData* data;

	LibItem(const LibItemData* data, int width, int height, int index);
	~LibItem();

protected:
	QRectF boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
	const int width, height;
	const int textMargin = 12;
	const int padding = 5;

};
