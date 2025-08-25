#include "PbScheduler.h"

#include <queue>

PbScheduler::PbScheduler(const TlModel& tlModel, std::shared_ptr<PbJobQueue> videoJobQueue,
                         std::shared_ptr<PbJobQueue> audioJobQueue, const size_t videoJobLimit,
                         const size_t audioJobLimit): tlModel(tlModel),
                                                      videoJobQueue(videoJobQueue),
                                                      audioJobQueue(audioJobQueue),
                                                      videoJobLimit(videoJobLimit),
                                                      audioJobLimit(audioJobLimit)
{
}

PbScheduler::~PbScheduler()
{
}

void PbScheduler::run()
{
	auto cmp = [](const ClipIndex& left, const ClipIndex& right) { return left.clip->startPts > right.clip->startPts; };
	std::priority_queue<ClipIndex, std::vector<ClipIndex>, decltype(cmp)> trackHeadQueue(cmp);

	for (int i = 0; i < tlModel.tracks.size(); ++i)
	{
		if (!tlModel.tracks[i]->clips.empty())
		{
			trackHeadQueue.push({0, i, tlModel.tracks[i]->clips[0]});
		}
	}

	while (!trackHeadQueue.empty())
	{
		const ClipIndex& cli = trackHeadQueue.top();
		trackHeadQueue.pop();
		videoJobQueue->pushIfCapacity(cli.clip, cli.clip->startPts, videoJobLimit);

		if (cli.clipIndex < tlModel.tracks[cli.trackIndex]->clips.size() - 1)
		{
			trackHeadQueue.push({
				cli.clipIndex + 1, cli.trackIndex, tlModel.tracks[cli.trackIndex]->clips[cli.clipIndex + 1]
			});
		}
	}
}
