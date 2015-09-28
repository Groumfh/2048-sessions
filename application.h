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
};

