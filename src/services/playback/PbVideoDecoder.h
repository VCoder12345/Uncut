#pragma once
#include "PbJobQueue.h"

class PbVideoDecoder 
{
public:
	void run();

private:
	std::shared_ptr<PbJobQueue> jobQueue;
	double fragmentLength;
	double decodingMin;
};
