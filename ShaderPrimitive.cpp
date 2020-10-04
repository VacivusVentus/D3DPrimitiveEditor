// ---------------------------------------------------------------------------

#pragma hdrstop

#include "ShaderPrimitive.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#include <vcl.h>

ShaderPrimitive::ShaderPrimitive() : rendererVars(RendererVars::getInstance()), m_lineVS(NULL), m_lineVS_N(NULL),
    m_lineVS_Sel(NULL), m_lineVS_UCursor(NULL), m_triFilVS(NULL), m_penmainPS(NULL), m_triFilVS_line(NULL),
    correct(false), m_triFilRVS_line(NULL), m_PSStyleLine_dashed(NULL), m_PSStyleLine_points(NULL),
    m_lineVS_ULCursor(NULL), m_frameForLineVS(NULL), m_frameForRectTypesVS(NULL)
{
    ID3D11Device *m_device = rendererVars.getDevice3D();
    if (!createVS(L"LINE_VS", m_device, &m_lineVS))
    {
        return;
    }
    if (!createPS(L"LINE_PS_N", m_device, &m_lineVS_N))
    {
        return;
    }
    if (!createPS(L"LINE_PS_SEL", m_device, &m_lineVS_Sel))
    {
        return;
    }
    if (!createPS(L"LINE_PS_UC", m_device, &m_lineVS_UCursor))
    {
        return;
    }
    // ------------------------------------------------------------
    if (!createVS(L"TRIANGLE_VS_FILL_VS", m_device, &m_triFilVS))
    {
        return;
    }
    if (!createVS(L"TRIANGLE_VS_FILL_VS_RECT", m_device, &m_triFilRVS))
    {
        return;
    }

    if (!createPS(L"PEN_MAIN_PS", m_device, &m_penmainPS))
    {
        return;
    }

    if (!createVS(L"TRI_ELIPS_NLINE", m_device, &m_triFilVS_line))
    {
        return;
    }
    if (!createVS(L"TRI_RECT_VS_LINE", m_device, &m_triFilRVS_line))
    {
        return;
    }
    if (!createPS(L"PS_LINE_DASHED", m_device, &m_PSStyleLine_dashed))
    {
        return;
    }
    if (!createPS(L"PS_LINE_POINTS", m_device, &m_PSStyleLine_points))
    {
        return;
    }
    if (!createPS(L"FORLINE_UC_PS", m_device, &m_lineVS_ULCursor))
    {
        return;
    }
    if (!createVS(L"FRAME_FOR_LINE", m_device, &m_frameForLineVS))
    {
        return;
    }
    if (!createVS(L"FRAME_FOR_RECT_PS", m_device, &m_frameForRectTypesVS))
    {
        return;
    }
    correct = true;
}

std::vector<unsigned char>ShaderPrimitive::loadShader(const wchar_t* resName)
{
    std::vector<unsigned char>bytes;
    TResourceStream* Res = new TResourceStream((int)HInstance, resName, RT_RCDATA);
    auto size = Res->Size;
    bytes.resize(size);
    Res->Read(&bytes[0], size);
    Res->Free();
    return bytes;
}

bool __fastcall ShaderPrimitive::createVS(const wchar_t* resName, ID3D11Device *m_device, ID3D11VertexShader **shader)
{
    std::vector<unsigned char>bytes = loadShader(resName);
    HRESULT hr = m_device->CreateVertexShader(&bytes[0], bytes.size(), NULL, shader);
    if (FAILED(hr))
    {
        return false;
    }
    return true;
}

bool __fastcall ShaderPrimitive::createPS(const wchar_t* resName, ID3D11Device *m_device, ID3D11PixelShader **shader)
{
    std::vector<unsigned char>bytes = loadShader(resName);
    HRESULT hr = m_device->CreatePixelShader(&bytes[0], bytes.size(), NULL, shader);
    if (FAILED(hr))
    {
        return false;
    }
    return true;
}

void __fastcall ShaderPrimitive::bindLineShaderGroup(ID3D11DeviceContext *m_context, LineStyle lineStyle,
    bool undercursor)
{
    m_context->VSSetShader(m_lineVS, NULL, NULL);
    if (undercursor)
    {
        m_context->PSSetShader(m_lineVS_ULCursor, NULL, NULL);
        return;
    }
    setLineStyle(m_context, lineStyle);
}

void __fastcall ShaderPrimitive::bindEllipseShaderGroup(ID3D11DeviceContext *m_context, bool undercursor)
{
    m_context->VSSetShader(m_triFilVS, NULL, NULL);
    if (undercursor)
    {
        m_context->PSSetShader(m_lineVS_UCursor, NULL, NULL);
        return;
    }
    m_context->PSSetShader(m_lineVS_N, NULL, NULL);
}

void __fastcall ShaderPrimitive::bindEllipseLinesShaderGroup(ID3D11DeviceContext *m_context, LineStyle lineStyle,
    bool undercursor)
{
    m_context->VSSetShader(m_triFilVS_line, NULL, NULL);
    if (undercursor)
    {
        m_context->PSSetShader(m_lineVS_ULCursor, NULL, NULL);
        return;
    }
    setLineStyle(m_context, lineStyle);
}

void __fastcall ShaderPrimitive::bindRectShaderGroup(ID3D11DeviceContext *m_context, bool undercursor)
{
    m_context->VSSetShader(m_triFilRVS, NULL, NULL);
    if (undercursor)
    {
        m_context->PSSetShader(m_lineVS_UCursor, NULL, NULL);
        return;
    }
    m_context->PSSetShader(m_lineVS_N, NULL, NULL);
}

void __fastcall ShaderPrimitive::bindRectLineShaderGroup(ID3D11DeviceContext *m_context, LineStyle lineStyle,
    bool undercursor)
{
    m_context->VSSetShader(m_triFilRVS_line, NULL, NULL);
    if (undercursor)
    {
        m_context->PSSetShader(m_lineVS_ULCursor, NULL, NULL);
        return;
    }
    setLineStyle(m_context, lineStyle);

}

void __fastcall ShaderPrimitive::bindRectLineFrameShaderGroup(ID3D11DeviceContext *m_context)
{
    m_context->VSSetShader(m_frameForLineVS, NULL, NULL);
    m_context->PSSetShader(m_lineVS_Sel, NULL, NULL);
}

void __fastcall ShaderPrimitive::bindRectRectTypeFrameShaderGroup(ID3D11DeviceContext *m_context)
{
    m_context->VSSetShader(m_frameForRectTypesVS, NULL, NULL);
    m_context->PSSetShader(m_lineVS_Sel, NULL, NULL);
}

void ShaderPrimitive::setLineStyle(ID3D11DeviceContext *m_context, LineStyle lineStyle)
{
    if (lineStyle == Dashed)
    {
        m_context->PSSetShader(m_PSStyleLine_dashed, NULL, NULL);
    }
    else if (lineStyle == Points)
    {
        m_context->PSSetShader(m_PSStyleLine_points, NULL, NULL);
    }
    else
    {
        m_context->PSSetShader(m_penmainPS, NULL, NULL);
    }
}

void ShaderPrimitive::releaseAllShaders()
{
    RELEASE(m_frameForRectTypesVS);
    RELEASE(m_frameForLineVS);
    RELEASE(m_lineVS_ULCursor);
    RELEASE(m_PSStyleLine_points);
    RELEASE(m_PSStyleLine_dashed);
    RELEASE(m_triFilRVS_line);
    RELEASE(m_triFilVS_line);
    RELEASE(m_penmainPS);
    RELEASE(m_triFilRVS);
    RELEASE(m_triFilVS);
    RELEASE(m_lineVS_UCursor);
    RELEASE(m_lineVS_Sel);
    RELEASE(m_lineVS_N);
    RELEASE(m_lineVS);
}
