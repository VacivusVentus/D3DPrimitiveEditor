object SurfaceRender: TSurfaceRender
  Left = 0
  Top = 0
  Width = 683
  Height = 491
  TabOrder = 0
  OnResize = FrameResize
  object Panel1: TPanel
    Left = 0
    Top = 472
    Width = 683
    Height = 19
    Align = alBottom
    TabOrder = 0
    object ScrollXBar: TScrollBar
      Left = 1
      Top = 1
      Width = 663
      Height = 17
      Align = alClient
      PageSize = 0
      TabOrder = 0
      OnChange = ScrollXBarChange
    end
    object Panel2: TPanel
      Left = 664
      Top = 1
      Width = 18
      Height = 17
      Align = alRight
      TabOrder = 1
    end
  end
  object ScrollYBar: TScrollBar
    Left = 666
    Top = 0
    Width = 17
    Height = 472
    Align = alRight
    Kind = sbVertical
    PageSize = 0
    TabOrder = 1
    OnChange = ScrollYBarChange
  end
  object Renderer: TPanel
    Left = 0
    Top = 0
    Width = 666
    Height = 472
    Align = alClient
    TabOrder = 2
    OnResize = RendererResize
  end
end
