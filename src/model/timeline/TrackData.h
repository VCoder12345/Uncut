#pragma once
#include "ClipData.h"
#include <vector>

class TrackData
{
public:
	std::vector<ClipData*> clips;


	~TrackData();

	void addClip(ClipData* clipData);

	void removeClip(ClipData* clip);
};
