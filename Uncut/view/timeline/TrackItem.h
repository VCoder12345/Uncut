#pragma once

#include <QGraphicsRectItem>
#include <QPainter>
#include <model/timeline/TrackData.h>


class TrackItem  : public QGraphicsRectItem
{

public:

	TrackItem(TrackData* data);
	~TrackItem();

	void updateWidth(qreal visibleLeft, qreal visibleRight);
	qreal getClipHeight();
	qreal getY();
	
private:
	TrackData* data;
};
