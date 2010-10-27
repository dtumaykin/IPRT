#ifndef _PRIMITIVES_
#define _PRIMITIVES_
#include"structures.h"
#include"util.h"
#include<cmath>
#include<fstream>

class Primitive{
public:
	virtual double calcCross(XYZ ray, XYZ source) = 0;
	virtual double calcMaxCross(XYZ ray, XYZ source) = 0;
	virtual XYZ getNormal(XYZ crossPos) = 0;
	Color getColor(){return m_color;}
	double getSpecularLevel(){return m_specularLevel;}
	double getMetalizing(){return m_metalizing;}
	double getRefractionLevel(){return m_refractionLevel;}
	double getTransparency(){return m_transparency;}
protected:
	Color m_color;
	double m_specularLevel;
	double m_metalizing;
	double m_transparency;
	double m_refractionLevel;
public:
	bool emoTexturing;
};

class Sphere : public Primitive{
public:
	void loadSphere(std::ifstream * input);
	double calcCross(XYZ ray, XYZ source);
	double calcMaxCross(XYZ ray, XYZ source);
	XYZ getNormal(XYZ crossPos);
private:
	XYZ m_pos;
	double m_radius;
};

class Polygon : public Primitive{
public:
	void loadPolygon(std::ifstream * input);
	void create_Polygon(XYZ p1, XYZ p2, XYZ p3, Color col, double specularLevel, double MetLevel, double TransLevel, double RefractLevel);
	double calcCross(XYZ ray, XYZ source);
	double calcMaxCross(XYZ ray, XYZ source);
	XYZ getNormal(XYZ crossPos);
protected:
	XYZ m_p1, m_p2, m_p3;
};

class EmoPolygon : public Polygon{
public:
	double calcCross(XYZ ray, XYZ source);
	XYZ getNormal(XYZ crossPos);
};

#endif//_PRIMITIVES_