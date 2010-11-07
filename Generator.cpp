#include "MusicGen.h"
#include <math.h>

namespace MusicGen
{

Generator::Generator(PRNG& aPrng,
                     unsigned ccount,
                     unsigned pcount,
                     const unsigned* channelSampleLengths,
                     unsigned _groupLength)
: currentBeat(0),
  prng(aPrng),
  channelCount(ccount),
  paramCount(pcount),
  groupLength(_groupLength)
{
	params = new float[paramCount];
	for (unsigned i = 0; i < paramCount; ++i)
	{
		params[i] = 0.5f;
	}
	channels = new Channel*[channelCount];
	for (unsigned i = 0; i < channelCount; ++i)
	{
		channels[i] = new Channel(this, channelSampleLengths[i]);
	}
}

Generator::~Generator()
{
	delete [] params;
	for (unsigned i = 0; i < channelCount; ++i)
	{
		delete channels[i];
	}
	delete [] channels;
}

void Generator::beat(unsigned* channelSamples)
{
	if (currentBeat == 0)
	{
		// special case
		for (unsigned i = 0; i < channelCount; ++i)
		{
			channelSamples[i] = channels[i]->currentSample->index;
		}
	}
	else
	{
		// update the group
		if (currentBeat % groupLength == 0)
		{
			currentGroup = currentGroup->pickSuccessor();
		}
		// update each channel
		for (unsigned i = 0; i < channelCount; ++i)
		{
			if (currentBeat % channels[i]->sampleLength == 0)
			{
				channels[i]->currentSample = channels[i]->currentSample->pickSuccessor();
				channelSamples[i] = channels[i]->currentSample->index;
			}
			else
			{
				channelSamples[i] = KEEP;
			}
		}
	}
	++currentBeat;
}

void Generator::setParams(const float* newParams)
{
	memcpy(params, newParams, paramCount * sizeof(float));
}

void Generator::setInitialGroup(Group* group)
{
	currentGroup = group;
}

Channel* Generator::channelAtIndex(unsigned idx)
{
	return channels[idx];
}

float Generator::fitness(const float* targetParams)
{
	float diffs = 0.0f;
	for (unsigned i = 0; i < paramCount; ++i)
	{
		diffs += fabsf(targetParams[i] - params[i]);
	}
	return diffs / (float)paramCount;
}

}
