#pragma once
#include <qimage.h>
#include <qlist.h>
#include <vector>
#include <QObject>
#include <QString>

class LibItemData {
public:
	LibItemData(QImage previewImg, QString name) : previewImg(previewImg), name(name) {

	}

	QImage previewImg;
	QString name;
};

class LibModel : public QObject
{
	Q_OBJECT

public:
	void addItem(LibItemData* data) {
		itemDataList.push_back(data);

		emit itemAdded(data);
	}

	~LibModel() {
		for (LibItemData* data : itemDataList) {
			delete data;
		}
	}

signals:
	void itemAdded(const LibItemData* data);

private:
	std::vector<LibItemData*> itemDataList;
};

