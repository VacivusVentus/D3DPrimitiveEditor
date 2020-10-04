// ---------------------------------------------------------------------------

#pragma hdrstop

#include <vcl.h>
#include "RendererVars.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

RendererVars::RendererVars() : m_device(NULL), m_context(NULL), dxgi_device(NULL), dxgi_adapter(NULL),
	dxgi_factory(NULL), rast_state(NULL), m_blendState(NULL)
{

	succeseed = false;
	D3D_FEATURE_LEVEL levels[] =
	{D3D_FEATURE_LEVEL_11_0};
	HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, //
		levels, ARRAYSIZE(levels), D3D11_SDK_VERSION, &m_device, NULL, &m_context);
	if (hr) // ERROR
	{
		return;
	}
	if (m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_device)) // ERROR
	{
		return;
	}
	if (dxgi_device->GetAdapter(&dxgi_adapter)) // ERROR
	{
		return;
	}
	if (dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgi_factory)) // ERROR
	{
		return;
	}

	D3D11_RASTERIZER_DESC rasterizer;
	memset(&rasterizer, 0, sizeof rasterizer);
	rasterizer.AntialiasedLineEnable = true; // для уменьшения ступей у линий
	// Остальное по умолчанию
	rasterizer.DepthClipEnable = true;
	rasterizer.FillMode = D3D11_FILL_SOLID;
	rasterizer.CullMode = D3D11_CULL_BACK;
	if (FAILED(m_device->CreateRasterizerState(&rasterizer, &rast_state)))
		return;
	m_context->RSSetState(rast_state);

	D3D11_BLEND_DESC desc;
	desc.AlphaToCoverageEnable = true;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	if (FAILED(m_device->CreateBlendState(&desc, &m_blendState)))
		return;
	m_context->OMSetBlendState(m_blendState, 0, 0xffffffff);
	succeseed = true;
};

RendererVars::~RendererVars()
{
	RELEASE(m_blendState);
	RELEASE(rast_state);
	RELEASE(dxgi_factory);
	RELEASE(dxgi_adapter);
	RELEASE(dxgi_device);
	RELEASE(m_context);
	RELEASE(m_device);
}

bool __fastcall RendererVars::createConstantBuffer(int size, ID3D11Device *m_device, ID3D11Buffer **cbuffer)
{
	D3D11_BUFFER_DESC cameradesc =
	{0};
	cameradesc.ByteWidth = ((size >> 5) + 1) << 5;
	cameradesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	HRESULT hr = m_device->CreateBuffer(&cameradesc, NULL, cbuffer);
	if (FAILED(hr))
	{
		ShowMessage("Не удалось выделить буфер");
		return false;
	}
	return true;
}
