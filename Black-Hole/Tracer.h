#pragma once

#include "glm/glm.hpp"
#include "Types.h"


#include <string>
#include "Image.h"

class CTracer
{
public:
	CTracer() { Load(); }
	CTracer(const SCamera &c, const CScene &s) :m_camera(c), m_Scene(s) { Load(); }
	void tset(const SCamera &c, const CScene &s) {
		m_camera = c; m_Scene = s; //Load();
	}
	SRay MakeRay(glm::uvec2 pixelPos);  // Create ray for specified pixel
	glm::vec3 TraceRay(SRay ray);       // Trace ray, compute its color
	void RenderImage();
	

	void SaveImageToFile(std::string fileName);

	//Image LoadImageFromFile(std::string fileName);
	
	glm::vec4 lookupDt(float, float);
	glm::vec3 lookupSt(float, float);

	SCamera m_camera;
	CScene  m_Scene;
	Image ImgDisk;
	Image ImgScr;
	Image ImgStars;

private:
	void Load() {
		Image_load(&ImgDisk, "data/disk_sp580.png");
		if (!ImgDisk.data){ fprintf(stderr, "not data/fd.jpg\n");}
		Image_load(&ImgStars, "data/stars_sp445.png");
		if (!ImgStars.data){ fprintf(stderr, "not data/stars_sp445.png\n");}
	
	}

	
};
