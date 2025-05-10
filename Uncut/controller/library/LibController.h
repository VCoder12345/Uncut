#pragma once
#include <QObject>
#include "model/library/LibModel.h"
#include <qstringlist.h>

class LibController : public QObject
{
	Q_OBJECT

public:
	LibController(LibModel* model);
	

public slots:
	void onItemImport();
	void onFilesDropped(const QStringList& filePaths);

private:
	LibModel* model;

	void importItemFromFile(const QString& filePath);
};

