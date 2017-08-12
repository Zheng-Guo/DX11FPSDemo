#pragma once
#include <windows.h>

class Clock
{
private:
	double countsPerSecond;
	__int64 startCount;
	__int64 currentCount;
	__int64 previousCount;
public:
	Clock();
	void startClock();
	void tick();
	double getGameTime() const;
	double getDeltaTime() const;
};