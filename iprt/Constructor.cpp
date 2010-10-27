#include"Constructor.h"

void Create_Box(Polygon ** polygones, XYZ c, XYZ d, XYZ r, XYZ f, Color col, double SpLevel, double MetLevel, double TransLevel, double RefractLevel)
{
	XYZ p1, p2, p3, p4;
	p1.x = c.x + r.x - c.x + d.x - c.x;
	p1.y = c.y + r.y - c.y + d.y - c.y;
	p1.z = c.z + r.z - c.z + d.z - c.z;
	p2.x = c.x + r.x - c.x + f.x - c.x;
	p2.y = c.y + r.y - c.y + f.y - c.y;
	p2.z = c.z + r.z - c.z + f.z - c.z;
	p3.x = c.x + f.x - c.x + d.x - c.x;
	p3.y = c.y + f.y - c.y + d.y - c.y;
	p3.z = c.z + f.z - c.z + d.z - c.z;
	p4.x = c.x + f.x - c.x + d.x - c.x + r.x - c.x;
	p4.y = c.y + f.y - c.y + d.y - c.y + r.y - c.y;
	p4.z = c.z + f.z - c.z + d.z - c.z + r.z - c.z;
	polygones[0]->create_Polygon(d, r, c, col, SpLevel, MetLevel, TransLevel, RefractLevel);
	polygones[1]->create_Polygon(p1, r, d, col, SpLevel, MetLevel, TransLevel, RefractLevel);
	polygones[2]->create_Polygon(c, f, d, col, SpLevel, MetLevel, TransLevel, RefractLevel);
	polygones[3]->create_Polygon(p3, f, d, col, SpLevel, MetLevel, TransLevel, RefractLevel);
	polygones[4]->create_Polygon(c, r, f, col, SpLevel, MetLevel, TransLevel, RefractLevel);
	polygones[5]->create_Polygon(p2, r, f, col, SpLevel, MetLevel, TransLevel, RefractLevel);
	polygones[6]->create_Polygon(f, p3, p2, col, SpLevel, MetLevel, TransLevel, RefractLevel);
	polygones[7]->create_Polygon(p2, p3, p4, col, SpLevel, MetLevel, TransLevel, RefractLevel);
	polygones[8]->create_Polygon(p2, r, p1, col, SpLevel, MetLevel, TransLevel, RefractLevel);
	polygones[9]->create_Polygon(p2, p1, p4, col, SpLevel, MetLevel, TransLevel, RefractLevel);
	polygones[10]->create_Polygon(p3, p1, d, col, SpLevel, MetLevel, TransLevel, RefractLevel);
	polygones[11]->create_Polygon(p1, p3, p4, col, SpLevel, MetLevel, TransLevel, RefractLevel);
}

void Create_Pyramid(Polygon ** polygones, XYZ p1, XYZ p2, XYZ p3, XYZ p4, Color col, double SpLevel, double MetLevel, double TransLevel, double RefractLevel)
{
	polygones[0]->create_Polygon(p1, p2, p3, col, SpLevel, MetLevel, TransLevel, RefractLevel);
	polygones[1]->create_Polygon(p1, p2, p4, col, SpLevel, MetLevel, TransLevel, RefractLevel);
	polygones[2]->create_Polygon(p1, p3, p4, col, SpLevel, MetLevel, TransLevel, RefractLevel);
	polygones[3]->create_Polygon(p2, p3, p4, col, SpLevel, MetLevel, TransLevel, RefractLevel);
}

/*void Create_Conus(Polygon ** polygones, XYZ top, XYZ bot, int gr, int r, Color col, double SpLevel)
{
	XYZ * p = new XYZ[gr];
	p[
	polygones[0]->create_Polygon(p1, p2, p3, col, SpLevel);
	polygones[1]->create_Polygon(p1, p2, p4, col, SpLevel);
	polygones[2]->create_Polygon(p1, p3, p4, col, SpLevel);
	polygones[3]->create_Polygon(p2, p3, p4, col, SpLevel);
}*/