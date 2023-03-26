#pragma once
#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp>

struct Coord;
class GameWindow;

void coordToInts(const Coord& c, int& x, int& y);

void serializeToJson(const GameWindow& d, const std::string& filename);

void deserializeFromJson(GameWindow& d, const std::string& filename);