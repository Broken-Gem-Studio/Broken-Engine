#include "EngineApplication.h"
#include "ModuleEditorUI.h"
#include "BrokenEngineMain.h"


EngineApplication* EngineApp = NULL;

Broken::Application* Broken::CreateApplication() {
	return new EngineApplication();
}

EngineApplication::EngineApplication() {
	EngineApp = this;
	SetConfigPath("Settings/EditorConfig.json");

	//We initiate our own modules
	editorui = new ModuleEditorUI();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(threading);
	AddModule(fs);
	AddModule(event_manager);
	AddModule(input);
	AddModule(time);
	
	
	AddModule(textures);
	AddModule(hardware);
	
	AddModule(resources);
	AddModule(window);
	AddModule(camera);
	AddModule(undo);
	AddModule(editorui);
	AddModule(gui);
	AddModule(ui_system);
	
	// Scenes
	AddModule(scene_manager);

	//Physics and particles
	AddModule(physics);
	AddModule(particles);

	//Audio
	AddModule(audio);
	
	//Gameplay (Scripting)
	AddModule(scripting);
	// Renderer last!
	AddModule(renderer3D);
}

EngineApplication::~EngineApplication() {
}
