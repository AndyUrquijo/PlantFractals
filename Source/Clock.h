/*
.	Author:		Andres Urquijo
.	Filename:	Clock.h
*/
#pragma once
class Clock
{	long long freq, start;

public:
	Clock();
	
	//Starts/Restarts the timer
	void Start();
	//Returns the elapsed time and resets the timer
	float Tick();
	//Returns the elapsed time without affecting the timer
	float Watch() const;
};

