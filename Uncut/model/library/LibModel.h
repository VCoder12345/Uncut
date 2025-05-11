#pragma once
#include <qimage.h>
#include <qlist.h>
#include <vector>
#include <QObject>
#include <QString>
#include <utils/TimeHelper.h>

class LibItemData {
public:
	LibItemData(QImage previewImg, QString filePath, QString name, TimeObj duration) : previewImg(previewImg), filePath(filePath), name(name), duration(duration), selected(false) {

	}

	QImage previewImg;
	QString name;
	TimeObj duration;
	bool selected;
	QString filePath;
};

class LibModel : public QObject
{
	Q_OBJECT

public:
	void addItem(LibItemData* data) {
		itemDataList.push_back(data);

		emit itemAdded(data);
	}

	int getSelectedItem() {
		return selectedItem;
	}

	void setSelectedItem(int selected) {
		int oldSelected = this->selectedItem;
		if(oldSelected >= 0)
			itemDataList[oldSelected]->selected = false;

		itemDataList[selected]->selected = true;
		this->selectedItem = selected;

		emit itemSelected(oldSelected, selected, itemDataList[selected]);
	}

	~LibModel() {
		for (LibItemData* data : itemDataList) {
			delete data;
		}
	}

signals:
	void itemAdded(const LibItemData* data);
	void itemSelected(int oldSelected, int selected, const LibItemData* data);

private:
	std::vector<LibItemData*> itemDataList;
	int selectedItem = -1;
};

