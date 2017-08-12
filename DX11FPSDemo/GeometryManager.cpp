#include "GeometryManager.h"

GeometryManager* GeometryManager::instance = nullptr;

GeometryManager::GeometryManager() :numberOfVerticesOnCircle(30),
numberOfLongitudes(60),
numberOfLatitudes(30)
{
	initCubeGeometry();
	initCylinderGeometry();
	initSphereGeometry();
}

void GeometryManager::initCubeGeometry()
{
	Vertex v[] =
	{
		// Front Face
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,0.0f, 0.0f, -1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,0.0f,  0.0f, -1.0f),
		Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f,  0.0f, -1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f),

		// Back Face
		Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f,0.0f, 0.0f, 1.0f),
		Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f),
		Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f,0.0f,  0.0f, 1.0f),

		// Top Face
		Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,0.0f, 1.0f, 0.0f),
		Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,0.0f, 1.0f,  0.0f),
		Vertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f),
		Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f),

		// Bottom Face
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,0.0f, -1.0f, 0.0f),
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f),
		Vertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f),
		Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,0.0f, -1.0f,  0.0f),

		// Left Face
		Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,-1.0f, 0.0f,  0.0f),
		Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,-1.0f, 0.0f,  0.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f,-1.0f,  0.0f, 0.0f),
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, 0.0f, 0.0f),

		// Right Face
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f),
		Vertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f),
		Vertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f),
		Vertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  0.0f),
	};
	cubeVertices.insert(cubeVertices.begin(), v, v + 24);

	UINT indices[] =
	{
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};
	cubeIndices.insert(cubeIndices.begin(), indices, indices + 36);
}

void GeometryManager::initCylinderGeometry()
{
	Vertex *v = new Vertex[numberOfVerticesOnCircle * 4 + 4];
	float deltaAngle = 6.28f / numberOfVerticesOnCircle;
	float uvInterval = 1.f / numberOfVerticesOnCircle;
	float radius = 1.f / 6.28f;
	for (int i = 0; i < numberOfVerticesOnCircle; ++i)
	{
		v[i] = Vertex(cos(deltaAngle*i), 1.f, sin(deltaAngle*i), uvInterval*i, 0, cos(deltaAngle*i), 0.f, sin(deltaAngle*i));
		v[i + numberOfVerticesOnCircle + 1] = Vertex(cos(deltaAngle*i), -1.f, sin(deltaAngle*i), uvInterval*i, 1, cos(deltaAngle*i), 0.f, sin(deltaAngle*i));
		v[i + numberOfVerticesOnCircle * 2 + 3] = Vertex(cos(deltaAngle*i), 1.f, sin(deltaAngle*i), 0.5f + radius*cos(deltaAngle*i), 0.5f + radius*sin(deltaAngle*i), 0.f, 1.f, 0.f);
		v[i + numberOfVerticesOnCircle * 3 + 4] = Vertex(cos(deltaAngle*i), -1.f, sin(deltaAngle*i), 0.5f + radius*cos(deltaAngle*i), 0.5f + radius*sin(deltaAngle*i), 0.f, -1.f, 0.f);
	}
	v[numberOfVerticesOnCircle] = Vertex(1.f, 1.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f);
	v[numberOfVerticesOnCircle * 2 + 1] = Vertex(1.f, -1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f);

	v[numberOfVerticesOnCircle * 2 + 2] = Vertex(0.f, 1.f, 0.f, 0.5f, 0.5f, 0.f, 1.f, 0.f);
	v[numberOfVerticesOnCircle * 3 + 3] = Vertex(0.f, -1.f, 0.f, 0.5f, 0.5f, 0.f, -1.f, 0.f);

	cylinderVertices.insert(cylinderVertices.begin(), v, v + numberOfVerticesOnCircle * 4 + 4);

	UINT *indices = new UINT[numberOfVerticesOnCircle * 12];
	for (int i = 0; i < numberOfVerticesOnCircle - 1; ++i)
	{
		UINT v1 = i, v2 = i + 1, v3 = i + numberOfVerticesOnCircle + 1, v4 = v2 + numberOfVerticesOnCircle + 1;
		int index = i * 6;
		indices[index] = v1;
		indices[index + 1] = v2;
		indices[index + 2] = v3;
		indices[index + 3] = v3;
		indices[index + 4] = v2;
		indices[index + 5] = v4;
	}
	int lastIndex = (numberOfVerticesOnCircle - 1) * 6;
	indices[lastIndex] = numberOfVerticesOnCircle - 1;
	indices[lastIndex + 1] = numberOfVerticesOnCircle;
	indices[lastIndex + 2] = numberOfVerticesOnCircle * 2;
	indices[lastIndex + 3] = numberOfVerticesOnCircle * 2;
	indices[lastIndex + 4] = numberOfVerticesOnCircle;
	indices[lastIndex + 5] = numberOfVerticesOnCircle * 2 + 1;

	UINT topCenter = numberOfVerticesOnCircle * 2 + 2, bottomCenter = numberOfVerticesOnCircle * 3 + 3;
	for (int i = 0; i < numberOfVerticesOnCircle; ++i)
	{
		UINT v1 = numberOfVerticesOnCircle * 2 + 3 + i, v2 = topCenter, v3 = numberOfVerticesOnCircle * 2 + 4 + i, v4 = numberOfVerticesOnCircle * 3 + 4 + i, v5 = numberOfVerticesOnCircle * 3 + 5 + i, v6 = bottomCenter;
		if (v3 >= numberOfVerticesOnCircle * 3 + 3)
		{
			v3 = numberOfVerticesOnCircle * 2 + 3;
			v5 = numberOfVerticesOnCircle * 3 + 4;
		}
		int topIndex = i * 3 + numberOfVerticesOnCircle * 6, bottomIndex = i * 3 + numberOfVerticesOnCircle * 9;
		indices[topIndex] = v1;
		indices[topIndex + 1] = v2;
		indices[topIndex + 2] = v3;
		indices[bottomIndex] = v4;
		indices[bottomIndex + 1] = v5;
		indices[bottomIndex + 2] = v6;
	}

	cylinderIndices.insert(cylinderIndices.begin(), indices, indices + numberOfVerticesOnCircle * 12);

	delete[] v;
	delete[] indices;
}

void GeometryManager::initSphereGeometry()
{
	Vertex *v = new Vertex[(numberOfLongitudes + 1)*(numberOfLatitudes + 1)];
	float longitudeAngle = 6.28f / numberOfLongitudes, latitudeAngle = 3.14f / numberOfLatitudes;
	float uInterval = 1.f / numberOfLongitudes, vInterval = 1.f / numberOfLatitudes;
	for (int i = 1; i<numberOfLatitudes; ++i)
	{
		float yCoordinate = cos(latitudeAngle*i);
		for (int j = 0; j<numberOfLongitudes; ++j)
		{
			v[i*(numberOfLongitudes + 1) + j] = Vertex(sin(latitudeAngle*i)*cos(longitudeAngle*j), yCoordinate, sin(latitudeAngle*i)*sin(longitudeAngle*j), uInterval*j, vInterval*i, sin(latitudeAngle*i)*cos(longitudeAngle*j), yCoordinate, sin(latitudeAngle*i)*sin(longitudeAngle*j));
		}
		v[i*(numberOfLongitudes + 1) + numberOfLongitudes] = Vertex(sin(latitudeAngle*i), yCoordinate, 0, 1, vInterval*i, sin(latitudeAngle*i), yCoordinate, 0);
	}
	float edgeUOffset = uInterval / 2;
	for (int i = 0; i<numberOfLongitudes; ++i)
	{
		v[i] = Vertex(0.f, 1.f, 0.f, edgeUOffset + uInterval*i, 0.f, 0.f, 1.f, 0.f);
		v[i + (numberOfLongitudes + 1)*numberOfLatitudes] = Vertex(0.f, -1.f, 0.f, edgeUOffset + uInterval*i, 1.f, 0.f, -1.f, 0.f);
	}

	sphereVertices.insert(sphereVertices.begin(), v, v + (numberOfLongitudes + 1)*(numberOfLatitudes + 1));

	UINT *indices = new UINT[(numberOfLatitudes - 1)*numberOfLongitudes * 6];
	for (int i = 0; i<numberOfLatitudes; ++i)
	{
		for (int j = 0; j<numberOfLongitudes; ++j)
		{
			if (i == 0)
			{
				indices[j * 3] = j;
				indices[j * 3 + 1] = numberOfLongitudes + j + 2;
				indices[j * 3 + 2] = numberOfLongitudes + j + 1;
			}
			else if (i == numberOfLatitudes - 1)
			{
				indices[numberOfLongitudes * 3 + (numberOfLatitudes - 2)*numberOfLongitudes * 6 + j * 3] = (numberOfLatitudes - 1)*(numberOfLongitudes + 1) + j;
				indices[numberOfLongitudes * 3 + (numberOfLatitudes - 2)*numberOfLongitudes * 6 + j * 3 + 1] = (numberOfLatitudes - 1)*(numberOfLongitudes + 1) + j + 1;
				indices[numberOfLongitudes * 3 + (numberOfLatitudes - 2)*numberOfLongitudes * 6 + j * 3 + 2] = numberOfLatitudes*(numberOfLongitudes + 1) + j;
			}
			else
			{
				indices[numberOfLongitudes * 3 + ((i - 1)*numberOfLongitudes + j) * 6] = i*(numberOfLongitudes + 1) + j;
				indices[numberOfLongitudes * 3 + ((i - 1)*numberOfLongitudes + j) * 6 + 1] = i*(numberOfLongitudes + 1) + j + 1;
				indices[numberOfLongitudes * 3 + ((i - 1)*numberOfLongitudes + j) * 6 + 2] = (i + 1)*(numberOfLongitudes + 1) + j;
				indices[numberOfLongitudes * 3 + ((i - 1)*numberOfLongitudes + j) * 6 + 3] = (i + 1)*(numberOfLongitudes + 1) + j;
				indices[numberOfLongitudes * 3 + ((i - 1)*numberOfLongitudes + j) * 6 + 4] = i*(numberOfLongitudes + 1) + j + 1;
				indices[numberOfLongitudes * 3 + ((i - 1)*numberOfLongitudes + j) * 6 + 5] = (i + 1)*(numberOfLongitudes + 1) + j + 1;
			}
		}
	}

	sphereIndices.insert(sphereIndices.begin(), indices, indices + (numberOfLatitudes - 1)*numberOfLongitudes * 6);

	delete[] v;
	delete[] indices;
}

GeometryManager * GeometryManager::getInstance()
{
	if (!instance)
	{
		instance = new GeometryManager();
	}
	return instance;
}

GeometryManager::~GeometryManager()
{
	delete instance;
	instance = nullptr;
}

int GeometryManager::getVertexSize() const
{
	return sizeof(Vertex);
}

GeometryManager::Vertex* GeometryManager::getCubeVertitces()
{
	return &cubeVertices[0];
}

int GeometryManager::getCubeVertexCount() const
{
	return cubeVertices.size();
}

UINT* GeometryManager::getCubeIndices()
{
	return &cubeIndices[0];
}

int GeometryManager::getCubeIndexCount() const
{
	return cubeIndices.size();
}

GeometryManager::Vertex * GeometryManager::getCylinderVertices()
{

	return &cylinderVertices[0];
}

int GeometryManager::getCylinderVertexCount() const
{
	return cylinderVertices.size();
}

UINT * GeometryManager::getCylinderIndices()
{
	return &cylinderIndices[0];
}

int GeometryManager::getCylinderIndexCount() const
{
	return cylinderIndices.size();
}

GeometryManager::Vertex * GeometryManager::getSphereVertices()
{
	return &sphereVertices[0];
}

int GeometryManager::getSphereVertexCount() const
{
	return sphereVertices.size();
}

UINT * GeometryManager::getSphereIndices()
{
	return &sphereIndices[0];
}

int GeometryManager::getSphereIndexCount() const
{
	return sphereIndices.size();
}
