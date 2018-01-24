#include "stdafx.h"
#include "Przeciwnik.h"


Przeciwnik::Przeciwnik(pos_t pos, pos_t vel , pos_t acc, SDL_Texture* text)
{
	position = pos;
	velocity = vel;
	acceleration = acc;
	texture = text;
}

void Przeciwnik::move(int dt)
{
	velocity = (velocity + acceleration * dt);
	position = position + velocity * dt + acceleration * dt * dt * 0.5;
}

Przeciwnik::~Przeciwnik()
{
}
