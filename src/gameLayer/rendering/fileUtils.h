#include <fstream>
#include <sstream>
#include <array>

std::string readTextFromFile(const std::string& path)
{
	std::ifstream file(path);

	if (!file.is_open()) return "";

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string fullText = buffer.str();

	file.close();
	return fullText;
}

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