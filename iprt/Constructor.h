#ifndef _CONSTRUCTOR_
#define _CONSTRUCTOR_

#include "Primitives.h"
#include "structures.h"
#include "util.h"

void Create_Box(Polygon ** polygones, XYZ c, XYZ d, XYZ r, XYZ f, Color col, double SpLevel, double MetLevel, double TransLevel, double RefractLevel);

void Create_Pyramid(Polygon ** polygones, XYZ p1, XYZ p2, XYZ p3, XYZ p4, Color col, double SpLevel, double MetLevel, double TransLevel, double RefractLevel);

//void Create_Conus(Polygon ** polygones, XYZ top, XYZ bot, int gr, int r, Color col, double SpLevel);

#endif//_CONSTRUCTOR_