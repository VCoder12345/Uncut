#pragma once

#include <QObject>
#include "TrackData.h"
#include <vector>


class TlModel  : public QObject
{
	Q_OBJECT

public:
	ClipData* slcClip;
	std::vector<TrackData*> tracks;

	TlModel();
	~TlModel();

	void addClip(ClipData* clipData, int trackIndex);
	void addTrack();
	void select(ClipData* clip);
	void clearSelection(bool emitEvent = true);
	void addSelect(ClipData* clip);
	void moveClip(ClipData* clip, double newPos, int newTrackIndex);
	void removeLastTrack();
	void removeSelection();
	void removeClip(ClipData* clipData);


signals:
	void clipAdded(ClipData* clipData, int trackIndex);
	void trackAdded(TrackData* data);
	void clipsSelected();
	void clipMoved();
	void lastTrackRemoved();
	void clipRemoved(ClipData* slcClip);

private:
	qreal defaultTrackHeight = 100;
};
