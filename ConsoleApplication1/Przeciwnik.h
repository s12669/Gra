#pragma once
#include <array>
#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
using pos_t = std::array<double, 2>; 

inline pos_t operator +(const pos_t &a, const pos_t &b) {
	return { a[0] + b[0], a[1] + b[1] };
}

inline pos_t operator *(const pos_t &a, const double &b) {
	return { a[0] * b, a[1] * b };
}

class Przeciwnik
{
public:
	Przeciwnik(pos_t pos, pos_t vel, pos_t acc, SDL_Texture* text);
	~Przeciwnik();
	pos_t position;
	pos_t velocity;
	pos_t acceleration;
	void move(int dt);
	SDL_Texture* texture;
};

