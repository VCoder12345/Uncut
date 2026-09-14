#include "ClipItem.h"
#include <QPainter>

#include "utils/UiHelper.h"

ClipItem::ClipItem(ClipData* data, double wps, int height, double opacity) : data(data), wps(wps), height(height), opacity(opacity)
{
	if (data->previewFrame)
	{
		this->previewImage = data->previewFrame->toImage();
	}else
	{
		this->previewImage = QImage();
	}

	setZValue(5);
}


ClipItem::~ClipItem()
{
}

QRectF ClipItem::boundingRect() const {
	return QRectF(0, 0, getWidth(), getHeight());
}

double ClipItem::getHeight() const
{
	return height;
}

double ClipItem::getWidth() const
{
	double duration = data->duration();
	return duration * wps;
}

void ClipItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	painter->setOpacity(opacity);
	painter->fillRect(boundingRect(), QBrush(Qt::blue));
	

	QFont font("Arial", 10);
	painter->setFont(font);

	QFontMetrics metrics(font);

	double nameLblSize = metrics.height();


	double previewY = nameLblSize + namePadding;
	double previewWidth = getWidth();
	double previewHeight = getHeight() - previewY;
	QImage scaledImg = this->previewImage.scaled(previewWidth, previewHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

	QString shortenedName = UiHelper::shortenTextTo(data->name, previewWidth, font);
	painter->drawText(0, nameLblSize, shortenedName);

	painter->drawImage(0, previewY, scaledImg);

	if (data->selected) {
		painter->setPen(QPen(Qt::red));
		painter->drawRect(boundingRect());
	}
}
