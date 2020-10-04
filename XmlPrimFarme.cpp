// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "XmlPrimFarme.h"
#include "SettingStyleUnit.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TXMLPrimitiveElement *XMLPrimitiveElement;

// ---------------------------------------------------------------------------
__fastcall TXMLPrimitiveElement::TXMLPrimitiveElement(TComponent* Owner) : TFrame(Owner)
{
}
// ---------------------------------------------------------------------------

void TXMLPrimitiveElement::setPrimitiveInfo(Primitive *primitive)
{
    this->primitive = primitive;
    PrimitiveName->Caption = primitive->getTypePrimitive();
}

void __fastcall TXMLPrimitiveElement::styleButtonClick(TObject *Sender)
{
    SettingStyleDialog->setPrimitive(primitive);
    SettingStyleDialog->ShowModal();
}
// ---------------------------------------------------------------------------

void __fastcall TXMLPrimitiveElement::delButtonClick(TObject *Sender)
{
    primitive->deletePrimitive();
}
// ---------------------------------------------------------------------------
