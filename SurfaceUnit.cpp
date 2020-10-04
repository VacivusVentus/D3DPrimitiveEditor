// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SurfaceUnit.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSurfaceRender *SurfaceRender;

// ---------------------------------------------------------------------------
__fastcall TSurfaceRender::TSurfaceRender(TComponent* Owner) : TFrame(Owner), rendererVars(RendererVars::getInstance()),
    m_swapchain(NULL), m_rendertargetview(NULL), rendering(NULL), m_cbuffer(NULL)
{
    posX = 0;
    posY = 0;
    ScrollYBar->Max = Renderer->Height << 1;
    ScrollXBar->Max = Renderer->Width << 1;
    thWorking = false;
    size[0] = Renderer->Width;
    size[1] = Renderer->Width;
    if (!(rendererVars.createConstantBuffer(sizeof matrix, rendererVars.getDevice3D(), &m_cbuffer)))
    {
        return;
    }
}

// ---------------------------------------------------------------------------
void TSurfaceRender::ReleaseGUI()
{
    if (rendering)
    {
        thWorking = false;
        rendering->join();
        delete rendering;
        rendering = NULL;
    }
    RELEASE(m_rendertargetview);
    RELEASE(m_swapchain);
}

void TSurfaceRender::ReleaseSurface()
{
    ReleaseGUI();
    RELEASE(m_cbuffer);
}

void TSurfaceRender::ReInitRenderer()
{

    ReleaseGUI();

    DXGI_SWAP_CHAIN_DESC scdesc =
    {0};
    scdesc.OutputWindow = Renderer->Handle;
    scdesc.Windowed = true;
    scdesc.BufferDesc.Width = Renderer->Width;
    scdesc.BufferDesc.Height = Renderer->Height;
    scdesc.BufferDesc.RefreshRate.Denominator = 1;
    scdesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scdesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    scdesc.SampleDesc.Count = 4;
    scdesc.BufferCount = 2;
    HRESULT hr = rendererVars.dxgiFactory()->CreateSwapChain(rendererVars.getDevice3D(), &scdesc, &m_swapchain);
    if (!SUCCEEDED(hr))
    {
        return;
    }
    ID3D11Texture2D *texture = NULL;
    m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&texture);
    if (!texture)
    {
        return;
    }
    hr = rendererVars.getDevice3D()->CreateRenderTargetView(texture, NULL, &m_rendertargetview);
    RELEASE(texture);
    if (!SUCCEEDED(hr))
    {
        return;
    }

    auto func = [this](ID3D11DeviceContext * m_context, bool *working)
    {
        float color[] =
        {1.0, 1.0, 1.0, 1.0};
        D3D11_VIEWPORT vport =
        {0};
        vport.TopLeftX = 0.0f;
        vport.TopLeftY = 0.0f;
        vport.Width = Renderer->Width;
        vport.Height = Renderer->Height;
        vport.MinDepth = 0.0f;
        vport.MaxDepth = 1.0f;
        m_context->OMSetRenderTargets(1, &m_rendertargetview, NULL);
        m_context->RSSetViewports(1, &vport);
        updateMatrix();
        while (*working)
        {
            m_context->ClearRenderTargetView(m_rendertargetview, color);
            if (drawEveryPrimitive)
            {
                drawEveryPrimitive();
            }
            m_swapchain->Present(2, 0);
        }
    };
    thWorking = true;
    rendering = new std::thread(func, rendererVars.getDeviceContext(), &thWorking);
}

void __fastcall TSurfaceRender::FrameResize(TObject *Sender)
{
    if (!rendererVars.getDevice3D())
        return;
    ReInitRenderer();
}

// ---------------------------------------------------------------------------
void __fastcall TSurfaceRender::ScrollYBarChange(TObject *Sender)
{
    posY = ScrollYBar->Position;
    updateMatrix();
}
// ---------------------------------------------------------------------------

void __fastcall TSurfaceRender::ScrollXBarChange(TObject *Sender)
{
    posX = ScrollXBar->Position;
    updateMatrix();
}

// ---------------------------------------------------------------------------
void TSurfaceRender::updateMatrix()
{
    memset(matrix, 0, sizeof matrix);
    matrix[15] = 1.0;
    matrix[0] = 2.0 / (float)Renderer->Width;
    matrix[5] = -2.0 / (float)Renderer->Height;
    matrix[10] = 1.0;
    matrix[3] = -posX * matrix[0];
    matrix[7] = -posY * matrix[5];
    ID3D11DeviceContext * context = rendererVars.getDeviceContext();
    context->VSSetConstantBuffers(0, 1, &m_cbuffer);
    context->UpdateSubresource(m_cbuffer, 0, 0, matrix, 0, 0);
}

void __fastcall TSurfaceRender::getDivXY(int &dX, int &dY)
{
    dX += posX;
    dY += posY;
}
void __fastcall TSurfaceRender::RendererResize(TObject *Sender)
{
    updateMatrix();
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceRender::setScrollRange(int x, int y)
{

    ScrollYBar->Max = Renderer->Height + x;
    ScrollXBar->Max = Renderer->Width + y;
}
