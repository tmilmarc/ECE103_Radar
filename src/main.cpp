//Starts application
#include <SDL3/SDL_main.h>
#include "Application.h"

int main(int argc, char* argv[])
{
    SDL_SetMainReady();

    Application app;

    if (!app.Initialize())
    {
        return -1;
    }

    app.Run();
    app.Shutdown();

    return 0;
}