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

	Vector3D N = ray.intersection.normal;
	N.normalize();
	Vector3D L = _pos - ray.intersection.point;
	L.normalize();
	Vector3D V = -ray.dir;
	V.normalize();
	Vector3D R = 2.*(L.dot(N) * N) - L;
	R.normalize();

	Colour Ia = (*ray.intersection.mat).ambient * _col_ambient;

	Colour Id = (*ray.intersection.mat).diffuse * (std::max(0.0, N.dot(L)) * _col_diffuse);

	Colour Is = (*ray.intersection.mat).specular * (std::max(0.0, pow(V.dot(R), (*ray.intersection.mat).specular_exp)) * _col_specular);

	ray.col = Ia + Id + Is;

	ray.col.clamp();

}
