#pragma once
#include <QObject>
#include <model/library/LibModel.h>

class PreviewController : public QObject
{
	Q_OBJECT

public slots:
	void onLibItemSelected(int oldSelected, int selected, const LibItemData* data);

private:

};

