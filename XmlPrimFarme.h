// ---------------------------------------------------------------------------

#ifndef XmlPrimFarmeH
#define XmlPrimFarmeH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>

#include "Primitive.h"

// ---------------------------------------------------------------------------
class TXMLPrimitiveElement : public TFrame {
__published: // IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TLabel *PrimitiveName;
	TSpeedButton *delButton;
	TSpeedButton *styleButton;
	void __fastcall styleButtonClick(TObject *Sender);
	void __fastcall delButtonClick(TObject *Sender);
	Primitive *getPrimitive() {return primitive;}
	void reset(){primitive = NULL;}
private: // User declarations
	Primitive *primitive;

public: // User declarations
	__fastcall TXMLPrimitiveElement(TComponent* Owner);
	void setPrimitiveInfo(Primitive *primitive);
};

// ---------------------------------------------------------------------------
extern PACKAGE TXMLPrimitiveElement *XMLPrimitiveElement;
// ---------------------------------------------------------------------------
#endif
