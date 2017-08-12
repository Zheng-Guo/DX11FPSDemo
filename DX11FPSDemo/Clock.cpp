#include "Clock.h"

Clock::Clock() :startCount(0),
currentCount(0),
previousCount(0)
{
	LARGE_INTEGER frequencyCount;
	QueryPerformanceFrequency(&frequencyCount);
	countsPerSecond = frequencyCount.QuadPart;
}

void Clock::startClock()
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	startCount = currentCount = previousCount = count.QuadPart;
}

void Clock::tick()
{
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);
	previousCount = currentCount;
	currentCount = current.QuadPart;
}

double Clock::getGameTime() const
{
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);
	return (current.QuadPart - startCount) / countsPerSecond;
}

double Clock::getDeltaTime() const
{
	return (currentCount - previousCount) / countsPerSecond;
}


