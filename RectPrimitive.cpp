// ---------------------------------------------------------------------------

#pragma hdrstop

#include "RectPrimitive.h"
#include <algorithm>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

RectPrimitive::RectPrimitive() : Primitive(), vars(RendererVars::getInstance()), shaders(ShaderPrimitive::getInstance())
{
	if (!vars.createConstantBuffer(sizeof buffervs, vars.getDevice3D(), &cBufferVS))
	{
		return;
	}
}

void RectPrimitive::draw(ID3D11DeviceContext *m_context)
{
	m_context->VSSetConstantBuffers(1, 1, &cBufferVS);
	m_context->PSSetConstantBuffers(0, 1, &cBufferPS);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	shaders.bindRectShaderGroup(m_context, isUnderCrs);
	m_context->Draw(6, 0);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	shaders.bindRectLineShaderGroup(m_context, lineStyle, isUnderCrs);
	m_context->DrawInstanced(5, widthline, 0, 0);
	if (isSelect)
	{
		shaders.bindRectRectTypeFrameShaderGroup(m_context);
		m_context->DrawInstanced(5, 1 + widthline, 0, 0);
	}
}

void RectPrimitive::frameOnlyDraw(ID3D11DeviceContext *m_context)
{
	m_context->VSSetConstantBuffers(1, 1, &cBufferVS);
	m_context->PSSetConstantBuffers(0, 1, &cBufferPS);
	if (isUnderCrs)
	{
		const float framecolor[] =
		{1.0, 1.0, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0};
		vars.getDeviceContext()->UpdateSubresource(cBufferPS, 0, 0, framecolor, 0, 0);
		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		shaders.bindRectShaderGroup(m_context, isUnderCrs);
		m_context->Draw(6, 0);
	}
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	RectPrimitive::shaders.bindRectRectTypeFrameShaderGroup(m_context);
	m_context->DrawInstanced(5, 2, 0, 0);
}

void RectPrimitive::releasePrimitive()
{
	RELEASE(cBufferPS);
	RELEASE(cBufferVS);
}

void __fastcall RectPrimitive::setFirstPoint(int x, int y)
{
	xy[0] = x;
	xy[1] = y;
	vars.getDeviceContext()->UpdateSubresource(cBufferVS, 0, 0, &buffervs, 0, 0);
}

void __fastcall RectPrimitive::setSecondPoint(int x, int y)
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

bool __fastcall RectPrimitive::isUnderCursor(int x, int y)
{
	center[0] = buffervs.coord[0];
	center[1] = buffervs.coord[1];
	float coord[] =
	{buffervs.coord[0] - buffervs.coord[2], buffervs.coord[1] - buffervs.coord[3],
		buffervs.coord[0] + buffervs.coord[2], buffervs.coord[1] + buffervs.coord[3]};
	float *rcoord = new float[2];
	if (!rcoord)
		return false;
	rotateCoordToModelAxis(x, y, rcoord);
	// Находиться ли курсор в прямоугольном секторе линии
	if (!inRect(rcoord[0], rcoord[1], coord))
	{
		delete[]rcoord;
		return false;
	}
	editMode = dectRectEditMode(rcoord[0], rcoord[1], coord, center);
	delete[]rcoord;
	return true;
}
