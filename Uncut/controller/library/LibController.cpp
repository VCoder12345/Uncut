#include "LibController.h"
#include <QDebug>
#include <qfiledialog.h>
#include <qstandardpaths.h>

void LibController::onItemImport()
{
	QString videoDir = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
	QString filePath = QFileDialog::getOpenFileName(nullptr, tr("Import File"), videoDir, tr("Images (*.png *.jpg);;Video Files (*.mov *.mp4 *.avi *.mkv);;Audio Files (*.wav *.mp3);;Files (*)"));
	
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


void LibController::importItemFromFile(const QString& filePath)
{
	QImage img;
	img.load(filePath);
	LibItemData* data = new LibItemData(img, filePath);
	model->addItem(data);
}
