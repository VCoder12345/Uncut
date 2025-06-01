#pragma once
#include <QObject>
#include "model/library/LibModel.h"
#include <qstringlist.h>
#include <services/decoder/VideoDecoder.h>
#include <view/Uncut.h>

class LibController : public QObject
{
	Q_OBJECT

public:
	LibController(Uncut& window, LibModel* model);

	void importItemFromFile(const QString& filePath);
	

public slots:
	void onItemImport();
	void onFilesDropped(const QStringList& filePaths);
	void onItemSelected(int selected, const LibItemData* data);

private:
	LibModel* model;
	VideoDecoder videoDecoder;

	
};

