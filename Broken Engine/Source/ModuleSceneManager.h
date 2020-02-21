#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Module.h"
#include <vector>
#include "Math.h"
#include "Color.h"
#include "Quadtree.h"

class GameObject;
struct aiScene;
struct ImportMaterialData;
struct par_shapes_mesh_s;
class ResourceMesh;
struct Event;

class ModuleSceneManager : public Module
{
public:

	ModuleSceneManager(bool start_enabled = true);
	~ModuleSceneManager();

	bool Init(json file) override;
	bool Start() override;
	//void ONEvent(const Event& event) const override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	bool CleanUp() override;

	// --- Creators ---
	GameObject* CreateEmptyGameObject();
	void CreateGrid(float target_distance);
	GameObject* LoadCube();
	GameObject* LoadSphere();

	void DestroyGameObject(GameObject* go);

	// --- Getters ---
	GameObject* GetSelectedGameObject() const;
	GameObject* GetRootGO() const;
	uint GetPointLineVAO() const;

	// --- Setters ---
	void SetSelectedGameObject(GameObject* go);

	// --- Utilities ---
	void DrawGrid();
	void Draw();
	void RedoOctree();
	void SetStatic(GameObject* go);
	void RecursiveDrawQuadtree(QuadtreeNode * node) const;
	void SelectFromRay(LineSegment& ray);

	// --- Save/Load ----
	void SaveStatus(json &file) const override;
	void LoadStatus(const json & file) override;
	//void SaveScene();
	//void LoadScene();
	//void RecursiveFreeScene(GameObject* go);

	// --- Draw Wireframe using given vertices ---
	template <typename Box>
	static void DrawWire(const Box& box, Color color, uint VAO)
	{
		float3 corners[8];
		box.GetCornerPoints(corners);
		DrawWireFromVertices(corners, color, VAO);
	};

private:
	// --- Event Callbacks ---
	static void ONResourceSelected(const Event& e);
	static void ONGameObjectDestroyed(const Event& e);

private:
	void GatherGameObjects(std::vector<GameObject*> & scene_gos, GameObject* go);
	GameObject* CreateRootGameObject();
	void DrawScene();

	// --- Primitives ---
	void LoadParMesh(par_shapes_mesh_s* mesh, ResourceMesh* new_mesh) const;
	void CreateCube(float sizeX, float sizeY, float sizeZ, ResourceMesh* rmesh);
	void CreateSphere(float Radius, int slices, int slacks, ResourceMesh* rmesh);

	static void DrawWireFromVertices(const float3* corners, Color color, uint VAO);
public:
	// --- Actually this is an octree ---
	Quadtree tree;
	std::vector<GameObject*> NoStaticGo;
	bool display_tree = false;
	bool display_boundingboxes = false;


private:
	uint PointLineVAO = 0;
	uint Grid_VAO = 0;
	uint Grid_VBO = 0;
	uint go_count = 0;
	GameObject* root = nullptr;
	GameObject* SelectedGameObject = nullptr;

	ResourceMesh* cube = nullptr;
	ResourceMesh* sphere = nullptr;
};

#endif