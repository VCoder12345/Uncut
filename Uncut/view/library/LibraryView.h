#pragma once

#include <QGraphicsView>
#include "ui_LibraryView.h"
#include "model/library/LibModel.h"
#include "LibItem.h"
#include <QDragEnterEvent>
#include <QDropEvent>

class LibraryView : public QGraphicsView
{
	Q_OBJECT

public:
	LibraryView(QWidget *parent = nullptr);
	~LibraryView();

	void setModel(LibModel* model);
	void layoutImages();

protected:
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dragMoveEvent(QDragMoveEvent* event) override;
	void dropEvent(QDropEvent* event) override;

public slots:
	void itemAdded(const LibItemData* data);

signals:
	void filesDropped(const QStringList& filePaths);

private:
	Ui::LibraryViewClass ui;
	LibModel* model = nullptr;
	QGraphicsScene* scene;
	std::vector<LibItem*> libItems;
	const int itemWidth = 100, itemHeight = 100;
	const int spacingW = 10, spacingH = 10;

};
