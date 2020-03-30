#ifndef __SCRIPTINGPHYSICS_H__
#define __SCRIPTINGPHYSICS_H__

#include "BrokenCore.h"

class lua_State;

BE_BEGIN_NAMESPACE
class BROKEN_API ScriptingPhysics {
public:
	ScriptingPhysics();
	~ScriptingPhysics();

public:
	void SetMass(float mass, uint gameobject_UUID = -1);
	float GetMass(uint gameobject_UUID = -1);

	int GetLinearVelocity(lua_State* L, uint gameobject_UUID = -1);
	int GetAngularVelocity(lua_State* L, uint gameobject_UUID = -1);
	void SetLinearVelocity(float x, float y, float z, uint gameobject_UUID = -1);
	void SetAngularVelocity(float x, float y, float z, uint gameobject_UUID = -1);

	void AddTorque(float forceX, float forceY, float forceZ, int ForceMode, uint gameobject_UUID = -1);
	void AddForce(float forceX, float forceY, float forceZ, int ForceMode, uint gameobject_UUID = -1);

	void SetKinematic(bool enable, uint gameobject_UUID = -1);
	void UseGravity(bool enable, uint gameobject_UUID = -1);

};
BE_END_NAMESPACE
#endif // __SCRIPTINGPHYSICS_H__