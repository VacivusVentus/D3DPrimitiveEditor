// ---------------------------------------------------------------------------

#pragma hdrstop

#include "EllipsePrimitive.h"
#include <algorithm>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

EllipsePrimitive::EllipsePrimitive() : Primitive(), vars(RendererVars::getInstance()),
    shaders(ShaderPrimitive::getInstance())
{
    memset(&buffervs, 0, sizeof buffervs);
    if (!vars.createConstantBuffer(sizeof buffervs, vars.getDevice3D(), &cBufferVS))
    {
        return;
    }
}

void EllipsePrimitive::draw(ID3D11DeviceContext *m_context)
{

    m_context->VSSetConstantBuffers(1, 1, &cBufferVS);
    m_context->PSSetConstantBuffers(0, 1, &cBufferPS);
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    shaders.bindEllipseShaderGroup(m_context, isUnderCrs);
    m_context->DrawInstanced(3, 41, 0, 0);
	m_context->IASetPrimitiveTopology((lineStyle == LineStyle::Points) ? D3D11_PRIMITIVE_TOPOLOGY_POINTLIST : D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	shaders.bindEllipseLinesShaderGroup(m_context, lineStyle, isUnderCrs);
    m_context->DrawInstanced(81, widthline, 0, 0);
    if (isSelect)
    {
		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		shaders.bindRectRectTypeFrameShaderGroup(m_context);
		m_context->DrawInstanced(8, 1, 0, 0);
    }
}

void EllipsePrimitive::releasePrimitive()
{
    RELEASE(cBufferPS);
    RELEASE(cBufferVS);
}

void __fastcall EllipsePrimitive::setFirstPoint(int x, int y)
{
    xy[0] = x;
    xy[1] = y;
    vars.getDeviceContext()->UpdateSubresource(cBufferVS, 0, 0, &buffervs, 0, 0);
}

void __fastcall EllipsePrimitive::setSecondPoint(int x, int y)
{
	buffervs.coord[0] = (xy[0] + x) >> 1;
	buffervs.coord[1] = (xy[1] + y) >> 1;
	buffervs.coord[2] = (std::max(xy[0], x) - std::min(xy[0], x)) >> 1;
	buffervs.coord[3] = (std::max(xy[1], y) - std::min(xy[1], y)) >> 1;

    vars.getDeviceContext()->UpdateSubresource(cBufferVS, 0, 0, &buffervs, 0, 0);

    center[0] = (buffervs.coord[2] + buffervs.coord[0]) / 2.0f;
    center[1] = (buffervs.coord[3] + buffervs.coord[1]) / 2.0f;

    center[2] = (buffervs.coord[2] - buffervs.coord[0]) / 2.0f;
    center[3] = (buffervs.coord[3] - buffervs.coord[1]) / 2.0f;
}

bool __fastcall EllipsePrimitive::isUnderCursor(int x, int y)
{
	center[0] = buffervs.coord[0];
	center[1] = buffervs.coord[1];
	float coord[] = {buffervs.coord[0] - buffervs.coord[2], buffervs.coord[1] - buffervs.coord[3], buffervs.coord[0] + buffervs.coord[2], buffervs.coord[1] + buffervs.coord[3]};
	float *rcoord = new float[2];
	if (!rcoord)
		return false;
    rotateCoordToModelAxis(x, y, rcoord);
    // Находиться ли курсор в прямоугольном секторе линии
	if (!inRect(rcoord[0], rcoord[1], coord))
	{
        return false;
	}
	float rx = buffervs.coord[0];
	float ry = buffervs.coord[1];
    float cx = rcoord[0] - center[0];
    float cy = rcoord[1] - center[1];
    float sndp[] =
    {rcoord[0], rcoord[1]};
    delete[]rcoord;

    if (isSelect)
    {
        //detectNearOperation(sndp[0], sndp[1]);
		editMode = dectRectEditMode( sndp[0], sndp[1],coord, center);
        return true;
    }
    float test = (cx * cx) / (rx * rx) + (cy * cy) / (ry * ry);
    if (test <= 1.0)
    {
		editMode = dectRectEditMode( sndp[0], sndp[1], coord, center);
		return true;
    }
    return false;
}


