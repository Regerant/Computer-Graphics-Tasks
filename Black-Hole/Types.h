#pragma once

#include "glm/glm.hpp"
#include "vector"

struct SRay 
{
	glm::vec3 m_start;
	glm::vec3 m_dir;
};

struct SCamera 
{
	SCamera() :rs(512, 512), eye(0., -16., 1.), v(0., 0., 1.), a(1.),
		m_pixels(rs.x*rs.y), N(1.0) 
		{
			calc();
		}
	
	void cset(glm::uvec2 r, glm::vec3 p, glm::vec3 _v, float _a) 
	{
		rs = r; eye = p; v = _v; a = _a; N = 1.;
		m_pixels.resize(rs.x*rs.y);
		calc();
	}

	glm::vec3 pos(glm::uvec2 pix) 
	{
		glm::vec3 t;
		float uc, vr;
		uc = W*((pix.x + 0.5) / rs.x - 0.5);
		vr = H*((pix.y + 0.5) / rs.y - 0.5);
		t = eye - N*n + uc*u + vr*v;
		return t;
	}
	
	glm::vec3 dir(glm::uvec2 pix) 
	{
		glm::vec3 t = pos(pix) - eye;
		return glm::normalize(t);
	}

	glm::uvec2 rs;       // Image resolution: w, h
	glm::vec3 eye, v;     // Camera position and orientation to up
	float a;             // Tan view angles to up
	glm::vec3 u, n;       // Orthonormal basis
	float N, H, W;         // parameters
	std::vector<glm::vec3> m_pixels;  // Pixel array

private:
	void calc() 
	{
		n = glm::normalize(eye);
		u = glm::normalize(glm::cross(v, n));
		v = glm::normalize(glm::cross(n, u));
		a = atan(a);
		H = 2 * N*tan(a / 2);
		W = H*(rs.x / rs.y);
	}
};

class CScene 
{
public:
	CScene() :iter(250), step(0.15), ra(2.0), Ra(8.0) {}
	CScene(int i, float s, float r, float R) :iter(i), step(s), ra(r), Ra(R) {}
	void scset(int i, float s, float r, float R) { iter = i; step = s; ra = r; Ra = R; }
	int iter;
	float step, ra, Ra;
};
