#pragma once
#include <d3d11.h>
#include <xnamath.h>
#include <vector>

using namespace std;

class GeometryManager
{

	struct Vertex
	{
		Vertex() {}
		Vertex(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz)
			: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
	};
private:
	int numberOfVerticesOnCircle;
	int numberOfLongitudes;
	int numberOfLatitudes;
	static GeometryManager *instance;
	GeometryManager();
	vector<Vertex> cubeVertices;
	vector<UINT> cubeIndices;
	vector<Vertex> cylinderVertices;
	vector<UINT> cylinderIndices;
	vector<Vertex> sphereVertices;
	vector<UINT> sphereIndices;
	void initCubeGeometry();
	void initCylinderGeometry();
	void initSphereGeometry();
public:
	static GeometryManager* getInstance();
	~GeometryManager();
	int getVertexSize() const;
	Vertex* getCubeVertitces();
	int getCubeVertexCount() const;
	UINT* getCubeIndices();
	int getCubeIndexCount() const;
	Vertex* getCylinderVertices();
	int getCylinderVertexCount() const;
	UINT* getCylinderIndices();
	int getCylinderIndexCount() const;
	Vertex* getSphereVertices();
	int getSphereVertexCount() const;
	UINT* getSphereIndices();
	int getSphereIndexCount() const;
};