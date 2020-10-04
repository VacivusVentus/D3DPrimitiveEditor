// ---------------------------------------------------------------------------

#ifndef SurfaceUnitH
#define SurfaceUnitH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <d3d11.h>
#include <dxgi.h>
#include <windows.h>
#include <Vcl.ExtCtrls.hpp>
#include <thread>
#include <Vcl.ExtCtrls.hpp>
#include "RendererVars.h"

// ---------------------------------------------------------------------------
class TSurfaceRender : public TFrame {
__published: // IDE-managed Components
	TPanel *Panel1;
	TScrollBar *ScrollXBar;
	TPanel *Panel2;
	TScrollBar *ScrollYBar;
	TPanel *Renderer;
	void __fastcall FrameResize(TObject *Sender);
	void __fastcall ScrollYBarChange(TObject *Sender);
	void __fastcall ScrollXBarChange(TObject *Sender);

    void __fastcall getDivXY(int &dX, int &dY);
    void __fastcall RendererResize(TObject *Sender);
    void __fastcall setScrollRange(int x, int y);

 protected:
 void updateMatrix();

private: // User declarations
	RendererVars &rendererVars;
	IDXGISwapChain *m_swapchain;
	ID3D11RenderTargetView *m_rendertargetview;
	std::thread *rendering;
	bool thWorking;
	float matrix[16];
	float divA[2];
	int size[2];
    ID3D11Buffer *m_cbuffer;
    int posX, posY;

public: // User declarations
	__fastcall TSurfaceRender(TComponent* Owner);
	void ReleaseGUI();
	void ReleaseSurface();
	void ReInitRenderer();

   void __fastcall(__closure *drawEveryPrimitive)() = NULL;
};


// ---------------------------------------------------------------------------
extern PACKAGE TSurfaceRender *SurfaceRender;
// ---------------------------------------------------------------------------
#endif
