#pragma once
#include "Math.h"
#include "Color.h"
#include "ComponentMesh.h"


enum PrimitiveTypes
{
	Primitive_Grid,
	Primitive_Cube,
	Primitive_Sphere,
};

class Primitive
{
public:

	Primitive();
	virtual ~Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const float3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:

	Color color;
	float4x4 transform = math::float4x4::identity;
	bool axis, wire;

protected:

	PrimitiveTypes type;
	ComponentMesh mesh;

	//float* Vertices = nullptr;
	//uint VerticesID = 0;
	//uint verticesSize = 0;

	//uint* Indices = nullptr;
	//uint IndicesID = 0;
	//uint IndicesSize = 0;

	//float* TexCoords = nullptr;
	//uint  TextureCoordsID = 0;
	//uint TexCoordsSize = 0;

	//uint TexID = 0;

};

// ============================================
class PrimitiveCube : public Primitive
{
public:
	PrimitiveCube(float sizeX, float sizeY, float sizeZ, bool checkers = false);
	/*void InnerRender() const;*/
public:
	float3 size = { 0.0f,0.0f,0.0f };
};

// ============================================
class PrimitiveSphere : public Primitive
{
public:
	PrimitiveSphere(float size, int slices, int slacks, bool checkers = false);
	PrimitiveSphere(float radius);
	void InnerRender() const;
public:
	float radius;
};


// ============================================
class PrimitiveGrid : public Primitive
{
public:
	PrimitiveGrid();
	void InnerRender() const;
public:
	float3 normal;
	float constant;
};
