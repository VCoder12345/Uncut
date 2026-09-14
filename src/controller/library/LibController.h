#pragma once
#include <QObject>
#include "model/library/LibModel.h"
#include <view/Uncut.h>

#include "services/exchange/ExchangeService.h"

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
	void onItemPressed(int selected, const LibItemData* data);

private:
	LibModel* model;
	ExchangeService exchangeService;
};

