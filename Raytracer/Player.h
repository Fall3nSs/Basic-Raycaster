#pragma once
#include <SDL3/SDL.h>

class Player 
{
private:
public:
	SDL_FRect playerObject;
	float speed = 4.0f;
	float pdx = 0, pdy = 0,pa = 0; // Player direction
	Player(float x, float y, float width, float height) {
		playerObject.x = x;
		playerObject.y = y;
		playerObject.w = width;
		playerObject.h = height;
	}
};