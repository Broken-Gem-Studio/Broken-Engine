#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"
#include "Math.h"

struct aiMesh;
struct aiScene;

class ComponentMesh : public Component
{
public:

	ComponentMesh();
	virtual ~ComponentMesh();

	void ImportMesh(const aiMesh* mesh, uint MATTextureID);

public:
	float4x4 transform = math::float4x4::identity;

	float* Vertices = nullptr;
	uint VerticesID = 0; // unique vertex in VRAM
	uint VerticesSize = 0;

	uint* Indices = nullptr;
	uint IndicesID = 0; // index in VRAM
	uint IndicesSize = 0;

	float3* Normals = nullptr;
	uint NormalsSize = 0;

	float* TexCoords = nullptr;
	uint  TextureCoordsID = 0;
	uint TexCoordsSize = 0;

	uint  TextureID = 0;

	unsigned char* Colours = nullptr;
	uint ColoursSize = 0;
};

#endif

