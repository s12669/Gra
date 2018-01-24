#pragma once
#include "Przeciwnik.h"
class Fizykabg
{
public:
	Fizykabg();
	Fizykabg(pos_t pos, pos_t vel, pos_t acc);
	~Fizykabg();
	pos_t position;
	pos_t velocity;
	pos_t acceleration;
	void move(int dt);
};

