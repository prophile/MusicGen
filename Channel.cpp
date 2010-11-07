#include "MusicGen.h"

namespace MusicGen
{

Channel::Channel(Generator* gen, unsigned sampleLength)
: currentSample(0),
  sampleLength(sampleLength),
  generator(gen)
{
}

Channel::~Channel()
{
}

void Channel::setInitialSample(Sample* sample)
{
	currentSample = sample;
}

}
