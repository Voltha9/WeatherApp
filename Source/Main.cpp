#include "Application.h"

int main()
{
    Application app;

    while (app.IsRunning())
    {
        app.Events();
        app.Update();
        app.Render();
    }

    return EXIT_SUCCESS;
}