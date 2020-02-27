#ifndef __PANEL_PHYSICS_H__
#define __PANEL_PHYSICS_H__

#include "Panel.h"

#include "PhysX_3.4/Include/PxPhysicsAPI.h"

class PanelPhysics : public Panel
{
public:

	PanelPhysics(char* name);
	~PanelPhysics();

	bool Draw();

private:
	physx::PxVec3 gravity;
	float staticFriction, dynamicFriction, restitution;

	physx::PxVec3 tmpGravity;
	float tmpStaticFriction, tmpDynamicFriction, tmpRestitution;
};
#endif