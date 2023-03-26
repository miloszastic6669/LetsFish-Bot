#include "serialization.h"
#include "gameWindow.h"

void coordToInts(const Coord& c, int& x, int& y)
{
	x = c.x;
	y = c.y;
}

void serializeToJson(const GameWindow& d, const std::string& filename) 
{
	nlohmann::json j;
	//upperLeft
	j["UpperLeftX"] = d.getUpperLeft().x;
	j["UpperLeftY"] = d.getUpperLeft().y;
	//bottomRight
	j["BottomRightX"] = d.getBottomRight().x;
	j["BottomRightY"] = d.getBottomRight().y;

	std::ofstream file(filename);
	file << j;
}

void deserializeFromJson(GameWindow& d, const std::string& filename)
{
	std::ifstream file(filename);
	nlohmann::json j;
	file >> j;
	Coord pos;
	//upperLeft
	pos.x = j["UpperLeftX"];
	pos.y = j["UpperLeftY"];
	d.setUpperLeft(pos);
	
	//bottomRight
	pos.x = j["BottomRightX"];
	pos.y = j["BottomRightY"];
	d.setBottomRight(pos);
}
