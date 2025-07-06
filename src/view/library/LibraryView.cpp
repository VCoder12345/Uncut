#include "LibraryView.h"
#include "LibItem.h"
#include <qmimedata.h>
#include <qmimedatabase.h>

LibraryView::LibraryView(QWidget *parent)
	: QGraphicsView(parent)
{
	ui.setupUi(this);
	setRenderHint(QPainter::Antialiasing);

	scene = new QGraphicsScene(this);
	setScene(scene);
	
	setAcceptDrops(true);
	viewport()->setAcceptDrops(true);
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

LibraryView::~LibraryView()
{
}

void LibraryView::setModel(LibModel* model) {
	this->model = model;

	connect(model, &LibModel::itemAdded, this, &LibraryView::itemAdded);
	connect(model, &LibModel::itemSelected, this, &LibraryView::onItemSelect);
}

void LibraryView::layoutImages()
{
	int fieldWidth = itemWidth + spacingW;
	int fieldHeight = itemHeight + spacingH;
	int numItems = libItems.size();
	int viewWidth = viewport()->width();
	int cols = std::max(1, viewWidth / fieldWidth);
	int rows = (int)std::ceil(numItems / (double)cols);

	for (int i = 0; i < numItems; ++i) {
		int c = i % cols;
		int r = i / cols;

		libItems[i]->setPos(c * fieldWidth, r * fieldHeight);
	}



	scene->setSceneRect(0, 0, viewWidth, rows * fieldHeight);

	update();
}

void LibraryView::dragEnterEvent(QDragEnterEvent* event)
{
	event->acceptProposedAction();
}

void LibraryView::dragMoveEvent(QDragMoveEvent* event)
{
	
}

void LibraryView::dropEvent(QDropEvent* event)
{
	event->acceptProposedAction();
	
	QStringList filePaths;
	for (const QUrl& url : event->mimeData()->urls()) {
		filePaths << url.toLocalFile();
	}
	emit filesDropped(filePaths);
}

void LibraryView::mouseDoubleClickEvent(QMouseEvent* event)
{
	LibItem* item = static_cast<LibItem*>(itemAt(event->pos()));

	if (item) {
		int selected = item->index;
		emit itemSelected(selected, item->data);
	}

	QGraphicsView::mouseDoubleClickEvent(event);
}

void LibraryView::mousePressEvent(QMouseEvent* event)
{
	LibItem* item = static_cast<LibItem*>(itemAt(event->pos()));

	if (item) {
		int selected = item->index;
		emit itemPressed(selected, item->data);
	}

	QGraphicsView::mousePressEvent(event);
}

void LibraryView::onItemSelect(int oldSelected, int selected, const LibItemData* data)
{
	if(oldSelected >= 0)
		libItems[oldSelected]->update();
	libItems[selected]->update();

	update();
}

void LibraryView::itemAdded(const LibItemData* data)
{
	LibItem* item = new LibItem(data, itemWidth, itemHeight, libItems.size());
	scene->addItem(item);
	libItems.push_back(item);

	layoutImages();
	ensureVisible(item);
}
