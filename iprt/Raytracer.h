#ifndef _RAYTRACER_
#define _RAYTRACER_

#include<vector>
#include<cmath>
#include"structures.h"
#include"Primitives.h"
#include"util.h"

//Main class, that do ray tracing
class Raytracer{
public:
	//Class constructor that creates ambient
	Raytracer(XYZ eyePos, XYZ planePos, XYZ planeOX, XYZ planeOY, double planeSizeX, 
		double planeSizeY, int resoluionX, int resolutionY);

	//No need to comment
	Color getPixelColor(int x, int y);

	//Some methods to set ambient
	void AddPrimitive(Primitive * s);
	void AddLight(Light l);

	void SetBackground(Color c);
	void SetGlobalLight(Color c);

	~Raytracer();
private:
	//Two <vectors> to hold scene primitives and lights
	std::vector<Primitive *> m_primitives;
	std::vector<Light> m_lights;

	//Eye position
	XYZ m_eyePos;

	//Coords of center of screen and of first pixel
	XYZ m_planeCenter;
	XYZ m_planeZero;

	//Vectors parrallel to axes oX and oY
	XYZ m_planeOX;
	XYZ m_planeOY;

	//Screen dimensions
	double m_planeSizeX, m_planeSizeY;

	//Screen resolution
	int m_resolutionX, m_resolutionY;

	//Pixel dimension
	double m_pixelSizeX, m_pixelSizeY;

	Color m_globalLight;
	Color m_background;

	//Вспомогательный метод, который вычисляет ближайшую точку пересечения луча с одной из сфер.
	//ray - направлени луча,
	//source - источнк луча,
	//s и len - указатели на обьекты, в которые заносятся сфера, с которой произошло пересечение и
	//длина до пересечения соответственно.
	//Если пересечение существует метод вернёт true, иначе false
	bool calculateCross(XYZ ray, XYZ source, Primitive ** s, double * len, bool refractive);

	Color calcCrossColor(XYZ ray, XYZ source, int iterNum, double currRefraction, bool refractive);
};

#endif//_RAYTRACER_
