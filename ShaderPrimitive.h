// ---------------------------------------------------------------------------

#ifndef ShaderPrimitiveH
#define ShaderPrimitiveH

#include <vector>
#include "RendererVars.h"

// ---------------------------------------------------------------------------
enum LineStyle {
	Solid = 0, Dashed, Points
};

// ---------------------------------------------------------------------------
class ShaderPrimitive {
public:
	static ShaderPrimitive &getInstance() {
		static ShaderPrimitive shaders;
		return shaders;
	}

	void __fastcall setSelect(bool select);

	bool selected() const {
		return isSelect;
	}
	void __fastcall bindLineShaderGroup(ID3D11DeviceContext *m_context,
		LineStyle lineStyle, bool undercursor = false);
	void __fastcall bindEllipseShaderGroup(ID3D11DeviceContext *m_context, bool undercursor = false);
	void __fastcall bindEllipseLinesShaderGroup(ID3D11DeviceContext *m_context,
		LineStyle lineStyle, bool undercursor = false);
	void __fastcall bindRectShaderGroup(ID3D11DeviceContext *m_context, bool undercursor = false);
	void __fastcall bindRectLineShaderGroup(ID3D11DeviceContext *m_context,
		LineStyle lineStyle, bool undercursor = false);

    void __fastcall bindRectLineFrameShaderGroup(ID3D11DeviceContext *m_context);
    void __fastcall bindRectRectTypeFrameShaderGroup(ID3D11DeviceContext *m_context);
	void releaseAllShaders();

protected:
	std::vector<unsigned char>loadShader(const wchar_t* resName);
	bool __fastcall createVS(const wchar_t* resName, ID3D11Device *m_device,
		ID3D11VertexShader **shader);
	bool __fastcall createPS(const wchar_t* resName, ID3D11Device *m_device,
		ID3D11PixelShader **shader);

	bool isCorrect() const {
		return correct;
	};
	void setLineStyle(ID3D11DeviceContext *m_context, LineStyle lineStyle);
	void setLineStyleEllipse(ID3D11DeviceContext *m_context, LineStyle lineStyle);

private:
	ShaderPrimitive();

	bool isSelect;
	bool correct; // Инициализация шейдеров
	ID3D11VertexShader *m_lineVS;

	ID3D11PixelShader *m_lineVS_N;
	ID3D11PixelShader *m_lineVS_Sel;
	ID3D11PixelShader *m_lineVS_UCursor;
	ID3D11VertexShader *m_triFilVS;
	ID3D11VertexShader *m_triFilRVS;

	ID3D11PixelShader *m_penmainPS;
	ID3D11VertexShader *m_triFilVS_line;
	ID3D11VertexShader *m_triFilRVS_line;

	ID3D11PixelShader *m_PSStyleLine_dashed;
	ID3D11PixelShader *m_PSStyleLine_points;
	ID3D11PixelShader *m_lineVS_ULCursor;


	ID3D11VertexShader *m_frameForLineVS;
	ID3D11VertexShader *m_frameForRectTypesVS;
	RendererVars &rendererVars;
};
#endif
