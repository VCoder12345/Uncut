#pragma once

#include <QObject>

#include "PbJobQueue.h"
#include "model/timeline/TlModel.h"

struct ClipIndex
{
	int clipIndex;
	int trackIndex;
	ClipData* clip;
};

class PbScheduler : public QObject
{
	Q_OBJECT

public:
	PbScheduler(const TlModel& tlModel, std::shared_ptr<PbJobQueue> videoJobQueue,
		std::shared_ptr<PbJobQueue> audioJobQueue, const size_t videoJobLimit, const size_t audioJobLimit);

	~PbScheduler();

private:
	const TlModel& tlModel;
	std::shared_ptr<PbJobQueue> videoJobQueue;
	std::shared_ptr<PbJobQueue> audioJobQueue;
	const size_t videoJobLimit, audioJobLimit;

	void run();
};
