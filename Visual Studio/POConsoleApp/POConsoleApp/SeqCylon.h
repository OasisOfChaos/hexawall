#pragma once
#include "Sequence.h"

#include <iostream>

using namespace std;

class SeqCylon : public Sequence
{
public:
	SeqCylon(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}
	~SeqCylon() { std::cout << "SeqCylon::destructor()" << endl; }

	void tick() {
		std::cout << "SeqCylon::tick()" << endl;
	}
	void tock() {}
};

//
//class SequenceOne : public Sequence
//{
//public:
//	SequenceOne(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}
//	~SequenceOne();
//
//	void tick();
//	void tock();
//};
