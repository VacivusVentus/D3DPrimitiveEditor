// ---------------------------------------------------------------------------

#ifndef RectPrimitiveH
#define RectPrimitiveH
#include "Primitive.h"

// ---------------------------------------------------------------------------
class RectPrimitive : public Primitive
{
public:
    RectPrimitive();
    void draw(ID3D11DeviceContext *m_context) override;
    void frameOnlyDraw(ID3D11DeviceContext *m_context);
	void releasePrimitive() override;
    void __fastcall setFirstPoint(int x, int y)override;
    void __fastcall setSecondPoint(int x, int y)override;

    char *getTypePrimitive() override
    {
        return "Прямоугольник";
    }
    bool __fastcall isUnderCursor(int x, int y) override;


    const char *type()override {return "rect";}
	 

private:

	int xy[2];
	float coords[4];

protected:
    RendererVars &vars;
    ShaderPrimitive &shaders;
};
#endif
