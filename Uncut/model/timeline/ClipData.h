#pragma once
#include <qstring.h>
#include <services/decoder/VideoFrame.h>
#include <memory>

class ClipData {
public:
	double startPts, endPts;
	double pos;
	bool selected = false;
	QString filePath;
	QImage previewImage;
	int trackIndex = -1;

	ClipData(QString previewImage, QString filePath, double startPts, double endPts, double pos) : previewImage(previewImage), filePath(filePath),
		startPts(startPts), endPts(endPts), pos(pos) {}

	double duration() const {
		return endPts - startPts;
	}
};
