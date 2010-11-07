#include "MusicGen.h"

namespace MusicGen
{

// implemented here only to force vtable in here
PRNG::PRNG()
{
}

PRNG::~PRNG()
{
}

SimplePRNG::SimplePRNG(uint32_t seed)
: state(seed)
{
}

SimplePRNG::~SimplePRNG()
{
}

uint32_t SimplePRNG::current() const
{
	return state & 0x3FFFFFFF;
}

void SimplePRNG::next()
{
	state *= 1103515245;
	state += 12345;
}

uint32_t SimplePRNG::max() const
{
	return 0x3FFFFFFF;
}

}
