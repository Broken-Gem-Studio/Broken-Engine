#ifndef __COMPONENT_PARTICLE_EMITTER__
#define __COMPONENT_PARTICLE_EMITTER__

#include "Component.h"
#include "Math.h"

#include "PhysX_3.4/Include/PxPhysicsAPI.h"

BE_BEGIN_NAMESPACE

class Particle;
class ResourceTexture;

class BROKEN_API ComponentParticleEmitter : public Component
{
	friend class ModuleParticles;
public:

	ComponentParticleEmitter(GameObject* ContainerGO);
	~ComponentParticleEmitter();

	void Update() override;

	void Enable() override;
	void Disable() override;

	void UpdateParticles(float dt);
	//void DrawComponent() override;
	void DrawParticles();
	void ChangeParticlesColor(float4 color);

	static inline Component::ComponentType GetType() { return Component::ComponentType::ParticleEmitter; };

	// --- Save & Load ---
	json Save() const override;
	void Load(json& node) override;
	void CreateInspectorNode() override;


	//Scripting functions
	void Play();
	void Stop();
	void SetEmisionRate(float ms);
	void SetParticlesPerCreation(int particlesAmount);
	void SetExternalAcceleration(float x, float y, float z);
	void SetParticlesVelocity(float x, float y, float z);
	void SetVelocityRF(float x, float y, float z);
	void SetOffsetPosition(float x, float y, float z);
	void SetOffsetRotation(float x, float y, float z);
	void SetLooping(bool active);
	void SetDuration(int duration);
	void SetLifeTime(int ms);
	void SetParticlesScale(float x, float y);
	void SetParticlesScaleRF(float randomFactor);
	void UpdateActorLayer(const int* layerMask);

private:

	void SortParticles();

	void CreateParticles(uint particlesAmount);

	double GetRandomValue(double min, double max); //MUST EREASE IN THE FUTURE

private:
	physx::PxParticleSystem* particleSystem = nullptr;

	std::vector<Particle*> particles;
	std::map<float, int> drawingIndices;

	unsigned int maxParticles = 2000;
	bool perParticleRestOffset = false;

	physx::PxParticleExt::IndexPool* indexPool;

	uint validParticles = 0;

	//Emitter properties
	float3 emitterPosition = { 0,0,0 };
	float3 eulerRotation = float3::zero;
	Quat emitterRotation = Quat::identity;
	int particlesPerCreation = 1;
	physx::PxVec3 size = { 0,0,0 };
	float emisionRate = 500.0f;	//in milliseconds
	physx::PxVec3 externalAcceleration = { 0,10,0 };
	physx::PxVec3 particlesVelocity = { 0,0,0 };
	physx::PxVec3 velocityRandomFactor = { 5,5,5 };
	bool loop = true;
	bool emisionActive = true;
	int duration = 1000;
	uint emisionStart = 0;

	//Particle properties
	int particlesLifeTime = 1000;
	float2 particlesScale = { 1,1 };
	float scaleOverTime = 0.0f;
	float particlesScaleRandomFactor = 1;
	ResourceTexture* texture = nullptr;

	//Colors
	bool colorGradient = false;
	float4 particlesColor = float4::one;
	float4 particlesColor2 = float4::one;
	float4 particleColorVariation = float4::one;

	float spawnClock = 0.0f;
};
BE_END_NAMESPACE

#endif //__COMPONENT_PARTICLE_EMITTER__
