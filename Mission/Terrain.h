//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: terrain.h
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Represents a 3D terrain.       
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __terrainH__
#define __terrainH__

#include <d3dx9.h>
#include <string>
#include <vector>
#include "MiniMapCam.h"

#define BMP_HEIGHTMAPTEX "resources/field.bmp"

class Terrain
{
public:
	Terrain() {};
	bool setUp(
		IDirect3DDevice9* device,
		std::string heightmapFileName,
		int numVertsPerRow,
		int numVertsPerCol,
		int cellSpacing,    // space between cells
		float heightScale);

	~Terrain();

	int  getHeightmapEntry(int row, int col);
	void setHeightmapEntry(int row, int col, int value);
	//
	void setUpMatrices();//하이트 맵 월드행렬
	float getHeight(float x, float z);
	void cleanUp();

	bool  loadTexture(std::string fileName);
	bool  draw(bool drawTris);
	D3DXVECTOR3 computeNormalVector(int cellRow, int cellCol);
	//bool  lightTerrain(D3DXVECTOR3* directionToLight);

private:
	IDirect3DDevice9*       g_pd3dDevice;
	IDirect3DTexture9*      _tex;
	IDirect3DVertexBuffer9* _vb;
	IDirect3DIndexBuffer9*  _ib;
	//
	D3DXMATRIXA16 _matHeight;//지형의 월드 행렬

	int _numVertsPerRow;
	int _numVertsPerCol;
	int _cellSpacing;

	int _numCellsPerRow;
	int _numCellsPerCol;
	int _width;
	int _depth;
	int _numVertices;
	int _numTriangles;

	float _heightScale;

	std::vector<int> _heightmap;

	// helper methods
	bool  readRawFile(std::string fileName);
	bool  computeVertices();
	bool  computeIndices();
	bool  lightTerrain(D3DXVECTOR3* directionToLight);
	float computeShade(int cellRow, int cellCol, D3DXVECTOR3* directionToLight);
	//내가
	float Lerp(float a, float b, float t);

	struct TerrainVertex
	{
		TerrainVertex() {}
		TerrainVertex(float x, float y, float z, float u, float v)
		{
			_x = x; _y = y; _z = z; _u = u; _v = v;
		}
		float _x, _y, _z;
		float _u, _v;

		static const DWORD FVF;
	};
};

#endif // __terrainH__
