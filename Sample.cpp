#include "MusicGen.h"
#include "Lottery.h"

namespace MusicGen
{

using namespace Internal;

Sample::Sample(Generator* gen, unsigned idx)
: generator(gen),
  index(idx)
{
	targetParams = new float[gen->paramCount];
	for (int i = 0; i < gen->paramCount; ++i)
	{
		targetParams[i] = 0.5f;
	}
}

Sample::~Sample()
{
	delete [] targetParams;
}

void Sample::addGroup(Group* grp)
{
	groups.push_back(grp);
}

void Sample::setTargetParams(const float* params)
{
	memcpy(targetParams, params, sizeof(float) * generator->paramCount);
}

void Sample::addSuccessor(Sample* sample)
{
	successors.push_back(sample);
}

Sample* Sample::pickSuccessor()
{
	// algorithm:
	// (1) select a list of valid successors (ie no group conflicts)
	// (2) add them into a lottery, based on fitness
	// (3) pick one
	Lottery<Sample*> lottery(generator->prng);
	for (std::vector<Sample*>::iterator I = successors.begin(),
	                                    E = successors.end();
	                                    I != E;
	                                    ++I)
	{
		Sample* sample = *I;
		bool valid = false;
		if (sample->groups.empty())
		{
			valid = true;
		}
		else
		{
			for (std::vector<Group*>::iterator iter = sample->groups.begin();
											   iter != sample->groups.end();
											   ++iter)
			{
				if (*iter == generator->currentGroup)
				{
					valid = true;
				}
			}
		}
		if (valid)
		{
			uint32_t tickets = (uint32_t)(generator->fitness(sample->targetParams) * 1024.0f);
			if (tickets == 0)
				tickets = 1;
			lottery.addChoice(sample, tickets);
		}
	}
	return lottery.choose();
}

}
