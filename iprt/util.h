#ifndef _UTIL_
#define _UTIL_
#include"structures.h"
#include<cmath>
	
XYZ normalizeVector(XYZ a);

double calcVectorLen(XYZ a);

double sqr(double a);

double scalarMultiplication(XYZ a, XYZ b);

double getSquare(XYZ p1, XYZ p2, XYZ p3);

#endif//_UTIL_
