#include "TrackItem.h"
#include <QPainter>

TrackItem::TrackItem(TrackData* data) : QGraphicsRectItem(), data(data) {}

TrackItem::~TrackItem()
{}

void TrackItem::updateWidth(qreal visibleLeft, qreal visibleRight)
{
	setRect(visibleLeft, 0, visibleRight - visibleLeft, data->getHeight());
}

qreal TrackItem::getClipHeight()
{
	return data->getHeight();
}

qreal TrackItem::getY() {
	return data->getY();
}


