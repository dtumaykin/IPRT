#ifndef _STRUCTURES_
#define _STRUCTURES_

//Цвет
struct Color
{
	unsigned short r, g, b;
};

//Позиция в трёхмерном пространстве
struct XYZ
{
	double x, y, z;
};


//Источник света
struct Light
{
	XYZ pos;
	Color col;
};

#endif//_STRUCTURES_
