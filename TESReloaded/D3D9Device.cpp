#include "D3D9Device.h"

TESRDirect3DDevice9::TESRDirect3DDevice9(IDirect3D9 *d3d, IDirect3DDevice9 *device) : m_d3d(d3d), m_device(device) {

}

TESRDirect3DDevice9::~TESRDirect3DDevice9() {

}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::QueryInterface(REFIID riid, void **ppvObj) {
  return m_device->QueryInterface(riid, ppvObj);
}

COM_DECLSPEC_NOTHROW ULONG STDMETHODCALLTYPE TESRDirect3DDevice9::AddRef(void) {
  return m_device->AddRef();
}

COM_DECLSPEC_NOTHROW ULONG STDMETHODCALLTYPE TESRDirect3DDevice9::Release(void) {
  ULONG count = m_device->Release();

  if (count == 0)
    delete this;

  return count;
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::TestCooperativeLevel(void) {
  return m_device->TestCooperativeLevel();
}

COM_DECLSPEC_NOTHROW UINT STDMETHODCALLTYPE TESRDirect3DDevice9::GetAvailableTextureMem(void) {
  return m_device->GetAvailableTextureMem();
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::EvictManagedResources(void) {
  return m_device->EvictManagedResources();
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetDirect3D(IDirect3D9 **ppD3D9) {

  HRESULT hr = m_device->GetDirect3D(ppD3D9);

  if (SUCCEEDED(hr))
    *ppD3D9 = m_d3d;

  return hr;
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetDeviceCaps(D3DCAPS9 *pCaps) {
  return m_device->GetDeviceCaps(pCaps);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE *pMode) {
  return m_device->GetDisplayMode(iSwapChain, pMode);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters) {
  return m_device->GetCreationParameters(pParameters);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9 *pCursorBitmap) {
  return m_device->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE TESRDirect3DDevice9::SetCursorPosition(int X, int Y, DWORD Flags) {
  m_device->SetCursorPosition(X, Y, Flags);
}

COM_DECLSPEC_NOTHROW BOOL STDMETHODCALLTYPE TESRDirect3DDevice9::ShowCursor(BOOL bShow) {
  return m_device->ShowCursor(bShow);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DSwapChain9 **pSwapChain) {
  return m_device->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9 **pSwapChain) {
  return m_device->GetSwapChain(iSwapChain, pSwapChain);
}

COM_DECLSPEC_NOTHROW UINT STDMETHODCALLTYPE TESRDirect3DDevice9::GetNumberOfSwapChains(void) {
  return m_device->GetNumberOfSwapChains();
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::Reset(D3DPRESENT_PARAMETERS *pPresentationParameters) {
  return m_device->Reset(pPresentationParameters);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::Present(CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion) {
  return m_device->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9 **ppBackBuffer) {
  return m_device->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS *pRasterStatus) {
  return m_device->GetRasterStatus(iSwapChain, pRasterStatus);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetDialogBoxMode(BOOL bEnableDialogs) {
  return m_device->SetDialogBoxMode(bEnableDialogs);
}

COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE TESRDirect3DDevice9::SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP *pRamp) {
  return m_device->SetGammaRamp(iSwapChain, Flags, pRamp);
}

COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE TESRDirect3DDevice9::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP *pRamp) {
  return m_device->GetGammaRamp(iSwapChain, pRamp);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9 **ppTexture, HANDLE *pSharedHandle) {
  return m_device->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9 **ppVolumeTexture, HANDLE *pSharedHandle) {
  return m_device->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9 **ppCubeTexture, HANDLE *pSharedHandle) {
  return m_device->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9 **ppVertexBuffer, HANDLE *pSharedHandle) {
  return m_device->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9 **ppIndexBuffer, HANDLE *pSharedHandle) {
  return m_device->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9 **ppSurface, HANDLE *pSharedHandle) {
  return m_device->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9 **ppSurface, HANDLE *pSharedHandle) {
  return m_device->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::UpdateSurface(IDirect3DSurface9 *pSourceSurface, CONST RECT *pSourceRect, IDirect3DSurface9 *pDestinationSurface, CONST POINT *pDestPoint) {
  return m_device->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::UpdateTexture(IDirect3DBaseTexture9 *pSourceTexture, IDirect3DBaseTexture9 *pDestinationTexture) {
  return m_device->UpdateTexture(pSourceTexture, pDestinationTexture);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetRenderTargetData(IDirect3DSurface9 *pRenderTarget, IDirect3DSurface9 *pDestSurface) {
  return m_device->GetRenderTargetData(pRenderTarget, pDestSurface);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9 *pDestSurface) {
  return m_device->GetFrontBufferData(iSwapChain, pDestSurface);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::StretchRect(IDirect3DSurface9 *pSourceSurface, CONST RECT *pSourceRect, IDirect3DSurface9 *pDestSurface, CONST RECT *pDestRect, D3DTEXTUREFILTERTYPE Filter) {
  return m_device->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::ColorFill(IDirect3DSurface9 *pSurface, CONST RECT *pRect, D3DCOLOR color) {
  return m_device->ColorFill(pSurface, pRect, color);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9 **ppSurface, HANDLE *pSharedHandle) {
  return m_device->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9 *pRenderTarget) {
  return m_device->SetRenderTarget(RenderTargetIndex, pRenderTarget);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9 **ppRenderTarget) {
  return m_device->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetDepthStencilSurface(IDirect3DSurface9 *pNewZStencil) {
  return m_device->SetDepthStencilSurface(pNewZStencil);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetDepthStencilSurface(IDirect3DSurface9 **ppZStencilSurface) {
  return m_device->GetDepthStencilSurface(ppZStencilSurface);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::BeginScene(void) {
  return m_device->BeginScene();
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::EndScene(void) {
  return m_device->EndScene();
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::Clear(DWORD Count, CONST D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) {
  return m_device->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix) {
  return m_device->SetTransform(State, pMatrix);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX *pMatrix) {
  return m_device->GetTransform(State, pMatrix);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix) {
  return m_device->MultiplyTransform(State, pMatrix);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetViewport(CONST D3DVIEWPORT9 *pViewport) {
  return m_device->SetViewport(pViewport);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetViewport(D3DVIEWPORT9 *pViewport) {
  return m_device->GetViewport(pViewport);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetMaterial(CONST D3DMATERIAL9 *pMaterial) {
  return m_device->SetMaterial(pMaterial);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetMaterial(D3DMATERIAL9 *pMaterial) {
  return m_device->GetMaterial(pMaterial);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetLight(DWORD Index, CONST D3DLIGHT9 *pLight) {
  return m_device->SetLight(Index, pLight);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetLight(DWORD Index, D3DLIGHT9 *pLight) {
  return m_device->GetLight(Index, pLight);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::LightEnable(DWORD Index, BOOL Enable) {
  return m_device->LightEnable(Index, Enable);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetLightEnable(DWORD Index, BOOL *pEnable) {
  return m_device->GetLightEnable(Index, pEnable);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetClipPlane(DWORD Index, CONST float *pPlane) {
  return m_device->SetClipPlane(Index, pPlane);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetClipPlane(DWORD Index, float *pPlane) {
  return m_device->GetClipPlane(Index, pPlane);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) {
  return m_device->SetRenderState(State, Value);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetRenderState(D3DRENDERSTATETYPE State, DWORD *pValue) {
  return m_device->GetRenderState(State, pValue);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9 **ppSB) {
  return m_device->CreateStateBlock(Type, ppSB);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::BeginStateBlock(void) {
  return m_device->BeginStateBlock();
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::EndStateBlock(IDirect3DStateBlock9 **ppSB) {
  return m_device->EndStateBlock(ppSB);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetClipStatus(CONST D3DCLIPSTATUS9 *pClipStatus) {
  return m_device->SetClipStatus(pClipStatus);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetClipStatus(D3DCLIPSTATUS9 *pClipStatus) {
  return m_device->GetClipStatus(pClipStatus);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetTexture(DWORD Sampler, IDirect3DBaseTexture9 **ppTexture) {
  return m_device->GetTexture(Sampler, ppTexture);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetTexture(DWORD Sampler, IDirect3DBaseTexture9 *pTexture) {
  return m_device->SetTexture(Sampler, pTexture);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD *pValue) {
  return m_device->GetTextureStageState(Stage, Type, pValue);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) {
  return m_device->SetTextureStageState(Stage, Type, Value);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD *pValue) {
  return m_device->GetSamplerState(Sampler, Type, pValue);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) {
  return m_device->SetSamplerState(Sampler, Type, Value);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::ValidateDevice(DWORD *pNumPasses) {
  return m_device->ValidateDevice(pNumPasses);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY *pEntries) {
  return m_device->SetPaletteEntries(PaletteNumber, pEntries);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY *pEntries) {
  return m_device->GetPaletteEntries(PaletteNumber, pEntries);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetCurrentTexturePalette(UINT PaletteNumber) {
  return m_device->SetCurrentTexturePalette(PaletteNumber);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetCurrentTexturePalette(UINT *PaletteNumber) {
  return m_device->GetCurrentTexturePalette(PaletteNumber);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetScissorRect(CONST RECT *pRect) {
  return m_device->SetScissorRect(pRect);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetScissorRect(RECT *pRect) {
  return m_device->GetScissorRect(pRect);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetSoftwareVertexProcessing(BOOL bSoftware) {
  return m_device->SetSoftwareVertexProcessing(bSoftware);
}

COM_DECLSPEC_NOTHROW BOOL STDMETHODCALLTYPE TESRDirect3DDevice9::GetSoftwareVertexProcessing(void) {
  return m_device->GetSoftwareVertexProcessing();
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetNPatchMode(float nSegments) {
  return m_device->SetNPatchMode(nSegments);
}

COM_DECLSPEC_NOTHROW float STDMETHODCALLTYPE TESRDirect3DDevice9::GetNPatchMode(void) {
  return m_device->GetNPatchMode();
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) {
  return m_device->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
  return m_device->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride) {
  return m_device->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void *pIndexData, D3DFORMAT IndexDataFormat, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride) {
  return m_device->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9 *pDestBuffer, IDirect3DVertexDeclaration9 *pVertexDecl, DWORD Flags) {
  return m_device->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9 *pVertexElements, IDirect3DVertexDeclaration9 **ppDecl) {
  return m_device->CreateVertexDeclaration(pVertexElements, ppDecl);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetVertexDeclaration(IDirect3DVertexDeclaration9 *pDecl) {
  return m_device->SetVertexDeclaration(pDecl);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetVertexDeclaration(IDirect3DVertexDeclaration9 **ppDecl) {
  return m_device->GetVertexDeclaration(ppDecl);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetFVF(DWORD FVF) {
  return m_device->SetFVF(FVF);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetFVF(DWORD *pFVF) {
  return m_device->GetFVF(pFVF);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::CreateVertexShader(CONST DWORD *pFunction, IDirect3DVertexShader9 **ppShader) {
  return m_device->CreateVertexShader(pFunction, ppShader);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetVertexShader(IDirect3DVertexShader9 *pShader) {
  return m_device->SetVertexShader(pShader);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetVertexShader(IDirect3DVertexShader9 **ppShader) {
  return m_device->GetVertexShader(ppShader);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetVertexShaderConstantF(UINT StartRegister, CONST float *pConstantData, UINT Vector4fCount) {
  return m_device->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetVertexShaderConstantF(UINT StartRegister, float *pConstantData, UINT Vector4fCount) {
  return m_device->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetVertexShaderConstantI(UINT StartRegister, CONST int *pConstantData, UINT Vector4iCount) {
  return m_device->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetVertexShaderConstantI(UINT StartRegister, int *pConstantData, UINT Vector4iCount) {
  return m_device->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetVertexShaderConstantB(UINT StartRegister, CONST BOOL *pConstantData, UINT BoolCount) {
  return m_device->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetVertexShaderConstantB(UINT StartRegister, BOOL *pConstantData, UINT BoolCount) {
  return m_device->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9 *pStreamData, UINT OffsetInBytes, UINT Stride) {
  return m_device->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9 **ppStreamData, UINT *pOffsetInBytes, UINT *pStride) {
  return m_device->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetStreamSourceFreq(UINT StreamNumber, UINT Setting) {
  return m_device->SetStreamSourceFreq(StreamNumber, Setting);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetStreamSourceFreq(UINT StreamNumber, UINT *pSetting) {
  return m_device->GetStreamSourceFreq(StreamNumber, pSetting);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetIndices(IDirect3DIndexBuffer9 *pIndexData) {
  return m_device->SetIndices(pIndexData);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetIndices(IDirect3DIndexBuffer9 **ppIndexData) {
  return m_device->GetIndices(ppIndexData);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::CreatePixelShader(CONST DWORD *pFunction, IDirect3DPixelShader9 **ppShader) {
  return m_device->CreatePixelShader(pFunction, ppShader);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetPixelShader(IDirect3DPixelShader9 *pShader) {
  return m_device->SetPixelShader(pShader);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetPixelShader(IDirect3DPixelShader9 **ppShader) {
  return m_device->GetPixelShader(ppShader);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetPixelShaderConstantF(UINT StartRegister, CONST float *pConstantData, UINT Vector4fCount) {
  return m_device->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetPixelShaderConstantF(UINT StartRegister, float *pConstantData, UINT Vector4fCount) {
  return m_device->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetPixelShaderConstantI(UINT StartRegister, CONST int *pConstantData, UINT Vector4iCount) {
  return m_device->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetPixelShaderConstantI(UINT StartRegister, int *pConstantData, UINT Vector4iCount) {
  return m_device->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::SetPixelShaderConstantB(UINT StartRegister, CONST BOOL *pConstantData, UINT  BoolCount) {
  return m_device->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::GetPixelShaderConstantB(UINT StartRegister, BOOL *pConstantData, UINT BoolCount) {
  return m_device->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::DrawRectPatch(UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO *pRectPatchInfo) {
  return m_device->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::DrawTriPatch(UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO *pTriPatchInfo) {
  return m_device->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::DeletePatch(UINT Handle) {
  return m_device->DeletePatch(Handle);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9 **ppQuery) {
  return m_device->CreateQuery(Type, ppQuery);
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::DumpFrameScript(void) {
  return D3D_OK;
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE TESRDirect3DDevice9::DumpFrameSurfaces(void) {
  return D3D_OK;
}
