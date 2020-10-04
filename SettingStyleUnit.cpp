// ---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SettingStyleUnit.h"
// ---------------------------------------------------------------------
#pragma link "cspin"
#pragma resource "*.dfm"
TSettingStyleDialog *SettingStyleDialog;

// ---------------------------------------------------------------------
__fastcall TSettingStyleDialog::TSettingStyleDialog(TComponent* AOwner) : TForm(AOwner)
{
}

// ---------------------------------------------------------------------
void __fastcall TSettingStyleDialog::setFillColorButtonClick(TObject *Sender)
{
    if (ColorDialogSet->Execute())
    {
        unsigned int c = ColorDialogSet->Color;
        float *styleFig = primitive->getStyle();
        for (int i = 0; i < 3; ++i)
        {
            styleFig[i] = (float)(c & 255) / 256.0f;
            c >>= 8;
        }
        primitive->updateResource();
    }
}

// ---------------------------------------------------------------------------
void __fastcall TSettingStyleDialog::setPrimitive(Primitive *primitive)
{
    this->primitive = primitive;
    widthLineUpDown->Position = *primitive->getWidthLine();
    widthLineLabel->Caption = IntToStr(widthLineUpDown->Position);
    LineStyle *ls = primitive->getLineStyle();
    lineStyleComboBox->ItemIndex = *ls;
    angleLabelEdit->Text = FloatToStr(*primitive->getAngle());
}

void __fastcall TSettingStyleDialog::setPenColorButtonClick(TObject *Sender)
{
    if (ColorDialogSet->Execute())
    {
        unsigned int c = ColorDialogSet->Color;
        float *styleFig = primitive->getStyle();
        for (int i = 4; i < 7; ++i)
        {
            styleFig[i] = (float)(c & 255) / 256.0f;
            c >>= 8;
        }
        primitive->updateResource();
    }
}
// ---------------------------------------------------------------------------

void __fastcall TSettingStyleDialog::widthLineUpDownChanging(TObject *Sender, bool &AllowChange)

{
    widthLineLabel->Caption = IntToStr(widthLineUpDown->Position);
    int *v = primitive->getWidthLine();
    v[0] = widthLineUpDown->Position;
}
// ---------------------------------------------------------------------------

void __fastcall TSettingStyleDialog::lineStyleComboBoxChange(TObject *Sender)
{
    LineStyle *ls = primitive->getLineStyle();
    *ls = (LineStyle)lineStyleComboBox->ItemIndex;
}
// ---------------------------------------------------------------------------

void __fastcall TSettingStyleDialog::setAngleButtonClick(TObject *Sender)
{
    *primitive->getAngle() = StrToFloat(angleLabelEdit->Text);
    primitive->updateResource();
}
// ---------------------------------------------------------------------------
