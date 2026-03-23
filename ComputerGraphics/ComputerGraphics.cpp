#include <iostream>
#include "Application.h"

int main() {
    
    Application* GraphicsApp = new Application();
    if (GraphicsApp->startup() == true) {
        while (GraphicsApp->update() == true) {
            GraphicsApp->draw();
        }
        GraphicsApp->shutdown();
    }
    delete GraphicsApp;
    return 0;
}



