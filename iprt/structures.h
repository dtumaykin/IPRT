#ifndef _STRUCTURES_
#define _STRUCTURES_

//Char between 0 and 255
struct Color
{
	unsigned short r, g, b;
};

//Point in the virtual space
struct XYZ
{
	double x, y, z;
};


//Light - only one type for now - general light -- TODO: pointed light
struct Light
{
	XYZ pos;
	Color col;
};

#endif//_STRUCTURES_
