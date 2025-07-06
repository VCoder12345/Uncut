#pragma once
#include <qimage.h>
#include <qlist.h>
#include <vector>
#include <QObject>
#include <QString>
#include <utils/TimeHelper.h>
#include "services/decoder/VideoFrame.h"

class LibItemData {
public:
	LibItemData(std::shared_ptr<VideoFrame> previewFrame, QString filePath, QString name, TimeObj duration) : previewFrame(previewFrame), filePath(filePath), name(name), duration(duration), selected(false) {

	}

	std::shared_ptr<VideoFrame> previewFrame;
	QString name;
	TimeObj duration;
	bool selected;
	QString filePath;
};

class LibModel : public QObject
{
	Q_OBJECT

public:
  void addItem(LibItemData *data);

  int getSelectedItem();

  void setSelectedItem(int selected);

  ~LibModel();

signals:
	void itemAdded(const LibItemData* data);
	void itemSelected(int oldSelected, int selected, const LibItemData* data);

private:
	std::vector<LibItemData*> itemDataList;
	int selectedItem = -1;
};

