#include <array>

std::array<float, 16> buildProjectionMatrix(float width, float height)
{
	std::array<float, 16> projection{};
	projection[0] =  2.f / width ;
	projection[5] = -2.f / height;
	projection[10] = 1;
	projection[12] = -1;
	projection[13] = 1;
	projection[15] = 1;
	return projection;
}