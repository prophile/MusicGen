#include "MusicGen.h"

#include "Lottery.h"

namespace MusicGen
{

using namespace Internal;

Group::Group(Generator* gen)
: generator(gen)
{
	params = new float[generator->paramCount];
	for (unsigned i = 0; i < generator->paramCount; ++i)
	{
		params[i] = 0.5f;
	}
}

void Group::setTargetParams(const float* newParams)
{
	memcpy(params, newParams, sizeof(float) * generator->paramCount);
}

void Group::addSuccessor(Group* grp)
{
	successors.push_back(grp);
}

Group* Group::pickSuccessor()
{
	Lottery<Group*> lottery(generator->prng);
	for (std::vector<Group*>::iterator I = successors.begin(),
	                                   E = successors.end();
	                                   I != E;
	                                   ++I)
	{
		Group* group = *I;
		uint32_t tickets = (uint32_t)(generator->fitness(group->params) * 1024.0f);
		if (tickets == 0)
			tickets = 1;
		lottery.addChoice(group, tickets);
	}
	return lottery.choose();
}

}
