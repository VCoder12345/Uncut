#pragma once

#include <QGraphicsItem>
#include <QImage>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "model/library/LibModel.h"

class LibItem  : public QGraphicsItem
{
public:
	LibItem(const LibItemData* data, int width, int height);
	~LibItem();

protected:
	QRectF boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
	const LibItemData* data;
	const int width, height;


};
