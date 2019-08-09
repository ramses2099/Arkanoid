#pragma once

#include <iostream>
#include <algorithm>
#include <ctime>
#include "Vec2D.h"

using namespace jp;

constexpr float pI{ 3.14159265358979f };

class MathUtils
{
public:
	MathUtils() {};
	~MathUtils() {};
	
	/**
	* Scales value using min and max. This is the inverse of MathUtils.lerp(...).
	* @method norm
	* @static
	* @param  {Number} value The value to be scaled.
	* @param  {Number} min
	* @param  {Number} max
	* @return {Number}       Returns the scaled value.
	 */
	static float norm(float value, float min, float max)
	{
		return (value - min) / (max - min);
	}

	/**
	* Linear interpolate norm from min and max. This is the inverse of BB.MathUtils.norm(...).
	* @method lerp
	* @static
	* @param  {Number} value
	* @param  {Number} min
	* @param  {Number} max
	* @return {Number}       Returns the lerped norm.
	*/
	static float lerp(float value, float min, float max)
	{
		return (max - min) * (value + min);
	}

	/**
	* Constrains value using min and max as the upper and lower bounds.
	* @method clamp
	* @static
	* @param  {Number} value The value to be clamped.
	* @param  {Number} min   The lower limit to clamp value by.
	* @param  {Number} max   The upper limit to clamp value by.
	* @return {Number}       The clamped value.
	*/
	static float clamp(float value, float min, float max)
	{
		return std::max(min, std::min(max, value));
	}

	/**
	* Maps (scales) value between sourceMin and sourceMax to destMin and destMax.
	* @method map
	* @static
	* @param  {Number} value The value to be mapped.
	* @param  {Number} sourceMin
	* @param  {Number} sourceMax
	* @param  {Number} destMin
	* @param  {Number} destMax
	* @return {Number} Returns the mapped value.
	*/
	static float map(float value, float sourceMin, float sourceMax, float destMin, float destMax)
	{
		return lerp(norm(value, sourceMin, sourceMax), destMin, destMax);
	}

	/**
	* Get the distance between two points.
	* @method  dist
	* @static
	* @param  {Number} p1x The x value of the first point.
	* @param  {Number} p1y The y value of the first point.
	* @param  {Number} p2x The x value of the second point.
	* @param  {Number} p2y The y value of the second point.
	* @return {Number} Returns the distance between (p1x, p1y) and (p2x, p2y).
	*/
	static float dist(float p1x, float p1y, float p2x, float p2y)
	{
		return std::sqrt(std::powf((p2x - p1x), 2) + std::powf((p2y - p1y), 2));
	}

	/**
	* Get the angle between two points in radians. For degrees process this
	* return value through BB.MathUtils.radToDegree(...).
	* @method angleBtwn
	* @static
	* @param  {Number} p1x The x value of the first point.
	* @param  {Number} p1y The y value of the first point.
	* @param  {Number} p2x The x value of the second point.
	* @param  {Number} p2y The y value of the second point.
	* @return {Number} Returns the angle between (p1x, p1y) and (p2x, p2y) in
	* radians.
	*/
	static float angleBtw(float p1x, float p1y, float p2x, float p2y)
	{
		return std::atan2f((p2x - p1x), (p2y - p1y));
	}

	/**
	* Translate radians into degrees.
	* @method  radToDeg
	* @static
	* @param  {[type]} radians
	* @return {[type]}         Returns radians in degrees.
	*/
	static float radToDeg(float radians)
	{
		return radians * (180.0f * pI);
	}

	/**
	* Translate degrees into radians.
	* @method  degToRad
	* @static
	* @param  {[type]} degrees
	* @return {[type]}         Returns degrees in radians.
	*/
	static float degToRad(float degrees)
	{
		return degrees * (pI / 180.0f);
	}

	/**
	* Translate from polar coordinates to cartesian coordinates.
	* @method polarToCartesian
	* @static
	* @param  {Number} radius  The straight line distance from the origin.
	* @param  {Number} degrees The angle in degrees measured clockwise from the
	* positive x axis.
	* @return {Vec2D}         An Vec2D of length two where the first element is
	* the x value and the second element is the y value.
	*/
	static Vec2D polarToCartesian(float radius, float degrees)
	{
		return Vec2D((radius * std::cosf(degrees)), (radius * std::sinf(degrees)));

	}

	/**
	* Translate from cartesian to polar coordinates.
	* @method cartesianToPolar
	* @static
	* @param  {Number} x The x coordinate.
	* @param  {Number} y The y coordinate.
	* @return {Vec2D}  An Vec2D of length two where the first element is the
	* polar radius and the second element is the polar angle in degrees
	* measured clockwise from the positive x axis.
	*/
	static Vec2D cartesianToPolar(float x, float y)
	{
		return Vec2D(std::sqrtf((x*x) + (y*y)), std::atan(y / x));
	}
	
	/**
	* return a random int between a min and a max
	* @method randomInt
	* @static
	* @param  {Number} min minimum value ( default to 0 if only one argument is passed )
	* @param  {Number} max maximum value
	* @return {Number}  random integer
	*/
	static int randomInt(int min, int max) 
	{
		srand((unsigned)time(0));		
		return static_cast<int>(std::floor(min + rand() * (max - min + 1)));
	}

	/**
	* return a random float between a min and a max
	* @method randomFloat
	* @static
	* @param  {Number} min minimum value ( default to 0 if only one argument is passed )
	* @param  {Number} max maximum value
	* @return {Number}  random float
	*/
	static float randomFloat(float min, float max)
	{
		srand((unsigned)time(0));
		return static_cast<float>(std::floorf(min + rand() * (max - min + 1)));
	}

};
