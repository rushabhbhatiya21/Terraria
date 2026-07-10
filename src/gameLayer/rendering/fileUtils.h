#include <fstream>
#include <sstream>

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