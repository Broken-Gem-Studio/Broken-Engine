#include "ImporterScene.h"
#include "Application.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"



#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentRenderer.h"
#include "ModuleSceneManager.h"
#include "GameObject.h"

ImporterScene::ImporterScene() 
{
}

ImporterScene::~ImporterScene()
{
}

bool ImporterScene::Import(const char & File_path, const ImportData & IData) const
{
	ImportSceneData Sdata = (ImportSceneData&) IData;
	

	// --- Import scene from path ---
	const aiScene* scene = aiImportFile(&File_path, aiProcessPreset_TargetRealtime_MaxQuality);


	if (scene != nullptr && scene->HasMeshes())
	{

		// --- Create new Component Material to store scene's, meshes will use this for now since we do not want to create a material for every mesh if not needed ---
		ComponentMaterial* Material = App->scene_manager->CreateMaterialFromScene(*scene, File_path);

		// --- Use scene->mNumMeshes to iterate on scene->mMeshes array ---

		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			// --- Create Game Object ---
			GameObject* new_object = App->scene_manager->CreateEmptyGameObject();

			// --- Get Scene mesh number i ---
			aiMesh* mesh = scene->mMeshes[i];

			if (mesh)
			{
				// --- Create new Component Mesh to store current scene mesh data ---
				ComponentMesh* new_mesh = (ComponentMesh*)new_object->AddComponent(Component::ComponentType::Mesh);

				// --- Import mesh data (fill new_mesh)---
				new_mesh->ImportMesh(mesh);

				// --- Create new Component Renderer to draw mesh ---
				ComponentRenderer* Renderer = (ComponentRenderer*)new_object->AddComponent(Component::ComponentType::Renderer);

				if (Material)
				{
					// --- Set Object's Material ---
					new_object->SetMaterial(Material);
				}

			}

		}

		// --- Free scene ---
		aiReleaseImport(scene);

	}
	else
		LOG("|[error]: Error loading FBX %s", File_path);













	return true;
}