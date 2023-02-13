#include "Timer.h"
#include <iostream>

Timer::Timer(Entity entity, float startingTime)
    //this syntax calls the parent constructor, only part of implementation?
    : Component(entity), initialTime(startingTime), timeRemaining(startingTime)
{
}

void Timer::End()
{
    std::cout << "TIMER END: RING! RING! -> from " << (Timer*)this << std::endl;
    if (repeat)
    {
        timeRemaining = initialTime;
    }
    else
        active = false;
}

void Timer::Tick()
{
    if (timeRemaining > 0)
    {
        timeRemaining -= .02;
    }
    else
    {
        End();
    }
}

Timer::~Timer()
{
}

void Timer::process()
{
    if (active)
    {
        Tick();
    }
}
