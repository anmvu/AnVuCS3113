#include "Vector.h"



Vector Vector::operator*(const Vector& Vec)
{
	Vector vecToReturn;

	vecToReturn.x = this->x * Vec.x;
	vecToReturn.y = this->y * Vec.y;
	vecToReturn.z = this->z * Vec.z;

	return vecToReturn;
}