#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <string>
#include "Globals.h"


class Resource 
{
	friend class ModuleResources;

public:
	enum ResourceType
	{
		FOLDER,
		MESH,
		TEXTURE,
		MATERIAL,
		META,
		UNKNOWN,
	};

	Resource(ResourceType type);
	~Resource();

	// --- Getters ---
	uint GetUID() const;
	ResourceType GetType() const;
	const char* GetOriginalFile() const;
	const char* GetResourceFile() const;
	const char* GetName() const;

	// --- Utilities ---
	virtual void LoadOnMemory() {};
	virtual void FreeMemory() {};

public:
	uint instances = 0;

protected:
	uint UID = 0;
	ResourceType type = UNKNOWN;

	std::string resource_file = "";
	std::string original_file = "";
	std::string name = "";
};

#endif // !__RESOURCE_H__
