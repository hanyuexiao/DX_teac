#include <head.h>

#include "ModelManager.h"
#include "Scene.h"

//3d渲染管线（固定流程）

//摄影流程,5种坐标系（4次转换）
//1、拍谁（渲染目标---飞机）本地坐标系
// 
//				动画矩阵的转换（WORLD变换）
// 
//2、选景(确定好世界坐标系）（可以忽略）
//				相机矩阵（View变换）  D3DXMatrixLookAtLH();相机位置，被看点位置，朝上方向
//									  世界坐标*相机坐标系矩阵的逆矩阵=相对坐标
//										SetTransform(D3DTS_VIEW,xxxx);
//3、摆好镜头（相机的设置），确定好相机坐标系
//4、拍摄（渲染）--2D成像，投影空间坐标系（近大远小）
//5、显示（窗口坐标系）
//
//LPD3DXMESH g_pTerrainMesh = nullptr;//地形的接口（网格模型）
//LPDIRECT3DTEXTURE9 g_pTerrainTex1=nullptr;//地形的贴图接口1
//
//
//LPD3DXMESH g_pMesh=nullptr;//模型的接口
//
//vector<LPDIRECT3DTEXTURE9> g_vecTextures;//贴图数组
//
//D3DXMATERIAL* g_pMaterials = nullptr;//大材质数组（灯光材质和纹理材质），具体的个数需要从模型中读取
//DWORD numMaterials;
//
//
//
////飞机初始方向是面向我们的（根据模型的初始位置来定义）
//D3DXVECTOR3 planePos(0, 0, 0);//飞机位置
//D3DXVECTOR3 planeAxisX(-1, 0, 0);//飞机的X轴
//D3DXVECTOR3 planeAxisY(0, 1, 0);//飞机的Y轴
//D3DXVECTOR3 planeAxisZ(0, 0, -1);//飞机的Z轴
//
//
//
//D3DXMATRIX planeMatrix;//飞机的坐标系矩阵
//
//float xAxisAngle=0.0f;//单位是弧度   3.14  =180度
//float yAxisAngle = 0.0f;
//float zAxisAngle = 0.0f;
//float moveSpeed=1.0f;
//
//D3DXMATRIX matCameraAxisMatrix;//相机的坐标系矩阵
//D3DXMATRIX m_matView;//绘制模型的矩阵
//D3DXMATRIX	matProj;
//POINT mousePos;//鼠标的位置
//int offsetPosX;//偏移量X
//int offsetPosY;//偏移量Y
//
//
////相机的设置
//D3DXVECTOR3 vEye = D3DXVECTOR3(0, 20, -30);//相机位置
//D3DXVECTOR3 vAt = D3DXVECTOR3(0, 0, 0);//相机目标位置
//D3DXVECTOR3 vUp = D3DXVECTOR3(0, 1, 0);//相机的上方向
//
////定义相机的坐标系参数
//D3DXVECTOR3 CameraPos = D3DXVECTOR3(0, 20, -10);//相机位置
//D3DXVECTOR3 CameraAxisX = D3DXVECTOR3(1, 0, 0);//相机的X轴
//D3DXVECTOR3 CameraAxisY = D3DXVECTOR3(0, 1, 0);//相机的Y轴
//D3DXVECTOR3 CameraAxisZ = D3DXVECTOR3(0, 0, 1);//相机的Z轴
//


//float CameraXAxisAngle = 0.0f;//单位是弧度   3.14  =180度
//float CameraYAxisAngle = 0.0f;
//float zAxisAngle = 0.0f;
//float CameraMoveSpeed = 1.0f;

//事件处理源头函数
LRESULT EventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
	case WM_DESTROY://窗口被销毁
	{
		PostQuitMessage(0);//退出消息
		break;
	}
	case WM_LBUTTONDOWN://鼠标左键按下事件	
	case WM_LBUTTONUP://鼠标左键抬起事件
	case WM_MOUSEMOVE://鼠标移动事件
	case WM_KEYDOWN://事件是由延迟的（适合左开关）
	{
		switch (wParam)//检测按键的键码值
		{
		case VK_ESCAPE://按下回车键
			PostQuitMessage(0);//退出消息
			break;

		}
		CScene::GetSingleObjPtr()->EventProcess(msg,wParam,lParam);

	}

	//lparam中  高字节保存鼠标的Y坐标，低字节保存鼠标的X坐标

	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//
////异步事件
//void AsyncEvent()
//{
//	if (GetAsyncKeyState('W'))
//	{
//		xAxisAngle=-0.1f;
//	}
//	if (GetAsyncKeyState('S'))
//	{
//		xAxisAngle = 0.1f;
//	}
//	if (GetAsyncKeyState('A'))
//	{
//		zAxisAngle= -0.1f;
//	}
//	if (GetAsyncKeyState('D'))
//	{
//		zAxisAngle = 0.1f;
//	}
//	if (GetAsyncKeyState('Q'))
//	{
//		yAxisAngle = -0.1f;
//	}
//	if (GetAsyncKeyState('E'))
//	{
//		yAxisAngle = 0.1f;
//
//	}
//
//	if (GetAsyncKeyState('F'))
//	{
//		//飞机向前移动
//		planePos -= planeAxisZ * moveSpeed;
//
//	}
//	if (GetAsyncKeyState('V'))
//	{
//		//飞机向后移动
//		planePos += planeAxisZ * moveSpeed;
//	}
//
//	//控制相机的动画
//	//鼠标左右移动，上下移动。把鼠标的偏移量转换成旋转弧度
//	D3DXMATRIX rotMatrix;
//
//	////处理旋转逻辑，更改方向
//	if (xAxisAngle)
//	{
//		//做完逻辑后，一定要重新赋值为0
//		//求旋转矩阵（围绕飞机的X轴旋转）
//		D3DXMatrixRotationAxis(&rotMatrix, &planeAxisX, xAxisAngle);
//		//D3DXMatrixRotationX 该X轴，代表世界的X轴，永远固定的（1，0，0）；所以不能使用固定的轴旋转
//		//改Y，Z
//		D3DXVec3TransformNormal(&planeAxisY, &planeAxisY, &rotMatrix);
//		D3DXVec3TransformNormal(&planeAxisZ, &planeAxisZ, &rotMatrix);
//		xAxisAngle = 0;
//	}
//
//	if (yAxisAngle)
//	{
//		//做完逻辑后，一定要重新赋值为0
//		//求旋转矩阵（围绕飞机的Y轴旋转）
//		D3DXMatrixRotationAxis(&rotMatrix, &planeAxisY, yAxisAngle);
//		//改X，Z
//		D3DXVec3TransformNormal(&planeAxisX, &planeAxisX, &rotMatrix);
//		D3DXVec3TransformNormal(&planeAxisZ, &planeAxisZ, &rotMatrix);
//		yAxisAngle = 0;
//	}
//
//	if (zAxisAngle)
//	{
//		//做完逻辑后，一定要重新赋值为0
//		//求旋转矩阵（围绕飞机的Z轴旋转）
//		D3DXMatrixRotationAxis(&rotMatrix, &planeAxisZ, zAxisAngle);
//		//改X，Y
//		D3DXVec3TransformNormal(&planeAxisX, &planeAxisX, &rotMatrix);
//		D3DXVec3TransformNormal(&planeAxisY, &planeAxisY, &rotMatrix);
//		zAxisAngle = 0;
//	}
//
//
//
//
//	
//}
//
//struct MYCUSTOMVERTEX
//{
//	D3DXVECTOR3 pos;//位置
//	DWORD color;//颜色
//	D3DXVECTOR2 texCoord;//纹理坐标（如果有贴图的话）
//};
////声明灵活顶点格式
//#define MYCUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE|D3DFVF_TEX1) //自定义顶点格式（位置，颜色）




INT _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{

	//初始化窗口
	HWND hwnd=CGraphic::GetSingleObjPtr()->InitWindow(EventProc,800,600,_T("DX_3D"));
	CGraphic::GetSingleObjPtr()->InitDX(hwnd);

	//加载模型库文件
	CModelManager::GetSingleObjPtr()->Init(_T("./resource/3D/config/models.xml"));

	//一次性加载场景配置文件ini
	CScene::GetSingleObjPtr()->InitScene(_T("./resource/3D/config/sceneInfo.ini"));


	
	//1、拍谁（渲染目标---飞机）本地坐标系
	//D3DXCreateTeapot(CGraphic::GetSingleObjPtr()->m_pDevice,&g_pMesh,nullptr);

	//LPD3DXBUFFER materialsBuff;
	////D3DXCreateBox()
	////加载模型FBX  库
	////D3DXLoadMeshFromX(_T("./resource/3D/binglong.x"),D3DXMESH_MANAGED, CGraphic::GetSingleObjPtr()->m_pDevice,0, &materialsBuff,0, &numMaterials,&g_pMesh);
	//D3DXLoadMeshFromX(_T("./resource/3D/airplane2.x"), D3DXMESH_MANAGED, CGraphic::GetSingleObjPtr()->m_pDevice, 0, &materialsBuff, 0, &numMaterials, &g_pMesh);
	////取出飞机模型的顶点数组
	//void *arrayPlaneVertex;
	//g_pMesh->LockVertexBuffer(0,&arrayPlaneVertex);
	//g_pMesh->UnlockVertexBuffer();

	//D3DXVECTOR3 minVPos,maxVPos;

	////计算模型的本地坐标的包围盒坐标
	//								 //顶点数组                顶点数                       单个顶点的字节数         返回包围盒模型的最小和最大值
	////D3DXComputeBoundingBox((D3DXVECTOR3*)arrayPlaneVertex,g_pMesh->GetNumVertices(),g_pMesh->GetNumBytesPerVertex(),&minVPos,&maxVPos);

	////D3DXComputeBoundingSphere()
	////根据最小和最大值定义box的顶点数组

	////模型是有矩阵偏移的，包围盒的世界坐标需要计算出来

	////最终绘制的是世界坐标的包围盒
	//// 
	//// 
	////自定义模型  FVF(Fexible Vertex Format)格式  坐标，法线，纹理坐标，材质，颜色。。。。。
	//D3DXCreateMeshFVF(2,4,D3DXMESH_MANAGED, MYCUSTOMFVF,CGraphic::GetSingleObjPtr()->m_pDevice,&g_pTerrainMesh);

	////加载贴图
	//D3DXCreateTextureFromFile(CGraphic::GetSingleObjPtr()->m_pDevice,_T("./resource/3D/Grass.jpg"),&g_pTerrainTex1);
	////4个顶点和2个三角形的空间分配出来，需要主动去赋值（顶点和索引）
	//MYCUSTOMVERTEX* pVertices;//顶点数组
	//WORD* pIndex;//索引数组(索引数组的类型是WORD，表示索引的个数不能超过65535个)
	//g_pTerrainMesh->LockVertexBuffer(0,(void**) & pVertices);
	////给顶点数组赋值
	//pVertices[0].pos = D3DXVECTOR3(-5.0f, 5.0f,0); pVertices[0].color = D3DCOLOR_ARGB(88,255, 0, 0); pVertices[0].texCoord=D3DXVECTOR2(0,0);  //左上角
	//pVertices[1].pos = D3DXVECTOR3( 5.0f, 5.0f,0); pVertices[1].color = D3DCOLOR_ARGB(88,255, 0, 0); pVertices[1].texCoord = D3DXVECTOR2(5,0);//右上角
	//pVertices[2].pos = D3DXVECTOR3(-5.0f,-5.0f,0); pVertices[2].color = D3DCOLOR_ARGB(88,255, 0, 0); pVertices[2].texCoord = D3DXVECTOR2(0,5);//左下角
	//pVertices[3].pos = D3DXVECTOR3( 5.0f,-5.0f,0); pVertices[3].color = D3DCOLOR_ARGB(88,255, 0, 0); pVertices[3].texCoord = D3DXVECTOR2(5,5);//右下角

	//g_pTerrainMesh->UnlockVertexBuffer();//通知显卡，可以读取该块数据

	//g_pTerrainMesh->LockIndexBuffer(0, (void**)&pIndex);//锁定索引数组
	////给索引赋值
	//pIndex[0] = 0; pIndex[1] = 1; pIndex[2] = 2;//第一个三角形
	//pIndex[3] = 1; pIndex[4] = 3; pIndex[5] = 2;//第二个三角形
	//g_pTerrainMesh->UnlockIndexBuffer();

	////DWORD* attri;//材质属性数组
	////g_pTerrainMesh->LockAttributeBuffer(0,&attri);
	////attri[0]=0; attri[1] = 0;
	////attri[2] = 1; attri[3] = 1;
	////attri[4] = 2; attri[5] = 2;
	////attri[6] = 3; attri[7] = 3;
	////attri[8] = 4; attri[9] = 4;
	////attri[10] = 5; attri[11] = 6;
	////g_pTerrainMesh->UnlockAttributeBuffer();
	////开始拆分材质内容（灯光材质和贴图材质）
	//// 
	//g_pMaterials=(D3DXMATERIAL * )materialsBuff->GetBufferPointer();//得到保存大材质信息的结构体数组的指针

	//for (int i = 0; i < numMaterials; i++)
	//{
	//	if(g_pMaterials[i].pTextureFilename)
	//	{
	//		//加载贴图
	//		LPDIRECT3DTEXTURE9 pTexture;

	//		string path="./resource/3D/";
	//		path += g_pMaterials[i].pTextureFilename;

	//		if(FAILED(D3DXCreateTextureFromFileA(CGraphic::GetSingleObjPtr()->m_pDevice, path.c_str(), &pTexture)))
	//			g_vecTextures.push_back(nullptr);//没有贴图
	//		g_vecTextures.push_back(pTexture);//保存贴图
	//	}
	//	else
	//	{
	//		g_vecTextures.push_back(nullptr);//没有贴图
	//	}
	//}

	//
	//渲染状态的设置
	//关闭光照
	CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_LIGHTING,false);

	//渲染模式(点，线框，实心）
	CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);//线框模式	D3DFILL_WIREFRAME
	while (TRUE)
	{
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
		//_kbhit()异步处理按键事件

			static DWORD preTime=GetTickCount();
			DWORD curTime=GetTickCount();

			float elapsedTime=(curTime-preTime)*0.001;
			if (elapsedTime > 0.016)
			{
				//AsyncEvent();

				//场景类的更新函数
				CScene::GetSingleObjPtr()->Update(elapsedTime);

				
				CGraphic::GetSingleObjPtr()->BeginDraw();
				//公共的可变逻辑
					//3、摆好镜头（相机的设置），确定好相机坐标系

				//场景类的绘制函数
				CScene::GetSingleObjPtr()->Render();
				//一次性定义好最终矩阵
			//物体的动画矩阵可以用坐标系矩阵来表示

				//1 0 0 0    m_vecAxisX=(1,0,0)   0
				//0 1 0 0	 m_vecAxisY=(0,1,0)   0
				//0 0 1 0	 m_vecAxisZ=(0,0,1)   0
				//0 0 0 1	 m_vecPos  =(0,0,0)   1
			//特有的动画逻辑写在绘制之前
				//D3DXMatrixIdentity(&planeMatrix);

				

				//planeMatrix._11 = planeAxisX.x; planeMatrix._12 = planeAxisX.y; planeMatrix._13 = planeAxisX.z; planeMatrix._14 = 0;
				//planeMatrix._21 = planeAxisY.x; planeMatrix._22 = planeAxisY.y; planeMatrix._23 = planeAxisY.z; planeMatrix._24 = 0;
				//planeMatrix._31 = planeAxisZ.x; planeMatrix._32 = planeAxisZ.y; planeMatrix._33 = planeAxisZ.z; planeMatrix._34 = 0;
				//planeMatrix._41 = planePos.x;   planeMatrix._42 = planePos.y;   planeMatrix._43 = planePos.z;   planeMatrix._44 = 1;

				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTransform(D3DTS_WORLD, &planeMatrix);//应用矩阵，实现世界变换（本地-》世界）
				////6、绘制

				//for (int i = 0; i < numMaterials; i++)
				//{

				//	CGraphic::GetSingleObjPtr()->m_pDevice->SetTexture(0, g_vecTextures[i]);//设置贴图
				//	g_pMesh->DrawSubset(i);//绘制网格	

				//}


				//绘制地形
					//激活alpha混合				SRC*f+Dest*（1-f);  
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);//开启alpha混合
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);//SRC代表即将要绘制的物体
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);//dest代表场景中已知的物体

				//alpha测试，跟自定义的alpha值去比较。通过就绘制，不通过就透明
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);//开启alpha测试
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);//大于
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_ALPHAREF, 87);//自定义的alpha值（0-255）  0x88=136
				//D3DXMATRIX matTerrain;
				//D3DXMatrixIdentity(&matTerrain);
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTransform(D3DTS_WORLD, &matTerrain);
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTexture(0, g_pTerrainTex1);

				////设置贴图和顶点颜色融合的方法（一种是颜色融合，一种是透明融合）6句话
				//							//默认融合方法：颜色部分相乘融合，Alpha部分只取贴图的alpha
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);//颜色相乘融合  ARG1*ARG2
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);//贴图的颜色
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);//顶点颜色


				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);//默认alpha来自于贴图
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);//贴图的颜色
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);//顶点颜色


				//g_pTerrainMesh->DrawSubset(0);
				////绘制天空
				//D3DXMATRIX matSky;
				//D3DXMatrixIdentity(&matSky);
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTransform(D3DTS_WORLD, &matSky);
				//for (int i = 0; i < 6; i++)
				//{
				//	
				//	//CGraphic::GetSingleObjPtr()->m_pDevice->SetTexture(0, g_pSkyTex[i]);
				//	g_pTerrainMesh->DrawSubset(i);
				//}

				CGraphic::GetSingleObjPtr()->EndDraw();
				preTime=curTime;
			}
			else
				Sleep(1);
		
		}

		if (msg.message == WM_QUIT)
			break;
	}
		return 0;
}



//动画矩阵==坐标系矩阵的关系
//如果物体是静态的，可以直接使用动画矩阵（平移，旋转，缩放）
			//平移：D3DXMatrixTranslation( D3DXMATRIX *pOut, FLOAT x, FLOAT y, FLOAT z )
			//旋转：
			//	矩阵：
			//		D3DXMatrixRotationX(D3DXMATRIX* pOut, FLOAT Angle)
			//		D3DXMatrixRotationY(D3DXMATRIX* pOut, FLOAT Angle)
			//		D3DXMatrixRotationZ(D3DXMATRIX* pOut, FLOAT Angle)
			//		D3DXMatrixRotationAxis( D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pV, FLOAT Angle )//指定任意轴
			//		D3DXMatrixRotationYawPitchRoll(D3DXMATRIX* pOut, FLOAT yaw, FLOAT pitch, FLOAT roll);//复合旋转（绕Y轴旋转（Yaw偏航)，X轴旋转(Pitch倾斜)，Z轴旋转(Roll翻滚)）
			//	四元数：A = a + bi; (复数)ai + bj + ck + d; (i, j, k是虚数单位)
			//		D3DXQUATERNION(a, b, c, d);    
			//		D3DXMatrixRotationQuaternion(D3DXMATRIX* pOut, CONST D3DXQUATERNION* pQ);//四元数转矩阵
			//		D3DXQUaternionRotationMatrix(D3DXQUATERNION* pOut, CONST D3DXMATRIX* pM);//矩阵转四元数

			//		//D3DXQUATERNION(v, W);//v旋转向量，w旋转角度
			//		D3DXQuaternionRotationAxis( D3DXQUATERNION *pOut, CONST D3DXVECTOR3 *pV, FLOAT Angle )
			/*缩放：
					D3DXMatrixScaling( D3DXMATRIX *pOut, FLOAT sx, FLOAT sy, FLOAT sz );*/
					//复合动画（平移+旋转+缩放）    d=a*b*c(缩放*旋转*平移)     
					//地球围绕太阳转（地球自转+公转）    父子关系（层次）    地球要继承父亲的动画     
																	//地球的动画矩阵=地球的本地坐标系矩阵*太阳的动画矩阵
																	//单独按键控制太阳移动

					//人物动画（层次动画）    腰部：腹部，腹部：胸部，胸部：脖子，脖子：头部
											//		:臀部：大腿，大腿：小腿，小腿：脚

//如果物体是动态的，受按键的控制，可以使用坐标系矩阵（平移，旋转，缩放），一次性定义好最终矩阵


					//物体的动画矩阵可以用坐标系矩阵来表示

							

							//飞机模型的运动控制：AD   实现 翻滚(Z 轴旋转）
												  /*WS   实现 偏航（X轴旋转）
												  QE   实现 左右旋转倾斜（Y轴旋转）
												  FV   实现前后移动（平移）*/
													