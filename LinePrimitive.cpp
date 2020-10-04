// ---------------------------------------------------------------------------

#pragma hdrstop

#include "LinePrimitive.h"
#include <algorithm>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

LinePrimitive::LinePrimitive() : Primitive(), vars(RendererVars::getInstance()), shaders(ShaderPrimitive::getInstance())
{
	if (!vars.createConstantBuffer(sizeof buffervs, vars.getDevice3D(), &cBufferVS))
	{
		return;
	}
	memset(&buffervs.coord, 0, sizeof buffervs.coord);
	vars.getDeviceContext()->UpdateSubresource(cBufferVS, 0, 0, &buffervs, 0, 0);

}

void LinePrimitive::draw(ID3D11DeviceContext *m_context)
{
	m_context->VSSetConstantBuffers(1, 1, &cBufferVS);
	m_context->PSSetConstantBuffers(0, 1, &cBufferPS);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	shaders.bindLineShaderGroup(m_context, lineStyle, isUnderCrs);
	m_context->DrawInstanced(2, 1 + ((widthline - 1) << 1), 0, 0);

	if (isSelect)
	{
		shaders.bindRectLineFrameShaderGroup(m_context);
		m_context->Draw(8, 0);
	}
}

void LinePrimitive::releasePrimitive()
{
	RELEASE(cBufferPS);
	RELEASE(cBufferVS);
}

void __fastcall LinePrimitive::setFirstPoint(int x, int y)
{
	xy[0] = x;
	xy[1] = y;
	vars.getDeviceContext()->UpdateSubresource(cBufferVS, 0, 0, &buffervs, 0, 0);
}

void __fastcall LinePrimitive::setSecondPoint(int x, int y)
{
	buffervs.coord[0] = xy[0];
	buffervs.coord[1] = xy[1];
	buffervs.coord[2] = x;
	buffervs.coord[3] = y;
	vars.getDeviceContext()->UpdateSubresource(cBufferVS, 0, 0, &buffervs, 0, 0);
	center[0] = (buffervs.coord[2] + buffervs.coord[0]) / 2.0f;
	center[1] = (buffervs.coord[3] + buffervs.coord[1]) / 2.0f;
	// Для линии половина длин по осям ненужны
}

bool __fastcall LinePrimitive::isUnderCursor(int x, int y)
{
	float *rcoord = new float[2];
	if (!rcoord)
		return false;
	rotateCoordToModelAxis(x, y, rcoord);
	// Находиться ли курсор в прямоугольном секторе линии
	if (!inRect(rcoord[0], rcoord[1], static_cast<float*>(buffervs.coord)))
	{
		return false;
	}
	float d[6];
	memcpy(d, buffervs.coord, sizeof buffervs.coord);
	d[4] = rcoord[0];
	d[5] = rcoord[1];
	delete[]rcoord;
	float l0 = std::sqrt(std::pow(d[2] - d[0], 2) + std::pow(d[3] - d[1], 2));
	float l1 = std::sqrt(std::pow(d[2] - d[4], 2) + std::pow(d[3] - d[5], 2));
	float l2 = std::sqrt(std::pow(d[4] - d[0], 2) + std::pow(d[5] - d[1], 2));
	float l = std::abs(l1 + l2 - l0);

	if (l < 5)
	{
		detectNearOperation(d[4], d[5]);
		return true; // Если разница в длине не превышает погрешности, значит курсор на линии
	}
	return false;
}

void LinePrimitive::detectNearOperation(float x, float y)
{
	float d[] =
	{buffervs.coord[0] - x, buffervs.coord[1] - y, buffervs.coord[2] - x, buffervs.coord[3] - y};
	float length = std::sqrt(std::pow(2.0f * center[0], 2) + std::pow(2.0f * center[1], 2));
	editMode = EditMode::Move;
	// левая/правая точка указаны условно.
	// следует понимать под левой начальную точку, а по правой: конечную
	float lftLength = std::sqrt(d[0] * d[0] + d[1] * d[1]);
	if (lftLength / length < 0.1)
	{
		editMode = EditMode::LChgXY0;
	}
	float rhtLength = std::sqrt(d[2] * d[2] + d[3] * d[3]);
	if (rhtLength / length < 0.1)
	{
		editMode = EditMode::LChgXY1;
	}
}

void __fastcall LinePrimitive::dragEditMouse(float x, float y)
{
	float dx = x - startEditXY[0];
	float dy = y - startEditXY[1];

	float convert[2];
	float *_conv = convert;
	rotateCoordToCursorAxis(dx, dy, _conv);

	if (editMode == EditMode::Move || editMode == EditMode::LChgXY0)
	{
		buffervs.coord[0] += convert[0];
		buffervs.coord[1] += convert[1];
	}

	if (editMode == EditMode::Move || editMode == EditMode::LChgXY1)
	{
		buffervs.coord[2] += convert[0];
		buffervs.coord[3] += convert[1];
	}
	startEditXY[0] = x;
	startEditXY[1] = y;
	updateResource();
}

void __fastcall LinePrimitive::movePrimitiveRatherPoint(float divX, float divY)
{
	float begin[2];
	float *cloud = begin;
	rotateCoordToCursorAxis(divX, divY, cloud);
	buffervs.coord[0] += begin[0];
	buffervs.coord[1] += begin[1];
	buffervs.coord[2] += begin[0];
	buffervs.coord[3] += begin[1];
}

void __fastcall LinePrimitive::rotatePrimitiveRatherPoint(float *lcenter, float angle)
{
	float *newpoint0 = buffervs.coord;
	float *newpoint1 = &buffervs.coord[2];
	rotateAroundPoint(buffervs.coord[0], buffervs.coord[1], newpoint0, lcenter, -angle);
	rotateAroundPoint(buffervs.coord[2], buffervs.coord[3], newpoint1, lcenter, -angle);
	updateResource(false);
}

void LinePrimitive::convertRectToCoord()
{
	memcpy(center, buffervs.coord, sizeof(buffervs.coord) >> 1);
}
