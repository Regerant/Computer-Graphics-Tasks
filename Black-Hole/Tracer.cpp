#include "Tracer.h"
#include <cmath>
using namespace glm;

SRay CTracer::MakeRay (uvec2 pixelPos) 
{
	SRay r;
	//---------------------------------
	r.m_start = m_camera.pos(pixelPos);
	r.m_dir = m_camera.dir(pixelPos);
	//---------------------------------
	return r;
}

vec4 CTracer::lookupDt(float x, float y) //измененный кор. диска
{ 
    int W, H;
    
	int i, j;
	
	W = ImgDisk.width, j; //W считываем с ImgDisk
	H = ImgDisk.height, i;
	
	int chn = ImgDisk.channels; //кол-во каналов взяли из ImgDisk 

	x *= W; 
	y *= H;
	
	j = int(x); 
	i = int(y);

    if(j + 1 - x < x - j && j + 1 < W) //округление
    { 
		j++;
	}
	
	if(i + 1 - y < y - i && i + 1 < H)
	{
		i++;
	}
	
	auto ptr = ImgDisk.data; //ptr-указатель на data 
  
	ptr += chn * (W*i + j); 

	unsigned char r = ptr[0];
	unsigned char g = ptr[1];
	unsigned char b = ptr[2];
	unsigned char al = ptr[3]; //прозрачность
	
	float r_2, g_2, b_2, al_2; 

	r_2 = (float)r / 255;
	g_2 = (float)g / 255;
	b_2 = (float)b / 255;
	al_2 = (float)al / 255;

	return vec4(r_2, g_2, b_2, al_2);
}


vec3 CTracer::TraceRay(SRay ray) 
{
	vec3 color(0, 0, 1);
	//-------------------------------------------------------------------------------------------
	int iter = m_Scene.iter;
	float tay = m_Scene.step;
	float ra = m_Scene.ra;
	float Ra = m_Scene.Ra;
	
	vec3 r = ray.m_start; //нач положение
	vec3 v = ray.m_dir; // вектор-направления
	vec3 a, r0;
	float l, am; //df - движение фотона
	float tay2 = tay * tay * 0.5;

	for(int i = 0; i < iter; ++i) 
	{
		r0 = r;
		l = length(r);
		am = 1 / (2  * l * l * l);
		a = -r * am;
		r += v * tay + a * tay2;
		v += a * tay;
		v = normalize(v);

		if(length(r) <= 1)	
		{
			return vec3(0, 0, 0);
		}

		if(r0.z * r.z <= 0) //  пересечение с плоскостью z = 0 
		{  
			vec3 dir = r - r0;
			dir = normalize(dir);
			float t = -r0.z / dir.z;
			vec3 P = r0 + dir  * t; // P = l * t + r0 - формула
			l = length(P);
	
			if(l >= ra && l <= Ra) 
			{
				float x = P.x;
				float y = P.y;
			
				x = (x + Ra) / (2 * Ra);
				y = (y + Ra) / (2 * Ra);
			
				vec4 color4 = lookupDt(x,y);
			
				if(color4[3] > 0)
				{
					color = vec3(color4);
			
					return color;
				}
			}
		}
		//фотон попал за круг
		if((i == iter - 1) || (i > iter / 2 && length(a * tay) < 0.005 && length(r) > Ra))
		{
			float phi = atan2f(v.x, v.y), teta = asin(v.z) + 3.14159265358979323846 / 2;
			phi /= (2 * 3.14159265358979323846);
			teta /= M_PI;
		
			return lookupSt(phi, teta);
		}
	}
	//----------------------------------------------------------------------------------------------------
	return color;
}


// вектор-скорость передать v
vec3 CTracer::lookupSt(float x,float y)// Размещаем вектор на изображении
{  
	int W = ImgStars.width, j;
	int H = ImgStars.height, i;
	int chn = ImgStars.channels;

	x *= W;
	y *= H;
	
	j = int(x);
	i = int(y);

	if(j + 1 - x < x - j && j + 1 < W)
	{
		j++;
	}
    
    if(i + 1 - y < y - i && i + 1 < H)
    {
		i++;
	}
	
	auto ptr = ImgStars.data;
	
	ptr += chn * (W * i + j);
	
	unsigned char r = ptr[0];
	unsigned char g = ptr[1];
	unsigned char b = ptr[2];
	
	//форматирование
	float r1 = (float)r / 255;
	float g1 = (float)g / 255;
	float b1 = (float)b / 255;

	return vec3(r1, g1, b1);
}


void CTracer::RenderImage() 
{
	int xRes = m_camera.rs.x;
	int yRes = m_camera.rs.y;

	for(int i = 0; i < yRes; ++i)
	{
		for(int j = 0; j < xRes; ++j)
		{
			SRay ray = MakeRay(uvec2(j, i));
			m_camera.m_pixels[i * xRes + j] = TraceRay(ray); 
			//узнали куда попал фотон, вернули соотв. пиксель и записали в камеру
		}
	}
}

void CTracer::SaveImageToFile(std::string fileName)
{
	Image img; //обьект
	
	int w = m_camera.rs.x;
	int h = m_camera.rs.y;
	int chn = 3;	
	
	Image_create(&img, w, h, chn, true); 
	int pitch = w * chn; 
	unsigned char* d = img.data;
	
	int imgDispl = 0; //смешение по образу
	int pxlDispl = 0; //смещение по камере 
	
	for(int i = 0; i < h; ++i)
	{
		for(int j = 0; j < w; ++j)
		{
			vec3 color = m_camera.m_pixels[pxlDispl + j]; //взяли 1 пиксель из камеры
	
			d[imgDispl + j * chn] = clamp(color.r, 0.0f, 1.0f) * 255.0f;
			d[imgDispl + j * chn + 1] = clamp(color.g, 0.0f, 1.0f) * 255.0f;
			d[imgDispl + j * chn + 2] = clamp(color.b, 0.0f, 1.0f) * 255.0f;
		}
    
		imgDispl += pitch;
		pxlDispl += w;
	}
	
	Image_save(&img, fileName.c_str());
	Image_free(&img);
}
	
