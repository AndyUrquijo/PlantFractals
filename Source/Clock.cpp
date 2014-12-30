/*
.	Author:		Andres Urquijo
.	Filename:	Clock.cpp
*/
#include "Clock.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

Clock::Clock() {
	QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
	Start();
}

void Clock::Start() {
	QueryPerformanceCounter((LARGE_INTEGER *)&start);
}

float Clock::Watch() const {
	long long finish;
	QueryPerformanceCounter((LARGE_INTEGER *)&finish);
	return float((finish - start) / double(freq));
}

float Clock::Tick() {
	long long finish;
	QueryPerformanceCounter((LARGE_INTEGER *)&finish);
	double time = (finish - start) / double(freq);
	start = finish;
	return float(time);
}
