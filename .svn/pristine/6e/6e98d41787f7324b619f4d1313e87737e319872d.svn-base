#pragma once
#ifndef OBJMESH_H
#define OBJMESH_H

#include <d3dx9.h>
#include "D3DObj\/D3DObjMesh.h"

class ObjMesh
{
protected:
	CD3DMesh* g_mesh;//메시 객체

	D3DXVECTOR3 scale;//크기
	D3DXVECTOR3 position;//위치

	LPDIRECT3DTEXTURE9 g_meshTextures;//메시에 사용될 텍스쳐
	LPCSTR _objFileName;
	LPDIRECT3DDEVICE9 g_pd3dDevice;

	D3DXMATRIXA16 _matObj;

public: 
	ObjMesh();
	ObjMesh(LPDIRECT3DDEVICE9 device) { g_pd3dDevice = device; };
	~ObjMesh();
	//위치,크기 변경
	void setScale(float sx, float sy, float sz) { scale.x = sx; scale.y = sy;	scale.z = sz; }
	void setPosition(float tx, float ty, float tz) { position.x = tx; position.y = ty;	position.z = tz; }

	bool Init(LPDIRECT3DDEVICE9 device);
	bool LoadTexture(LPCSTR textFileName);
	bool SetupMatrices();
	bool SetUpTexture();
	void Draw();
	HRESULT Setup(LPCSTR fileName);
};
#endif