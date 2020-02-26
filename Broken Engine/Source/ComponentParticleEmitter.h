#ifndef __COMPONENT_PARTICLE_EMITTER__
#define __COMPONENT_PARTICLE_EMITTER__

#include "Component.h"
#include "Math.h"

#include "PhysX_3.4/Include/PxPhysicsAPI.h"

using namespace  physx;

class Particle
{
	PxVec3 position;
	uint index;
};

class ComponentParticleEmitter : public Component
{
public:

	ComponentParticleEmitter(GameObject* ContainerGO);
	~ComponentParticleEmitter();

	void Enable() override;
	void Disable() override;

	void UpdateParticles(float dt);

	static inline Component::ComponentType GetType() { return Component::ComponentType::ParticleEmitter; };

	// --- Save & Load ---
	json Save() const override;
	void Load(json& node) override;
	void CreateInspectorNode() override;

private:
	
	PxParticleSystem* particleSystem=nullptr;

	int maxParticles = 200;
	bool perParticleRestOffset = false;

	PxParticleExt::IndexPool* indexPool;

	//Emitter properties
	PxVec3 size;
	float emisionRate;	//in milliseconds
	uint validParticles=0;

	//Particle properties
	PxVec3 particlesVelocity;
	uint particlesDuration;
	

	float spawnClock=0.0f;
};

#endif //__COMPONENT_PARTICLE_EMITTER__