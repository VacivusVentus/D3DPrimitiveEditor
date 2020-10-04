// ---------------------------------------------------------------------------

#ifndef LinePrimitiveH
#define LinePrimitiveH
#include "Primitive.h"

// ---------------------------------------------------------------------------
class LinePrimitive : public Primitive {
public:
    LinePrimitive();
    void draw(ID3D11DeviceContext *m_context) override;
    void releasePrimitive() override;
    void __fastcall setFirstPoint(int x, int y)override;
    void __fastcall setSecondPoint(int x, int y)override;

    char *getTypePrimitive() override {
        return "Линия";
    }
    bool __fastcall isUnderCursor(int x, int y) override;


	const char *type()override {return "line";}
	void __fastcall dragEditMouse(float x, float y) override;
	void __fastcall movePrimitiveRatherPoint(float divX, float divY) override;
	void __fastcall rotatePrimitiveRatherPoint(float *center, float angle) override;
	void convertRectToCoord() override;

	float *getCoords() override{
		return &buffervs.coord[0];
	}
private:
    void detectNearOperation(float x, float y);
protected:
    RendererVars &vars;
    ShaderPrimitive &shaders;
    int xy[2];
};
#endif
