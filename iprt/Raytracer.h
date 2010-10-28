#ifndef _RAYTRACER_
#define _RAYTRACER_

#include<vector>
#include<cmath>
#include"structures.h"
#include"Primitives.h"
#include"util.h"

//Класс, который занимается трассировкой лучей
class Raytracer{
public:
	//Конструктор класса. Принимает все то, что написано в презентации интела и записывает в класс то, что нам
	//надо. Кое какие параметры в будущем можно убрать или измнить... Но это потом.
	Raytracer(XYZ eyePos, XYZ planePos, XYZ planeOX, XYZ planeOY, double planeSizeX, 
		double planeSizeY, int resoluionX, int resolutionY);

	//Метод получения цвета заданного пикселя
	Color getPixelColor(int x, int y);

	//Тут всё очевидно - этими методами добавляем сферу и источник света соответственно
	void AddPrimitive(Primitive * s);
	void AddLight(Light l);

	void SetBackground(Color c);
	void SetGlobalLight(Color c);

	~Raytracer();
private:
	//Вектора хранят все сферы и источники света в сфере
	std::vector<Primitive *> m_primitives;
	std::vector<Light> m_lights;

	//Позиция глаза
	XYZ m_eyePos;

	//Координаты центра экрана, и точки, в которой находится пиксель (0, 0)
	XYZ m_planeCenter;
	XYZ m_planeZero;

	//Векторы, параллельные осям OX и OY экрана
	XYZ m_planeOX;
	XYZ m_planeOY;

	//Размеры экрана
	double m_planeSizeX, m_planeSizeY;

	//Разрешение экрана
	int m_resolutionX, m_resolutionY;

	//Размеры пикселя в пространстве
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
