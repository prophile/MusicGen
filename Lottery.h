#ifndef _INCLUDED_LOTTERY
#define _INCLUDED_LOTTERY

#include "MusicGen.h"
#include <map>

namespace MusicGen
{

namespace Internal
{

template <class T>

class Lottery
{
private:
	PRNG& prng;
	uint32_t maxTicket;
	std::map<uint32_t, T> tickets;
public:
	Lottery(PRNG& rng) : prng(rng), maxTicket(0) {}

	void addChoice(const T& choice, uint32_t ticketCount)
	{
		tickets.insert(std::make_pair(maxTicket += ticketCount, choice));
	}
	const T& choose()
	{
		uint32_t id = prng() % maxTicket;
		typename std::map<uint32_t, T>::iterator iter = tickets.upper_bound(id);
		// iter cannot be tickets.end()
		return iter->second;
	}
};

}

}

#endif
