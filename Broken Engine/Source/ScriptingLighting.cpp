#include "ScriptingLighting.h"

// -- Modules --
#include "Application.h"
#include "ModuleScripting.h"
#include "ModuleSceneManager.h"

// -- Components -
#include "GameObject.h"
#include "Components.h"
#include "ComponentLight.h"

#include "ScriptData.h"
#include "ResourceScene.h"

// -- Utilities --
//#include "TranslatorUtilities.h"

using namespace Broken;

// --- Setters ---
void ScriptingLighting::SetIntensity(float intensity, uint gameobject_UUID)
{
	GameObject* go = App->scene_manager->currentScene->GetGOWithUID(gameobject_UUID);
	if (go)
	{
		ComponentLight* light = go->GetComponent<ComponentLight>();
		if (light)
			light->SetLightIntensity(intensity);
		else
			ENGINE_CONSOLE_LOG("![Script]: (SetIntensity) Light Component is null");
	}
	else
		ENGINE_CONSOLE_LOG("![Script]: (SetIntensity) Could not find GameObject with UUID %d", gameobject_UUID);
}

void ScriptingLighting::SetDistanceMultiplier(float distMult, uint gameobject_UUID)
{
	GameObject* go = App->scene_manager->currentScene->GetGOWithUID(gameobject_UUID);
	if (go)
	{
		ComponentLight* light = go->GetComponent<ComponentLight>();
		if (light)
			light->SetLightDistanceMultiplier(distMult);
		else
			ENGINE_CONSOLE_LOG("![Script]: (SetDistanceMultiplier) Light Component is null");
	}
	else
		ENGINE_CONSOLE_LOG("![Script]: (SetDistanceMultiplier) Could not find GameObject with UUID %d", gameobject_UUID);
}

void ScriptingLighting::SetType(int type, uint gameobject_UUID)
{
	if (type <= -1 || type >= 3)
	{
		ENGINE_CONSOLE_LOG("![Script]: (SetType) Error! Light Type Parameter Invalid!");
		return;
	}

	GameObject* go = App->scene_manager->currentScene->GetGOWithUID(gameobject_UUID);
	if (go)
	{
		ComponentLight* light = go->GetComponent<ComponentLight>();
		if (light)
			light->SetLightType((LightType)type);
		else
			ENGINE_CONSOLE_LOG("![Script]: (SetType) Light Component is null");
	}
	else
		ENGINE_CONSOLE_LOG("![Script]: (SetType) Could not find GameObject with UUID %d", gameobject_UUID);
}

void ScriptingLighting::SetColor(float r, float g, float b, uint gameobject_UUID)
{
	GameObject* go = App->scene_manager->currentScene->GetGOWithUID(gameobject_UUID);
	if (go)
	{
		ComponentLight* light = go->GetComponent<ComponentLight>();
		if (light)
			light->SetLightColor(float3(r, g, b));
		else
			ENGINE_CONSOLE_LOG("![Script]: (SetColor) Light Component is null");
	}
	else
		ENGINE_CONSOLE_LOG("![Script]: (SetColor) Could not find GameObject with UUID %d", gameobject_UUID);
}

void ScriptingLighting::SetAttenuation(float K, float L, float Q, uint gameobject_UUID)
{
	GameObject* go = App->scene_manager->currentScene->GetGOWithUID(gameobject_UUID);
	if (go)
	{
		ComponentLight* light = go->GetComponent<ComponentLight>();
		if (light)
			light->SetLightAttenuationFactors(K, L, Q);
		else
			ENGINE_CONSOLE_LOG("![Script]: (SetAttenuation) Light Component is null");
	}
	else
		ENGINE_CONSOLE_LOG("![Script]: (SetAttenuation) Could not find GameObject with UUID %d", gameobject_UUID);
}

void ScriptingLighting::SetCutoff(float innerCutoff, float outerCutoff, uint gameobject_UUID)
{
	GameObject* go = App->scene_manager->currentScene->GetGOWithUID(gameobject_UUID);
	if (go)
	{
		ComponentLight* light = go->GetComponent<ComponentLight>();
		if (light)
			return light->SetLightInOutCutoff(innerCutoff, outerCutoff);
		else
			ENGINE_CONSOLE_LOG("![Script]: (SetCutoff) Light Component is null");
	}
	else
		ENGINE_CONSOLE_LOG("![Script]: (SetCutoff) Could not find GameObject with UUID %d", gameobject_UUID);
}

// --- Getters ---
float ScriptingLighting::GetIntensity(uint gameobject_UUID) const
{
	float ret = 0.0f;

	GameObject* go = App->scene_manager->currentScene->GetGOWithUID(gameobject_UUID);
	if (go)
	{
		ComponentLight* light = go->GetComponent<ComponentLight>();
		if (light)
			ret = light->GetLightIntensity();
		else
			ENGINE_CONSOLE_LOG("![Script]: (GetIntensity) Light Component is null");
	}
	else
		ENGINE_CONSOLE_LOG("![Script]: (GetIntensity) Could not find GameObject with UUID %d", gameobject_UUID);

	return ret;
}

float ScriptingLighting::GetDistanceMultiplier(uint gameobject_UUID) const
{
	float ret = 0.0f;

	GameObject* go = App->scene_manager->currentScene->GetGOWithUID(gameobject_UUID);
	if (go)
	{
		ComponentLight* light = go->GetComponent<ComponentLight>();
		if (light)
			ret = light->GetLightDistanceMultiplier();
		else
			ENGINE_CONSOLE_LOG("![Script]: (GetDistanceMultiplier) Light Component is null");
	}
	else
		ENGINE_CONSOLE_LOG("![Script]: (GetDistanceMultiplier) Could not find GameObject with UUID %d", gameobject_UUID);

	return ret;
}

int ScriptingLighting::GetType(uint gameobject_UUID) const
{
	int ret = -1;
	GameObject* go = App->scene_manager->currentScene->GetGOWithUID(gameobject_UUID);

	if (go)
	{
		ComponentLight* light = go->GetComponent<ComponentLight>();
		if (light)
		{
			ret = (int)light->GetLightType();
			if(ret <= -1 || ret >= 3)
				ENGINE_CONSOLE_LOG("![Script]: (GetType) WARNING! Light Type Invalid");
		}
		else
			ENGINE_CONSOLE_LOG("![Script]: (GetType) Light Component is null");
	}
	else
		ENGINE_CONSOLE_LOG("![Script]: (GetType) Could not find GameObject with UUID %d", gameobject_UUID);

	return ret;
}

luabridge::LuaRef ScriptingLighting::GetColor(uint gameobject_UUID, lua_State* L) const
{
	float3 color = float3(0.0f);
	GameObject* go = App->scene_manager->currentScene->GetGOWithUID(gameobject_UUID);

	if (go)
	{
		ComponentLight* light = go->GetComponent<ComponentLight>();
		if (light)
			color = light->GetLightColor();
		else
			ENGINE_CONSOLE_LOG("![Script]: (GetColor) Light Component is null");
	}
	else
		ENGINE_CONSOLE_LOG("![Script]: (GetColor) Could not find GameObject with UUID %d", gameobject_UUID);

	luabridge::LuaRef table = luabridge::newTable(L);
	table.append(color.x);
	table.append(color.y);
	table.append(color.z);

	return table;
}

luabridge::LuaRef ScriptingLighting::GetAttenuation(uint gameobject_UUID, lua_State* L) const
{
	float3 att = float3(0.0f);
	GameObject* go = App->scene_manager->currentScene->GetGOWithUID(gameobject_UUID);

	if (go)
	{
		ComponentLight* light = go->GetComponent<ComponentLight>();
		if (light)
			att = light->GetLightAttenuationKLQ();
		else
			ENGINE_CONSOLE_LOG("![Script]: (GetAttenuation) Light Component is null");
	}
	else
		ENGINE_CONSOLE_LOG("![Script]: (GetAttenuation) Could not find GameObject with UUID %d", gameobject_UUID);

	luabridge::LuaRef table = luabridge::newTable(L);
	table.append(att.x);
	table.append(att.y);
	table.append(att.z);

	return table;
}

luabridge::LuaRef ScriptingLighting::GetCutoff(uint gameobject_UUID, lua_State* L) const
{
	float2 cutoff = float2(0.0f);
	GameObject* go = App->scene_manager->currentScene->GetGOWithUID(gameobject_UUID);

	if (go)
	{
		ComponentLight* light = go->GetComponent<ComponentLight>();
		if (light)
			cutoff = light->GetLightInOutCutoff();
		else
			ENGINE_CONSOLE_LOG("![Script]: (GetCutoff) Light Component is null");
	}
	else
		ENGINE_CONSOLE_LOG("![Script]: (GetCutoff) Could not find GameObject with UUID %d", gameobject_UUID);

	luabridge::LuaRef table = luabridge::newTable(L);
	table.append(cutoff.x);
	table.append(cutoff.y);

	return table;
}