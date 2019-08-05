#pragma once

#include <string>
#include <time.h>
#include <random>
#include <cmath>
#include <iostream>


/* Includes - SFML */
#include <SFML/System/Vector2.hpp>

/* Defines */
#define PI 3.14159265358979323846F

namespace jp {

	class Vec2D
	{

	public:
		//fields
		float x, y;
		//constructor
		Vec2D();
		Vec2D(float x, float y);
		Vec2D(const Vec2D& v);
		Vec2D(sf::Vector2f v);
		~Vec2D();
		//constructor

		//set
		void set(Vec2D & v);
		void set(sf::Vector2f& v);
		void set(float x, float y);
		void zero();
		Vec2D clone();
		//set


		//operation

		void add(Vec2D & v);
		void sub(Vec2D & v);
		void div(float scale);
		void mult(float scale);

		float length();
		void setLength(float scale);
		void normalize();
		float getAngle();
		void setAngle(float scale);
		float getAngleDeg();
		void setAngleDeg(float scale);
		void rotateBy(float scale);

		float dot(Vec2D& v);
		float cross(Vec2D& v);
		float dist(Vec2D& v);
		void limit(float lim);

		const std::string to_str();
		const sf::Vector2f sf();

	    //operation

		//operators

		Vec2D operator+(const Vec2D& v) const;
		Vec2D operator-(const Vec2D& v) const;
		Vec2D operator*(const float& scale) const;
		
		//dot produt
		float operator*(const Vec2D& v) const;

		//cross product
		float operator^(const Vec2D& v) const;

		Vec2D operator/(const float& scale) const;
		Vec2D operator/(const Vec2D& v) const;


		bool operator==(const Vec2D & v) const;
		void operator=(const Vec2D & v);

		void operator+=(const Vec2D & v);
		void operator-=(const Vec2D & v);
		void operator*=(const Vec2D & v);
		void operator/=(const Vec2D & v);
		//operators


	};

}
