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
	QString name;
	std::shared_ptr<VideoFrame> previewFrame;
	int trackIndex = -1;

	ClipData(std::shared_ptr<VideoFrame> previewFrame, QString filePath, QString name, double startPts, double endPts, double pos) : previewFrame(previewFrame), filePath(filePath),
		name(name), startPts(startPts), endPts(endPts), pos(pos)
	{
	}

	double duration() const {
		return endPts - startPts;
	}
};
