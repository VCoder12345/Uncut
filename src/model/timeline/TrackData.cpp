#include "TrackData.h"

TrackData::~TrackData()
{
	for (ClipData* clipData : clips) {
		delete clipData;
	}
}

void TrackData::addClip(ClipData* clipData)
{
	//keep list sorted by startPts, insert via lower_bound = binary search
	auto it = std::lower_bound(clips.begin(), clips.end(), clipData, [](ClipData* a, ClipData* b) { return a->startPts < b->startPts; });
		
	clips.insert(it, clipData);
}

void TrackData::removeClip(ClipData* clip)
{
	clips.erase(std::find(clips.begin(), clips.end(), clip));
}
