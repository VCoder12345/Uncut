#pragma once

#include <QGraphicsRectItem>
#include <QPainter>
#include <model/timeline/TrackData.h>


class TrackItem  : public QGraphicsRectItem
{

public:

	TrackItem(TrackData* data, qreal y, qreal height);
	~TrackItem();

	void updateWidth(qreal visibleLeft, qreal visibleRight);
	qreal getHeight();
	qreal getY();
	
private:
	TrackData* data;
	qreal y, height;
};
