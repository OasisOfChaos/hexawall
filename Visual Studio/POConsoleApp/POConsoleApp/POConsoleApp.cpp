// POConsoleApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "fled.h"
#include "Sequence.h"
#include <vector>

#include "SequenceManager.h"

//#include "C:\Users\Lix\Documents\Arduino\libraries\FastLED\src\pixeltypes.h"

CRGB leds[NUM_LEDS];

SequenceManager sequenceManager;

int main()
{
    leds[0].red = 50;
    leds[0].green = 100;
    leds[0].blue = 150;

    leds[1].red = 150;
    leds[1].green = 10;
    leds[1].blue = 15;

    leds[2].red = 10;
    leds[2].green = 11;
    leds[2].blue = 12;

    //auto beeper = std::unique_ptr<Sequence>(new Sequence(leds, NUM_LEDS));
    ////beeper->test(leds, NUM_LEDS);
    //beeper->tick();

    std::cout << "Hello World!\n";


    sequenceManager.setSequence(SequenceName::Boot);
    sequenceManager.setSequence(SequenceName::Cylon);

    sequenceManager.tick();

    exit(0);

    //Sequence* test = new Sequence(leds, NUM_LEDS);
    //test->tick();
    //delete test;

    Sequence* test2 = new SequenceOne(leds, NUM_LEDS);
    test2->tick();
    test2->tock();
    delete test2;

    Sequence* test3 = new SequenceTwo(leds, NUM_LEDS);
    test3->tick();
    test3->tock();
    delete test3;

    //std::array<std::unique_ptr<Sequence>> arr = {
    //    std::make_unique<SequenceOne>(leds, NUM_LEDS),
    //    std::make_unique<SequenceTwo>(leds, NUM_LEDS)
    //};

    Sequence* sequences[] = {
        new SequenceOne(leds, NUM_LEDS),
        new SequenceTwo(leds, NUM_LEDS)
    };

    for (int i = 0; i < 2; i++) {
        sequences[i]->tick();
        sequences[i]->tock();
    }

    //delete[] sequences;

    for (int i = 0; i < 2; i++) {
        delete sequences[i];
    }

    //delete[] sequences;


}


//class Test
//{
//public:
//    Test() { std::cout << "Created    " << this << "\n"; }
//    ~Test() { std::cout << "Destroyed  " << this << "\n"; }
//    Test(Test const& rhs) { std::cout << "Copied     " << this << "\n"; }
//    Test& operator=(Test const& rhs) { std::cout << "Assigned   " << this << "\n"; }
//};

