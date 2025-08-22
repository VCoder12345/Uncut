#include "ExchangeService.h"
#include <QDataStream>

QMimeData* ExchangeService::libItemToMIME(const LibItemData* data) const
{

	QByteArray dropData;

	QDataStream dropStream(&dropData, QIODevice::WriteOnly);
	dropStream << data->filePath;
	dropStream << data->name;
	dropStream << data->durationInSecs;

	QMimeData* mimeData = new QMimeData;
	mimeData->setData(libItemMimeType, dropData);
	return mimeData;
}

const QString& ExchangeService::getLibItemMimeFormat() const
{
	return libItemMimeType;
}

ClipData* ExchangeService::libItemMimeToClipData(const QMimeData* mimeData)
{
	QByteArray itemData = mimeData->data("application/x-libitemdata");
	QDataStream dataStream(&itemData, QIODevice::ReadOnly);
	

	QString filePath, name;
	double durationInSecs;
	dataStream >> filePath >> name >> durationInSecs;

	if (auto videoInfo = videoDecoder.getVideoInfo(filePath))
	{
		std::shared_ptr<VideoFrame> previewFrame = videoInfo.value().firstFrame;
		return new ClipData(previewFrame, filePath, name, 0, durationInSecs, 0);
	}

}
