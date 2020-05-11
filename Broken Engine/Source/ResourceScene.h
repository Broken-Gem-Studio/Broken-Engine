#ifndef __RESOURCE_SCENE_H__
#define __RESOURCE_SCENE_H__

#include "Resource.h"
#include <unordered_map>
#include "Math.h"

BE_BEGIN_NAMESPACE
class GameObject;

class BROKEN_API ResourceScene : public Resource {
public:
	ResourceScene(uint UID, const char* source_file);
	~ResourceScene();

	bool LoadInMemory() override;
	void FreeMemory() override;

	void DeactivateAllGameObjects();
	void ActivateAllGameObjects();

	void CopyInto(ResourceScene* target);

	GameObject* GetGOWithName(const char* GO_name);
	GameObject* GetGOWithUID(uint UID);

	std::unordered_map<uint,GameObject*> NoStaticGameObjects;
	std::unordered_map<uint, GameObject*> StaticGameObjects;

	AABB octreeBox;

private:
	void OnOverwrite() override;
	void OnDelete() override;
};
BE_END_NAMESPACE
#endif //__RESOURCE_SCENE_H__