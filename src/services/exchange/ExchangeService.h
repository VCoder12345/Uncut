#pragma once
#include "model/library/LibModel.h"
#include <qiodevice.h>
#include <QMimeData>

#include "model/timeline/ClipData.h"
#include "services/decoder/VideoDecoder.h"

class ExchangeService
{
public:
	QMimeData* libItemToMIME(const LibItemData* data) const;

	const QString& getLibItemMimeFormat() const;

	ClipData* libItemMimeToClipData(const QMimeData* mimeData);

private:
	QString libItemMimeType = "application/x-libitemdata";
	VideoDecoder videoDecoder;
};


