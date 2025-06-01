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
	std::shared_ptr<VideoFrame> frame;
	int trackIndex = -1;
	int index = -1;

	ClipData(std::shared_ptr<VideoFrame> frame, QString filePath, double startPts, double endPts, double pos) : frame(frame), filePath(filePath),
		startPts(startPts), endPts(endPts), pos(pos) {}

	double duration() const {
		return endPts - startPts;
	}
};
