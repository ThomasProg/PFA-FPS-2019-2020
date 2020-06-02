#ifndef _MENU_HPP_
#define _MENU_HPP_

#include "scene.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define PURPLE     {66.f / 255.f, 0, 66.f / 255.f, 1.f}
#define PINK       {153.f / 255.f, 102.f / 255.f, 204.f / 255.f, 1.f}
#define GREY       {153.f / 255.f, 153.f / 255.f, 153.f / 255.f, 1.f}
#define DARKERGREY {100.f / 255.f, 100.f / 255.f, 100.f / 255.f, 1.f}

class Game;

class Menu : public Resources::Scene
{
private:
    Game& game;
    bool isLoadAvailable = false;

public:
    Menu(Game& game);

    bool isMainMenuOpen{true};
    int imageWidth = 0;
    int imageHeight = 0;
    GLuint imageText = 0;

    void update() override;   
    void renderUI() override; 
    void render() override;
    void inputs() override;

    static void preparePanel(ImVec2 pos, ImVec2 size, ImVec4 buttonColor, ImVec4 buttonColorActive, ImVec4 windowColor);
    void mainMenu();
    bool setupMainMenuButtons(); // returns true if we quit the game, prevents memory error
};

#endif