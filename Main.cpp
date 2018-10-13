#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include "Libraries/E_Window/Mask.h"
#include "Libraries/E_MenuButton/Mask.h"
#include "Libraries/E_Menu/Mask.h"
#include "Libraries/E_Canvas/Mask.h"

int main(int argc, char** argv)
{
    Fl_Window* hWindow = new Fl_Window(640, 480);
    E_Menu* hMenu = new E_Menu(0, 0, 200, 480, "Меню");
    E_Canvas* hCanvas = new E_Canvas(200, 0, 440, 480, "Холст");

    hMenu->createButton("Создать стену");
    hMenu->createButton("Создать дверь");
    hMenu->createDivider();
    hMenu->createButton("Создать");
    hMenu->createButton("Открыть");
    hMenu->createButton("Сохранить");
    hMenu->createButton("Сохранить как");
    hMenu->createDivider();
    hMenu->createButton("Закрыть");

    hWindow->color(E_COLOR1);
    hWindow->show(argc, argv);

    return Fl::run();
}
