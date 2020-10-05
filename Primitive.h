// ---------------------------------------------------------------------------

#ifndef PrimitiveH
#define PrimitiveH

#include "RendererVars.h"
#include "ShaderPrimitive.h"

// ---------------------------------------------------------------------------
enum EditMode
{
	CREATE = 0, Move, ResizeX0, ResizeY0, ResizeX1, ResizeY1, Rotate, LChgXY0, LChgXY1
};

// 0-ничего,
// 1 - перемещение,
// Кроме линии 2-5
// 2-измение размера по горизонале слева
// 3-измение размера по вертикали сверху
// 4-измение размера по горизонале справа
// 5-измение размера по вертикали снизу
// 6-вращение
// Только для линии
// 7-измениение координат первой точки у линии
// 7-измениение координат второй точки у линии
// ---------------------------------------------------------------------------
class Primitive
{
public:
	Primitive();
	virtual ~Primitive();

	virtual void draw(ID3D11DeviceContext *m_context) = NULL;

	virtual void releasePrimitive() = NULL;
	virtual void __fastcall setFirstPoint(int x, int y) = NULL;
	virtual void __fastcall setSecondPoint(int x, int y) = NULL;
	virtual char *getTypePrimitive() = NULL;

	/*
	 */
	static EditMode __fastcall dectRectEditMode(float x, float y, float *coord, float *center);
	virtual void __fastcall startEditPoint(float x, float y);
	virtual void __fastcall dragEditMouse(float x, float y);
	virtual bool __fastcall isUnderCursor(int x, int y) = NULL;
	virtual const char *type() = NULL;
	virtual void __fastcall rotatePrimitiveRatherPoint(float *center, float angle);

	virtual float *getCoords()
	{
		reCalcCoord();
		return &rectCoord[0];
	}
	virtual void __fastcall movePrimitiveRatherPoint(float divX, float divY);

	void __fastcall rotateCoordToModelAxisDirect(int x, int y, float *&wcoord);
	// --------------------------------------------------------------------------
	// Груповое редактирование примитивов
	// --------------------------------------------------------------------------
	// Быстрый возврат переменных объекта
	// ----------------------------------------------------------------------------
	void updateResource(bool fromang = true);
	virtual void convertRectToCoord();

	float *getStyle() const
	{
		return (float*)psBuffer.colors;
	}

	int* getWidthLine()
	{
		return &widthline;
	}

	void deletePrimitive()
	{
		del = true;
	}

	bool isDel() const
	{
		return del;
	}

	LineStyle *getLineStyle()
	{
		return &lineStyle;
	}

	void resetUnderCursor()
	{
		isUnderCrs = false;
	}

	void setUnderCursor()
	{
		isUnderCrs = true;
	}

	void resetSelect()
	{
		isSelect = false;
	}

	void setSelect()
	{
		isSelect = true;
	}

	float *getAngle()
	{
		return &angle;
	}

	float *getInsideAngle()
	{
		return &buffervs.params[0];
	}

	EditMode getCurEditMode() const
	{
		return editMode;
	}

	float *getStartEditXY() const
	{
		return (float*)startEditXY;
	}

	float *getCenter() const
	{
		return (float*)center;
	}

	// ----------------------------------------------------------------------
protected:
	bool __fastcall inRect(float x, float y, float *coords);
	void __fastcall rotateCoordToCursorAxis(int x, int y, float *&wcoord);

	RendererVars &vars;
	ShaderPrimitive &shaders;

	struct
	{
		float colors[8];
		uint32_t params[4];
	}psBuffer;

	ID3D11Buffer *cBufferPS;
	int widthline;
	bool del;
	bool isUnderCrs;
	bool isSelect;
	LineStyle lineStyle;
	float angle;
	float center[4]; // центр x, y(0, 1) Половина длин по осям dx, dy(2, 3)
	float startEditXY[2];
	float rectCoord[4];

	struct
	{
		float coord[4]; // координаты линии
		float params[4]; // уголол, резерв, резерв, резерв
	}

	buffervs;
	ID3D11Buffer *cBufferVS;
	EditMode editMode;

	void __fastcall rotateCoordToModelAxis(int x, int y, float *&wcoord);
	void __fastcall reCalcCoord();
	void __fastcall rotateAroundPoint(float x, float y, float *&wcoord, float *point, float angle);
};
#endif
