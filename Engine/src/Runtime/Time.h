#ifndef TIME
#pragma once

#include <iostream>
#include <mutex>
#include <SDL3/SDL.h>

class Time
{
private:
	// Member variables
	static uint64_t time;
	static uint64_t previousTime;

public:
	// Constructor
	Time() {}

	// Updating Values
	static bool init()
	{
		time = SDL_GetTicksNS();
		previousTime = 0;

		return true;
	}
	static void UpdateTime()
	{
		previousTime = time;
		time = SDL_GetTicksNS();
	}

	// Getters
	static float getTime() { return SDL_GetTicksNS(); }
	static float getTimeInSeconds() { return SDL_GetTicksNS() / 1000000000.0f; }
	static float deltaTime() { return (time - previousTime) / 1000000000.0f; }
	static int getFPS() { return (int)round(1.0f / deltaTime()); }
	static float getHertz() { return (1.0f / deltaTime()); }
};

#endif // !TIME