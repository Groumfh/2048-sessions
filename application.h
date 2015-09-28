#pragma once

#include "base.h"

class Application
{
public:

	Application(int argc, char **argv);
	~Application();

	bool isInitialized() const;
	bool initGL();
	int run();

private:

	class Impl_;
	std::unique_ptr<Impl_> impl_;
	class LifeManager_;
	std::unique_ptr<LifeManager_> lifeManager_;
	std::vector<std::string> createMenu();
};

