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

	Point3D ray_origin = worldToModel * ray.origin;
	Vector3D ray_dir = worldToModel * ray.dir;

	double t;
	t = -(ray_origin[2] / ray_dir[2]);

	if (t <= 0) {
		return false;
	}

	double x_intersect, y_intersect;
	x_intersect = ray_origin[0] + t * ray_dir[0];
	y_intersect = ray_origin[1] + t * ray_dir[1];

	// Intersect
	if (x_intersect <= 0.5 && x_intersect >= -0.5 && y_intersect <= 0.5 && y_intersect >= -0.5) {
		if (ray.intersection.none || t < ray.intersection.t_value) {
			ray.intersection.t_value = t;
			ray.intersection.point = modelToWorld * Point3D(x_intersect, y_intersect, 0);
			ray.intersection.normal = worldToModel.transpose() * Vector3D(0, 0, 1);
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

	Point3D ray_origin = worldToModel * ray.origin;
	Vector3D ray_dir = worldToModel * ray.dir;

	Vector3D center_to_ray = ray_origin - Point3D(0, 0, 0);

	// Quadratic formula to find intersection
	double a, b, c, d;
	a = ray_dir.dot(ray_dir);
	b = ray_dir.dot(center_to_ray);
	c = (center_to_ray).dot(center_to_ray) - 1;
	d = 4 * (b * b - a * c);

	if (d < 0) {
		return false;
	}

	double t, t1, t2;

	t1 = (-(2 * b) + sqrt(d)) / (2 * a);
	t2 = (-(2 * b) - sqrt(d)) / (2 * a);

	if (t1 < 0 && t2 < 0) {
		return false;
	}
	if (t1 > 0 && t2 < 0) {
		t = t1;
	}
	if (t1 > t2 && t2 > 0) {
		t = t2;
	}

	// Calculate intersection
	double x_intersect, y_intersect, z_intersect;
	x_intersect = ray_origin[0] + t * ray_dir[0];
	y_intersect = ray_origin[1] + t * ray_dir[1];
	z_intersect = ray_origin[2] + t * ray_dir[2];

	if (ray.intersection.none || t < ray.intersection.t_value) {
		ray.intersection.t_value = t;
		ray.intersection.point = modelToWorld * Point3D(x_intersect, y_intersect, z_intersect);
		ray.intersection.normal = worldToModel.transpose() * Vector3D(x_intersect, y_intersect, z_intersect);
		ray.intersection.normal.normalize();
		ray.intersection.none = false;
		return true;
	}

	return false;
}

bool UnitCylinder::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// intersection for unit cylinder with height = 1 and top and bottom base circles are at
	// (0,0,0.5), (0,0,-0.5), radius = 1 
	// idea, mathematically intersect cylinder of inifinite height with two planes
	// call these the caps at z =-0.5, 0.5
	Ray3D ro;
	ro.origin = worldToModel*ray.origin;
    ro.dir = worldToModel*ray.dir;
	Point3D sphereOrigin(0,0,0);
	double lambdaStar_1;
	double lambdaStar_2;
	double lambda_1;
	double lambda_2;

	//Use quadratic formula to find intersection 
	double A = ro.dir[0]*ro.dir[0] + ro.dir[1]*ro.dir[1];
	double B = (ro.origin[0]*ro.dir[0] + ro.origin[1] * ro.dir[1]);
	double C = ro.origin[0]*ro.origin[0] + ro.origin[1]*ro.origin[1] - 1;

	//Find discriminant
	double D = B*B-A*C;
	Point3D intersectionPoint;
	Vector3D normal_1;
	Vector3D normal_2;
	Vector3D normal;

	//If the discriminant is negative there is no intersection
	//Else, get the lambda for the side of the cylinder
	if (D<0)
		return false;

	// calculate solutions, and take the minimum (closest) non-negative 
	// number
	lambda_1 = -B/A + sqrt(D) / A;
	lambda_2 = -B/A - sqrt(D) / A;
	if (lambda_1 < 0 && lambda_2 < 0)
		return false;
	else if (lambda_1 > 0 && lambda_2 < 0)
		lambdaStar_2 = lambda_1;
	else
		lambdaStar_2 = lambda_2;

	//See if ray intersetions either of the caps 
	//and take the minimum position value (may intersect both caps)
	lambda_1 = (-0.5-ro.origin[2])/ro.dir[2];
	lambda_2 = (0.5-ro.origin[2])/ro.dir[2];
	if (lambda_1 < lambda_2){
		lambdaStar_1 = lambda_1;
		Point3D normal_temp(0,0,-1);
		normal_1 = normal_temp - sphereOrigin;
		normal_1.normalize();
	}
	else{
		lambdaStar_1 = lambda_2;
		Point3D normal_temp(0,0,1);
		normal_1 = normal_temp - sphereOrigin;
		normal_1.normalize();
	}

	intersectionPoint = ro.origin + lambdaStar_1 * ro.dir;
	if (lambdaStar_1* lambdaStar_1 < 0.001){
		return false;
	}
	//Use the first lambda to check if it intersects with the cap, top or bottom
	if (intersectionPoint[0]*intersectionPoint[0] + intersectionPoint[1] * intersectionPoint[1] <= 1)
	{
		if (!ray.intersection.none > ray.intersection.t_value){
			return false;
		}
		ray.intersection.point = intersectionPoint;
		ray.intersection.normal = normal_1;
		ray.intersection.t_value = lambdaStar_1;
		ray.intersection.none = false;
		return true;
	}


	 //if not intersected with the caps, use the second lamdba to check 
	 //if intersects with the side
	intersectionPoint = ro.origin + lambdaStar_2 * ro.dir;
	if (lambdaStar_2 * lambdaStar_2 < 0.001)
		return false;
	
	normal_2[0] = intersectionPoint[0];
	normal_2[1] = intersectionPoint[1];
	normal_2[2] = 0;
	normal_2.normalize();



	if (intersectionPoint[2] < 0.5 && intersectionPoint[2] > -0.5)
	{
		if (!ray.intersection.none > ray.intersection.t_value)
			return false;
		
		ray.intersection.point = modelToWorld * intersectionPoint;
		Point3D normalll;
		normalll[0] = intersectionPoint[0];
		normalll[1] = intersectionPoint[1];
		normalll[2] = 0;
		ray.intersection.normal = modelToWorld * (normalll - sphereOrigin);
		ray.intersection.t_value = lambdaStar_2;
		ray.intersection.none = false;
		return true;
	}
	else
		return false;

}
