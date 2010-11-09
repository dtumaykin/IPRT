#include"Raytracer.h"

#define ANTIALIAS_AM 1 //Antialias level

Raytracer::Raytracer(XYZ eyePos, XYZ planePos, XYZ planeOX, XYZ planeOY,
					 double planeSizeX, double planeSizeY, int resolutionX, int resolutionY)
{
	m_planeOX = normalizeVector(planeOX);
	m_planeOY = normalizeVector(planeOY);
						 
	m_eyePos = eyePos;
	m_planeSizeX = planeSizeX;
	m_planeSizeY = planeSizeY;
	m_resolutionX = resolutionX;
	m_resolutionY = resolutionY;
	
	//Some necessary calcs
	m_pixelSizeX = planeSizeX/resolutionX;
	m_pixelSizeY = planeSizeY/resolutionY;
	m_planeCenter.x = m_eyePos.x + planePos.x;
	m_planeCenter.y = m_eyePos.y + planePos.y;
	m_planeCenter.z = m_eyePos.z + planePos.z;
	m_planeZero.x = m_planeCenter.x - m_planeOX.x*m_planeSizeX/2 - m_planeOY.x*m_planeSizeY/2;
	m_planeZero.y = m_planeCenter.y - m_planeOX.y*m_planeSizeX/2 - m_planeOY.y*m_planeSizeY/2;
	m_planeZero.z = m_planeCenter.z - m_planeOX.z*m_planeSizeX/2 - m_planeOY.z*m_planeSizeY/2;
}

void Raytracer::AddPrimitive(Primitive * s)
{
	m_primitives.push_back(s);
}

void Raytracer::AddLight(Light l)
{
	m_lights.push_back(l);
}

void Raytracer::SetBackground(Color c)
{
	m_background = c;
}

void Raytracer::SetGlobalLight(Color c)
{
	m_globalLight = c;
}

bool Raytracer::calculateCross(XYZ ray, XYZ source, Primitive ** p, double * len, bool refractive)
{	
	Primitive * primMin;
	double lenMin = -1;

	//Проходим в цикле по всем сферам, чтобы найти ближайшее пересечение
	for(unsigned int i = 0; i<m_primitives.size(); i++){
		double crossLen;
		if (refractive)
			crossLen = m_primitives[i]->calcMaxCross(ray, source);	
		else
			crossLen = m_primitives[i]->calcCross(ray, source);	
		if(crossLen != -1)
			if((lenMin == -1) || (crossLen<lenMin)){
				lenMin = crossLen;
				primMin = m_primitives[i];
			}
	}
	//Если в минимальное расстояние ничего не занесено - значит пересечений не было - возвращаем false
	if(lenMin == -1) return false;
	//Если всё хорошо, то заносим значения по указателям и возвращаем true
	*len = lenMin;
	*p = primMin;
	return true;
}

Color Raytracer::calcCrossColor(XYZ ray, XYZ source, int iterNum, double currRefraction, bool refractive){
	//Изначально результат равен цвету фона
	Color result;
	result = m_background;

	if(iterNum>10) return result;

	ray = normalizeVector(ray);
	double len;
	Primitive * p;

	//Если пересечения нет - возвращаем цвет фона
	if(!calculateCross(ray, source, &p, &len, refractive))
		return result;
	else{
                result.r = static_cast<unsigned short>(p->getColor().r*double(m_globalLight.r)/255.0f);
                result.g = static_cast<unsigned short>(p->getColor().g*double(m_globalLight.g)/255.0f);
                result.b = static_cast<unsigned short>(p->getColor().b*double(m_globalLight.b)/255.0f);
		//Иначе начинаем считать тени
		//Находим координаты точки пересечения
		XYZ crossPos; 
		crossPos.x = ray.x*len + source.x;
		crossPos.y = ray.y*len + source.y;
		crossPos.z = ray.z*len + source.z;
		
		//Находим нормаль в точке пересечения
		XYZ norm = p->getNormal(crossPos);
		if (scalarMultiplication(norm,ray) > 0)
		{
			norm.x = - norm.x;
			norm.y = - norm.y;
			norm.z = - norm.z;
		}
		norm = normalizeVector(norm);


		// считаем отражение
		XYZ temp, reflectRay;
		Color reflectColor = m_background;
//		if (currRefraction == 1)
//		{
			temp.x = (-2)*scalarMultiplication(norm,ray)*norm.x;
			temp.y = (-2)*scalarMultiplication(norm,ray)*norm.y;
			temp.z = (-2)*scalarMultiplication(norm,ray)*norm.z;
			reflectRay.x = temp.x + ray.x;
			reflectRay.y = temp.y + ray.y;
			reflectRay.z = temp.z + ray.z;
			reflectRay = normalizeVector(reflectRay);
			reflectColor = calcCrossColor(reflectRay,crossPos,iterNum+1, currRefraction, refractive);
			if (reflectColor.r>p->getColor().r) reflectColor.r=p->getColor().r;
			if (reflectColor.g>p->getColor().g) reflectColor.g=p->getColor().g;
			if (reflectColor.b>p->getColor().b) reflectColor.b=p->getColor().b;
//		}

		// считаем преломление
		XYZ temp2, refractRay, temp3;
		double refractionScaler;
		Color refractColor = m_background;
		bool fullReflection = false;
		if (p->getTransparency())
		{
			temp2.x = (-1)*scalarMultiplication(norm,ray)*norm.x;
			temp2.y = (-1)*scalarMultiplication(norm,ray)*norm.y;
			temp2.z = (-1)*scalarMultiplication(norm,ray)*norm.z;
			temp3.x = temp2.x + ray.x;
			temp3.y = temp2.y + ray.y;
			temp3.z = temp2.z + ray.z;
			if (currRefraction == 1)
			{
//				temp3.x /= p->getRefractionLevel();
//				temp3.y /= p->getRefractionLevel();
//				temp3.z /= p->getRefractionLevel();
//				refractRay.x = temp3.x - temp2.x;
//				refractRay.y = temp3.y - temp2.y;
//				refractRay.z = temp3.z - temp2.z;
//				refractRay = normalizeVector(refractRay);
//				refractColor = calcCrossColor(refractRay, crossPos, iterNum+1, p->getRefractionLevel(), true);
				if (sqr(p->getRefractionLevel()) > sqr(calcVectorLen(temp3)))
				{
					temp2 = normalizeVector(temp2);
					refractionScaler = sqrt(1 - sqr(calcVectorLen(temp3)/p->getRefractionLevel()));
					temp2.x *= refractionScaler;
					temp2.y *= refractionScaler;
					temp2.z *= refractionScaler;
					temp3.x /= p->getRefractionLevel();
					temp3.y /= p->getRefractionLevel();
					temp3.z /= p->getRefractionLevel();
					refractRay.x = temp3.x - temp2.x;
					refractRay.y = temp3.y - temp2.y;
					refractRay.z = temp3.z - temp2.z;
					refractRay = normalizeVector(refractRay);
					refractColor = calcCrossColor(refractRay, crossPos, iterNum+1, p->getRefractionLevel(), true);
				}
				else
				{
					fullReflection = true;
				}
			}
			else
			{
//				temp3.x *= p->getRefractionLevel();
//				temp3.y *= p->getRefractionLevel();
//				temp3.z *= p->getRefractionLevel();
//				refractRay.x = temp3.x - temp2.x;
//				refractRay.y = temp3.y - temp2.y;
//				refractRay.z = temp3.z - temp2.z;
//				refractRay = normalizeVector(refractRay);
//				refractColor = calcCrossColor(refractRay,crossPos,iterNum+1, 1, false);
				if (1 > sqr(p->getRefractionLevel()*calcVectorLen(temp3)))
				{
					temp2 = normalizeVector(temp2);
					refractionScaler = sqrt(1 - sqr(p->getRefractionLevel()*calcVectorLen(temp3)));
					temp2.x *= refractionScaler;
					temp2.y *= refractionScaler;
					temp2.z *= refractionScaler;
					temp3.x *= p->getRefractionLevel();
					temp3.y *= p->getRefractionLevel();
					temp3.z *= p->getRefractionLevel();
					refractRay.x = temp3.x - temp2.x;
					refractRay.y = temp3.y - temp2.y;
					refractRay.z = temp3.z - temp2.z;
					refractRay = normalizeVector(refractRay);
					refractColor = calcCrossColor(refractRay, crossPos, iterNum+1, p->getRefractionLevel(), true);
				}
				else
				{
					fullReflection = true;
				}
			}
			if (refractColor.r>p->getColor().r) refractColor.r=p->getColor().r;
			if (refractColor.g>p->getColor().g) refractColor.g=p->getColor().g;
			if (refractColor.b>p->getColor().b) refractColor.b=p->getColor().b;
		}

//		if (refractive)
//		{
//			result.r =static_cast<unsigned short>((1-p->getTransparency())*p->getColor().r*double(m_globalLight.r)/255.0f);
//			result.g =static_cast<unsigned short>((1-p->getTransparency())*p->getColor().g*double(m_globalLight.g)/255.0f);
//			result.b =static_cast<unsigned short>((1-p->getTransparency())*p->getColor().b*double(m_globalLight.b)/255.0f);
//		}
//		else
//		{
                        result.r =static_cast<unsigned short>((1-p->getTransparency())*reflectColor.r*p->getSpecularLevel()*double(m_globalLight.r)/255.0f);
                        result.g =static_cast<unsigned short>((1-p->getTransparency())*reflectColor.g*p->getSpecularLevel()*double(m_globalLight.g)/255.0f);
                        result.b =static_cast<unsigned short>((1-p->getTransparency())*reflectColor.b*p->getSpecularLevel()*double(m_globalLight.b)/255.0f);
                        result.r +=static_cast<unsigned short>((1-p->getTransparency())*p->getColor().r*(1-p->getSpecularLevel())*double(m_globalLight.r)/255.0f);
                        result.g +=static_cast<unsigned short>((1-p->getTransparency())*p->getColor().g*(1-p->getSpecularLevel())*double(m_globalLight.g)/255.0f);
                        result.b +=static_cast<unsigned short>((1-p->getTransparency())*p->getColor().b*(1-p->getSpecularLevel())*double(m_globalLight.b)/255.0f);
//		}
//		result.r +=static_cast<unsigned short>(p->getTransparency()*refractColor.r*double(m_globalLight.r)/255.0f);
//		result.g +=static_cast<unsigned short>(p->getTransparency()*refractColor.g*double(m_globalLight.g)/255.0f);
//		result.b +=static_cast<unsigned short>(p->getTransparency()*refractColor.b*double(m_globalLight.b)/255.0f);
//		result.r +=static_cast<unsigned short>(p->getTransparency()*refractColor.r);
//		result.g +=static_cast<unsigned short>(p->getTransparency()*refractColor.g);
//		result.b +=static_cast<unsigned short>(p->getTransparency()*refractColor.b);
		if (!fullReflection)
		{ 
                        result.r +=static_cast<unsigned short>(p->getTransparency()*refractColor.r*(1.0-(1.0-p->getTransparency()*(double(m_globalLight.r)/255.0f))));
                        result.g +=static_cast<unsigned short>(p->getTransparency()*refractColor.g*(1.0-(1.0-p->getTransparency()*(double(m_globalLight.g)/255.0f))));
                        result.b +=static_cast<unsigned short>(p->getTransparency()*refractColor.b*(1.0-(1.0-p->getTransparency()*(double(m_globalLight.b)/255.0f))));
		}

		//Входим в цикл по лампочкам, чтобы узнать в тени ли лампочка
		bool isShadow = true;
		for(unsigned int i = 0; i < m_lights.size(); i++){
			XYZ lightRay;
			Primitive * tempP;
			double tempT;
			//Находим направление вектора от точки пересечения до лампочки
			lightRay.x = m_lights[i].pos.x - crossPos.x;
			lightRay.y = m_lights[i].pos.y - crossPos.y;
			lightRay.z = m_lights[i].pos.z - crossPos.z;
			double lightLen = calcVectorLen(lightRay);

			//Если ничто наш вектор не загораживает - объект не в тени
			if((!calculateCross(lightRay, crossPos, &tempP, &tempT, refractive)) || (tempP->getTransparency()))
				isShadow = false;

			//Если пересечение находится за источником света - то обьект всё равно не в тени
			if(tempT>lightLen)
				isShadow = false;

			lightRay = normalizeVector(lightRay);

			XYZ normToRay;
			normToRay.x = norm.x - lightRay.x;
			normToRay.y = norm.y - lightRay.y;
			normToRay.z = norm.z - lightRay.z;

			double normToRayLen = calcVectorLen(normToRay);

			if(!isShadow){
				//Высчитываем множитель полутени = cos угла между нормалью и лучом до источника света
				//Высчитываем по теореме косинусов 
				double lightScaler = pow((2 - sqr(normToRayLen))/2, p->getMetalizing());
				if(lightScaler<0) lightScaler = 0; 

				//Высчитываем интенсивность света
//				if (refractive)
//				{
//					result.r+=unsigned short((double((1-p->getTransparency())*(double)p->getColor().r)/1.0)*(double(m_lights[i].col.r)/255.0f)*lightScaler);
//					result.g+=unsigned short((double((1-p->getTransparency())*(double)p->getColor().g)/1.0)*(double(m_lights[i].col.g)/255.0f)*lightScaler);
//					result.b+=unsigned short((double((1-p->getTransparency())*(double)p->getColor().b)/1.0)*(double(m_lights[i].col.b)/255.0f)*lightScaler);
//				}
//				else
//				{
                                        result.r+=static_cast<unsigned short>((double((1-p->getTransparency())*(double)reflectColor.r*p->getSpecularLevel()))*(double(m_lights[i].col.r)/255.0f)*lightScaler);
                                        result.g+=static_cast<unsigned short>((double((1-p->getTransparency())*(double)reflectColor.g*p->getSpecularLevel()))*(double(m_lights[i].col.g)/255.0f)*lightScaler);
                                        result.b+=static_cast<unsigned short>((double((1-p->getTransparency())*(double)reflectColor.b*p->getSpecularLevel()))*(double(m_lights[i].col.b)/255.0f)*lightScaler);
                                        result.r+=static_cast<unsigned short>((double((1-p->getTransparency())*(double)p->getColor().r*(1-p->getSpecularLevel()))/1.0)*(double(m_lights[i].col.r)/255.0f)*lightScaler);
                                        result.g+=static_cast<unsigned short>((double((1-p->getTransparency())*(double)p->getColor().g*(1-p->getSpecularLevel()))/1.0)*(double(m_lights[i].col.g)/255.0f)*lightScaler);
                                        result.b+=static_cast<unsigned short>((double((1-p->getTransparency())*(double)p->getColor().b*(1-p->getSpecularLevel()))/1.0)*(double(m_lights[i].col.b)/255.0f)*lightScaler);
//				}
//				result.r+=unsigned short((double(p->getTransparency()*(double)refractColor.r))*(double(m_lights[i].col.r)/255.0f)*lightScaler);
//				result.g+=unsigned short((double(p->getTransparency()*(double)refractColor.g))*(double(m_lights[i].col.g)/255.0f)*lightScaler);
//				result.b+=unsigned short((double(p->getTransparency()*(double)refractColor.b))*(double(m_lights[i].col.b)/255.0f)*lightScaler);
				if (!fullReflection)
				{
                                        result.r+=static_cast<unsigned short>((double(p->getTransparency()*(double)refractColor.r))*(1.0-(1.0-p->getTransparency()*((double(m_lights[i].col.r)/255.0f)*lightScaler))));
                                        result.g+=static_cast<unsigned short>((double(p->getTransparency()*(double)refractColor.g))*(1.0-(1.0-p->getTransparency()*((double(m_lights[i].col.g)/255.0f)*lightScaler))));
                                        result.b+=static_cast<unsigned short>((double(p->getTransparency()*(double)refractColor.b))*(1.0-(1.0-p->getTransparency()*((double(m_lights[i].col.b)/255.0f)*lightScaler))));
				}
				if(result.r>255) result.r = 255;
				if(result.g>255) result.g = 255;
				if(result.b>255) result.b = 255;
			} 
		}
	}
	return result;
}

Color Raytracer::getPixelColor(int x, int y)
{	
	Color result;
	result.r = 0;
	result.g = 0;
	result.b = 0;
	double r = 0, g = 0, b = 0;
	
	for(int i = 0; i < ANTIALIAS_AM; i++){
		for(int j = 0; j < ANTIALIAS_AM; j++){
			XYZ pixelPos;
			pixelPos.x = m_planeZero.x + m_planeOX.x * (m_pixelSizeX * x + m_pixelSizeX*(i/double(ANTIALIAS_AM))) 
				+ m_planeOY.x * (m_pixelSizeY * y + m_pixelSizeY*(j/double(ANTIALIAS_AM)));
			pixelPos.y = m_planeZero.y + m_planeOX.y * (m_pixelSizeX * x + m_pixelSizeX*(i/double(ANTIALIAS_AM))) 
				+ m_planeOY.y * (m_pixelSizeY * y + m_pixelSizeY*(j/double(ANTIALIAS_AM)));
			pixelPos.z = m_planeZero.z + m_planeOX.z * (m_pixelSizeX * x + m_pixelSizeX*(i/double(ANTIALIAS_AM))) 
				+ m_planeOY.z * (m_pixelSizeY * y + m_pixelSizeY*(j/double(ANTIALIAS_AM)));

			XYZ ray;
			ray.x = pixelPos.x - m_eyePos.x;
			ray.y = pixelPos.y - m_eyePos.y;
			ray.z = pixelPos.z - m_eyePos.z;
			ray = normalizeVector(ray);
			Color c = calcCrossColor(ray, m_eyePos, 0, 1, false);
			r += double(c.r)/sqr(ANTIALIAS_AM);
			g += double(c.g)/sqr(ANTIALIAS_AM);
			b += double(c.b)/sqr(ANTIALIAS_AM);
		}
	}
        result.r =static_cast<unsigned short>(r);
        result.g =static_cast<unsigned short>(g);
        result.b =static_cast<unsigned short>(b);
	return result;
}

Raytracer::~Raytracer(){
	for(unsigned int i = 0; i<m_primitives.size(); i++)
		delete m_primitives[i];
}
