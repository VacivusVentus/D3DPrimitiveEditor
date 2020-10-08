// ---------------------------------------------------------------------------

#pragma hdrstop

#include "PrimitiveManager.h"
#include <windows.h>
#include <algorithm>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

PrimitiveManager::PrimitiveManager(HWND hWnd) : m_context(NULL), operationID(OperationID::NONE), underCursor(NULL),
	rectPrimitive(NULL), hwnd(hWnd)
{
	primitives.clear();
	skipmove = false;
	leftButtonDown = false;
	rectPrimitive = new RectPrimitive();
}

PrimitiveManager::~PrimitiveManager()
{
	delete rectPrimitive;
	rectPrimitive = NULL;
	for (std::vector<Primitive*>::iterator p = primitives.begin(), endIter = primitives.end(); p < endIter; ++p)
	{
		(*p)->releasePrimitive();
		delete(*p);
	}
	primitives.clear();
	selected.clear();
}

bool __fastcall PrimitiveManager::addPrimitive(Primitive *primitive)
{
	if (!primitive)
		return false;
	primitives.push_back(primitive);
	current = primitive;
	operationID = OperationID::ADDNEWPRIMITIVE;
	skipmove = true;
	return true;
}

void PrimitiveManager::drawPrimitives(HWND handle)
{
	if (!m_context)
	{
		m_context = RendererVars::getInstance().getDeviceContext();
		if (!m_context)
		{
			return;
		}
	}
	for (std::vector<Primitive*>::iterator p = primitives.begin(), endIter = primitives.end(); p < endIter; ++p)
	{
		(*p)->draw(m_context);
		if ((*p)->isDel())
		{
			auto deletethis = *p;
			PostMessage(handle, WM_USER + 1, (WPARAM) deletethis, 0);
			primitives.erase(p);
			break;
		}
	}
	if (rectPrimitive)
	{
		if (selected.size() > 1)
		{
			rectPrimitive->frameOnlyDraw(m_context);
		}
	}
}

void __fastcall PrimitiveManager::leftMouseDown(int x, int y, bool shift)
{

	if (operationID == OperationID::ADDNEWPRIMITIVE) //—оздание нового примитива
	{
		current->setFirstPoint(x, y);

		skipmove = false;
	}
	else
	{
		if (underCursor)
		{
			if (!shift && underCursor != rectPrimitive)
			{
				for (auto sel : selected)
					sel->resetSelect();
				selected.clear();
			}
			bool add = underCursor != rectPrimitive;
			if (add)
			{
				for (auto sel : selected)
				{
					if (underCursor == sel)
					{
						add = false;
						break;
					}
				}
			}
			if (add)
			{
				selected.push_back(underCursor);
			}
			if (shift && selected.size() > 1)
			{
				float *ccord = rectPrimitive->getCoords();
				getSelectedRect(ccord);
				rectPrimitive->updateResource();
			}
			operationID = OperationID::EDITPRIMITIVE;
			underCursor->setSelect();
		}
		else
		{
			if (!selected.empty())
			{
				for (auto sel : selected)
				{
					sel->resetSelect();
				}
				selected.clear();
				operationID = NONE;
			}
		}
	}
	if (operationID == OperationID::EDITPRIMITIVE)
	{
		if (!selected.empty())
			leftButtonDown = true;
		if (!underCursor)
		{
			operationID = NONE;
		}
		if (selected.size() == 1)
		{
			selected[0]->startEditPoint(x, y);
		}
		if (selected.size() > 1)
		{
			rectPrimitive->startEditPoint(x, y);
		}
	}
}

void __fastcall PrimitiveManager::moveCursor(int x, int y)
{
	if (skipmove)
		return;
	if (operationID == OperationID::EDITPRIMITIVE && leftButtonDown)
	{
		if (selected.size() == 1)
		{
			selected[0]->dragEditMouse(x, y);
		}
		else if (selected.size() > 1)
		{
			dragEditGroupMouse(x, y);
			float *ccord = rectPrimitive->getCoords();
			getSelectedRect(ccord);
			rectPrimitive->updateResource(false);
		}
	}
	if (operationID == OperationID::ADDNEWPRIMITIVE)
	{
		current->setSecondPoint(x, y);
	}
	else
	{
		if (underCursor)
		{
			underCursor->resetUnderCursor();
			underCursor = NULL;
			SendMessage(hwnd, WM_USER + 2, EditMode::CREATE, 0);
		}

		for (auto p : primitives)
		{
			if (p->isUnderCursor(x, y))
			{
				if (underCursor)
				{
					underCursor->resetUnderCursor();
					underCursor = NULL;
				}
				underCursor = p;
				underCursor->setUnderCursor();
			}
		}
		if (selected.size() > 1)
		{
			if (rectPrimitive->isUnderCursor(x, y))
			{
				underCursor = rectPrimitive;
				rectPrimitive->setUnderCursor();
			}
		}
		if(selected.size() && underCursor)
		{

			SendMessage(hwnd, WM_USER + 2, underCursor->getCurEditMode(), 0);
		}
	}
}

void __fastcall PrimitiveManager::resetCursor(int x, int y)
{
	if (operationID == OperationID::ADDNEWPRIMITIVE || leftButtonDown)
	{
		operationID = OperationID::NONE;
	}
	leftButtonDown = false;
}

void PrimitiveManager::updateGroupSelect() // –асчЄт рамки облости выделенных примитивов
{
	float coord[4] =
	{0};
	if (selected.empty())
		return;
	auto p = selected.begin();
	memcpy(coord, (*p)->getCoords(), sizeof coord);
	auto end_ = selected.end();
	for (; p < end_; ++p)
	{
		float *cloud = (*p)->getCoords();
		float m0[] =
		{std::min(cloud[0], cloud[2]), std::min(cloud[1], cloud[3]), std::max(cloud[0], cloud[2]),
				std::max(cloud[1], cloud[3])};
		for (int i = 0; i < 4; ++i)
		{
			float m1[] =
			{std::min(coord[i], cloud[i]), std::max(coord[i], cloud[i])};
			coord[i] = m1[(i < 2) ? 0 : 1];
		}
	}
}

void __fastcall PrimitiveManager::getMaxXY(float &x, float &y)
{
	x = 0.0f;
	y = 0.0f;
	for (auto p : primitives)
	{
		auto coords = p->getCoords();
		x = std::max(x, std::max(coords[0], coords[2]));
		y = std::max(y, std::max(coords[1], coords[3]));
	}
}

void __fastcall PrimitiveManager::getSelectedRect(float *&rect) //ќпределение максимальные и минимальные координаты,
{                                                       //занимаемые примитивом
	if (selected.empty())
	{
		return;
	}
	memset(rect, 0, sizeof(float) << 2); // обнуление массива дл€ заполнени€ мин/макс координат
	rect[0] = 1000000.0;
	rect[1] = 1000000.0;
	for (auto p : selected)
	{
		float coordarr[4];
		float *orig = p->getCoords();
		memcpy(coordarr, orig, sizeof coordarr);
		float *coords = coordarr;

		p->rotateCoordToModelAxisDirect(coords[0], coords[1], coords);
		float *crd = &coords[2];
		p->rotateCoordToModelAxisDirect(coords[2], coords[3], crd);

		rect[0] = std::min(rect[0], std::min(coords[0], coords[2]));
		rect[1] = std::min(rect[1], std::min(coords[1], coords[3]));

		rect[2] = std::max(rect[2], std::max(coords[0], coords[2]));
		rect[3] = std::max(rect[3], std::max(coords[1], coords[3]));
		/// ѕоиск минимум/максимум по второй диаганали       //////
		coords[0] = orig[0];
		coords[1] = orig[3];
		coords[2] = orig[2];
		coords[3] = orig[1];
		p->rotateCoordToModelAxisDirect(coords[0], coords[1], coords);
		p->rotateCoordToModelAxisDirect(coords[2], coords[3], crd);

		rect[0] = std::min(rect[0], std::min(coords[0], coords[2]));
		rect[1] = std::min(rect[1], std::min(coords[1], coords[3]));

		rect[2] = std::max(rect[2], std::max(coords[0], coords[2]));
		rect[3] = std::max(rect[3], std::max(coords[1], coords[3]));
	}
	rectPrimitive->convertRectToCoord();
}

void __fastcall PrimitiveManager::dragEditGroupMouse(float x, float y)
{
	auto editMode = rectPrimitive->getCurEditMode();
	float *startEditXY = rectPrimitive->getStartEditXY();
	float dx = x - startEditXY[0];
	float dy = y - startEditXY[1];
	float chgAngle = dy * M_PI / 180.0f;
	float *center = rectPrimitive->getCenter();
	for (auto sel : selected)
	{
		if (editMode == EditMode::Move)
		{
			sel->movePrimitiveRatherPoint(dx, dy);
		}
		else if (editMode == EditMode::Rotate)
		{
			sel->rotatePrimitiveRatherPoint(center, chgAngle);
		}
		else if (editMode == EditMode::ResizeY0)
		{
		}
		else if (editMode == EditMode::ResizeY1)
		{
		}
		else if (editMode == EditMode::ResizeX0)
		{
		}
		else if (editMode == EditMode::ResizeX1)
		{
		}
	}
	startEditXY[0] = x;
	startEditXY[1] = y;
}
