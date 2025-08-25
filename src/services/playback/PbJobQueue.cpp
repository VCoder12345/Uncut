#include "PbJobQueue.h"

size_t PbJobQueue::size()
{
	QMutexLocker locker(&mutex);
	return jobQueue.size();
}

void PbJobQueue::push(ClipData* clip, double pts)
{
	QMutexLocker locker(&mutex);
	jobQueue.push({clip, pts});
	emptyCond.wakeOne();
}

void PbJobQueue::pushIfCapacity(ClipData* clip, double pts, size_t capacity)
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
	PbJob job = jobQueue.top();
	jobQueue.pop();

	capacityCond.wakeOne();
	return job;
}
