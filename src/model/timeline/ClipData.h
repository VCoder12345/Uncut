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

	ClipData(std::shared_ptr<VideoFrame> previewFrame, const QString& filePath, const QString& name, double startPts, double endPts,
	         double pos) : startPts(startPts), endPts(endPts),
	                       pos(pos), filePath(filePath), name(name), previewFrame(previewFrame)
	{
	}

	double duration() const {
		return endPts - startPts;
	}
};
