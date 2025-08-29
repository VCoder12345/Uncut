#include "PbVideoDecoder.h"

void PbVideoDecoder::run()
{
	while (true)
	{
		PbJob job = jobQueue->pop();

		if (job.stream == nullptr)
		{
		}
	}
}
