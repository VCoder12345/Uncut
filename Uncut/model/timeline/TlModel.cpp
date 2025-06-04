#include "TlModel.h"


TlModel::TlModel()
{
}

TlModel::~TlModel()
{
	for (TrackData* track : tracks) {
		delete track;
	}
}

void TlModel::addClip(ClipData* clipData, int trackIndex) {
	clipData->trackIndex = trackIndex;
	tracks[trackIndex]->addClip(clipData);

	emit clipAdded(clipData, trackIndex);
}

void TlModel::addTrack()
{
	TrackData* data = new TrackData();
	tracks.push_back(data);
	emit trackAdded(data);
}

void TlModel::select(ClipData* clip)
{
	clearSelection(false);
	addSelect(clip);
}

void TlModel::clearSelection(bool emitEvent)
{
	if(slcClip)
		slcClip->selected = false;

	slcClip = nullptr;

	if(emitEvent)
		emit clipsSelected();
}

void TlModel::addSelect(ClipData* clip)
{
	clip->selected = true;
	slcClip = clip;
	emit clipsSelected();
}

void TlModel::moveClip(ClipData* clip, double newPos, int newTrackIndex)
{
	tracks[clip->trackIndex]->removeClip(clip);
	clip->trackIndex = newTrackIndex;
	clip->pos = newPos;
	
	tracks[newTrackIndex]->addClip(clip);
	emit clipMoved();
}

