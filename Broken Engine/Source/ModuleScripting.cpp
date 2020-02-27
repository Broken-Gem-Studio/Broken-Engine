#include "ModuleScripting.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "GameObject.h"
//#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "Resource.h"
#include "ResourceScript.h"
#include "ComponentScript.h"
#include "Scripting.h"
//#include "ComponentTransform.h"
//#include "ModuleResources.h"
//#include "ModuleEditor.h"
#include "ScriptVar.h"
#include <iterator>

#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/MathFunc.h"

#include "SDL/include/SDL_keyboard.h"
#include "SDL/include/SDL_mouse.h"

//This include MUST go after Lua includes
//#include "LuaBridge-241/include/LuaBridge.h"
#include "ScriptData.h"
#include "mmgr/mmgr.h"


namespace BrokenEngine {
	ModuleScripting::ModuleScripting(bool start_enabled) : Module(start_enabled) {
	}

	ModuleScripting::~ModuleScripting() {
	}

	bool ModuleScripting::DoHotReloading() {
		bool ret = true;

		if (App->GetAppState() == AppState::EDITOR) // 	Ask Aitor, only return true when no gameplay is happening	App->scene_intro->playing == false
		{
			//We do the necessary Hot Reloading
			for (std::vector<ScriptInstance*>::iterator it = class_instances.begin(); it != class_instances.end(); ++it) {
				//Remove the references to the data inside the virtual machine
				(*it)->my_table_class = 0;

				//Copy the necessary data into new instances which will hold the new compiled tables
				ScriptInstance* recompiled_instance = new ScriptInstance;

				recompiled_instance->my_component = (*it)->my_component;
				recompiled_instance->my_resource = (*it)->my_resource;

				recompiled_instances.push_back(recompiled_instance);
			}
			CleanUpInstances();
			//Close the virtual machine & Destroy it
			lua_close(L);

			//Create the new Virtual Machine
			L = luaL_newstate();
			luaL_openlibs(L);

			//Acquire All scripts to be compiled       (we will compile even scripts which are currently not attached to any gameobject)
			//to check if it still compiles after the change done in a given script which is unknown to us
			std::string extension = "lua";
			std::vector<std::string> files;
			//App->resources->GetAllFilesWithExtension(extension,files,App->resources->assets_dir); // Here we have to iterate all script resources from that list so we can get the files to recompile

			bool can_instantiate_scripts = true;
			//Compile all the scripts of the Engine
			for (int i = 0; i < files.size(); ++i) {
				if (JustCompile(files[i]) == false) {
					can_instantiate_scripts = false;
					ret = false;
					ENGINE_CONSOLE_LOG("[Warning] Fix all compiler Errors!");
					cannot_start = true;
				}
			}

			if (can_instantiate_scripts == true) {
				//If everything compiled just fine, give the recompiled instances the new version of the script
				for (std::vector<ScriptInstance*>::iterator it = recompiled_instances.begin(); it != recompiled_instances.end(); ++it)
					CompileScriptTableClass((*it));

				for (int i = 0; i < recompiled_instances.size(); ++i)
					class_instances.push_back(recompiled_instances[i]);

				recompiled_instances.clear();
			}

			hot_reloading_waiting = false;
		}
		else {
			hot_reloading_waiting = true;
		}

		if (ret == true)
			cannot_start = false;

		return ret;
	}

	//This function just compiles a script and checks if it compiled or it prompts the error to console
	bool ModuleScripting::JustCompile(std::string absolute_path) {
		bool ret = false;

		//MYTODO: D�dac Commented this so I can try and compile, must uncomment when SCRIPTING class contents are uncommented too
		luabridge::getGlobalNamespace(L)
			.beginNamespace("Debug")
			.beginClass <Scripting>("Scripting")
			.addConstructor<void(*) (void)>()
			.addFunction("LOG", &Scripting::LogFromLua)
			.addFunction("GetKey", &Scripting::GetKey)
			.addFunction("KeyState", &Scripting::GetKeyState)
			.addFunction("KeyDown", &Scripting::IsKeyDown)
			.addFunction("KeyUp", &Scripting::IsKeyUp)
			.addFunction("KeyRepeat", &Scripting::IsKeyRepeat)
			.addFunction("KeyIdle", &Scripting::IsKeyIdle)
			.addFunction("GetMouseButton", &Scripting::GetMouseButton)
			.addFunction("MouseButtonState", &Scripting::GetMouseButtonState)
			.addFunction("MouseButtonDown", &Scripting::IsMouseButtonDown)
			.addFunction("MouseButtonUp", &Scripting::IsMouseButtonUp)
			.addFunction("MouseButtonRepeat", &Scripting::IsMouseButtonRepeat)
			.addFunction("MouseButtonIdle", &Scripting::IsMouseButtonIdle)
			.addFunction("Translate", &Scripting::Translate)
			.addFunction("dt", &Scripting::GetDT)
			.endClass()
			.endNamespace();

		Scripting Scripting;

		//std::string absolute_path = App->fs->GetBasePath() + relative_path;
		//std::string absolute_path = App->file_system->GetPathToGameFolder(true) + relative_path; //If it compiles, remove this Reference line
		int compiled = luaL_dofile(L, absolute_path.c_str());

		if (compiled == LUA_OK) {
			//We don't need to do nothing here, LOG something at most
			ENGINE_CONSOLE_LOG("Compiled %s successfully!", absolute_path.c_str());
			ret = true;
		}
		else {
			std::string error = lua_tostring(L, -1);
			ENGINE_CONSOLE_LOG("%s", error.data());
		}

		return ret;
	}

	void ModuleScripting::CompileScriptTableClass(ScriptInstance* script) {
		//MYTODO: D�dac Commented this so I can try and compile, must uncomment when SCRIPTING class contents are uncommented too
		luabridge::getGlobalNamespace(L)
			.beginNamespace("Debug")
			.beginClass <Scripting>("Scripting")
			.addConstructor<void(*) (void)>()
			.addFunction("LOG", &Scripting::LogFromLua)
			.addFunction("GetKey", &Scripting::GetKey)
			.addFunction("KeyState", &Scripting::GetKeyState)
			.addFunction("KeyDown", &Scripting::IsKeyDown)
			.addFunction("KeyUp", &Scripting::IsKeyUp)
			.addFunction("KeyRepeat", &Scripting::IsKeyRepeat)
			.addFunction("KeyIdle", &Scripting::IsKeyIdle)
			.addFunction("GetMouseButton", &Scripting::GetMouseButton)
			.addFunction("MouseButtonState", &Scripting::GetMouseButtonState)
			.addFunction("MouseButtonDown", &Scripting::IsMouseButtonDown)
			.addFunction("MouseButtonUp", &Scripting::IsMouseButtonUp)
			.addFunction("MouseButtonRepeat", &Scripting::IsMouseButtonRepeat)
			.addFunction("MouseButtonIdle", &Scripting::IsMouseButtonIdle)
			.addFunction("Translate", &Scripting::Translate)
			.addFunction("dt", &Scripting::GetDT)
			.endClass()
			.endNamespace();

		Scripting Scripting;

		if (L != nullptr) {
			//Compile the file and run it, we're gonna optimize this to just compile the function the script contains to library later.
			int compiled = luaL_dofile(L, script->my_resource->absolute_path.c_str());

			if (compiled == LUA_OK) {
				//Get the function to instantiate the lua table (used as a class as known in C++)
				std::string get_function = "GetTable" + script->my_resource->script_name;
				//get_function = App->SubtractString(get_function,".",false,true,false); //Line of reference delete if code compiles
				App->fs->SplitFilePath(get_function.data(), nullptr, &get_function, nullptr);
				luabridge::LuaRef ScriptGetTable = luabridge::getGlobal(L, get_function.c_str());

				if (!ScriptGetTable.isNil()) {
					luabridge::LuaRef table(ScriptGetTable());

					script->my_table_class = table;
					FillScriptInstanceComponentVars(script);
				}

			}
			else {
				std::string error = lua_tostring(L, -1);
				ENGINE_CONSOLE_LOG("%s", error.data());
			}
		}
	}

	void ModuleScripting::SendScriptToModule(ComponentScript* script_component) {
		ScriptInstance* s_instance = new ScriptInstance;
		s_instance->my_component = script_component;
		s_instance->my_resource = script_component->script;

		class_instances.push_back(s_instance);
		JustCompile(script_component->script->absolute_path);
		CompileScriptTableClass(s_instance); //Compile so we can give the instance its table/class reference
	}

	ScriptFile* ModuleScripting::AddScriptFile(ComponentScript* script_component, std::string full_file_path) {
		ScriptFile* ret = nullptr;
		std::string filename;
		App->fs->SplitFilePath(full_file_path.data(), nullptr, &filename, nullptr);
		for (std::vector<ScriptFile*>::iterator it = script_files.begin(); it != script_files.end(); ++it) {
			//If this file is already in our list of files
			Resource* res = (Resource*)script_component->script;
			if ((*it)->resource_uid == res->GetUID()) {
				return (*it);
			}
		}

		ScriptFile* new_file = new ScriptFile;
		script_files.push_back(new_file);
		ret = new_file;

		return ret;
	}

	//FILL the ScriptVars of the component associated with this script
	void ModuleScripting::FillScriptInstanceComponentVars(ScriptInstance* script) {
		for (luabridge::Iterator iterator(script->my_table_class); !iterator.isNil(); ++iterator) {
			//Declare necessary vars for intialization & get variable name
			VarType variable_type = VarType::NONE;
			std::string str = (*iterator).first.tostring();
			ScriptVar variable;
			std::string var_value;

			// first == .key () and second == .value() in LUA
			//Fill values
			if ((*iterator).second.isNumber()) {
				variable_type = VarType::DOUBLE;
				var_value = (*iterator).second.tostring();
				double val = std::stod(var_value);
				variable = ScriptVar(val);
			}
			else if ((*iterator).second.isString()) {
				variable_type = VarType::STRING;
				var_value = (*iterator).second.tostring();
				variable = ScriptVar(var_value.data());
			}
			else if ((*iterator).second.isBool()) {
				variable_type = VarType::BOOLEAN;
				var_value = (*iterator).second.tostring();
				bool val = false;
				std::string tru = "true";
				if (!var_value.compare(tru)) {
					val = true;
				}
				variable = ScriptVar(val);
			}
			else if ((*iterator).second.isFunction())
				continue;

			//ASSIGN name to variable and push it if compatible
			variable.name = str;
			if (variable_type != VarType::NONE) {
				int variable_index = script->my_component->ScriptVarAlreadyInComponent(variable.name);
				if (variable_index > -1)  //If the var was already on the component (hot reloading cases)
				{
					//Check that the variable is still of the same type before changing any value
					if (variable.type == script->my_component->script_variables[variable_index].type) {
						script->my_component->script_variables[variable_index].editor_value = variable.editor_value;
					}
					else   // The variable changed of type
					{
						script->my_component->script_variables[variable_index] = variable;
					}
				}
				else {
					script->my_component->script_variables.push_back(variable);
				}
			}
		}
	}

	void ModuleScripting::DeleteScriptInstanceWithParentComponent(ComponentScript* script_component) {
		for (int i = 0; i < class_instances.size(); ++i) {
			if (class_instances[i] != nullptr && class_instances[i]->my_component == script_component) {
				//delete class_instances[i];
				class_instances.erase(class_instances.begin() + i);
			}
		}
	}

	void ModuleScripting::NotifyHotReloading() {
		hot_reloading_waiting = true;
	}


	bool ModuleScripting::Init(json file) {
		//Create the Virtual Machine
		L = luaL_newstate();
		luaL_openlibs(L);

		return true;
	}

	bool ModuleScripting::Start() {
		return true;
	}

	bool ModuleScripting::CleanUp() {
		CleanUpInstances();

		return true;
	}

	update_status ModuleScripting::Update(float realDT) {
		//If a script was changed during runtime, hot reload
		if (App->GetAppState() == AppState::EDITOR && hot_reloading_waiting) // Ask Aitor if this is correct (condition should return true only when no gameplay is being played)
			DoHotReloading();

		// Carles to Didac
		// 1. You can use the "IsWhatever" functions of App to check the current game state.
		// 2. "App->IsGameFirstFrame()" marks the first frame a GameUpdate() will happen, if you want to do anything right before the game plays in preparation
		// 3. Referring to your previous code, you expected DoHotReloading() to NOT run if the game is playing, I put the condition accordingly "!IsGamePlaying()"



		//TEST FUNCTION DEFINETIVELY SHOULD NOT BE HERE
		//MYTODO: D�dac PLEAse didac look into this why did you do this?
		/*if (App->scene_intro->selected_go != nullptr && App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
			GameObject* returned = GOFunctions::InstantiateGameObject(App->scene_intro->selected_go);*/
		return UPDATE_CONTINUE;
	}

	update_status ModuleScripting::GameUpdate(float gameDT) {
		//MYTODO: D�dac When we can compile, uncommenting this and getting it to works is priority NUMBER 1 it's the INCLUDE we use inside LUA!
		//luabridge::getGlobalNamespace(L)
		//	.beginNamespace("Debug")
		//	.beginClass <Scripting>("Scripting")
		//	.addConstructor<void(*) (void)>()

		//	// General
		//	.addFunction("LOG", &Scripting::LogFromLua)
		//	.addFunction("time", &Scripting::GetTime)
		//	.addFunction("dt", &Scripting::GetDT)

		//	// Input
		//	.addFunction("GetKey", &Scripting::GetKey)
		//	.addFunction("KeyState", &Scripting::GetKeyState)
		//	.addFunction("KeyDown", &Scripting::IsKeyDown)
		//	.addFunction("KeyUp", &Scripting::IsKeyUp)
		//	.addFunction("KeyRepeat", &Scripting::IsKeyRepeat)
		//	.addFunction("KeyIdle", &Scripting::IsKeyIdle)

		//	.addFunction("GetMouseButton", &Scripting::GetMouseButton)
		//	.addFunction("MouseButtonState", &Scripting::GetMouseButtonState)
		//	.addFunction("MouseButtonDown", &Scripting::IsMouseButtonDown)
		//	.addFunction("MouseButtonUp", &Scripting::IsMouseButtonUp)
		//	.addFunction("MouseButtonRepeat", &Scripting::IsMouseButtonRepeat)
		//	.addFunction("MouseButtonIdle", &Scripting::IsMouseButtonIdle)

		//	.addFunction("MouseInGame", &Scripting::IsMouseInGame)
		//	.addCFunction("MouseRaycastHit", &Scripting::GetMouseRaycastHit)

		//	// GameObjects
		//	.addFunction("Find", &Scripting::FindGameObject)
		//	.addFunction("FindUID", &Scripting::FindGameObjectUID)

		//	.addFunction("Instantiate", &Scripting::Instantiate)
		//	.addFunction("InstantiateByUID", &Scripting::InstantiateByUID)
		//	.addFunction("InstantiateByName", &Scripting::InstantiateByName)

		//	.addFunction("DestroyByUID", &Scripting::DestroyByUID)

		//	// Script Data
		//	.addFunction("Activate", &Scripting::Activate)
		//	.addFunction("activated", &Scripting::IsActivated)

		//	// Object Data
		//	//.addFunction("gameObject", &Scripting::GetGameObject)
		//	.addFunction("GO_name", &Scripting::GetObjectName)
		//	.addFunction("GO_Activate", &Scripting::ActivateObject)
		//	.addFunction("GO_activated", &Scripting::IsObjectActivated)
		//	.addFunction("GO_Destroy", &Scripting::DestroySelf)

		//	// Transform Position
		//	.addFunction("position_x", &Scripting::GetPositionX)
		//	.addFunction("position_y", &Scripting::GetPositionY)
		//	.addFunction("position_z", &Scripting::GetPositionZ)
		//	.addFunction("position", &Scripting::GetPosition)

		//	.addFunction("Translate", &Scripting::Translate)
		//	.addFunction("SetPosition", &Scripting::SetPosition)

		//	// Transform Rotation
		//	.addFunction("rotation_x", &Scripting::GetEulerX)
		//	.addFunction("rotation_y", &Scripting::GetEulerY)
		//	.addFunction("rotation_z", &Scripting::GetEulerZ)
		//	.addFunction("rotation", &Scripting::GetEulerRotation)

		//	.addFunction("EulerRotate", &Scripting::RotateByEuler)
		//	.addFunction("SetEulerRotation", &Scripting::SetEulerRotation)

		//	.addFunction("quat_x", &Scripting::GetQuatX)
		//	.addFunction("quat_y", &Scripting::GetQuatY)
		//	.addFunction("quat_z", &Scripting::GetQuatZ)
		//	.addFunction("quat_w", &Scripting::GetQuatW)
		//	.addFunction("quat", &Scripting::GetQuatRotation)

		//	.addFunction("QuatRotate", &Scripting::RotateByQuat)
		//	.addFunction("SetQuatRotation", &Scripting::SetQuatRotation)

		//	// Uility
		//	.addFunction("LookAt", &Scripting::LookAt)
		//	.addFunction("LookTo", &Scripting::LookTo)

			//.endClass()
			//.endNamespace();

		Scripting Scripting;
		//Building a class / Namespace so Lua can have this object to Call EngineLOG by calling
		if (cannot_start == false && App->GetAppState() == AppState::PLAY) {
			const uint origSize = class_instances.size();	// This avoids messing the iteration with newly Instantiated scripts
			for (uint i = 0; i < origSize; ++i) {
				current_script = class_instances[i];

				for (std::vector<ScriptVar>::iterator it = current_script->my_component->script_variables.begin(); it != current_script->my_component->script_variables.end(); ++it)
					if ((*it).changed_value) {
						switch ((*it).type) {
						case VarType::DOUBLE:
							current_script->my_table_class[(*it).name.c_str()] = (*it).editor_value.as_double_number;
							break;
						case VarType::STRING:
							current_script->my_table_class[(*it).name.c_str()] = (*it).editor_value.as_string;
							break;
						case VarType::BOOLEAN:
							current_script->my_table_class[(*it).name.c_str()] = (*it).editor_value.as_boolean;
							break;
						}
						(*it).changed_value = false;
					}

				if (current_script->awoken == false) {
					current_script->my_table_class["Awake"]();	// Awake is done first, regardless of the script being active or not
					current_script->awoken = true;
				}
				else if (current_script->my_component->GetActive()) //Check if the script instance and it's object holding it are active
				{
					GameObject* GO = current_script->my_component->GetContainerGameObject();

					if (GO->GetActive()) // Even if the component script is active, if its gameobject its not, we won't update
					{
						if (current_script->started == false) {
							current_script->my_table_class["Start"]();	// Start is done only once for the first time the script is active
							current_script->started = true;
						}
						else {
							current_script->my_table_class["Update"]();	// Update is done on every iteration of the script as long as it remains active
						}
					}
				}
			}
		}
		return UPDATE_CONTINUE;
	}


	void ModuleScripting::CleanUpInstances() {
		for (std::vector<ScriptInstance*>::iterator it = class_instances.begin(); it != class_instances.end(); ++it) {
			if ((*it) != nullptr)
				delete (*it);
		}

		class_instances.clear();
	}


	void ModuleScripting::Stop() {
		for (std::vector<ScriptInstance*>::iterator it = class_instances.begin(); it != class_instances.end(); ++it)
			(*it)->awoken = (*it)->started = false;
	}
}