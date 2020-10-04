// ---------------------------------------------------------------------------

#ifndef MainUnitH
#define MainUnitH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Menus.hpp>
#include "SurfaceUnit.h"
#include "PrimitiveManager.h"
#include "XMLConverter.h"
#include <Vcl.Dialogs.hpp>
#include "XmlPrimFarme.h"
#include "SettingStyleUnit.h"
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include <vector>
#include <mutex>

// ---------------------------------------------------------------------------
class TMainForm : public TForm {
__published: // IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TSurfaceRender *SurfaceRender1;
	TMainMenu *MainMenu1;
	TMenuItem *N1;
	TMenuItem *NewFileTool;
	TMenuItem *saveTool;
	TMenuItem *saveAsTool;
	TMenuItem *openXMLTool;
	TMenuItem *quitMenuItem;
	TMenuItem *N7;
	TMenuItem *N8;
	TMenuItem *AddLinePrimitiveTool;
	TMenuItem *AddElliopsePrimitiveTool;
	TMenuItem *AddRectPrimitiveTool;
	TScrollBox *PrimitiveScrollBox;
	TSplitter *Splitter1;
	TSaveDialog *XMLSaveDialog;
	TOpenDialog *XMLOpenDialog;
	TXMLDocument *XMLDocument1;

	void __fastcall quitMenuItemClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall AddLinePrimitiveToolClick(TObject *Sender);
	void __fastcall AddElliopsePrimitiveToolClick(TObject *Sender);
	void __fastcall AddRectPrimitiveToolClick(TObject *Sender);
	void __fastcall addPrimitiveToList();
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall saveAsToolClick(TObject *Sender);
	void __fastcall openXMLToolClick(TObject *Sender);
	void __fastcall saveToolClick(TObject *Sender);
	void __fastcall NewFileToolClick(TObject *Sender);
	void __fastcall SurfaceRender1RendererMouseDown(TObject *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall SurfaceRender1RendererMouseUp(TObject *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall SurfaceRender1RendererMouseMove(TObject *Sender,
		TShiftState Shift, int X, int Y);
	void __fastcall SurfaceRender1RendererDblClick(TObject *Sender);

private: // User declarations
	PrimitiveManager *primitiveManager;
	AnsiString filename;

	std::vector<TXMLPrimitiveElement*>xmlPrContainer;

	TXMLPrimitiveElement *delEl;
	bool mouseDown[2];

	std::mutex drawMutex;

public: // User declarations
	__fastcall TMainForm(TComponent* Owner);
	void __fastcall draw();

protected:
	void clearManegerElemetns();

	void __fastcall deleteElement(TMessage &msg);
	void __fastcall changeCursor(TMessage &msg);

	BEGIN_MESSAGE_MAP
	 MESSAGE_HANDLER(WM_USER + 1, TMessage, deleteElement);
	 MESSAGE_HANDLER(WM_USER + 2, TMessage, changeCursor);
	END_MESSAGE_MAP(TForm) // <-- error!
	};

// ---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
// ---------------------------------------------------------------------------
#endif
