#pragma once
#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
					GLfloat aIntensity, GLfloat dIntensity,
					GLfloat xDir, GLfloat yDir, GLfloat zDir);
	// Se agregan para manejar el punto del sol en el proyecto
	void SetDirection(glm::vec3 dir); 
	void SetIntensity(GLfloat aIntensity, GLfloat dIntensity);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientcolorLocation,
		GLfloat diffuseIntensityLocation, GLfloat directionLocation);

	~DirectionalLight();

private:
	glm::vec3 direction;
};

