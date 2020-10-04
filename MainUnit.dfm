object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'MainForm'
  ClientHeight = 590
  ClientWidth = 846
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 209
    Top = 0
    Height = 590
    ExplicitLeft = 216
    ExplicitTop = 328
    ExplicitHeight = 100
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 209
    Height = 590
    Align = alLeft
    TabOrder = 0
    object PrimitiveScrollBox: TScrollBox
      Left = 1
      Top = 1
      Width = 207
      Height = 588
      Align = alClient
      TabOrder = 0
    end
  end
  object Panel2: TPanel
    Left = 212
    Top = 0
    Width = 634
    Height = 590
    Align = alClient
    TabOrder = 1
    inline SurfaceRender1: TSurfaceRender
      Left = 1
      Top = 1
      Width = 632
      Height = 588
      Align = alClient
      TabOrder = 0
      ExplicitLeft = 1
      ExplicitTop = 1
      ExplicitWidth = 632
      ExplicitHeight = 588
      inherited Panel1: TPanel
        Top = 569
        Width = 632
        ExplicitTop = 569
        ExplicitWidth = 632
        inherited ScrollXBar: TScrollBar
          Width = 612
          ExplicitWidth = 612
        end
        inherited Panel2: TPanel
          Left = 613
          ExplicitLeft = 613
        end
      end
      inherited ScrollYBar: TScrollBar
        Left = 615
        Height = 569
        ExplicitLeft = 615
        ExplicitHeight = 569
      end
      inherited Renderer: TPanel
        Width = 615
        Height = 569
        OnDblClick = SurfaceRender1RendererDblClick
        OnMouseDown = SurfaceRender1RendererMouseDown
        OnMouseMove = SurfaceRender1RendererMouseMove
        OnMouseUp = SurfaceRender1RendererMouseUp
        ExplicitWidth = 615
        ExplicitHeight = 569
      end
    end
  end
  object MainMenu1: TMainMenu
    Left = 617
    Top = 248
    object N1: TMenuItem
      Caption = #1060#1072#1081#1083
      object NewFileTool: TMenuItem
        Caption = #1053#1086#1074#1099#1081
        OnClick = NewFileToolClick
      end
      object saveTool: TMenuItem
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
        OnClick = saveToolClick
      end
      object saveAsTool: TMenuItem
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1082#1072#1082'...'
        OnClick = saveAsToolClick
      end
      object openXMLTool: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100
        OnClick = openXMLToolClick
      end
      object quitMenuItem: TMenuItem
        Caption = #1042#1099#1093#1086#1076
        OnClick = quitMenuItemClick
      end
    end
    object N7: TMenuItem
      Caption = #1048#1085#1089#1090#1088#1091#1084#1077#1085#1090#1099
      object N8: TMenuItem
        Caption = #1055#1088#1080#1084#1080#1090#1080#1074#1099
        object AddLinePrimitiveTool: TMenuItem
          Caption = #1051#1080#1085#1080#1103
          OnClick = AddLinePrimitiveToolClick
        end
        object AddElliopsePrimitiveTool: TMenuItem
          Caption = #1054#1082#1088#1091#1078#1085#1086#1089#1090#1100
          OnClick = AddElliopsePrimitiveToolClick
        end
        object AddRectPrimitiveTool: TMenuItem
          Caption = #1055#1088#1103#1084#1086#1091#1075#1086#1083#1100#1085#1080#1082
          OnClick = AddRectPrimitiveToolClick
        end
      end
    end
  end
  object XMLSaveDialog: TSaveDialog
    FileName = 'D:\Work\BuilderPro\Editor\Shaders\hhfghfghgf'
    Filter = #1057#1093#1077#1084#1072' '#1087#1088#1080#1084#1080#1090#1080#1074#1086#1074'|*.sxml'
    Left = 268
    Top = 200
  end
  object XMLOpenDialog: TOpenDialog
    Filter = #1057#1093#1077#1084#1072' '#1087#1088#1080#1084#1080#1090#1080#1074#1086#1074'|*.sxml'
    Left = 308
    Top = 136
  end
  object XMLDocument1: TXMLDocument
    Left = 604
    Top = 376
  end
end
