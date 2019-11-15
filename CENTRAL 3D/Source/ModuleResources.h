#ifndef __MODULE_RESOURCES_H__
#define __MODULE_RESOURCES_H__

#include "Module.h"
#include "Globals.h"

class Resource;

class ModuleResources : public Module
{
public:

	ModuleResources(bool start_enabled = true);
	~ModuleResources();

	//bool Init(json file);
	//bool Start();
	//bool CleanUp();

private:
	std::map<uint, Resource*> resources;
};

#endif