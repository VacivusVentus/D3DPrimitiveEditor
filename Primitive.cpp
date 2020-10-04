// ---------------------------------------------------------------------------

#pragma hdrstop

#include "Primitive.h"
#include <algorithm>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

typedef struct PairLine
{
	float xy[4];

	PairLine(float x0, float y0, float x1, float y1)
	{
		xy[0] = x0;
		xy[1] = y0;
		xy[2] = x1;
		xy[3] = y1;
	}

	bool __fastcall onLine(float x, float y)
	{
		float length = std::sqrt(std::pow(xy[0] - xy[2], 2) + std::pow(xy[1] - xy[3], 2));

		float l0 = std::sqrt(std::pow(x - xy[2], 2) + std::pow(y - xy[3], 2));
		float l1 = std::sqrt(std::pow(xy[0] - x, 2) + std::pow(xy[1] - y, 2));
		float l = l0 + l1 - length;
		if (l < 5)
			return true;
		return false;
	}
} PairLine;

Primitive::Primitive() : vars(RendererVars::getInstance()), cBufferVS(NULL), shaders(ShaderPrimitive::getInstance()),
	widthline(1), isUnderCrs(false), isSelect(false)
{
	memset(&buffervs, 0, sizeof buffervs);
	del = false;
	lineStyle = LineStyle::Solid;
	if (!vars.createConstantBuffer(sizeof colors, vars.getDevice3D(), &cBufferPS))
	{
		return;
	}
	float colorlist[] =
	{0.5, 0.5, 0.5, 1.0, 0.0, 0.0, 0.0, 1.0};
	angle = 0.0f;
	memcpy(colors, colorlist, sizeof colorlist);
	auto context = vars.getDeviceContext();
	context->PSSetConstantBuffers(0, 1, &cBufferPS);
	context->UpdateSubresource(cBufferPS, 0, 0, colors, 0, 0);
	editMode = CREATE;
}

Primitive::~Primitive()
{
}

void Primitive::updateResource(bool fromang)
{
	vars.getDeviceContext()->UpdateSubresource(cBufferPS, 0, 0, colors, 0, 0);
	if (fromang)
		buffervs.params[0] = angle * M_PI / 180.0;
	else
		angle = buffervs.params[0] * 180.0 / M_PI;

	vars.getDeviceContext()->UpdateSubresource(cBufferVS, 0, 0, &buffervs, 0, 0);
}

void Primitive::convertRectToCoord()
{
	buffervs.coord[0] = (rectCoord[0] + rectCoord[2]) / 2.0f;
	buffervs.coord[1] = (rectCoord[1] + rectCoord[3]) / 2.0f;
	buffervs.coord[2] = (std::max(rectCoord[0], rectCoord[2]) - std::min(rectCoord[0], rectCoord[2])) / 2.0f;
	buffervs.coord[3] = (std::max(rectCoord[1], rectCoord[3]) - std::min(rectCoord[1], rectCoord[3])) / 2.0f;
	memcpy(center, buffervs.coord, sizeof(buffervs.coord) >> 1);
}

bool __fastcall Primitive::inRect(float x, float y, float *coords)
{ // Курсор в области примитива
	if (x <= std::min(coords[0], coords[2]))
		return false;
	if (x >= std::max(coords[0], coords[2]))
		return false;

	if (y <= std::min(coords[1], coords[3]))
		return false;
	if (y >= std::max(coords[1], coords[3]))
		return false;
	return true;
}

void __fastcall Primitive::rotateCoordToModelAxis(int x, int y, float *&wcoord)
{
	float ang = -buffervs.params[0];
	float rx = (float)x - center[0];
	float ry = (float)y - center[1];

	wcoord[0] = rx * std::cos(ang) + ry * std::sin(ang) + center[0];
	wcoord[1] = ry * std::cos(ang) - rx * std::sin(ang) + center[1];
}

void __fastcall Primitive::rotateCoordToModelAxisDirect(int x, int y, float *&wcoord)
{
	float ang = buffervs.params[0];
	float rx = (float)x - center[0];
	float ry = (float)y - center[1];

	wcoord[0] = rx * std::cos(ang) + ry * std::sin(ang) + center[0];
	wcoord[1] = ry * std::cos(ang) - rx * std::sin(ang) + center[1];
}

void __fastcall Primitive::rotateCoordToCursorAxis(int x, int y, float *&wcoord)
{
	float ang = buffervs.params[0];
	float rx = (float)x;
	float ry = (float)y;

	wcoord[0] = rx * std::cos(ang) + ry * std::sin(ang);
	wcoord[1] = ry * std::cos(ang) - rx * std::sin(ang);
}

EditMode __fastcall Primitive::dectRectEditMode(float x, float y, float *coord, float *center)
{
	float d[] =
	{coord[0] - x, coord[1] - y, coord[0] - x, coord[3] - y, coord[2] - x, coord[1] - y, coord[2] - x, coord[3] - y, };
	float length = std::sqrt(std::pow(2.0f * center[0], 2) + std::pow(2.0f * center[1], 2));

	for (int i = 0; i < 4; ++i)
	{
		int id = i << 1;
		float cLength = std::sqrt(d[id] * d[id] + d[(id + 1)] * d[(id + 1)]);
		if (cLength < 10)
		{
			return EditMode::Rotate;
		}
	}

	// --------------------------------------------------------
	PairLine pairs[] =
	{PairLine(coord[0], coord[1], coord[2], coord[1]), // topLine
		PairLine(coord[0], coord[3], coord[2], coord[3]), // bottomLine
		PairLine(coord[0], coord[1], coord[0], coord[3]), // leftLine
		PairLine(coord[2], coord[1], coord[2], coord[3]), // rightLine
	};
	EditMode editList[] =
	{EditMode::ResizeY0, EditMode::ResizeY1, EditMode::ResizeX0, EditMode::ResizeX1};
	for (int i = 0; i < 4; ++i)
	{
		if (pairs[i].onLine(x, y))
		{
			return editList[i];
		}
	}
	return EditMode::Move;
}

void __fastcall Primitive::startEditPoint(float x, float y)
{
	startEditXY[0] = x;
	startEditXY[1] = y;
}

void __fastcall Primitive::dragEditMouse(float x, float y)
{
	float begin[2];
	float start[2];
	float *cld = begin;
	rotateCoordToModelAxis(x, y, cld);
	float *s_cld = start;
	rotateCoordToModelAxis(startEditXY[0], startEditXY[1], s_cld);

	float dx = begin[0] - start[0];
	float dy = begin[1] - start[1];

	float coord[] =
	{buffervs.coord[0] - buffervs.coord[2], buffervs.coord[1] - buffervs.coord[3],
		buffervs.coord[0] + buffervs.coord[2], buffervs.coord[1] + buffervs.coord[3]};
	bool recalcCoord = false;
	if (editMode == EditMode::Move)
	{
		buffervs.coord[0] += x - startEditXY[0];
		buffervs.coord[1] += y - startEditXY[1];
		memcpy(center, buffervs.coord, sizeof(buffervs.coord) >> 1);
	}
	else if (editMode == EditMode::Rotate)
	{
		buffervs.params[0] -= (y - startEditXY[1]) * M_PI / 180.0f;
	}
	else if (editMode == EditMode::ResizeY0)
	{
		coord[1] += dy;
		recalcCoord = true;
	}
	else if (editMode == EditMode::ResizeY1)
	{
		coord[3] += dy;
		recalcCoord = true;
	}
	else if (editMode == EditMode::ResizeX0)
	{
		coord[0] += dx;
		recalcCoord = true;
	}
	else if (editMode == EditMode::ResizeX1)
	{
		coord[2] += dx;
		recalcCoord = true;
	}
	if (recalcCoord)
	{
		buffervs.coord[0] = (coord[0] + coord[2]) / 2.0f;
		buffervs.coord[1] = (coord[1] + coord[3]) / 2.0f;
		buffervs.coord[2] = (std::max(coord[0], coord[2]) - std::min(coord[0], coord[2])) / 2.0f;
		buffervs.coord[3] = (std::max(coord[1], coord[3]) - std::min(coord[1], coord[3])) / 2.0f;
		memcpy(center, buffervs.coord, sizeof(buffervs.coord) >> 1);
	}
	startEditXY[0] = x;
	startEditXY[1] = y;
	updateResource(false);
}

void __fastcall Primitive::movePrimitiveRatherPoint(float divX, float divY)
{

	buffervs.coord[0] += divX;
	buffervs.coord[1] += divY;
	memcpy(center, buffervs.coord, sizeof(buffervs.coord) >> 1);
	updateResource(false);
}

void __fastcall Primitive::rotatePrimitiveRatherPoint(float *lcenter, float angle)
{
	if (angle == 0.0f)
	{
		return;
	}
	float newpos[] =
	{0.0f, 0.0f};
	float *_newpos = &newpos[0];
	float *_newpos1 = &newpos[2];
	rotateAroundPoint(buffervs.coord[0], buffervs.coord[1], _newpos, lcenter, -angle);

	buffervs.params[0] -= angle;
	memcpy(buffervs.coord, newpos, sizeof(buffervs.coord) >> 1);
	memcpy(center, buffervs.coord, sizeof(buffervs.coord) >> 1);
	updateResource(false);

}

void __fastcall Primitive::reCalcCoord()
{
	float coord[] =
	{buffervs.coord[0] - buffervs.coord[2], buffervs.coord[1] - buffervs.coord[3],
		buffervs.coord[0] + buffervs.coord[2], buffervs.coord[1] + buffervs.coord[3]};
	memcpy(rectCoord, coord, sizeof coord);
}

void __fastcall Primitive::rotateAroundPoint(float x, float y, float *&wcoord, float *point, float angle)
{
	float ang = angle;
	float rx = (float)x - point[0];
	float ry = (float)y - point[1];

	wcoord[0] = rx * std::cos(ang) + ry * std::sin(ang) + point[0];
	wcoord[1] = ry * std::cos(ang) - rx * std::sin(ang) + point[1];
}
