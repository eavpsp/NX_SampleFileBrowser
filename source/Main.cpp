#include <string.h>
#include <stdio.h>
#include <MainApplication.hpp>
#include <switch.h>
#include <FileHandler.h>
// If you would like to initialize and finalize stuff before or after Plutonium, you can use libnx's userAppInit/userAppExit

directoryData dirData;
int showMadeDiag;
char currentDir = '\0';
char prevDir;
bool endApp = false;
bool inApp = false;
bool appStarted = false;
// Main entrypoint
int RunGame() {
    // First create our renderer, where one can customize SDL or other stuff's initialization.
    auto renderer_opts = pu::ui::render::RendererInitOptions(SDL_INIT_EVERYTHING, pu::ui::render::RendererHardwareFlags);
    renderer_opts.UseImage(pu::ui::render::IMGAllFlags);
    renderer_opts.UseAudio(pu::ui::render::MixerAllFlags);
    renderer_opts.UseTTF();
    auto renderer = pu::ui::render::Renderer::New(renderer_opts);
    auto mainApp = MainApplication::New(renderer);
    mainApp->Prepare();
    if(showMadeDiag == 1){
        mainApp->AppLoop();
    }
    showMadeDiag = 0;
    mainApp->Show(); //stuck here until close
    appStarted = false;
    return 0;
}

//creata a console mode
int main()
{
    //loop to set root dir ../ then check if dir exists until last point
    pathData.fileCount = 1;//never write to zero
   //strcpy(&pathData.pathTot,"");
    inApp = true;
    while(inApp)
    {
        if(!appStarted)
        {
            appStarted = true;

            RunGame();//stuck here in loop until close
        }
        if(endApp)
        {
            inApp = false;
        }
    }
   
   
    return 0;
}