/**
 * A quick cross-compilable utility class
 * which defines a bunch of useful static functions
 * that don't really have a place in a single
 * class per se. Not supposed to be instantiated
 */
#pragma once
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <vector>
#include "Vector3.h"
#include "Vector4.h"

namespace Util {

	// Linearly interpolates between 2 doubles, vectors, whatever. 
	double lerp(double t, double p0, double p1);
	Vector3 lerp(double t, Vector3 p0, Vector3 p1);
	Vector4 lerp(double t, Vector4 p0, Vector4 p1);

	// Clamp a number between max & min
	double clamp(double val, double valMin, double valMax); 

	// Set the random seed. If seed is 0, seed with the current time
	void seed(int s);

	// Get a random double between 0.0 and 1.0
	double drand();
	// Get a random double between an arbitrary range (inclusive)
	double drandRange(double low, double high);

	// Returns absolute value of a number
	double abs(double d);

	// Min & Max
	double min(double,double);
	double max(double,double);

	// Utilities for getting values from a row-major std::vector
	// template <typename T>
	// T getItemFromRowMajorVector(std::vector<T>&, int x, int y, int rows, int cols);

	// template <typename T>
	// void setItemFromRowMajorVector(std::vector<T>&, int x, int y, int rows, int cols, T item);
};