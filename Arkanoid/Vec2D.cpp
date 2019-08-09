#include "pch.h"
#include "Vec2D.h"


jp::Vec2D::Vec2D() :x(0), y(0)
{
}

jp::Vec2D::Vec2D(float x, float y) : x(x), y(y)
{
}

jp::Vec2D::Vec2D(const Vec2D & v) : x(v.x), y(v.y)
{
}

jp::Vec2D::Vec2D(sf::Vector2f v) : x(v.x), y(v.y)
{
}

jp::Vec2D::~Vec2D()
{
}

void jp::Vec2D::set(Vec2D & v)
{
	this->x = v.x;
	this->x = v.y;
}

void jp::Vec2D::set(sf::Vector2f & v)
{
	this->x = v.x;
	this->y = v.y;
}

void jp::Vec2D::set(float x, float y)
{
	this->x = x;
	this->y = y;
}

void jp::Vec2D::zero()
{
	this->x = 0;
	this->y = 0;
}

jp::Vec2D jp::Vec2D::clone()
{
	return Vec2D(this->x, this->y);
}

void jp::Vec2D::add(Vec2D & v)
{
	this->x += v.x;
	this->y += v.y;
}

void jp::Vec2D::sub(Vec2D & v)
{
	this->x -= v.x;
	this->y -= v.y;
}

void jp::Vec2D::div(float scale)
{
	this->x /= scale;
	this->y /= scale;
}

void jp::Vec2D::mult(float scale)
{
	this->x *= scale;
	this->y *= scale;
}

float jp::Vec2D::length()
{
	return std::sqrt((std::pow(this->x, 2) + std::pow(this->y, 2)));
}

void jp::Vec2D::setLength(float scale)
{
	float len = length();
	this->x = (this->x / len) * scale;
	this->y = (this->y / len) * scale;
}

void jp::Vec2D::normalize()
{
	float len = length();
	if (len != 0)
	{
		div(len);
	}

}

float jp::Vec2D::getAngle()
{
	return std::atan2(this->x, this->y);
}

void jp::Vec2D::setAngle(float scale)
{
	float len = length();
	this->x = std::cos(scale)*len;
	this->y = std::sin(scale)*len;

}

float jp::Vec2D::getAngleDeg()
{
	return (std::atan2(this->x, this->y) * 57.2957f);
}

void jp::Vec2D::setAngleDeg(float scale)
{
	float len = length();
	scale *= 0.0174532925f;
	this->x = cos(scale) * len;
	this->y = sin(scale) * len;
}

void jp::Vec2D::rotateBy(float scale)
{
	float angle = getAngle();
	float len = length();

	this->x = std::cos(scale + angle)* len;
	this->y = std::sin(scale + angle)* len;

}

float jp::Vec2D::dot(Vec2D & v)
{
	return ((this->x * v.x) + (this->y * v.y));
}

float jp::Vec2D::cross(Vec2D & v)
{
	return ((this->x * v.y) - (this->y * v.x));
}

float jp::Vec2D::dist(Vec2D & v)
{
	float dx = v.x - this->x;
	float dy = v.y - this->y;

	jp::Vec2D vd(dx, dy);

	return vd.length();
}

void jp::Vec2D::limit(float lim)
{
	if (length() > lim)
	{
		normalize();
		mult(lim);
	}

}

const std::string jp::Vec2D::to_str()
{
	char tmpbuf[256];
	sprintf_s(tmpbuf, "[%f, %f] \n", this->x, this->y);
	return tmpbuf;
}

const sf::Vector2f jp::Vec2D::sf()
{
	return sf::Vector2f(this->x, this->y);
}

jp::Vec2D jp::Vec2D::sub(Vec2D & v1, Vec2D & v2)
{
	float dx = v1.x - v2.x;
	float dy = v1.y - v2.y;

	jp::Vec2D vd(dx, dy);

	return vd;
}

jp::Vec2D jp::Vec2D::operator+(const Vec2D & v) const
{
	return Vec2D((this->x + v.x), (this->y + v.y));
}

jp::Vec2D jp::Vec2D::operator-(const Vec2D & v) const
{
	return Vec2D((this->x - v.x), (this->y - v.y));
}

jp::Vec2D jp::Vec2D::operator*(const float & scale) const
{
	return Vec2D((this->x * scale), (this->y * scale));
}

float jp::Vec2D::operator*(const Vec2D & v) const
{
	return ((this->x * v.x) + (this->y * v.y));
}

float jp::Vec2D::operator^(const Vec2D & v) const
{
	return  ((this->x * v.y) - (this->y * v.x));
}

jp::Vec2D jp::Vec2D::operator/(const float & scale) const
{
	return Vec2D((this->x / scale), (this->y / scale));
}

jp::Vec2D jp::Vec2D::operator/(const Vec2D & v) const
{
	return Vec2D((this->x / v.x), (this->y / v.y));
}

bool jp::Vec2D::operator==(const Vec2D & v) const
{
	return (this->x == v.x && this->y == v.y);
}

void jp::Vec2D::operator=(const Vec2D & v)
{
	this->x = v.x;
	this->y = v.y;
}

void jp::Vec2D::operator+=(const Vec2D & v)
{
	this->x += v.x;
	this->y += v.y;
}

void jp::Vec2D::operator-=(const Vec2D & v)
{
	this->x -= v.x;
	this->y -= v.y;
}

void jp::Vec2D::operator*=(const Vec2D & v)
{
	this->x *= v.x;
	this->y *= v.y;
}

void jp::Vec2D::operator/=(const Vec2D & v)
{
	this->x /= v.x;
	this->y /= v.y;
}

