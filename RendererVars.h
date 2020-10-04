//---------------------------------------------------------------------------

#ifndef RendererVarsH
#define RendererVarsH
#include <d3d11.h>
#include <dxgi.h>
//---------------------------------------------------------------------------
#define RELEASE(p) if(p){p->Release(); p = NULL;}
//---------------------------------------------------------------------------
class RendererVars {
private:
	ID3D11Device *m_device;
	ID3D11DeviceContext *m_context;
	IDXGIDevice *dxgi_device;
	IDXGIAdapter *dxgi_adapter;
	IDXGIFactory *dxgi_factory;
    ID3D11BlendState *m_blendState;
	ID3D11RasterizerState *rast_state;

	RendererVars();
	bool succeseed;

public:
	~RendererVars();
	static RendererVars &getInstance() {
		static RendererVars rendererVars;
		return rendererVars;
	}
	bool isSucceseed() const {return succeseed;}
	IDXGIFactory *dxgiFactory() const {return dxgi_factory;}
	ID3D11Device *getDevice3D() const{return m_device;}
	ID3D11DeviceContext *getDeviceContext() const{return m_context;}
	bool __fastcall createConstantBuffer(int size, ID3D11Device *m_device, ID3D11Buffer **cbuffer);
};
#endif
