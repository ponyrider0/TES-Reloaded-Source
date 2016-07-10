#include <nvapi.h>
#if defined(SKYRIM)
#include "skse\NiNodes.h"
#endif

#define RESZ_CODE 0x7fa05000

void RenderManager::GetCameraData()
{

	SceneGraph* WorldSceneGraph = *(SceneGraph**)kWorldSceneGraph;
	NiCamera* Camera = WorldSceneGraph->camera;

	if (Camera) {
		NiMatrix33* WorldRotate = &Camera->m_worldTransform.rot;
		NiPoint3* WorldTranslate = &Camera->m_worldTransform.pos;
		
		CameraForward.x = WorldRotate->data[0][0];
		CameraForward.y = WorldRotate->data[1][0];
		CameraForward.z = WorldRotate->data[2][0];
		CameraPosition.x = WorldTranslate->x;
		CameraPosition.y = WorldTranslate->y;
		CameraPosition.z = WorldTranslate->z;
	}

}

void RenderManager::SetCameraData()
{
	
	SceneGraph* WorldSceneGraph = *(SceneGraph**)kWorldSceneGraph;
	NiCamera* Camera = WorldSceneGraph->camera;

	if (Camera) {
		NiMatrix33* WorldRotate = &Camera->m_worldTransform.rot;
		NiPoint3* WorldTranslate = &Camera->m_worldTransform.pos;
		NiPoint3 Forward;
		NiPoint3 Up;
		NiPoint3 Right;

		Forward.x = WorldRotate->data[0][0];
		Forward.y = WorldRotate->data[1][0];
		Forward.z = WorldRotate->data[2][0];
		Up.x = WorldRotate->data[0][1];
		Up.y = WorldRotate->data[1][1];
		Up.z = WorldRotate->data[2][1];
		Right.x = WorldRotate->data[0][2];
		Right.y = WorldRotate->data[1][2];
		Right.z = WorldRotate->data[2][2];

#if defined(OBLIVION)		
		SetupCamera(WorldTranslate, &Forward, &Up, &Right, &Camera->CameraFrustum, (float*)&Camera->ViewPort);
#elif defined(SKYRIM)
		TheUtilityManager->ThisStdCall(0x00633670, this, Camera);
#endif
		CameraForward.x = Forward.x;
		CameraForward.y = Forward.y;
		CameraForward.z = Forward.z;
		CameraPosition.x = WorldTranslate->x;
		CameraPosition.y = WorldTranslate->y;
		CameraPosition.z = WorldTranslate->z;

	}

}

void RenderManager::SetFoV()
{

	float FoV = TheSettingManager->SettingsMain.FoV;

	if (FoV) {
#if defined(OBLIVION)
		if (!TheUtilityManager->IsMenuMode(1009) && !TheUtilityManager->IsMenuMode(1034)) {
			if ((*g_thePlayer)->FoV != FoV) {
				ThisStdCall(0x00664A40, (*g_thePlayer), FoV);
				TheShaderManager->ShaderConst.FoV = FoV;
			}
		}
#elif defined(SKYRIM)
		if (*SettingWorldFoV != FoV) {
			void (__cdecl * C71820)(float) = (void(__cdecl *)(float))0x00C71820;
			SceneGraph* WorldSceneGraph = *(SceneGraph**)kWorldSceneGraph;
			PlayerCamera* Camera = PlayerCamera::GetSingleton();
		
			TheUtilityManager->ThisStdCall(0x00B17960, WorldSceneGraph, FoV, 0, NULL, NULL);	
			C71820(FoV);
			Camera->worldFOV = FoV;
			Camera->firstPersonFOV = FoV;
			*SettingWorldFoV = FoV;
			*Setting1stPersonFoV = FoV;
			TheShaderManager->ShaderConst.FoV = FoV;
		}
#endif
	}

}

void RenderManager::SetNearDistance()
{

	if (TheSettingManager->SettingsMain.CameraMode) {
#if defined(OBLIVION)
		SceneGraph* WorldSceneGraph = *(SceneGraph**)kWorldSceneGraph;
		if (TheRenderManager->FirstPersonView && *SettingNearDistance != TheSettingManager->SettingsMain.CameraModeNearDistanceFirst) {
			*SettingNearDistance = TheSettingManager->SettingsMain.CameraModeNearDistanceFirst;
			ThisStdCall(0x00411160, WorldSceneGraph, WorldSceneGraph->cameraFOV, 1.0f);
		}
		else if (!TheRenderManager->FirstPersonView && *SettingNearDistance != TheSettingManager->SettingsMain.CameraModeNearDistanceThird) {
			*SettingNearDistance = TheSettingManager->SettingsMain.CameraModeNearDistanceThird;
			ThisStdCall(0x00411160, WorldSceneGraph, WorldSceneGraph->cameraFOV, 1.0f);
		}
#elif defined(SKYRIM)

#endif
	}

}

void RenderManager::Initialize()
{

	IDirect3D9 *pD3D;
	D3DDISPLAYMODE currentDisplayMode;
	
	CameraForward.x = CameraForward.y = CameraForward.z = CameraForward.w = 0.0f;
	CameraPosition.x = CameraPosition.y = CameraPosition.z = CameraPosition.w = 0.0f;
	LastFrame = NULL;
	DepthSurface = NULL;
	DepthTexture = NULL;
	DepthTextureINTZ = NULL;
	SaveGameScreenShotRECT = { 0, 0, 256, 144 };
	IsSaveGameScreenShot = 0;
	FirstPersonView = 0;
	device->CreateTexture(width, height, 1, D3DUSAGE_DEPTHSTENCIL, (D3DFORMAT)MAKEFOURCC('I','N','T','Z'), D3DPOOL_DEFAULT, &DepthTexture, NULL);
	device->GetDirect3D(&pD3D);
	pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &currentDisplayMode);
	RESZ = pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, currentDisplayMode.Format, D3DUSAGE_RENDERTARGET, D3DRTYPE_SURFACE, (D3DFORMAT)MAKEFOURCC('R','E','S','Z')) == D3D_OK;
	if (RESZ)
		_MESSAGE("AMD/Intel detected: RESZ supported.");
	else if (NvAPI_Initialize() == NVAPI_OK)
		_MESSAGE("NVIDIA detected: NVAPI supported.");
	else
		_MESSAGE("ERROR: Cannot grab the depth buffer.");

}

void RenderManager::ResolveDepthBuffer()
{

	if (RESZ) {
		IDirect3DBaseTexture9 *pCurrTX = NULL;
		IDirect3DVertexShader9 *pCurrVS = NULL;
		IDirect3DPixelShader9 *pCurrPS = NULL;
		IDirect3DVertexBuffer9 *pCurrVX = NULL;
		UINT32 dCurrVO;
		UINT32 dCurrVS;
		DWORD dCurrZE;
		DWORD dCurrZW;
		DWORD dCurrCW;
		D3DXVECTOR3 vDummyPoint(0.0f, 0.0f, 0.0f);

		device->GetTexture(0, &pCurrTX);
		device->GetVertexShader(&pCurrVS);
		device->GetPixelShader(&pCurrPS);
		device->GetStreamSource(0, &pCurrVX, &dCurrVO, &dCurrVS);

		device->GetRenderState(D3DRS_ZENABLE, &dCurrZE);
		device->GetRenderState(D3DRS_ZWRITEENABLE, &dCurrZW);
		device->GetRenderState(D3DRS_COLORWRITEENABLE, &dCurrCW);
		device->SetVertexShader(NULL);
		device->SetPixelShader(NULL);
		device->SetFVF(D3DFVF_XYZ);
		device->SetTexture(0, DepthTexture);
		device->SetRenderState(D3DRS_ZENABLE, FALSE);
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		device->SetRenderState(D3DRS_COLORWRITEENABLE, NULL);
		device->DrawPrimitiveUP(D3DPT_POINTLIST, 1, vDummyPoint, sizeof(D3DXVECTOR3));
		device->SetRenderState(D3DRS_ZENABLE, dCurrZE);
		device->SetRenderState(D3DRS_ZWRITEENABLE, dCurrZW);
		device->SetRenderState(D3DRS_COLORWRITEENABLE, dCurrCW);
		device->SetRenderState(D3DRS_POINTSIZE, RESZ_CODE);
		device->SetRenderState(D3DRS_POINTSIZE, NULL);

		device->SetTexture(0, pCurrTX);
		device->SetVertexShader(pCurrVS);
		device->SetPixelShader(pCurrPS);
		device->SetStreamSource(0, pCurrVX, dCurrVO, dCurrVS);
		
		if (pCurrTX) pCurrTX->Release();
		pCurrVX->Release();
	}
	else
	{
		if (!DepthSurface) {
			D3DSURFACE_DESC desc;
			device->GetDepthStencilSurface(&DepthSurface);
			DepthSurface->GetDesc(&desc);
			if (desc.Format == (D3DFORMAT)MAKEFOURCC('I','N','T','Z')) {
				void *Container = NULL;
				DepthSurface->GetContainer(IID_IDirect3DTexture9, &Container);
				DepthTextureINTZ = (IDirect3DTexture9 *)Container;
				NvAPI_D3D9_RegisterResource(DepthTextureINTZ);
			}
			else
				NvAPI_D3D9_RegisterResource(DepthSurface);
			NvAPI_D3D9_RegisterResource(DepthTexture);
		}
		if (!DepthTextureINTZ)
			NvAPI_D3D9_StretchRectEx(device, DepthSurface, NULL, DepthTexture, NULL, D3DTEXF_NONE);
		else
			NvAPI_D3D9_StretchRectEx(device, DepthTextureINTZ, NULL, DepthTexture, NULL, D3DTEXF_NONE);
	}

}