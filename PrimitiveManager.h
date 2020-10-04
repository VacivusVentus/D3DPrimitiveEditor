// ---------------------------------------------------------------------------

#ifndef PrimitiveManagerH
#define PrimitiveManagerH
#include "LinePrimitive.h"
#include "EllipsePrimitive.h"
#include "RectPrimitive.h"
#include <vector>

// ---------------------------------------------------------------------------
enum OperationID
{
    NONE = 0, ADDNEWPRIMITIVE, EDITPRIMITIVE
};

class PrimitiveManager
{
public:
	PrimitiveManager(HWND);
    ~PrimitiveManager();
    bool __fastcall addPrimitive(Primitive *primitive);
    void drawPrimitives(HWND);
    void __fastcall leftMouseDown(int x, int y, bool shift);
    void __fastcall moveCursor(int x, int y);
    void __fastcall resetCursor(int x, int y);

    Primitive *getPrimitiveUnderCursor() const
    {
        return underCursor;
    }

    Primitive *currentPrimitive() const
    {
        return current;
    }

    std::vector<Primitive*>&getPrimitives()
    {
        return primitives;
    }
    void __fastcall getMaxXY(float &x, float &y);
    void __fastcall getSelectedRect(float *&rect);

protected:
    void updateGroupSelect();
    void __fastcall dragEditGroupMouse(float x, float y);

private:
    std::vector<Primitive*>primitives;
    std::vector<Primitive*>selected;

    Primitive *underCursor;
    bool skipmove;

    Primitive *current;
    ID3D11DeviceContext *m_context;
    OperationID operationID;
	bool leftButtonDown;
    HWND hwnd;
    RectPrimitive *rectPrimitive; // Для отрисовки рамки выделение группы объектов
};
#endif
