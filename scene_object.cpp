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

bool Cylinder::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// Intersection for cylinder
	// height = 1
	// radius = 1
	// top circle (0, 0, 0.5)
	// bottom circle (0, 0, -0.5)

	Point3D ray_origin = worldToModel * ray.origin;
	Vector3D ray_dir = worldToModel * ray.dir;
	Point3D sphere_origin(0, 0, 0);

	// Quadratic formula to find intersection
	double a, b, c, d;
	a = ray_dir[0] * ray_dir[0] + ray_dir[1] * ray_dir[1];
	b = ray_origin[0] * ray_dir[0] + ray_origin[1] * ray_dir[1];
	c = ray_origin[0] * ray_origin[0] + ray_origin[1] * ray_origin[1] - 1;
	d = b * b - a * c;

	Point3D intersection;
	Vector3D normal_1, normal_2;

	if (d < 0) {
		return false;
	}

	double t1, t2, s1, s2;

	s1 = (-b  + sqrt(d)) / a;
	s2 = (-b  - sqrt(d)) / a;
	if (s1 < 0 && s2 < 0) {
		return false;
	} else if (s1 > 0 && s2 < 0) {
		t2 = s1;
	} else {
		t2 = s2;
	}

	s1 = (-0.5 - ray_origin[2]) / ray_dir[2];
	s2 = (0.5 - ray_origin[2]) / ray_dir[2];
	if (s1 < s2){
		t1 = s1;
		normal_1 = Vector3D(0, 0, -1);
	} else {
		t1 = s2;
		normal_1 = Vector3D(0, 0, 1);
	}

	if (t1* t1 < 0.001){
		return false;
	}

	// Intersection calculation
	intersection = ray_origin + t1 * ray_dir;
	if (intersection[0]*intersection[0] + intersection[1] * intersection[1] <= 1)
	{
		if (!ray.intersection.none < ray.intersection.t_value){
			ray.intersection.point = intersection;
			ray.intersection.normal = normal_1;
			ray.intersection.t_value = t1;
			ray.intersection.none = false;
			return true;
		}
	}

	if (t2 * t2 < 0.001) {
		return false;
	}
	intersection = ray_origin + t2 * ray_dir;
	normal_2[0] = intersection[0];
	normal_2[1] = intersection[1];
	normal_2[2] = 0;
	normal_2.normalize();

	if (intersection[2] < 0.5 && intersection[2] > -0.5)
	{
		if (ray.intersection.none < ray.intersection.t_value) {
			ray.intersection.point = modelToWorld * intersection;
			ray.intersection.normal = modelToWorld * (normal_2);
			ray.intersection.t_value = t2;
			ray.intersection.none = false;
			return true;
		}
	}

	return false;
}
