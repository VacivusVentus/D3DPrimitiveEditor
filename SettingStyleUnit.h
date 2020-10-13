//----------------------------------------------------------------------------
#ifndef SettingStyleUnitH
#define SettingStyleUnitH
//----------------------------------------------------------------------------
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Graphics.hpp>
#include <System.Classes.hpp>
#include <System.SysUtils.hpp>
#include <Winapi.Windows.hpp>
#include <System.hpp>
#include <Vcl.Dialogs.hpp>
#include "Primitive.h"
#include "cspin.h"
#include <Vcl.ComCtrls.hpp>
//----------------------------------------------------------------------------
class TSettingStyleDialog : public TForm
{
__published:
	TColorDialog *ColorDialogSet;
	TButton *setFillColorButton;
	TButton *setPenColorButton;
	TLabel *Label1;
	TUpDown *widthLineUpDown;
	TLabel *widthLineLabel;
	TComboBox *lineStyleComboBox;
	TLabeledEdit *angleLabelEdit;
    TButton *setAngleButton;
	void __fastcall setFillColorButtonClick(TObject *Sender);
	void __fastcall setPenColorButtonClick(TObject *Sender);
	void __fastcall widthLineUpDownChanging(TObject *Sender, bool &AllowChange);
	void __fastcall lineStyleComboBoxChange(TObject *Sender);
    void __fastcall setAngleButtonClick(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
private:
	Primitive *primitive;
    bool skip;
public:
	virtual __fastcall TSettingStyleDialog(TComponent* AOwner);
	void __fastcall setPrimitive(Primitive *primitive);
};
//----------------------------------------------------------------------------
extern PACKAGE TSettingStyleDialog *SettingStyleDialog;
//----------------------------------------------------------------------------
#endif    
