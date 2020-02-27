#ifndef __BROKENENGINE_H__
#define __BROKENENGINE_H__

// ---Entry Point------------------
#include "EntryPoint.h"
// --------------------------------

#include "Module.h"
// ---Modules----------------------
#include "ModuleEventManager.h"
#include "ModuleInput.h"
#include "ModuleTimeManager.h"
#include "ModuleHardware.h"
#include "ModuleFileSystem.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneManager.h"
#include "ModuleRenderer3D.h"
#include "ModuleTextures.h"
#include "ModuleResourceManager.h"
#include "ModuleScripting.h"
#include "ModuleThreading.h"
#include "ModuleGui.h"
// --------------------------------

// ---Panel------------------------
#include "Panel.h"
// --------------------------------

// ---GameObject && Components-----
#include "GameObject.h"
#include "Component.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentMeshRenderer.h"
#include "ComponentScript.h"
#include "ComponentTransform.h"
// --------------------------------

// ---Importers and Resources------
#include "JSONLoader.h"

#include "Importer.h"
#include "ImporterFolder.h"
#include "ImporterScene.h"
#include "ImporterModel.h"
#include "ImporterMaterial.h"
#include "ImporterShader.h"
#include "ImporterMesh.h"
#include "ImporterTexture.h"
#include "ImporterScript.h"
#include "ImporterMeta.h"

#include "Resource.h"
#include "ResourceFolder.h"
#include "ResourceModel.h"
#include "ResourceScene.h"
#include "ResourceMaterial.h"
#include "ResourceShaderProgram.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourceShaderObject.h"
#include "ResourceShader.h"
#include "ResourceScript.h"
#include "ResourceMeta.h"
// --------------------------------

// ---Others-----------------------
#include "Light.h"
// --------------------------------

#endif