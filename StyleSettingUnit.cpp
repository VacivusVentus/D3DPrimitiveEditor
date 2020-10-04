//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "StyleSettingUnit.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TStyleSettingDialog *StyleSettingDialog;
//--------------------------------------------------------------------- 
__fastcall TStyleSettingDialog::TStyleSettingDialog(TComponent* AOwner)
	: TOKBottomDlg(AOwner)
{
}
//--------------------------------------------------------------------- 
void __fastcall TStyleSettingDialog::HelpBtnClick(TObject *Sender)
{
	Application->HelpContext(HelpContext);
}
//--------------------------------------------------------------------- 
