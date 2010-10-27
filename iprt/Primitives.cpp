#include"Primitives.h"

void Sphere::loadSphere(std::ifstream * input){
	(*input)>>m_pos.x>>m_pos.y>>m_pos.z>>m_radius>>m_color.r>>m_color.g>>m_color.b>>m_specularLevel>>m_metalizing>>m_transparency>>m_refractionLevel;
//	m_pos.x -= 2000;
//	m_pos.y -= 2000;
//	m_pos.z -= 2000;
}

void Polygon::loadPolygon(std::ifstream * input){
	(*input)>>m_p1.x>>m_p1.y>>m_p1.z>>m_p2.x>>m_p2.y>>m_p2.z>>m_p3.x>>m_p3.y>>m_p3.z>>m_color.r>>m_color.g>>m_color.b>>m_specularLevel>>m_metalizing>>m_transparency>>m_refractionLevel;
//	m_p1.x -= 2000;
//	m_p1.y -= 2000;
//	m_p1.z -= 2000;
//	m_p2.x -= 2000;
//	m_p2.y -= 2000;
//	m_p2.z -= 2000;
//	m_p3.x -= 2000;
//	m_p3.y -= 2000;
//	m_p3.z -= 2000;
}

void Polygon::create_Polygon(XYZ p1, XYZ p2, XYZ p3, Color col, double specularLevel, double MetLevel, double TransLevel, double RefractLevel){
	m_p1 = p1;
	m_p2 = p2;
	m_p3 = p3;
	m_color = col;
	m_specularLevel = specularLevel;
	m_metalizing = MetLevel;
	m_transparency = TransLevel;
	m_refractionLevel = RefractLevel;
}

double Sphere::calcCross(XYZ ray, XYZ source){
	ray = normalizeVector(ray);

	double lenMin = -1;

	double a, b, c, d;
	double t0, t1;

	a = sqr(ray.x) + sqr(ray.y) + sqr(ray.z);
		
	b = 2*((source.x - m_pos.x) * ray.x +
		(source.y - m_pos.y) * ray.y +
		(source.z - m_pos.z) * ray.z);
		
	c = sqr(source.x - m_pos.x) + 
		sqr(source.y - m_pos.y) +
		sqr(source.z - m_pos.z) -
		sqr(m_radius);

	d = sqr(b) - 4*a*c;
		
	if (d<0)
		return -1;

	t0 = (-b - sqrt(d))/(2*a);
	t1 = (-b + sqrt(d))/(2*a);
	
	if(emoTexturing && t0 > -0.01) lenMin = t0; 
	else if (t0 > -0.01) lenMin = t0;

//	if(emoTexturing && t1 > 0 && (rand()%2)) lenMin = t1;
	if(emoTexturing && t1 > 0 && ((int)(t1-t0)%2)) lenMin = t1;
	else if (t1 < lenMin && t1 > 0.01) lenMin = t1;
	
	return lenMin;
}

double Sphere::calcMaxCross(XYZ ray, XYZ source){
	ray = normalizeVector(ray);

	double lenMin = -1;

	double a, b, c, d;
	double t0, t1;

	a = sqr(ray.x) + sqr(ray.y) + sqr(ray.z);
		
	b = 2*((source.x - m_pos.x) * ray.x +
		(source.y - m_pos.y) * ray.y +
		(source.z - m_pos.z) * ray.z);
		
	c = sqr(source.x - m_pos.x) + 
		sqr(source.y - m_pos.y) +
		sqr(source.z - m_pos.z) -
		sqr(m_radius);

	d = sqr(b) - 4*a*c;
		
	if (d<0)
		return -1;

	t0 = (-b - sqrt(d))/(2*a);
	t1 = (-b + sqrt(d))/(2*a);
	
	if(emoTexturing && t0 > -0.01) lenMin = t0; 
	else if (t0 > -0.01) lenMin = t0;

//	if(emoTexturing && t1 > 0 && (rand()%2)) lenMin = t1;
	if(emoTexturing && t1 > 0 && ((int)(t1-t0)%2)) lenMin = t1;
	else if (t1 > lenMin && t1 > 0.01) lenMin = t1;
	
	return lenMin;
}

double EmoPolygon::calcCross(XYZ ray, XYZ source){
	ray = normalizeVector(ray);

	double a, b, c, t;
	double var0, var1;
	
	// Надеюсь, что временный костыль
	if (!m_p1.x) m_p1.x = 0.0001;
	if (!m_p1.y) m_p1.y = 0.0001;
	if (!m_p1.z) m_p1.z = 0.0001;
	if (!m_p2.x) m_p2.x = 0.0001;
	if (!m_p2.y) m_p2.y = 0.0001;
	if (!m_p2.z) m_p2.z = 0.0001;
	if (!m_p3.x) m_p3.x = 0.0001;
	if (!m_p3.y) m_p3.y = 0.0001;
	if (!m_p3.z) m_p3.z = 0.0001;
	double dz = m_p2.z-m_p1.z;
	if (!dz) dz = 0.0001;
	double dzy = m_p3.z*m_p1.y-m_p2.z*m_p3.y;
	if (!dzy) dzy = 0.0001;
	double znam = m_p3.x*m_p1.y*dz+m_p1.x*dzy;
	if (!znam) znam = 0.0001;

	c = m_p1.x*(m_p3.y-m_p1.y)/znam;
	if (!c) c = 0.0001;
	a = c*(m_p2.z-m_p1.z)/m_p1.x;
	b = (-c*m_p2.z-1)/m_p1.y;

	var1 = a*ray.x + b*ray.y + c*ray.z;
		
	if (!var1)
		return -1;

	var0 = - a*source.x - b*source.y - c*source.z - 1;

	t = var0 / var1;
	
	if (scalarMultiplication(ray,getNormal(m_p1)) > 0)
	{
		t = - t;
	}

    if (t < 0)
		return -1;

	XYZ cross;
	cross.x = source.x + t*ray.x;
	cross.y = source.y + t*ray.y;
	cross.z = source.z + t*ray.z;

	double squareDiff = getSquare(m_p1,m_p2,m_p3) 
		- getSquare(cross,m_p2,m_p3) 
		- getSquare(m_p1,cross,m_p3) 
		- getSquare(m_p1,m_p2,cross);

	if (squareDiff < 0) squareDiff = - squareDiff;

	if (squareDiff > 0.00001 || t < 0.0001) return -1;	

	return t;
}

double Polygon::calcMaxCross(XYZ ray, XYZ source)
{
	return calcCross(ray,source);
}

double Polygon::calcCross(XYZ ray, XYZ source){
	ray = normalizeVector(ray);

	XYZ norm = getNormal(ray);
	if (scalarMultiplication(norm,ray) > 0)
	{
		norm.x = - norm.x;
		norm.y = - norm.y;
		norm.z = - norm.z;
	}
	norm = normalizeVector(norm);

	XYZ vec;
	vec.x = m_p1.x - ray.x - source.x;
	vec.y = m_p1.y - ray.y - source.y;
	vec.z = m_p1.z - ray.z - source.z;

	double d = scalarMultiplication(vec,norm);
	double e = - scalarMultiplication(ray,norm);

	if (!e)
		return -1;

	double t = 1 - d / e;

	XYZ cross;
	cross.x = source.x + t*ray.x;
	cross.y = source.y + t*ray.y;
	cross.z = source.z + t*ray.z;

	if (t < 0)
		return -1;

/*	double squareDiff = getSquare(m_p1,m_p2,m_p3) 
		- getSquare(cross,m_p2,m_p3) 
		- getSquare(m_p1,cross,m_p3) 
		- getSquare(m_p1,m_p2,cross);

	if (squareDiff < 0) squareDiff = - squareDiff;

	if (squareDiff > 0.000001 || (t < 0.00001 && !emoTexturing)) return -1;	
*/
	if (t < 0.00001 && !emoTexturing) return -1;	

	if (fabs(norm.x) >= fabs(norm.y) && fabs(norm.x) >= fabs(norm.z))
	{
		if ((norm.x*((cross.z-m_p1.z)*(m_p2.y-m_p1.y)-(cross.y-m_p1.y)*(m_p2.z-m_p1.z)) <= 0)
			&& (norm.x*((cross.z-m_p2.z)*(m_p3.y-m_p2.y)-(cross.y-m_p2.y)*(m_p3.z-m_p2.z)) <= 0)
			&& (norm.x*((cross.z-m_p3.z)*(m_p1.y-m_p3.y)-(cross.y-m_p3.y)*(m_p1.z-m_p3.z)) <= 0))
			return t;
		if ((norm.x*((cross.z-m_p1.z)*(m_p2.y-m_p1.y)-(cross.y-m_p1.y)*(m_p2.z-m_p1.z)) >= 0)
			&& (norm.x*((cross.z-m_p2.z)*(m_p3.y-m_p2.y)-(cross.y-m_p2.y)*(m_p3.z-m_p2.z)) >= 0)
			&& (norm.x*((cross.z-m_p3.z)*(m_p1.y-m_p3.y)-(cross.y-m_p3.y)*(m_p1.z-m_p3.z)) >= 0))
			return t;
	} 
	else if (fabs(norm.y) >= fabs(norm.x) && fabs(norm.y) >= fabs(norm.z))
	{
		if ((norm.y*((cross.z-m_p1.z)*(m_p2.x-m_p1.x)-(cross.x-m_p1.x)*(m_p2.z-m_p1.z)) <= 0)
			&& (norm.y*((cross.z-m_p2.z)*(m_p3.x-m_p2.x)-(cross.x-m_p2.x)*(m_p3.z-m_p2.z)) <= 0)
			&& (norm.y*((cross.z-m_p3.z)*(m_p1.x-m_p3.x)-(cross.x-m_p3.x)*(m_p1.z-m_p3.z)) <= 0))
			return t;
		if ((norm.y*((cross.z-m_p1.z)*(m_p2.x-m_p1.x)-(cross.x-m_p1.x)*(m_p2.z-m_p1.z)) >= 0)
			&& (norm.y*((cross.z-m_p2.z)*(m_p3.x-m_p2.x)-(cross.x-m_p2.x)*(m_p3.z-m_p2.z)) >= 0)
			&& (norm.y*((cross.z-m_p3.z)*(m_p1.x-m_p3.x)-(cross.x-m_p3.x)*(m_p1.z-m_p3.z)) >= 0))
			return t;
	}
	else
	{
		if ((norm.z*((cross.y-m_p1.y)*(m_p2.x-m_p1.x)-(cross.x-m_p1.x)*(m_p2.y-m_p1.y)) <= 0)
			&& (norm.z*((cross.y-m_p2.y)*(m_p3.x-m_p2.x)-(cross.x-m_p2.x)*(m_p3.y-m_p2.y)) <= 0)
			&& (norm.z*((cross.y-m_p3.y)*(m_p1.x-m_p3.x)-(cross.x-m_p3.x)*(m_p1.y-m_p3.y)) <= 0))
			return t;
		if ((norm.z*((cross.y-m_p1.y)*(m_p2.x-m_p1.x)-(cross.x-m_p1.x)*(m_p2.y-m_p1.y)) >= 0)
			&& (norm.z*((cross.y-m_p2.y)*(m_p3.x-m_p2.x)-(cross.x-m_p2.x)*(m_p3.y-m_p2.y)) >= 0)
			&& (norm.z*((cross.y-m_p3.y)*(m_p1.x-m_p3.x)-(cross.x-m_p3.x)*(m_p1.y-m_p3.y)) >= 0))
			return t;
	}

	return -1;
}

XYZ Sphere::getNormal(XYZ crossPos){
	XYZ norm;
	norm.x = crossPos.x - m_pos.x;
	norm.y = crossPos.y - m_pos.y;
	norm.z = crossPos.z - m_pos.z;
	return norm;
}

XYZ Polygon::getNormal(XYZ crossPos){
	XYZ norm;
/*	// Надеюсь, что временный костыль
	if (!m_p1.x) m_p1.x = 0.0001;
	if (!m_p1.y) m_p1.y = 0.0001;
	if (!m_p1.z) m_p1.z = 0.0001;
	if (!m_p2.x) m_p2.x = 0.0001;
	if (!m_p2.y) m_p2.y = 0.0001;
	if (!m_p2.z) m_p2.z = 0.0001;
	if (!m_p3.x) m_p3.x = 0.0001;
	if (!m_p3.y) m_p3.y = 0.0001;
	if (!m_p3.z) m_p3.z = 0.0001;
	double dz = m_p2.z-m_p1.z;
	if (!dz) dz = 0.0001;
	double dzy = m_p3.z*m_p1.y-m_p2.z*m_p3.y;
	if (!dzy) dzy = 0.0001;
	double znam = m_p3.x*m_p1.y*dz+m_p1.x*dzy;
	if (!znam) znam = 0.0001;

	norm.z = m_p1.x*(m_p3.y-m_p1.y)/znam;
	norm.x = norm.z*(m_p2.z-m_p1.z)/m_p1.x;
	norm.y = (-norm.z*m_p2.z-1)/m_p1.y;*/

	XYZ a, b;

	a.x = m_p2.x - m_p1.x;
	b.x = m_p3.x - m_p1.x;
	a.y = m_p2.y - m_p1.y;
	b.y = m_p3.y - m_p1.y;
	a.z = m_p2.z - m_p1.z;
	b.z = m_p3.z - m_p1.z;

	norm.x = a.y*b.z - a.z*b.y;
	norm.y = a.z*b.x - a.x*b.z;
	norm.z = a.x*b.y - a.y*b.x;

	return norm;
}

XYZ EmoPolygon::getNormal(XYZ crossPos){
	XYZ norm;
	// Надеюсь, что временный костыль
	if (!m_p1.x) m_p1.x = 0.0001;
	if (!m_p1.y) m_p1.y = 0.0001;
	if (!m_p1.z) m_p1.z = 0.0001;
	if (!m_p2.x) m_p2.x = 0.0001;
	if (!m_p2.y) m_p2.y = 0.0001;
	if (!m_p2.z) m_p2.z = 0.0001;
	if (!m_p3.x) m_p3.x = 0.0001;
	if (!m_p3.y) m_p3.y = 0.0001;
	if (!m_p3.z) m_p3.z = 0.0001;
	double dz = m_p2.z-m_p1.z;
	if (!dz) dz = 0.0001;
	double dzy = m_p3.z*m_p1.y-m_p2.z*m_p3.y;
	if (!dzy) dzy = 0.0001;
	double znam = m_p3.x*m_p1.y*dz+m_p1.x*dzy;
	if (!znam) znam = 0.0001;

	norm.z = m_p1.x*(m_p3.y-m_p1.y)/znam;
	norm.x = norm.z*(m_p2.z-m_p1.z)/m_p1.x;
	norm.y = (-norm.z*m_p2.z-1)/m_p1.y;
	return norm;
}