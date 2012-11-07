#ifndef COLOR_H
#define COLOR_H

class Color3f {
public:
	float r, g, b;
public:
	Color3ub():r(0), g(0), b(0);
	Color3ub(float rr, float gg, float bb):r(rr), g(gg), b(bb);
};

#endif
