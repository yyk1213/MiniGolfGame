#pragma once
#include "Camera.h"
#include "FollowCamera.h"
#include "ReadyCamera.h"
#include "WaitCamera.h"
#include "Ball.h"
#include "Terrain.h"
#include "Hole.h"
#include <string>
#include "RelayCamera.h"
#include "ObjMesh.h"
#include "DisplayCount.h"
#include "PowerProcess.h"
#include "Sky.h"
#include "BeginScreen.h"
#include "GameOver.h"
#include <windowsx.h>
#include "MiniMapCam.h"
#include "MissionItem.h"
#define BMP_MINIMAP "resources/mini_field.bmp"

/*헤더파일 선언 -> cpp파일 정의*/
extern IDirect3DDevice9* g_pd3dDevice;	
extern int Width;
extern int Height;
extern int g_iSwingCount;
extern WaitCamera g_WaitCamera;
extern bool g_bisInHall;

//함수정의
bool Setup();
void Update(float timeDelta, float timeElapsed);
void Render(float timeDelta, float timeElapsed);
bool Display(float timeDelta);
void Cleanup();
void DrawTrees();
