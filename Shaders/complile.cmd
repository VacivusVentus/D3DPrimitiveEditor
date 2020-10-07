del *.bin
fxc /T vs_5_0 /E main .\LineVertexShader.hlsl /Fo .\LineVertexShader.bin

fxc /T ps_5_0 /E main .\LinePixelShader.hlsl /Fo .\LinePixelShader.bin
fxc /T ps_5_0 /E mainSelect .\LinePixelShader.hlsl /Fo .\LinePixelShaderSelect.bin
fxc /T ps_5_0 /E mainUnderCursor .\LinePixelShader.hlsl /Fo .\LinePixelShaderUnderCursor.bin
fxc /T ps_5_0 /E mainlineUnderCursor .\LinePixelShader.hlsl /Fo .\mainlineUnderCursor.bin

fxc /T ps_5_0 /E PenMain .\LinePixelShader.hlsl /Fo .\PenMain.bin
fxc /T ps_5_0 /E PLineMain .\LinePixelShader.hlsl /Fo .\PLineMain.bin
fxc /T ps_5_0 /E PointsLineMain .\LinePixelShader.hlsl /Fo .\PointsLineMain.bin

fxc /T vs_5_0 /E mainEllipseFill .\TrianglePrimitiveVS.hlsl /Fo .\TrianglePrimitiveVSFill.bin
fxc /T vs_5_0 /E mainEllipseLine .\TrianglePrimitiveVS.hlsl /Fo .\TrianglePrimitiveVSELINE.bin

fxc /T vs_5_0 /E mainRectFill .\TrianglePrimitiveVS.hlsl /Fo .\TrianglePrimitiveRVSFill.bin
fxc /T vs_5_0 /E mainRectLine .\TrianglePrimitiveVS.hlsl /Fo .\TrianglePrimitiveRVSLine.bin

fxc /T ps_5_0 /E defrender .\TrianglePrimitivePS.hlsl /Fo .\TrianglePrimitivePS.bin


fxc /T vs_5_0 /E FrameForLine .\LineVertexShader.hlsl /Fo .\FrameForLine.bin
fxc /T vs_5_0 /E mainFrameforRect .\TrianglePrimitiveVS.hlsl /Fo .\mainFrameforRect.bin


fxc /T ps_5_0 /E PointsLineMainLR .\LinePixelShader.hlsl /Fo .\PointsLineMainLR.bin