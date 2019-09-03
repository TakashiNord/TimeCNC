//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("TimeProject.res");
USEFORM("Main.cpp", FormMain);
USEFORM("about.cpp", AboutForm);
USEUNIT("TimeCount.cpp");
USEOBJ("algobaset.obj");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TFormMain), &FormMain);
                 Application->CreateForm(__classid(TAboutForm), &AboutForm);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
