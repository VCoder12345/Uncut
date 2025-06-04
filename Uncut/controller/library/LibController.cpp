#include "LibController.h"
#include <QDebug>
#include <qfiledialog.h>
#include <qstandardpaths.h>
#include <utils/TimeHelper.h>
#include <QMimeData>
#include <QDrag>

void LibController::onItemImport()
{
	//QString videoDir = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
	QString videoDir = "D:/Videos";
	QString filePath = QFileDialog::getOpenFileName(nullptr, tr("Import File"), videoDir, tr("Video Files (*.mov *.mp4 *.avi *.mkv);;Audio Files (*.wav *.mp3);;Images (*.png *.jpg);;Files (*)"));
	
	importItemFromFile(filePath);
}

LibController::LibController(Uncut& window, LibModel* model) : model(model)
{
	LibraryView* libView = window.getLibView();
    libView->setModel(model);

	QObject::connect(window.getImportAction(), &QAction::triggered, this, &LibController::onItemImport);
    QObject::connect(libView, &LibraryView::filesDropped, this, &LibController::onFilesDropped);
    QObject::connect(libView, &LibraryView::itemSelected, this, &LibController::onItemSelected);
	connect(libView, &LibraryView::itemPressed, this, &LibController::onItemPressed);
}

void LibController::onFilesDropped(const QStringList& filePaths)
{
	for (const QString& filePath : filePaths) {
		importItemFromFile(filePath);
	}
}

void LibController::onItemSelected(int selected, const LibItemData* data)
{
	model->setSelectedItem(selected);
}

void LibController::onItemPressed(int selected, const LibItemData* data)
{
	QByteArray dropData;
	QDataStream dropStream(&dropData, QIODevice::WriteOnly);
	dropStream << data->filePath;
	
	QMimeData* mimeData = new QMimeData;
	mimeData->setData("application/x-libitemdata", dropData);
	QDrag* drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setPixmap(QPixmap::fromImage(data->previewFrame->toImage().scaledToHeight(30)));

	drag->exec(Qt::CopyAction);
}


void LibController::importItemFromFile(const QString& filePath)
{
	if(auto result = videoDecoder.getVideoInfo(filePath.toStdString().c_str())) {
		VideoInfo videoInfo = result.value();
		QString fileName = filePath.sliced(filePath.lastIndexOf("/") + 1);
		TimeObj durationObj = TimeObj::timeFromMicroseconds(videoInfo.duration);
		LibItemData* data = new LibItemData(videoInfo.firstFrame, filePath, fileName, durationObj);
		model->addItem(data);
	}
}
