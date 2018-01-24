#pragma once
#include <array>
#include <SDL.h>
using pos_t = std::array<double, 2>;

inline pos_t operator +(const pos_t &a, const pos_t &b) {
	return { a[0] + b[0], a[1] + b[1] };
}
inline pos_t operator -(const pos_t &a, const pos_t &b) {
	return { a[0] - b[0], a[1] - b[1] };
}
/*pos_t operator -(const pos_t &a) {
return { -a[0], -a[1] };
}*/
inline pos_t operator *(const pos_t &a, const pos_t &b) {
	return { a[0] * b[0], a[1] * b[1] };
}
inline pos_t operator /(const pos_t &a, const pos_t &b) {
	return { a[0] / b[0], a[1] / b[1] };
}
inline pos_t operator *(const pos_t &a, const double &b) {
	return { a[0] * b, a[1] * b };
}
inline bool operator ==(const pos_t &a, const double &b) {
	return a[0] == b && a[1] == b;
}
class Fizykabg
{
public:
	Fizykabg(pos_t pos, pos_t vel, pos_t acc);
	~Fizykabg();
	pos_t position;
	pos_t velocity;
	pos_t acceleration;
	void move(int dt);
};

