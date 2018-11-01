#pragma once

#include <iostream>

class Scene {
private:
	std::string scene_name;

public:

	Scene();
	Scene(const Scene &copy);
	~Scene();

	std::string getName();
};