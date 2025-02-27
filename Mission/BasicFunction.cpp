#pragma once
#include "BasicFunction.h"

/*.cpp파일 정의*/
IDirect3DDevice9* g_pd3dDevice = 0;
int Width = 1024;
int Height = 768;
int g_iSwingCount = 0;
//각 클래스 객체 생성
WaitCamera g_WaitCamera;
FollowCamera g_FollowCamera;
ReadyCamera g_ReadyCamera;
MissionItem g_Item;
RelayCamera g_RelayCamera;
PowerProcess g_PowerProcess;
Ball g_pBall;
Terrain g_terrain;
Hole g_Hole;
DisplayCount g_DisplayCount;
Sky g_sky;
gameType g_eType = BASIC;
MiniMapCam g_miniMapCam;
IDirect3DTexture9* mini_tex;	//미니맵 텍스쳐

float terrainHeight;
D3DXVECTOR3 g_vBallPosition;

float g_ballPower;
int g_fPower = 1;
D3DXVECTOR3 preBallPosition = D3DXVECTOR3(100.0f, 5.0f, -600.0f);		//첫 시작 좌표
D3DXVECTOR3 g_moveWaitCamera;
bool g_bisInHall = false;
bool g_isNoKey = true;
bool g_bDrop ;

//나무
std::vector<ObjMesh*> _TreeList;

/*임시 저장 변수*/
float g_icomX = 99999.0f;
float g_icomZ = 99999.0f;

LPCSTR g_texFileName = "ballTex/lowball.dds";

bool g_isEatItem = false;

//미니맵 내용이 들어갈 서피스
LPDIRECT3DSURFACE9 capturingTarget = NULL;
//-----------------------------------------------------------------------------//

// Setup

//-----------------------------------------------------------------------------//

bool Setup()
{

	g_FollowCamera.setDevice(g_pd3dDevice);
	g_ReadyCamera.setDevice(g_pd3dDevice);
	g_WaitCamera.setDevice(g_pd3dDevice);

	g_RelayCamera.setDevice(g_pd3dDevice);
	g_PowerProcess.setUp(g_pd3dDevice);
	g_DisplayCount.setUp(g_pd3dDevice);

	g_miniMapCam.setDevice(g_pd3dDevice);

	g_sky.Init(g_pd3dDevice);
	g_terrain.setUp(g_pd3dDevice, "resources/field.raw", 80, 80, 20, 0.1f);

	g_Hole.setUp(g_pd3dDevice);
	g_pBall.Setup(g_pd3dDevice, &g_terrain);
	g_vBallPosition = g_pBall.getBallPosition();
	g_ReadyCamera.setUp(g_vBallPosition);
	g_WaitCamera.setUp(g_vBallPosition);
	g_Item.setUp(g_pd3dDevice);
	//나무 셋팅
	DrawTrees();

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return true;
}

//-----------------------------------------------------------------------------//

// Cleanup

//-----------------------------------------------------------------------------//

void Cleanup()
{
	g_pBall.CleanUp();
	g_PowerProcess.Cleanup();
	g_Hole.CleanUp();
	g_WaitCamera.CleanUp();
	g_ReadyCamera.CleanUp();
	g_DisplayCount.Cleanup();
	g_miniMapCam.CleanUp();
	g_RelayCamera.CleanUp();
	g_sky.CleanUp();
	g_terrain.cleanUp();
}

void Update(float timeDelta, float timeElapsed)
{
	g_vBallPosition = g_pBall.getBallPosition();
	//홀 위치
	D3DXVECTOR3 holePosition = g_Hole.getHolePosition();
	D3DXVECTOR3 ball_holeD = g_vBallPosition - holePosition;
	float holeDistance = D3DXVec3Length(&ball_holeD);

	//power 값을 받아옴으로서 power값을 0.0이 아니게 만든다.
	if (GetAsyncKeyState('K') & 0x8000f && g_eType == POWERWAIT)	//z축
	{
		g_eType = READY;
		g_ReadyCamera.update(g_vBallPosition);
	}
	if (GetAsyncKeyState('L') & 0x8000f && g_eType == READY)	//z축
	{
		g_isNoKey = false;
		g_eType = BASIC;
	}

	switch (g_eType)		//타입을 다른데서 계속 변경해줄 필요 있음
	{
		//공이 던져지고 최종 위치
	case BASIC:
		g_WaitCamera.update(g_vBallPosition, g_moveWaitCamera, g_isNoKey);		//움직일 경로를 파악하여 
		g_isNoKey = true;
		g_eType = POWERWAIT;
		break;

	case SHOT:
	g_moveWaitCamera=g_pBall.update( timeDelta, timeElapsed, &g_FollowCamera,&g_Hole,&g_WaitCamera,preBallPosition,&g_bDrop,g_fPower,&g_RelayCamera);		//follow카메라로			
	if ((g_vBallPosition.y == terrainHeight+0.5f) && (g_vBallPosition.x == g_icomX) && (g_vBallPosition.z == g_icomZ))		{
			g_eType = BASIC;
			g_icomX = 999999.0f;		//0이아닌 임의로 정해진숫자.
			g_icomZ = 999999.0f;
			preBallPosition = g_vBallPosition;
		
			g_bDrop = false;
			g_iSwingCount++;
		}
		g_icomX = g_vBallPosition.x;
		g_icomZ = g_vBallPosition.z;
		break;

	case POWERWAIT:		//(waitcamera로 방향조a절하고 그 방향대로 공을 날리자.)
		g_PowerProcess.keyState(&g_fPower);
		D3DXVECTOR3 tempVec = g_WaitCamera.getPosition();
		g_pBall.setInitVec(tempVec, g_fPower);

		if (GetAsyncKeyState(VK_SPACE) & 0x0001f && g_fPower != 0)	//z축
		{
			g_eType = SHOT;
			
		}
		//파워를 입력받을 수 있는 STOP상태
		break;
	}
}

void Render(float timeDelta, float timeElapsed)
{

}

//-----------------------------------------------------------------------------//

// Display - 화면에 표시하는 부분

//-----------------------------------------------------------------------------//


bool Display(float timeDelta)
{
	if (g_pd3dDevice)
	{
		//백버퍼
		LPDIRECT3DSURFACE9 pBackBuffer = NULL;
		//가져오기
		g_pd3dDevice->GetRenderTarget(0, &pBackBuffer);
		
		//후면 버퍼 초기화
		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		//미니맵 카메라에서 바꿔주기
		if (g_eType == BASIC)
		{
			////백 버퍼와 같은 속성으로 렌더타겟을 만든다
			D3DSURFACE_DESC d3dsd;
			//후면 버퍼 정보를 불러옴
			pBackBuffer->GetDesc(&d3dsd);
			//랜더타겟 생성
			g_pd3dDevice->CreateRenderTarget(500, 420, D3DFMT_X8R8G8B8, d3dsd.MultiSampleType, d3dsd.MultiSampleQuality, false, &capturingTarget, NULL);

			//렌더타깃 설정--캡쳐링 타겟
			g_pd3dDevice->SetRenderTarget(0, capturingTarget);
			//서피스 clear
			g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 30), 1, 0);

			//그림 그리기
			g_pd3dDevice->BeginScene();

			//미니맵 카메라 설정
			g_miniMapCam.SetupMatrices();

			g_terrain.setUpMatrices();
			g_terrain.draw(false);

            g_pBall.mini_display(g_texFileName);
			g_Hole.mini_display();			
			g_Item.mini_display();

			for (int i = 0; i < _TreeList.size(); i++)
			{
				_TreeList[i]->SetupMatrices();
				_TreeList[i]->Draw();
			}

			//빛
			g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(1.0, 1.0, 1.0, 1.0));

			D3DXSaveSurfaceToFileA("BackbufferImage111.BMP", D3DXIFF_BMP, capturingTarget, NULL, NULL);

			g_pd3dDevice->EndScene();

 			g_WaitCamera.update(g_vBallPosition, g_moveWaitCamera, g_isNoKey);
			g_isNoKey = true;
			g_eType = POWERWAIT;
		}

		if (g_eType == POWERWAIT)		//각 상태별 다른 카메라클래스사용
		{
			D3DXVECTOR3 tempBallPos = g_pBall.getBallPosition();
			g_WaitCamera.keyState(timeDelta, tempBallPos);		//카메라 회전각 제한을 위해 ball의 좌표를 받는다.
			//파워 대기상태일 때 색을 바꿔준다.
			g_pBall.ColorKeyState(&g_texFileName); 
		}
		if (g_eType == READY)
		{
			g_ReadyCamera.keyState(timeDelta, &g_eType);
		}

		//메인화면으로 타겟 바꾸기
		g_pd3dDevice->SetRenderTarget(0, pBackBuffer);

		//후면 버퍼 초기화
		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

		g_pd3dDevice->BeginScene();
		
		g_terrain.setUpMatrices();
		g_terrain.draw(false);
		g_sky.SetUpMatrices();
		g_sky.Render();

		g_pBall.display(g_texFileName);

		if(!g_isEatItem)		
			g_Item.display();
		else		//item을 먹었으면 좋은점 -> 순간이동
		{
			D3DXVECTOR3 tempVec = g_Hole.getHolePosition();
			tempVec = D3DXVECTOR3(tempVec.x + 10.0f, tempVec.y+5.0f, tempVec.z + 10.0f);
			g_pBall.setInitVec(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f);	//좌표 고정	
			g_pBall.setBallPos(tempVec);
		}

		g_Hole.display();
		//count랑 power 표시
		g_DisplayCount.Display(g_iSwingCount);
		g_PowerProcess.Display(g_fPower);

		D3DXVECTOR3 ballPosition = g_pBall.getBallPosition();
		g_isEatItem = g_Item.checkItem(ballPosition);		//아이템 먹었는지 확인
		g_bisInHall = g_Hole.checkHole(ballPosition);	//홀에 들어갔으면 gameover check해주고 , swingCount가 5번이지만 들어가지 못한경우 holecheck 를 bool타입으로 해서 false true일 경우로 하자 

		if (g_bisInHall)
		{	
			g_iSwingCount++;

			return false;
		}
		else
		{
			//게임오버 체크		//swing count 5이상이라면 게임오버 화면,,,

			if (g_iSwingCount >= 15)
			{
				return false;
			}
		}

		static float timeElapsed = 0.0f;            //흐른 시간
		timeElapsed += timeDelta;

		Update(timeDelta, timeElapsed);


		for (int i = 1; i < _TreeList.size(); i++)
		{
			_TreeList[i]->SetupMatrices();
			_TreeList[i]->Draw();
		}
		D3DXSaveSurfaceToFileA("BackbufferImage111.BMP", D3DXIFF_BMP, capturingTarget, NULL, NULL);

		//사각형을 그린다
		RECT dstRect;
		dstRect.left = 40;
		dstRect.top = 30;
		dstRect.right = 280;
		dstRect.bottom = 260;
		// draw our our surface to screen
		if (!FAILED(g_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer)))
		{
			g_pd3dDevice->StretchRect(capturingTarget, NULL, pBackBuffer, &dstRect, D3DTEXF_NONE);
		}

		g_pd3dDevice->EndScene();
		g_pd3dDevice->Present(0,0,0,0);
		//후면 버퍼 해제
		//capturingTarget->Release();
		pBackBuffer->Release();
		return true;
	}
}
void DrawTrees()
{
	//미니맵 화면용 깃발-0번
	ObjMesh* _flag = new ObjMesh(g_pd3dDevice);			//깃발추가  //함수이름을 drawtrees 에서 drawobj으로 바꾸자
	_flag->Setup("resources/USMC_flag.obj");
	_flag->setPosition(200.0f, -3.5f, 270.0f);
	_flag->setScale(0.8f, 0.8f, 0.8f);
	_flag->LoadTexture("resources/lb_usmarineCorps_1.jpg");
	_TreeList.push_back(_flag);

	//메인 화면용 깃발-1번
	ObjMesh* _flag1 = new ObjMesh(g_pd3dDevice);			//깃발추가  //함수이름을 drawtrees 에서 drawobj으로 바꾸자
	_flag1->Setup("resources/USMC_flag.obj");
	_flag1->setPosition(200.0f, -3.5f, 270.0f);
	_flag1->setScale(0.2f, 0.2f, 0.2f);
	_flag1->LoadTexture("resources/lb_usmarineCorps_1.jpg");
	_TreeList.push_back(_flag1);

	ObjMesh* _tree = new ObjMesh(g_pd3dDevice);
	_tree->Setup("resources/tree_mango_var01.obj");

	_tree->setPosition(-53.0f, 0.0f, -453.0f);
	_tree->setScale(0.2, 0.2, 0.2);
	_tree->LoadTexture("obj/low_poly_tree00.png");
	_TreeList.push_back(_tree);

	ObjMesh* _tree1 = new ObjMesh(g_pd3dDevice);
	_tree1->Setup("resources/tree_mango_var02.obj");
	
	_tree1->setPosition(-90.0f, 0.0f, -460.0f);
	_tree1->setScale(0.15, 0.15, 0.15);
	_tree1->LoadTexture("obj/low_poly_tree01.png");
	_TreeList.push_back(_tree1);

	ObjMesh* _tree2 = new ObjMesh(g_pd3dDevice);
	_tree2->Setup("resources/tree_mango_var03.obj");
	
	_tree2->setPosition(-140.0f, 0.0f, -480.0f);
	_tree2->setScale(0.17, 0.17, 0.17);
	_tree2->LoadTexture("obj/low_poly_tree02.png");
	_TreeList.push_back(_tree2);

	ObjMesh* _tree6 = new ObjMesh(g_pd3dDevice);
	_tree6->Setup("resources/low_poly_tree.obj");
	_tree6->setPosition(-25.0f, 0.0f, -460.0f);
	_tree6->setScale(0.1, 0.1, 0.1);
	_tree6->LoadTexture("obj/low_poly_tree02.png");
	_TreeList.push_back(_tree6);

	ObjMesh* _tree7 = new ObjMesh(g_pd3dDevice);
	_tree7->Setup("resources/tree_mango_var03.obj");
	_tree7->setPosition(-110.0f, 0.0f, -453.0f);
	_tree7->setScale(0.17, 0.17, 0.17);
	_tree7->LoadTexture("obj/low_poly_tree00.png");
	_TreeList.push_back(_tree7);

	
	ObjMesh* _tree3 = new ObjMesh(g_pd3dDevice);
	_tree3->Setup("resources/tree_mango_var01.obj");
	
	_tree3->setPosition(370.0f, 0.0f,-59.0f);
	_tree3->setScale(0.18, 0.18, 0.18);
	_tree3->LoadTexture("obj/low_poly_tree00.png");
	_TreeList.push_back(_tree3);

	ObjMesh* _tree4 = new ObjMesh(g_pd3dDevice);
	_tree4->Setup("resources/tree_mango_var02.obj");
	
	_tree4->setPosition(400.0f, 0.0f,-75.0f);
	_tree4->setScale(0.20, 0.20, 0.20);
	_tree4->LoadTexture("obj/low_poly_tree01.png");
	_TreeList.push_back(_tree4);

	ObjMesh* _tree5 = new ObjMesh(g_pd3dDevice);
	_tree5->Setup("resources/tree_mango_var03.obj");
	
	_tree5->setPosition(320.0f, 0.0f, -100.0f);
	_tree5->setScale(0.15, 0.15, 0.15);
	_tree5->LoadTexture("obj/low_poly_tree02.png");
	_TreeList.push_back(_tree5);

}
