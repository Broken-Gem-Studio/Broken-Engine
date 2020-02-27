#ifndef __RESOURCE_SCENE_H__
#define __RESOURCE_SCENE_H__

#include "Resource.h"
#include <unordered_map>

BE_BEGIN_NAMESPACE
class GameObject;

class BROKEN_API ResourceScene : public Resource {
public:
	ResourceScene(uint UID, std::string source_file);
	~ResourceScene();

	bool LoadInMemory() override;
	void FreeMemory() override;

	std::unordered_map<uint, GameObject*> scene_gos;

private:
	void OnOverwrite() override;
	void OnDelete() override;
};
BE_END_NAMESPACE
#endif //__RESOURCE_SCENE_H__