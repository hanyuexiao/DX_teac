#include <head.h>

#include "ModelManager.h"
#include "Scene.h"

//3d��Ⱦ���ߣ��̶����̣�

//��Ӱ����,5������ϵ��4��ת����
//1����˭����ȾĿ��---�ɻ�����������ϵ
// 
//				���������ת����WORLD�任��
// 
//2��ѡ��(ȷ������������ϵ�������Ժ��ԣ�
//				�������View�任��  D3DXMatrixLookAtLH();���λ�ã�������λ�ã����Ϸ���
//									  ��������*�������ϵ����������=�������
//										SetTransform(D3DTS_VIEW,xxxx);
//3���ںþ�ͷ����������ã���ȷ�����������ϵ
//4�����㣨��Ⱦ��--2D����ͶӰ�ռ�����ϵ������ԶС��
//5����ʾ����������ϵ��
//
//LPD3DXMESH g_pTerrainMesh = nullptr;//���εĽӿڣ�����ģ�ͣ�
//LPDIRECT3DTEXTURE9 g_pTerrainTex1=nullptr;//���ε���ͼ�ӿ�1
//
//
//LPD3DXMESH g_pMesh=nullptr;//ģ�͵Ľӿ�
//
//vector<LPDIRECT3DTEXTURE9> g_vecTextures;//��ͼ����
//
//D3DXMATERIAL* g_pMaterials = nullptr;//��������飨�ƹ���ʺ�������ʣ�������ĸ�����Ҫ��ģ���ж�ȡ
//DWORD numMaterials;
//
//
//
////�ɻ���ʼ�������������ǵģ�����ģ�͵ĳ�ʼλ�������壩
//D3DXVECTOR3 planePos(0, 0, 0);//�ɻ�λ��
//D3DXVECTOR3 planeAxisX(-1, 0, 0);//�ɻ���X��
//D3DXVECTOR3 planeAxisY(0, 1, 0);//�ɻ���Y��
//D3DXVECTOR3 planeAxisZ(0, 0, -1);//�ɻ���Z��
//
//
//
//D3DXMATRIX planeMatrix;//�ɻ�������ϵ����
//
//float xAxisAngle=0.0f;//��λ�ǻ���   3.14  =180��
//float yAxisAngle = 0.0f;
//float zAxisAngle = 0.0f;
//float moveSpeed=1.0f;
//
//D3DXMATRIX matCameraAxisMatrix;//���������ϵ����
//D3DXMATRIX m_matView;//����ģ�͵ľ���
//D3DXMATRIX	matProj;
//POINT mousePos;//����λ��
//int offsetPosX;//ƫ����X
//int offsetPosY;//ƫ����Y
//
//
////���������
//D3DXVECTOR3 vEye = D3DXVECTOR3(0, 20, -30);//���λ��
//D3DXVECTOR3 vAt = D3DXVECTOR3(0, 0, 0);//���Ŀ��λ��
//D3DXVECTOR3 vUp = D3DXVECTOR3(0, 1, 0);//������Ϸ���
//
////�������������ϵ����
//D3DXVECTOR3 CameraPos = D3DXVECTOR3(0, 20, -10);//���λ��
//D3DXVECTOR3 CameraAxisX = D3DXVECTOR3(1, 0, 0);//�����X��
//D3DXVECTOR3 CameraAxisY = D3DXVECTOR3(0, 1, 0);//�����Y��
//D3DXVECTOR3 CameraAxisZ = D3DXVECTOR3(0, 0, 1);//�����Z��
//


//float CameraXAxisAngle = 0.0f;//��λ�ǻ���   3.14  =180��
//float CameraYAxisAngle = 0.0f;
//float zAxisAngle = 0.0f;
//float CameraMoveSpeed = 1.0f;

//�¼�����Դͷ����
LRESULT EventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
	case WM_DESTROY://���ڱ�����
	{
		PostQuitMessage(0);//�˳���Ϣ
		break;
	}
	case WM_LBUTTONDOWN://�����������¼�	
	case WM_LBUTTONUP://������̧���¼�
	case WM_MOUSEMOVE://����ƶ��¼�
	case WM_KEYDOWN://�¼������ӳٵģ��ʺ��󿪹أ�
	{
		switch (wParam)//��ⰴ���ļ���ֵ
		{
		case VK_ESCAPE://���»س���
			PostQuitMessage(0);//�˳���Ϣ
			break;

		}
		CScene::GetSingleObjPtr()->EventProcess(msg,wParam,lParam);

	}

	//lparam��  ���ֽڱ�������Y���꣬���ֽڱ�������X����

	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//
////�첽�¼�
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
//		//�ɻ���ǰ�ƶ�
//		planePos -= planeAxisZ * moveSpeed;
//
//	}
//	if (GetAsyncKeyState('V'))
//	{
//		//�ɻ�����ƶ�
//		planePos += planeAxisZ * moveSpeed;
//	}
//
//	//��������Ķ���
//	//��������ƶ��������ƶ���������ƫ����ת������ת����
//	D3DXMATRIX rotMatrix;
//
//	////������ת�߼������ķ���
//	if (xAxisAngle)
//	{
//		//�����߼���һ��Ҫ���¸�ֵΪ0
//		//����ת����Χ�Ʒɻ���X����ת��
//		D3DXMatrixRotationAxis(&rotMatrix, &planeAxisX, xAxisAngle);
//		//D3DXMatrixRotationX ��X�ᣬ���������X�ᣬ��Զ�̶��ģ�1��0��0�������Բ���ʹ�ù̶�������ת
//		//��Y��Z
//		D3DXVec3TransformNormal(&planeAxisY, &planeAxisY, &rotMatrix);
//		D3DXVec3TransformNormal(&planeAxisZ, &planeAxisZ, &rotMatrix);
//		xAxisAngle = 0;
//	}
//
//	if (yAxisAngle)
//	{
//		//�����߼���һ��Ҫ���¸�ֵΪ0
//		//����ת����Χ�Ʒɻ���Y����ת��
//		D3DXMatrixRotationAxis(&rotMatrix, &planeAxisY, yAxisAngle);
//		//��X��Z
//		D3DXVec3TransformNormal(&planeAxisX, &planeAxisX, &rotMatrix);
//		D3DXVec3TransformNormal(&planeAxisZ, &planeAxisZ, &rotMatrix);
//		yAxisAngle = 0;
//	}
//
//	if (zAxisAngle)
//	{
//		//�����߼���һ��Ҫ���¸�ֵΪ0
//		//����ת����Χ�Ʒɻ���Z����ת��
//		D3DXMatrixRotationAxis(&rotMatrix, &planeAxisZ, zAxisAngle);
//		//��X��Y
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
//	D3DXVECTOR3 pos;//λ��
//	DWORD color;//��ɫ
//	D3DXVECTOR2 texCoord;//�������꣨�������ͼ�Ļ���
//};
////���������ʽ
//#define MYCUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE|D3DFVF_TEX1) //�Զ��嶥���ʽ��λ�ã���ɫ��




INT _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{

	//��ʼ������
	HWND hwnd=CGraphic::GetSingleObjPtr()->InitWindow(EventProc,800,600,_T("DX_3D"));
	CGraphic::GetSingleObjPtr()->InitDX(hwnd);

	//����ģ�Ϳ��ļ�
	CModelManager::GetSingleObjPtr()->Init(_T("./resource/3D/config/models.xml"));

	//һ���Լ��س��������ļ�ini
	CScene::GetSingleObjPtr()->InitScene(_T("./resource/3D/config/sceneInfo.ini"));


	
	//1����˭����ȾĿ��---�ɻ�����������ϵ
	//D3DXCreateTeapot(CGraphic::GetSingleObjPtr()->m_pDevice,&g_pMesh,nullptr);

	//LPD3DXBUFFER materialsBuff;
	////D3DXCreateBox()
	////����ģ��FBX  ��
	////D3DXLoadMeshFromX(_T("./resource/3D/binglong.x"),D3DXMESH_MANAGED, CGraphic::GetSingleObjPtr()->m_pDevice,0, &materialsBuff,0, &numMaterials,&g_pMesh);
	//D3DXLoadMeshFromX(_T("./resource/3D/airplane2.x"), D3DXMESH_MANAGED, CGraphic::GetSingleObjPtr()->m_pDevice, 0, &materialsBuff, 0, &numMaterials, &g_pMesh);
	////ȡ���ɻ�ģ�͵Ķ�������
	//void *arrayPlaneVertex;
	//g_pMesh->LockVertexBuffer(0,&arrayPlaneVertex);
	//g_pMesh->UnlockVertexBuffer();

	//D3DXVECTOR3 minVPos,maxVPos;

	////����ģ�͵ı�������İ�Χ������
	//								 //��������                ������                       ����������ֽ���         ���ذ�Χ��ģ�͵���С�����ֵ
	////D3DXComputeBoundingBox((D3DXVECTOR3*)arrayPlaneVertex,g_pMesh->GetNumVertices(),g_pMesh->GetNumBytesPerVertex(),&minVPos,&maxVPos);

	////D3DXComputeBoundingSphere()
	////������С�����ֵ����box�Ķ�������

	////ģ�����о���ƫ�Ƶģ���Χ�е�����������Ҫ�������

	////���ջ��Ƶ�����������İ�Χ��
	//// 
	//// 
	////�Զ���ģ��  FVF(Fexible Vertex Format)��ʽ  ���꣬���ߣ��������꣬���ʣ���ɫ����������
	//D3DXCreateMeshFVF(2,4,D3DXMESH_MANAGED, MYCUSTOMFVF,CGraphic::GetSingleObjPtr()->m_pDevice,&g_pTerrainMesh);

	////������ͼ
	//D3DXCreateTextureFromFile(CGraphic::GetSingleObjPtr()->m_pDevice,_T("./resource/3D/Grass.jpg"),&g_pTerrainTex1);
	////4�������2�������εĿռ�����������Ҫ����ȥ��ֵ�������������
	//MYCUSTOMVERTEX* pVertices;//��������
	//WORD* pIndex;//��������(���������������WORD����ʾ�����ĸ������ܳ���65535��)
	//g_pTerrainMesh->LockVertexBuffer(0,(void**) & pVertices);
	////���������鸳ֵ
	//pVertices[0].pos = D3DXVECTOR3(-5.0f, 5.0f,0); pVertices[0].color = D3DCOLOR_ARGB(88,255, 0, 0); pVertices[0].texCoord=D3DXVECTOR2(0,0);  //���Ͻ�
	//pVertices[1].pos = D3DXVECTOR3( 5.0f, 5.0f,0); pVertices[1].color = D3DCOLOR_ARGB(88,255, 0, 0); pVertices[1].texCoord = D3DXVECTOR2(5,0);//���Ͻ�
	//pVertices[2].pos = D3DXVECTOR3(-5.0f,-5.0f,0); pVertices[2].color = D3DCOLOR_ARGB(88,255, 0, 0); pVertices[2].texCoord = D3DXVECTOR2(0,5);//���½�
	//pVertices[3].pos = D3DXVECTOR3( 5.0f,-5.0f,0); pVertices[3].color = D3DCOLOR_ARGB(88,255, 0, 0); pVertices[3].texCoord = D3DXVECTOR2(5,5);//���½�

	//g_pTerrainMesh->UnlockVertexBuffer();//֪ͨ�Կ������Զ�ȡ�ÿ�����

	//g_pTerrainMesh->LockIndexBuffer(0, (void**)&pIndex);//������������
	////��������ֵ
	//pIndex[0] = 0; pIndex[1] = 1; pIndex[2] = 2;//��һ��������
	//pIndex[3] = 1; pIndex[4] = 3; pIndex[5] = 2;//�ڶ���������
	//g_pTerrainMesh->UnlockIndexBuffer();

	////DWORD* attri;//������������
	////g_pTerrainMesh->LockAttributeBuffer(0,&attri);
	////attri[0]=0; attri[1] = 0;
	////attri[2] = 1; attri[3] = 1;
	////attri[4] = 2; attri[5] = 2;
	////attri[6] = 3; attri[7] = 3;
	////attri[8] = 4; attri[9] = 4;
	////attri[10] = 5; attri[11] = 6;
	////g_pTerrainMesh->UnlockAttributeBuffer();
	////��ʼ��ֲ������ݣ��ƹ���ʺ���ͼ���ʣ�
	//// 
	//g_pMaterials=(D3DXMATERIAL * )materialsBuff->GetBufferPointer();//�õ�����������Ϣ�Ľṹ�������ָ��

	//for (int i = 0; i < numMaterials; i++)
	//{
	//	if(g_pMaterials[i].pTextureFilename)
	//	{
	//		//������ͼ
	//		LPDIRECT3DTEXTURE9 pTexture;

	//		string path="./resource/3D/";
	//		path += g_pMaterials[i].pTextureFilename;

	//		if(FAILED(D3DXCreateTextureFromFileA(CGraphic::GetSingleObjPtr()->m_pDevice, path.c_str(), &pTexture)))
	//			g_vecTextures.push_back(nullptr);//û����ͼ
	//		g_vecTextures.push_back(pTexture);//������ͼ
	//	}
	//	else
	//	{
	//		g_vecTextures.push_back(nullptr);//û����ͼ
	//	}
	//}

	//
	//��Ⱦ״̬������
	//�رչ���
	CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_LIGHTING,false);

	//��Ⱦģʽ(�㣬�߿�ʵ�ģ�
	CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);//�߿�ģʽ	D3DFILL_WIREFRAME
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
		//_kbhit()�첽�������¼�

			static DWORD preTime=GetTickCount();
			DWORD curTime=GetTickCount();

			float elapsedTime=(curTime-preTime)*0.001;
			if (elapsedTime > 0.016)
			{
				//AsyncEvent();

				//������ĸ��º���
				CScene::GetSingleObjPtr()->Update(elapsedTime);

				
				CGraphic::GetSingleObjPtr()->BeginDraw();
				//�����Ŀɱ��߼�
					//3���ںþ�ͷ����������ã���ȷ�����������ϵ

				//������Ļ��ƺ���
				CScene::GetSingleObjPtr()->Render();
				//һ���Զ�������վ���
			//����Ķ����������������ϵ��������ʾ

				//1 0 0 0    m_vecAxisX=(1,0,0)   0
				//0 1 0 0	 m_vecAxisY=(0,1,0)   0
				//0 0 1 0	 m_vecAxisZ=(0,0,1)   0
				//0 0 0 1	 m_vecPos  =(0,0,0)   1
			//���еĶ����߼�д�ڻ���֮ǰ
				//D3DXMatrixIdentity(&planeMatrix);

				

				//planeMatrix._11 = planeAxisX.x; planeMatrix._12 = planeAxisX.y; planeMatrix._13 = planeAxisX.z; planeMatrix._14 = 0;
				//planeMatrix._21 = planeAxisY.x; planeMatrix._22 = planeAxisY.y; planeMatrix._23 = planeAxisY.z; planeMatrix._24 = 0;
				//planeMatrix._31 = planeAxisZ.x; planeMatrix._32 = planeAxisZ.y; planeMatrix._33 = planeAxisZ.z; planeMatrix._34 = 0;
				//planeMatrix._41 = planePos.x;   planeMatrix._42 = planePos.y;   planeMatrix._43 = planePos.z;   planeMatrix._44 = 1;

				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTransform(D3DTS_WORLD, &planeMatrix);//Ӧ�þ���ʵ������任������-�����磩
				////6������

				//for (int i = 0; i < numMaterials; i++)
				//{

				//	CGraphic::GetSingleObjPtr()->m_pDevice->SetTexture(0, g_vecTextures[i]);//������ͼ
				//	g_pMesh->DrawSubset(i);//��������	

				//}


				//���Ƶ���
					//����alpha���				SRC*f+Dest*��1-f);  
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);//����alpha���
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);//SRC������Ҫ���Ƶ�����
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);//dest����������֪������

				//alpha���ԣ����Զ����alphaֵȥ�Ƚϡ�ͨ���ͻ��ƣ���ͨ����͸��
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);//����alpha����
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);//����
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetRenderState(D3DRS_ALPHAREF, 87);//�Զ����alphaֵ��0-255��  0x88=136
				//D3DXMATRIX matTerrain;
				//D3DXMatrixIdentity(&matTerrain);
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTransform(D3DTS_WORLD, &matTerrain);
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTexture(0, g_pTerrainTex1);

				////������ͼ�Ͷ�����ɫ�ںϵķ�����һ������ɫ�ںϣ�һ����͸���ںϣ�6�仰
				//							//Ĭ���ںϷ�������ɫ��������ںϣ�Alpha����ֻȡ��ͼ��alpha
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);//��ɫ����ں�  ARG1*ARG2
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);//��ͼ����ɫ
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);//������ɫ


				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);//Ĭ��alpha��������ͼ
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);//��ͼ����ɫ
				//CGraphic::GetSingleObjPtr()->m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);//������ɫ


				//g_pTerrainMesh->DrawSubset(0);
				////�������
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



//��������==����ϵ����Ĺ�ϵ
//��������Ǿ�̬�ģ�����ֱ��ʹ�ö�������ƽ�ƣ���ת�����ţ�
			//ƽ�ƣ�D3DXMatrixTranslation( D3DXMATRIX *pOut, FLOAT x, FLOAT y, FLOAT z )
			//��ת��
			//	����
			//		D3DXMatrixRotationX(D3DXMATRIX* pOut, FLOAT Angle)
			//		D3DXMatrixRotationY(D3DXMATRIX* pOut, FLOAT Angle)
			//		D3DXMatrixRotationZ(D3DXMATRIX* pOut, FLOAT Angle)
			//		D3DXMatrixRotationAxis( D3DXMATRIX *pOut, CONST D3DXVECTOR3 *pV, FLOAT Angle )//ָ��������
			//		D3DXMatrixRotationYawPitchRoll(D3DXMATRIX* pOut, FLOAT yaw, FLOAT pitch, FLOAT roll);//������ת����Y����ת��Yawƫ��)��X����ת(Pitch��б)��Z����ת(Roll����)��
			//	��Ԫ����A = a + bi; (����)ai + bj + ck + d; (i, j, k��������λ)
			//		D3DXQUATERNION(a, b, c, d);    
			//		D3DXMatrixRotationQuaternion(D3DXMATRIX* pOut, CONST D3DXQUATERNION* pQ);//��Ԫ��ת����
			//		D3DXQUaternionRotationMatrix(D3DXQUATERNION* pOut, CONST D3DXMATRIX* pM);//����ת��Ԫ��

			//		//D3DXQUATERNION(v, W);//v��ת������w��ת�Ƕ�
			//		D3DXQuaternionRotationAxis( D3DXQUATERNION *pOut, CONST D3DXVECTOR3 *pV, FLOAT Angle )
			/*���ţ�
					D3DXMatrixScaling( D3DXMATRIX *pOut, FLOAT sx, FLOAT sy, FLOAT sz );*/
					//���϶�����ƽ��+��ת+���ţ�    d=a*b*c(����*��ת*ƽ��)     
					//����Χ��̫��ת��������ת+��ת��    ���ӹ�ϵ����Σ�    ����Ҫ�̳и��׵Ķ���     
																	//����Ķ�������=����ı�������ϵ����*̫���Ķ�������
																	//������������̫���ƶ�

					//���ﶯ������ζ�����    �������������������ز����ز������ӣ����ӣ�ͷ��
											//		:�β������ȣ����ȣ�С�ȣ�С�ȣ���

//��������Ƕ�̬�ģ��ܰ����Ŀ��ƣ�����ʹ������ϵ����ƽ�ƣ���ת�����ţ���һ���Զ�������վ���


					//����Ķ����������������ϵ��������ʾ

							

							//�ɻ�ģ�͵��˶����ƣ�AD   ʵ�� ����(Z ����ת��
												  /*WS   ʵ�� ƫ����X����ת��
												  QE   ʵ�� ������ת��б��Y����ת��
												  FV   ʵ��ǰ���ƶ���ƽ�ƣ�*/
													