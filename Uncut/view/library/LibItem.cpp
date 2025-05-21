#include "LibItem.h"


LibItem::LibItem(const LibItemData* data, int width, int height, int index) : data(data), width(width), height(height), index(index)
{
}

LibItem::~LibItem()
{}

QRectF LibItem::boundingRect() const {
	return QRectF(0, 0, width + 2*padding, height + textMargin + 2*padding);
}

void LibItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	QFont font("Arial", 10);
	painter->setFont(font);

	painter->drawImage(QRect(padding, padding, width, height), data->previewFrame->toImage());
	QString durationTxt;
	if (data->duration.hours == 0) {
		durationTxt = QString::asprintf("%02d:%02d", data->duration.minutes, data->duration.seconds);
	}
	else {
		durationTxt = QString::asprintf("%02d:%02d:%02d", data->duration.hours, data->duration.minutes, data->duration.seconds);
	}
	QString nameTxt = data->name;
	QFontMetrics metrics(font);
	int durTxtWidth = metrics.horizontalAdvance(durationTxt);
	int nameTxtWidth = metrics.horizontalAdvance(nameTxt);
	int txtGap = 5;
	int nameLimitW = width - durTxtWidth - txtGap;

	if (nameTxtWidth > nameLimitW) {
		int sz = nameTxt.size();
		for (int i = sz - 1; i >= 0; --i) {
			QString nameSlice = nameTxt.left(i) + "...";
			nameTxtWidth = metrics.horizontalAdvance(nameSlice);

			if (nameTxtWidth <= nameLimitW) {
				nameTxt = nameSlice;
				break;
			}
		}
	}
	

	painter->drawText(padding, height + padding, width, height + textMargin, Qt::AlignRight, durationTxt);

	painter->drawText(padding, height + padding, width, height + textMargin, Qt::AlignLeft, nameTxt);

	if (data->selected) {
		painter->setPen(QColor(0, 50, 200));
		painter->drawRoundedRect(QRect(1, 1, width + 2 * padding, height + textMargin + 2 * padding), 3, 3);
	}
}

void LibItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	QGraphicsItem::mousePressEvent(event);
	update();
}
