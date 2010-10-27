#include<iostream>
#include<fstream>
#include<time.h>
#include"Raytracer.h"
#include"libbmp/EasyBMP.h"
#include "Constructor.h"

void wait()
{
	std::cin.sync();
	std::cin.get();
}

int main()
{
	//loading config
	char path[256];
	char primeName[256];
	std::cout<<"Enter path to scene file : ";
	std::cin>>path;
	std::ifstream input(path);
	if(input.fail())
	{
		std::cout << "No configuration found";
		wait();
		return 0;
	}

	//creating enviroment
	XYZ eye;
	XYZ planePos;
	XYZ ox, oy;
	Color bgColor;
	Color globLight;
	double planeSizeX, planeSizeY;
	int planeResX, planeResY;

	input>>eye.x>>eye.y>>eye.z;
	input>>planePos.x>>planePos.y>>planePos.z;
	input>>ox.x>>ox.y>>ox.z;
	input>>oy.x>>oy.y>>oy.z;
	input>>planeSizeX>>planeSizeY>>planeResX>>planeResY;
	input>>bgColor.r>>bgColor.g>>bgColor.b;
	input>>globLight.r>>globLight.g>>globLight.b;

	Raytracer rt(eye, planePos, ox, oy, planeSizeX, planeSizeY, planeResX, planeResY);
	rt.SetBackground(bgColor);
	rt.SetGlobalLight(globLight);

	int spheresAm;
	input>>spheresAm;
	int primitivesAm = spheresAm;
	bool texturing = false;
	int oldPaper = 0;

	for(int i = 0; i<spheresAm; i++){
		input>>primeName;
		if (!strcmp(primeName,"SPHERE"))
		{
			Sphere * sp = new Sphere();
			sp->loadSphere(&input);
			rt.AddPrimitive(sp);
			sp->emoTexturing = texturing;
			texturing = false;
		}
		if (!strcmp(primeName,"POLYGON"))
		{
			Polygon * pg = new Polygon();
			pg->loadPolygon(&input);
			rt.AddPrimitive(pg);
			pg->emoTexturing = texturing;
			texturing = false;
		}
//		if (!strcmp(primeName,"EMOPOLYGON"))
//		{
//			EmoPolygon * pg = new EmoPolygon();
//			pg->loadPolygon(&input);
//			rt.AddPrimitive(pg);
//			pg->emoTexturing = texturing;
//			texturing = false;
//		}
		if (!strcmp(primeName,"EMOTEXTURING"))
		{
			i--;
			texturing = true;
		}
		if (!strcmp(primeName,"OLDPAPER"))
		{
			input>>oldPaper;
		}
		if (!strcmp(primeName,"BOX"))
		{
			XYZ c, r, d, f;
			Color col;
			double SpLevel, MetLevel, TransLevel, RefractLevel;
			Polygon * pg[12];
			primitivesAm += 11;
			for (int j = 0; j < 12; j++)
				pg[j] = new Polygon();
			input >> c.x >> c.y >> c.z >> r.x >> r.y >> r.z >> d.x >> d.y >> d.z >> f.x >> f.y >> f.z
				>> col.r >> col.g >> col.b >> SpLevel >> MetLevel>> TransLevel>> RefractLevel;
			Create_Box(pg, c, r, d, f, col, SpLevel, MetLevel, TransLevel, RefractLevel);
			for (int j = 0; j < 12; j++)
			{
				pg[j]->emoTexturing = texturing;
				rt.AddPrimitive (pg[j]);
			}
			texturing = false;
		}
		if (!strcmp(primeName,"CUBE"))
		{
			XYZ c, r, d, f;
			int h, l, w;
			double alf, bet;
			Color col;
			double SpLevel, MetLevel, TransLevel, RefractLevel;
			Polygon * pg[12];
			primitivesAm += 11;
			for (int j = 0; j < 12; j++)
				pg[j] = new Polygon();
			input >> c.x >> c.y >> c.z >> h >> l >> w >> alf >> bet >> col.r >> col.g >> col.b >> SpLevel >> MetLevel>> TransLevel>> RefractLevel;
			alf = alf / 180 * 3.14;
			bet = bet / 180 * 3.14;
			r.x = c.x - w * cos(alf);
			r.y = c.y;
			r.z = c.z + w * sin(alf);
			f.x = c.x + l * sin(alf) * cos(bet);
			f.y = c.y + l * sin(bet);
			f.z = c.z + l * cos(alf) * cos(bet);
			d.x = c.x + h * sin(alf) * sin(bet);
			d.y = c.y - h * cos(bet);
			d.z = c.z + h * cos(alf) * sin(bet);
			Create_Box(pg, c, r, d, f, col, SpLevel, MetLevel, TransLevel, RefractLevel);
			for (int j = 0; j < 12; j++)
			{
				pg[j]->emoTexturing = texturing;
				rt.AddPrimitive (pg[j]);
			}
			texturing = false;
		}
		if (!strcmp(primeName,"PYRAMIDE"))
		{
			XYZ p1, p2, p3, p4;
			Color col;
			double SpLevel, MetLevel, TransLevel, RefractLevel;
			Polygon * pg[4];
			primitivesAm += 3;
			for (int j = 0; j < 4; j++)
				pg[j] = new Polygon();
			input >> p1.x >> p1.y >> p1.z >> p2.x >> p2.y >> p2.z >> p3.x >> p3.y >> p3.z >> p4.x >> p4.y >> p4.z
				>> col.r >> col.g >> col.b >> SpLevel >> MetLevel>> TransLevel>> RefractLevel;
			Create_Pyramid(pg, p1, p2, p3, p4, col, SpLevel, MetLevel, TransLevel, RefractLevel);
			for (int j = 0; j < 4; j++)
			{
				pg[j]->emoTexturing = texturing;
				rt.AddPrimitive (pg[j]);
			}
			texturing = false;
		}
		/*if (!strcmp(primeName,"CONUS"))
		{
			XYZ top, bot;
			int gr;
			Color col;
			double SpLevel;
			Polygon * pg[2*col - 2];
			for (int j = 0; j < (2*col - 2); j++)
				pg[j] = new Polygon();
			input >> top.x >> top.y >> top.z >> bot.x >> bot.y >> bot.z >> gr >> r >> col.r >> col.g >> col.b >> SpLevel;
			Create_Conus(pg, gr, top, bot, col, SpLevel);
			for (int j = 0; j < (2*col - 2); j++)
				rt.AddPrimitive (pg[j]);
		}*/
	}

	spheresAm = primitivesAm;

	int lightsAm;
	input>>lightsAm;

	for(int i = 0; i<lightsAm; i++){
		Light l;
		input>>l.pos.x>>l.pos.y>>l.pos.z>>l.col.r>>l.col.g>>l.col.b;
		rt.AddLight(l);
	}

	BMP image;
	image.SetSize(planeResX, planeResY);
	int percentsLast = 0;
	int percentsDone = 0;
	for(int i = 0; i<planeResX; i++){
		for(int j = 0; j<planeResY; j++){
			Color c = rt.getPixelColor(i, j);
			image(i, j)->Red = ebmpBYTE(c.r);
			image(i, j)->Green = ebmpBYTE(c.g);
			image(i, j)->Blue = ebmpBYTE(c.b);
			percentsDone = int(double(i*planeResY + j)/double(planeResX*planeResY) * 100);
			if(percentsDone != percentsLast){
				percentsLast = percentsDone;
				std::cout<<"\r"<<percentsDone<<"% done. It took " << double(clock())/CLOCKS_PER_SEC/*tim*/ << " seconds.                  ";
			}
		}
	}

	std::cout<<"\rSaving image...";
	image.WriteToFile("out.bmp");

	std::cout<<"\rRaytracing done! "<< "It took " << float(clock())/CLOCKS_PER_SEC << " seconds. " << "Press enter to continue.\n";

	wait();
}
