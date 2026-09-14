#include "PbJobQueue.h"

size_t PbJobQueue::size()
{
	QMutexLocker locker(&mutex);
	return jobQueue.size();
}

void PbJobQueue::push(ClipData* clip, const double pts)
{
	QMutexLocker locker(&mutex);
	jobQueue.push({clip, pts, nullptr});
	emptyCond.wakeOne();
}

void PbJobQueue::pushIfCapacity(ClipData* clip, const double pts, const size_t capacity)
{
	while (jobQueue.size() >= capacity)
	{
		
		capacityCond.wait(&mutex);
	}

	push(clip, pts);
}

PbJob PbJobQueue::pop()
{
	QMutexLocker locker(&mutex);
	while (jobQueue.empty())
	{
		emptyCond.wait(&mutex);
	}
	PbJob job = std::move(const_cast<PbJob&>(jobQueue.top()));
	jobQueue.pop();

	capacityCond.wakeOne();
	return job;
}
