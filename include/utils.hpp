#pragma once

struct Color
{
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	Color(const Color&) = default;
	~Color() = default;

	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};
