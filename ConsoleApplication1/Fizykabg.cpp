//#include "stdafx.h"
#include "Fizykabg.h"


Fizykabg::Fizykabg(pos_t pos, pos_t vel, pos_t acc)
{
	position = pos;
	velocity = vel;
	acceleration = acc;
}

Fizykabg::Fizykabg()
{
	position = { 0.0,0.0 };
	velocity = { 0.0,0.0 };
	acceleration = { 0.0,0.0 };
}

void Fizykabg::move(int dt)
{
	velocity = (velocity + acceleration * dt);
	position = position + velocity * dt + acceleration * dt * dt * 0.5;
}


Fizykabg::~Fizykabg()
{
}
