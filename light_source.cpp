/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

void PointLight::shade( Ray3D& ray ) {
	// TODO: implement this function to fill in values for ray.col 
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp colour values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray 
	// is available.  So be sure that traverseScene() is called on the ray 
	// before this function.

	Vector3D normal = ray.intersection.normal;
	normal.normalize();

	Vector3D light_direction = _pos - ray.intersection.point;
	light_direction.normalize();

	Vector3D view_direction = -ray.dir;
	view_direction.normalize();

	Vector3D reflection_direction = 2 * light_direction.dot(normal) * normal - light_direction;
	reflection_direction.normalize();


	Colour ambient = ray.intersection.mat->ambient * _col_ambient;

	Colour diffuse = ray.intersection.mat->diffuse * (std::max(0.0, normal.dot(light_direction)) * _col_diffuse);

	Colour specular = ray.intersection.mat->specular * (std::max(0.0, pow(view_direction.dot(reflection_direction), (*ray.intersection.mat).specular_exp)) * _col_specular);

	ray.col = ambient + diffuse + specular; // phone

	//ray.col = ambient + diffuse; // diffuse

	//ray.col = ray.intersection.mat->diffuse; // scene signature

	ray.col.clamp();

}
