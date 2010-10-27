#include"util.h"

XYZ normalizeVector(XYZ a)
{
	double length = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
	XYZ result;
	result.x = a.x/length;
	result.y = a.y/length;
	result.z = a.z/length;
	return result;
}

double calcVectorLen(XYZ a)
{
	double length = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
	return length;
}

double scalarMultiplication(XYZ a, XYZ b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

double sqr(double a)
{
	return a*a;
}

double getSquare(XYZ p1, XYZ p2, XYZ p3)
{
//	double a, b, c, p;
//	a = sqrt(sqr(p2.x-p1.x) + sqr(p2.y-p1.y) + sqr(p2.z-p1.z));
//	b = sqrt(sqr(p3.x-p2.x) + sqr(p3.y-p2.y) + sqr(p3.z-p2.z));
//	c = sqrt(sqr(p1.x-p3.x) + sqr(p1.y-p3.y) + sqr(p1.z-p3.z));
//	p = (a + b + c) / 2;
//
//	return sqrt(p*(p-a)*(p-b)*(p-c));
	double minx, miny, minz;
	minx = p1.x;
	miny = p1.y;
	minz = p1.z;
	if (p2.x < p1.x) minx = p2.x;
	if (p2.y < p1.y) miny = p2.y;
	if (p2.z < p1.z) minz = p2.z;
	if (p3.x < p2.x) minx = p3.x;
	if (p3.y < p2.y) miny = p3.y;
	if (p3.z < p2.z) minz = p3.z;
	if (minx <= 0)
	{
		p1.x -= minx - 1;
		p2.x -= minx - 1;
		p3.x -= minx - 1;
	}
	if (miny <= 0)
	{
		p1.y -= miny - 1;
		p2.y -= miny - 1;
		p3.y -= miny - 1;
	}
	if (minz <= 0)
	{
		p1.z -= minz - 1;
		p2.z -= minz - 1;
		p3.z -= minz - 1;
	}
	double doubleS = p1.x*(p3.y-p2.y)+p2.x*(p1.y-p3.y)+p3.x*(p2.y-p1.y);
	if (doubleS < 0) doubleS = - doubleS;

	return doubleS;
}