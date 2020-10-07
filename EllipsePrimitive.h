// ---------------------------------------------------------------------------

#ifndef EllipsePrimitiveH
#define EllipsePrimitiveH
#include "Primitive.h"

// ---------------------------------------------------------------------------
class EllipsePrimitive : public Primitive {
public:
	EllipsePrimitive();
	void draw(ID3D11DeviceContext *m_context) override;
	void releasePrimitive() override;
	void __fastcall setFirstPoint(int x, int y) override;
	void __fastcall setSecondPoint(int x, int y) override;
    void updateResource(bool fromang ) override;

	char *getTypePrimitive() override {
		return "έλθορ";
	}

	bool __fastcall isUnderCursor(int x, int y) override;
	const char *type()override {return "ellipse";}
private:
	int xy[2];

protected:
	RendererVars &vars;
	ShaderPrimitive &shaders;
};
#endif
