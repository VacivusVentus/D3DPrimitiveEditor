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
	 //Вычисляем расположен ли курсор в прямоугольной области примитива
	static EditMode __fastcall dectRectEditMode(float x, float y, float *coord, float *center);
	/// Обработка действий пользователем, если объек выбран для редактирования
	virtual void __fastcall startEditPoint(float x, float y);
	virtual void __fastcall dragEditMouse(float x, float y);
	// Определяем находиься ли примитив под курсором
	virtual bool __fastcall isUnderCursor(int x, int y) = NULL;
	virtual const char *type() = NULL; //Тип примитива
	virtual void __fastcall rotatePrimitiveRatherPoint(float *center, float angle);

	virtual float *getCoords()
	{
		reCalcCoord(); //расчёт координыти левого верхеного угла и правого нижнего угла
		return &rectCoord[0];
	}
	virtual void __fastcall movePrimitiveRatherPoint(float divX, float divY);

	void __fastcall rotateCoordToModelAxisDirect(int x, int y, float *&wcoord);
	// --------------------------------------------------------------------------
	// Груповое редактирование примитивов
	// --------------------------------------------------------------------------
	// Быстрый возврат переменных объекта
	// ----------------------------------------------------------------------------
	void updateResource(bool fromang = true); //Обновление буферов для шейдеров, а также конвертация угла
	virtual void convertRectToCoord();     // перерасчёт перемещения центра(или координат точек для отрезка), а также
										  //изменнения угла поворота примитива, при вращение примитива относительно точки
										  //для окружности и прямоугольника наследуется из класса Primitive,
										  //а для линии переопределяется метод

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
		float colors[8];    // цве заливки и цвет линии
		uint32_t params[4]; // Множитель для перерасчёта длины отрезка в зависимости от толщины линии
	}psBuffer;

	ID3D11Buffer *cBufferPS;
	int widthline;
	bool del;   //Примитив необходимо удалить
	bool isUnderCrs; //Курсор наведен на примитив
	bool isSelect;  //Объект выбран или один из выбранных примитивов
	LineStyle lineStyle; //Номер стиля линии
	float angle; //угол поворота пимитива
	float center[4]; // центр x, y(0, 1) Половина длин по осям dx, dy(2, 3)
	float startEditXY[2]; // координаты курсора мыши при нажатие левой кнопки мыши, а также последнее положение курсора
						 //после выполнения пермещения, поворота и редактирование размеров, при услови, что до нажатия
						  // курсор был наведён на примитив
	float rectCoord[4]; // координаты окружности и прямоуголбника по верхнему левому углу и по нижниму правому.

	struct
	{
		float coord[4]; // координаты линии / Для прямоугольника и окружности x, y - координаты центра
                        //z,w - наименьшие расстояие от центра до грани по оси x и по оси y соотвтетственно
		float params[4]; // уголол, толщина линии для перерасчёта координат начала линий для обвода рамки окружности
		 // и прямоугольника резерв, резерв
	}

	buffervs;
	ID3D11Buffer *cBufferVS;
	EditMode editMode;

	void __fastcall rotateCoordToModelAxis(int x, int y, float *&wcoord);
	void __fastcall reCalcCoord();
	void __fastcall rotateAroundPoint(float x, float y, float *&wcoord, float *point, float angle);
};
#endif
