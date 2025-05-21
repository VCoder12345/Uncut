#include "LibController.h"
#include <QDebug>
#include <qfiledialog.h>
#include <qstandardpaths.h>
#include <utils/TimeHelper.h>

void LibController::onItemImport()
{
	//QString videoDir = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
	QString videoDir = "D:/Videos";
	QString filePath = QFileDialog::getOpenFileName(nullptr, tr("Import File"), videoDir, tr("Video Files (*.mov *.mp4 *.avi *.mkv);;Audio Files (*.wav *.mp3);;Images (*.png *.jpg);;Files (*)"));
	
	importItemFromFile(filePath);
}

LibController::LibController(LibModel* model) : model(model)
{
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
