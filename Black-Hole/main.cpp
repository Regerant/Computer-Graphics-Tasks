/*
 * конфиг
 * 
 * разрешение
 * позиция камеры
 * обзорка
 * корректировка
 * тетта угол
 * число итераций
 * радиус на вход выход
 * 
 */


#include "Tracer.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	SCamera camera;
	CTracer tracer;
	CScene scene;

	glm::uvec2 rs;
	glm::vec3 eye, v;
  
	float a;
	int iter; //iteration
	float step, ra,Ra;

	if(argc == 2) // There is input file in parameters
	{
		FILE* file = fopen(argv[1], "r");
		
		if(file)
		{
			if(fscanf(file, "%d%d", &rs.x, &rs.y) != 2)
				printf("Invalid config format! Using default parameters.\r\n");

			else if(fscanf(file, "%f%f%f", &eye.x, &eye.y, &eye.z) != 3)
				printf("Invalid config format! Using default parameters.\r\n");

			else if(fscanf(file, "%f%f%f", &v.x, &v.y, &v.z) != 3)
				printf("Invalid config format! Using default parameters.\r\n");

			else if(fscanf(file, "%f%d%f%f%f", &a, &iter, &step, &ra, &Ra) != 5)
				printf("Invalid config format! Using default parameters.\r\n");
		
			else 
			{
				camera.cset(rs, eye, v, a);
				scene.scset(iter, step, ra, Ra);
				tracer.tset(camera, scene);
			}

		fclose(file);
		}
    
    else
       printf ("Invalid config path! Using default parametrs.\r\n");
	} 
	
	else
		printf("No config! Using default parameters.\r\n");

	tracer.RenderImage();
	tracer.SaveImageToFile("Result.png"); 
	
	return 0;
}
     
