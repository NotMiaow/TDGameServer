#ifndef VECTOR_2_H__
#define VECTOR_2_H__

struct Vector2
{
	Vector2() { };
	Vector2(const float& x, const float& y) : x(x), y(y) { }
	~Vector2() { };
	float x;
	float y;
};

#endif
