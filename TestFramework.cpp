#include "MusicGen.h"
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include "Lottery.h"

using namespace MusicGen;

const char* SAMPLE_NAMES[] = {"Lead1", "Lead2", "LeadFill", "Chords1", "Chords2", "Bass1", "Bass2", "Kit1", "Kit2"};

static void testSeq()
{
	SimplePRNG simple(1);
	const unsigned int sampleLengths[] = {4, 4, 4, 4};
	Generator* gen = new Generator(simple, 4, 1, sampleLengths, 8);
	Group* onlyGroup = new Group(gen);
	onlyGroup->addSuccessor(onlyGroup);
	gen->setInitialGroup(onlyGroup);
	Channel* channel;
	// fill in the lead details
	channel = gen->channelAtIndex(0);
	Sample* lead1 = new Sample(gen, 0);
	Sample* lead2 = new Sample(gen, 1);
	Sample* leadFill = new Sample(gen, 2);
	lead1->addSuccessor(lead1);
	lead1->addSuccessor(lead2);
	lead2->addSuccessor(lead1);
	lead2->addSuccessor(lead2);
	lead2->addSuccessor(leadFill);
	leadFill->addSuccessor(lead1);
	channel->setInitialSample(lead1);
	// fill in the chord details
	channel = gen->channelAtIndex(1);
	Sample* chords1 = new Sample(gen, 3);
	Sample* chords2 = new Sample(gen, 4);
	chords1->addSuccessor(chords1);
	chords1->addSuccessor(chords2);
	chords2->addSuccessor(chords1);
	chords2->addSuccessor(chords2);
	channel->setInitialSample(chords1);
	// fill in the bass details
	channel = gen->channelAtIndex(2);
	Sample* bass1 = new Sample(gen, 5);
	Sample* bass2 = new Sample(gen, 6);
	bass1->addSuccessor(bass2);
	bass2->addSuccessor(bass1);
	channel->setInitialSample(bass1);
	// fill in the kit details
	channel = gen->channelAtIndex(3);
	Sample* kit1 = new Sample(gen, 7);
	Sample* kit2 = new Sample(gen, 8);
	kit1->addSuccessor(kit1);
	kit1->addSuccessor(kit2);
	kit2->addSuccessor(kit1);
	channel->setInitialSample(kit2);
	// do some stuff
	for (;;)
	{
		unsigned channels[4];
		gen->beat(channels);
		printf("-\n");
		for (int i = 0; i < 4; ++i)
		{
			if (channels[i] != Generator::KEEP)
			{
				printf("%d: %s\n", i, SAMPLE_NAMES[channels[i]]);
			}
		}
		usleep(500000);
	}
}

static void testLottery()
{
	SimplePRNG simple(time(NULL));
	Internal::Lottery<char> lottery(simple);
	lottery.addChoice('a', 4);
	lottery.addChoice('b', 1);
	for (int i = 0; i < 16; ++i)
	{
		putchar(lottery.choose());
	}
	putchar('\n');
}

int main()
{
	testSeq();
	return 0;
}
