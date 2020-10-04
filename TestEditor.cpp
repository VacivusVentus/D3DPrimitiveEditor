// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
// ---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("XmlPrimFarme.cpp", XMLPrimitiveElement); /* TFrame: File Type */
USEFORM("SurfaceUnit.cpp", SurfaceRender); /* TFrame: File Type */
USEFORM("MainUnit.cpp", MainForm);
USEFORM("SettingStyleUnit.cpp", SettingStyleDialog);
//---------------------------------------------------------------------------
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

// ---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
        Application->Initialize();
        Application->MainFormOnTaskBar = true;
        Application->CreateForm(__classid(TMainForm), &MainForm);
         Application->CreateForm(__classid(TSettingStyleDialog), &SettingStyleDialog);
         Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    catch (...)
    {
        try
        {
            throw Exception("");
        }
        catch (Exception &exception)
        {
            Application->ShowException(&exception);
        }
    }
    return 0;
}
// ---------------------------------------------------------------------------
