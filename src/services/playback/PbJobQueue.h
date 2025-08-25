#pragma once
#include <qmutex.h>
#include <queue>
#include <QWaitCondition>

#include "model/timeline/ClipData.h"

struct PbJob
{
	ClipData* clip;
	double pts;
};

struct JobComparator
{
	bool operator()(const PbJob& a, const PbJob& b) const
	{
		return a.pts > b.pts; // Min-heap based on pts
	}
};

class PbJobQueue
{
public:
	size_t size();

	void push(ClipData* clip, double pts);
	void pushIfCapacity(ClipData* clip, double pts, size_t capacity);

	PbJob pop();

private:
	QMutex mutex;
	std::priority_queue<PbJob, std::vector<PbJob>, JobComparator> jobQueue;
	QWaitCondition emptyCond, capacityCond;
};
