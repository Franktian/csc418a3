/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements scene_object.h

***********************************************************/

#include <cmath>
#include <iostream>
#include "scene_object.h"

bool UnitSquare::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// TODO: implement intersection code for UnitSquare, which is
	// defined on the xy-plane, with vertices (0.5, 0.5, 0), 
	// (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
	// (0, 0, 1).
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.

	Ray3D ro;
	ro.origin = worldToModel * ray.origin;
	ro.dir = worldToModel * ray.dir;
	float t = -(ro.origin[2] / ro.dir[2]);

	if (t <= 0) {
		return false;
	}

	float check_x = ro.origin[0] + t * ro.dir[0];
	float check_y = ro.origin[1] + t * ro.dir[1];
	Point3D io(check_x, check_y, 0.0);
	Vector3D norm(0.0, 0.0, 1.0);

	if (check_x <= 0.5 && check_x >= -0.5 && check_y <= 0.5 && check_y >= -0.5) {
		if (ray.intersection.none || t < ray.intersection.t_value) {
			ray.intersection.t_value = t;
			ray.intersection.point = modelToWorld * io;
			ray.intersection.normal = worldToModel.transpose() * norm;
			ray.intersection.normal.normalize();
			ray.intersection.none = false;
			return true;
		}
	}


	return false;
}

bool UnitSphere::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// TODO: implement intersection code for UnitSphere, which is centred 
	// on the origin.  
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.
	
	return false;
}

