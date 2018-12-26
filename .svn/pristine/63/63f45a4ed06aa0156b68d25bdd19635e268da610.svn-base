//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: terrain.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Represents a 3D terrain. 
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "terrain.h"
#include "BasicFunction.h"
#include "MiniMapCam.h"
#include <fstream>
#include <cmath>

const DWORD Terrain::TerrainVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

bool Terrain::setUp(IDirect3DDevice9* device,
	std::string heightmapFileName,
	int numVertsPerRow,
	int numVertsPerCol,
	int cellSpacing,
	float heightScale)
{
	g_pd3dDevice = device;
	_numVertsPerRow = numVertsPerRow;
	_numVertsPerCol = numVertsPerCol;
	_cellSpacing = cellSpacing;

	_numCellsPerRow = _numVertsPerRow - 1;
	_numCellsPerCol = _numVertsPerCol - 1;

	_width = _numCellsPerRow * _cellSpacing;
	_depth = _numCellsPerCol * _cellSpacing;

	_numVertices = _numVertsPerRow * _numVertsPerCol;
	_numTriangles = _numCellsPerRow * _numCellsPerCol * 2;

	_heightScale = heightScale;

	// load heightmap
	if (!readRawFile(heightmapFileName))
	{
		::MessageBox(0, "readRawFile - FAILED", 0, 0);
		::PostQuitMessage(0);
		return false;
	}

	// load texture
	if (!loadTexture(BMP_HEIGHTMAPTEX))
	{
		::MessageBox(0, "loadTexture - FAILED", 0, 0);
		::PostQuitMessage(0);
		return false;
	}

	// scale heights
	for (int i = 0; i < _heightmap.size(); i++)
		_heightmap[i] *= heightScale;

	// compute the vertices
	if (!computeVertices())
	{
		::MessageBox(0, "computeVertices - FAILED", 0, 0);
		::PostQuitMessage(0);
		return false;
	}

	// compute the indices
	if (!computeIndices())
	{
		::MessageBox(0, "computeIndices - FAILED", 0, 0);
		::PostQuitMessage(0);
		return false;
	}

	/*if (!lightTerrain(directionToLight))
	{
		::MessageBox(0, "lightTerrain - FAILED", 0, 0);
		::PostQuitMessage(0);
		return false;
	}*/

	return true;
}

Terrain::~Terrain()
{
	cleanUp();
}

int Terrain::getHeightmapEntry(int row, int col)
{
	return _heightmap[row * _numVertsPerRow + col];
}
//
void Terrain::setUpMatrices()
{
	D3DXMatrixIdentity(&_matHeight);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &_matHeight);
}

//void Terrain::MinisetUpMatrices()
//{
//	D3DXMatrixIdentity(&mini_mat);
//	g_pd3dDevice->SetTransform(D3DTS_WORLD, &mini_mat);
//}


void Terrain::setHeightmapEntry(int row, int col, int value)
{
	_heightmap[row * _numVertsPerRow + col] = value;
}

bool Terrain::computeVertices()
{
	HRESULT hr = 0;

	hr = g_pd3dDevice->CreateVertexBuffer(
		_numVertices * sizeof(TerrainVertex),
		D3DUSAGE_WRITEONLY,
		TerrainVertex::FVF,
		D3DPOOL_MANAGED,
		&_vb,
		0);

	if (FAILED(hr))
		return false;

	// coordinates to start generating vertices at
	int startX = -_width / 2;
	int startZ = _depth / 2;

	// coordinates to end generating vertices at
	int endX = _width / 2;
	int endZ = -_depth / 2;

	// compute the increment size of the texture coordinates
	// from one vertex to the next.
	float uCoordIncrementSize = 1.0f / (float)_numCellsPerRow;
	float vCoordIncrementSize = 1.0f / (float)_numCellsPerCol;

	TerrainVertex* v = 0;
	_vb->Lock(0, 0, (void**)&v, 0);

	int i = 0;
	for (int z = startZ; z >= endZ; z -= _cellSpacing)
	{
		int j = 0;
		for (int x = startX; x <= endX; x += _cellSpacing)
		{
			// compute the correct index into the vertex buffer and heightmap
			// based on where we are in the nested loop.
			int index = i * _numVertsPerRow + j;

			v[index] = TerrainVertex(
				(float)x,
				(float)_heightmap[index],
				(float)z,
				(float)j * uCoordIncrementSize,
				(float)i * vCoordIncrementSize);

			j++; // next column
		}
		i++; // next row
	}

	_vb->Unlock();

	return true;
}

bool Terrain::computeIndices()
{
	HRESULT hr = 0;

	hr = g_pd3dDevice->CreateIndexBuffer(
		_numTriangles * 3 * sizeof(WORD), // 3 indices per triangle
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_ib,
		0);

	if (FAILED(hr))
		return false;

	WORD* indices = 0;
	_ib->Lock(0, 0, (void**)&indices, 0);

	// index to start of a group of 6 indices that describe the
	// two triangles that make up a quad
	int baseIndex = 0;

	// loop through and compute the triangles of each quad
	for (int i = 0; i < _numCellsPerCol; i++)
	{
		for (int j = 0; j < _numCellsPerRow; j++)
		{
			indices[baseIndex] = i   * _numVertsPerRow + j;
			indices[baseIndex + 1] = i   * _numVertsPerRow + j + 1;
			indices[baseIndex + 2] = (i + 1) * _numVertsPerRow + j;

			indices[baseIndex + 3] = (i + 1) * _numVertsPerRow + j;
			indices[baseIndex + 4] = i   * _numVertsPerRow + j + 1;
			indices[baseIndex + 5] = (i + 1) * _numVertsPerRow + j + 1;

			// next quad
			baseIndex += 6;
		}
	}

	_ib->Unlock();

	return true;
}

bool Terrain::loadTexture(std::string fileName)
{
	HRESULT hr = 0;

	hr = D3DXCreateTextureFromFile(
		g_pd3dDevice,
		fileName.c_str(),
		&_tex);

	if (FAILED(hr))
		return false;

	return true;
}

bool Terrain::lightTerrain(D3DXVECTOR3* directionToLight)
{
	HRESULT hr = 0;

	D3DSURFACE_DESC textureDesc;
	_tex->GetLevelDesc(0 /*level*/, &textureDesc);

	// make sure we got the requested format because our code that fills the
	// texture is hard coded to a 32 bit pixel depth.
	if (textureDesc.Format != D3DFMT_X8R8G8B8)
		return false;

	D3DLOCKED_RECT lockedRect;
	_tex->LockRect(
		0,          // lock top surface level in mipmap chain
		&lockedRect,// pointer to receive locked data
		0,          // lock entire texture image
		0);         // no lock flags specified

	DWORD* imageData = (DWORD*)lockedRect.pBits;
	for (int i = 0; i < textureDesc.Height; i++)
	{
		for (int j = 0; j < textureDesc.Width; j++)
		{
			// index into texture, note we use the pitch and divide by 
			// four since the pitch is given in bytes and there are 
			// 4 bytes per DWORD.
			int index = i * lockedRect.Pitch / 4 + j;

			// 셀의 현재 컬러를 얻는다.
			D3DXCOLOR c(imageData[index]);

			// 셀에 음영을 적용한다.
			c *= computeShade(i, j, directionToLight);;

			// 음영을 입힌 컬러를 저장한다.
			imageData[index] = (D3DCOLOR)c;
		}
	}

	_tex->UnlockRect(0);

	return true;
}
float Terrain::Lerp(float a, float b, float t)
{
	return a - (a*t) + (b*t);
}

float Terrain::computeShade(int cellRow, int cellCol, D3DXVECTOR3* directionToLight)
{
	// 사각형의 세 버텍스 높이를 얻는다.
	float heightA = getHeightmapEntry(cellRow, cellCol);
	float heightB = getHeightmapEntry(cellRow, cellCol + 1);
	float heightC = getHeightmapEntry(cellRow + 1, cellCol);

	// 사각형의 두 벡터를 구성한다.
	D3DXVECTOR3 u(_cellSpacing, heightB - heightA, 0.0f);
	D3DXVECTOR3 v(0.0f, heightC - heightA, -_cellSpacing);

	// 사각형의 두 벡터 외적을 이용해 
	// 법선을 찾아낸다.
	D3DXVECTOR3 n;
	D3DXVec3Cross(&n, &u, &v);
	D3DXVec3Normalize(&n, &n);

	float cosine = D3DXVec3Dot(&n, directionToLight);

	if (cosine < 0.0f)
		cosine = 0.0f;

	return cosine;
}

bool Terrain::readRawFile(std::string fileName)
{
	// Restriction: RAW file dimensions must be >= to the
	// dimensions of the terrain.  That is a 128x128 RAW file
	// can only be used with a terrain constructed with at most
	// 128x128 vertices.

	// A height for each vertex
	std::vector<BYTE> in(_numVertices);

	std::ifstream inFile(fileName.c_str(), std::ios_base::binary);

	//if (inFile == 0)
		//return false;

	inFile.read(
		(char*)&in[0], // buffer
		in.size());// number of bytes to read into buffer

	inFile.close();

	// copy BYTE vector to int vector
	_heightmap.resize(_numVertices);

	for (int i = 0; i < in.size(); i++)
		_heightmap[i] = in[i];

	return true;
}

float Terrain::getHeight(float x, float z)
{
	// Translate on xz-plane by the transformation that takes
	// the terrain START point to the origin.
	x = ((float)_width / 2.0f) + x;
	z = ((float)_depth / 2.0f) - z;

	// Scale down by the transformation that makes the 
	// cellspacing equal to one.  This is given by 
	// 1 / cellspacing since; cellspacing * 1 / cellspacing = 1.
	x /= (float)_cellSpacing;
	z /= (float)_cellSpacing;

	// From now on, we will interpret our positive z-axis as
	// going in the 'down' direction, rather than the 'up' direction.
	// This allows to extract the row and column simply by 'flooring'
	// x and z:

	float col = ::floorf(x);
	float row = ::floorf(z);

	// get the heights of the quad we're in:
	// 
	//  A   B
	//  *---*
	//  | / |
	//  *---*  
	//  C   D

	float A = getHeightmapEntry(row, col);
	float B = getHeightmapEntry(row, col + 1);
	float C = getHeightmapEntry(row + 1, col);
	float D = getHeightmapEntry(row + 1, col + 1);

	//
	// Find the triangle we are in:
	//

	// Translate by the transformation that takes the upper-left
	// corner of the cell we are in to the origin.  Recall that our 
	// cellspacing was nomalized to 1.  Thus we have a unit square
	// at the origin of our +x -> 'right' and +z -> 'down' system.
	float dx = x - col;
	float dz = z - row;

	// Note the below compuations of u and v are unneccessary, we really
	// only need the height, but we compute the entire vector to emphasis
	// the books discussion.
	float height = 0.0f;
	if (dz < 1.0f - dx)  // upper triangle ABC
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C

						  // Linearly interpolate on each vector.  The height is the vertex
						  // height the vectors u and v originate from {A}, plus the heights
						  // found by interpolating on each vector u and v.
		height = A + Lerp(0.0f, uy, dx) + Lerp(0.0f, vy, dz);
	}
	else // lower triangle DCB
	{
		float uy = C - D; // D->C
		float vy = B - D; // D->B

						  // Linearly interpolate on each vector.  The height is the vertex
						  // height the vectors u and v originate from {D}, plus the heights
						  // found by interpolating on each vector u and v.
		height = D + Lerp(0.0f, uy, 1.0f - dx) + Lerp(0.0f, vy, 1.0f - dz);
	}

	return height;
}

void Terrain::cleanUp()
{
	_vb->Release();
	_ib->Release();
	_tex->Release();
}

bool Terrain::draw( bool drawTris)
{
	HRESULT hr = 0;

	if (g_pd3dDevice)
	{
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &_matHeight);

		g_pd3dDevice->SetStreamSource(0, _vb, 0, sizeof(TerrainVertex));
		g_pd3dDevice->SetFVF(TerrainVertex::FVF);
		g_pd3dDevice->SetIndices(_ib);

		g_pd3dDevice->SetTexture(0, _tex);

		// turn off lighting since we're lighting it ourselves
		g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);

		hr = g_pd3dDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			0,
			_numVertices,
			0,
			_numTriangles);

		g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, true);

		if (drawTris)
		{
			g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			hr = g_pd3dDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0,
				0,
				_numVertices,
				0,
				_numTriangles);

			g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}

		if (FAILED(hr))
			return false;
	}

	return true;
}

D3DXVECTOR3 Terrain::computeNormalVector(int cellRow, int cellCol)
{
	// 사각형의 세 버텍스 높이를 얻는다.
	float heightA = getHeightmapEntry(cellRow, cellCol);
	float heightB = getHeightmapEntry(cellRow, cellCol + 1);
	float heightC = getHeightmapEntry(cellRow + 1, cellCol);

	// 사각형의 두 벡터를 구성한다.
	D3DXVECTOR3 u(_cellSpacing, heightB - heightA, 0.0f);
	D3DXVECTOR3 v(0.0f, heightC - heightA, -_cellSpacing);

	// 사각형의 두 벡터 외적을 이용해 
	// 법선을 찾아낸다.
	D3DXVECTOR3 n;
	D3DXVec3Cross(&n, &u, &v);
	D3DXVec3Normalize(&n, &n);

	return n;
}