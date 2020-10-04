object SettingStyleDialog: TSettingStyleDialog
  Left = 227
  Top = 108
  BorderStyle = bsDialog
  Caption = 'Dialog'
  ClientHeight = 116
  ClientWidth = 458
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 183
    Top = 36
    Width = 82
    Height = 13
    Caption = #1058#1086#1083#1097#1080#1085#1072' '#1083#1080#1085#1080#1080':'
  end
  object widthLineLabel: TLabel
    Left = 271
    Top = 36
    Width = 12
    Height = 13
    Caption = '10'
  end
  object setFillColorButton: TButton
    Left = 0
    Top = 0
    Width = 177
    Height = 25
    Caption = #1059#1089#1090#1072#1085#1086#1074#1080#1090#1100' '#1094#1074#1077#1090' '#1079#1072#1083#1080#1074#1082#1080
    TabOrder = 0
    OnClick = setFillColorButtonClick
  end
  object setPenColorButton: TButton
    Left = 0
    Top = 31
    Width = 177
    Height = 25
    Caption = #1059#1089#1090#1072#1085#1086#1074#1080#1090#1100' '#1094#1074#1077#1090' '#1087#1077#1088#1072
    TabOrder = 1
    OnClick = setPenColorButtonClick
  end
  object widthLineUpDown: TUpDown
    Left = 287
    Top = 32
    Width = 17
    Height = 25
    Position = 5
    TabOrder = 2
    OnChanging = widthLineUpDownChanging
  end
  object lineStyleComboBox: TComboBox
    Left = 320
    Top = 33
    Width = 105
    Height = 21
    TabOrder = 3
    OnChange = lineStyleComboBoxChange
    Items.Strings = (
      #1057#1087#1083#1086#1096#1085#1072#1103
      #1055#1091#1085#1082#1090#1080#1088#1085#1072#1103
      #1058#1086#1095#1082#1080)
  end
  object angleLabelEdit: TLabeledEdit
    Left = 112
    Top = 87
    Width = 121
    Height = 21
    EditLabel.Width = 75
    EditLabel.Height = 13
    EditLabel.Caption = #1059#1075#1086#1083' '#1087#1086#1074#1086#1088#1086#1090#1072
    TabOrder = 4
  end
  object setAngleButton: TButton
    Left = 239
    Top = 85
    Width = 106
    Height = 25
    Caption = #1055#1088#1080#1084#1077#1085#1080#1090#1100' '#1091#1075#1086#1083
    TabOrder = 5
    OnClick = setAngleButtonClick
  end
  object ColorDialogSet: TColorDialog
    Left = 32
    Top = 56
  end
end
