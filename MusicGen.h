#ifndef _INCLUDED_MUSIC_GEN
#define _INCLUDED_MUSIC_GEN

#include <vector>
#include <inttypes.h>

namespace MusicGen
{

class Channel;
class Group;

class PRNG
{
private:
public:
	PRNG();
	virtual ~PRNG();

	virtual uint32_t current() const = 0;
	virtual void next() = 0;
	virtual uint32_t max() const = 0;

	uint32_t operator()()
	{
		uint32_t tmp;
		return tmp = current(), next(), tmp;
	}
};

// a simple linear congruential generator,
// with the same parameters as glibc rand() et al
class SimplePRNG : public PRNG
{
private:
	uint32_t state;
public:
	SimplePRNG(uint32_t seed);
	virtual ~SimplePRNG();

	virtual uint32_t current() const;
	virtual void next();
	virtual uint32_t max() const;
};

class Generator
{
private:
	unsigned currentBeat;
	PRNG& prng;
	unsigned channelCount;
	unsigned paramCount;
	Channel** channels;
	Group* currentGroup;
	float* params;
	unsigned groupLength;
	friend class Sample;
	friend class Channel;
	friend class Group;
	float fitness(const float* params);
public:
	Generator(PRNG& prng,
	          unsigned channels,
	          unsigned params,
	          const unsigned* channelSampleLengths,
	          unsigned groupLength);
	~Generator();

	const static unsigned KEEP = -1U;

	void beat(unsigned* channelSamples);
	void setParams(const float* params);
	void setInitialGroup(Group* group);
	Channel* channelAtIndex(unsigned idx);
};

class Group
{
private:
	std::vector<Group*> successors;
	Generator* generator;
	float* params;
	friend class Generator;
	friend class Channel;
	friend class Sample;
	Group* pickSuccessor();
public:
	Group(Generator* gen);
	~Group();

	void setTargetParams(const float* params);
	void addSuccessor(Group* grp);
};

class Sample;

class Channel
{
private:
	Sample* currentSample;
	unsigned sampleLength;
	Generator* generator;
	friend class Sample;
	friend class Group;
	friend class Generator;
	Channel(Generator* gen, unsigned sampleLength);
	~Channel();
public:
	void setInitialSample(Sample* sample);
};

class Sample
{
private:
	unsigned index;
	float* targetParams;
	std::vector<Sample*> successors;
	std::vector<Group*> groups;
	Generator* generator;
	Sample* pickSuccessor();
	friend class Generator;
	friend class Channel;
	friend class Group;
public:
	Sample(Generator* gen, unsigned index);
	~Sample();

	void addGroup(Group* group);
	void setTargetParams(const float* params);
	void addSuccessor(Sample* sample);
};

}

#endif
