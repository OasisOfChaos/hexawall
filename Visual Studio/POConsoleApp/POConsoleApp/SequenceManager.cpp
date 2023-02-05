#include "SequenceManager.h"

#include <iostream>

#include "fled.h"
#include "SeqCylon.h"

using namespace std;

SequenceManager::SequenceManager()
{
	cout << "SequenceManager constructor" << endl;
}

SequenceManager::~SequenceManager()
{
	cout << "SequenceManager destructor" << endl;
}

void SequenceManager::tick()
{
	if (!SequenceChanging) {
		CurrentSequence->tick();
	}
}

void SequenceManager::setSequence(SequenceName sequence)
{
	SequenceChanging = true;

	//clear current sequence
	delete CurrentSequence;

	switch (sequence)
	{
	case SequenceName::Boot:
		CurrentSequence = new SequenceOne(LEDArray, LEDCount);
		break;

	case SequenceName::Cylon:
		CurrentSequence = new SeqCylon(LEDArray, LEDCount);
		break;

	case SequenceName::SolidColor:
		CurrentSequence = new SeqCylon(LEDArray, LEDCount);
		break;

	default:
		break;
	}

	CurrentSequenceName = sequence;

	SequenceChanging = false;
}