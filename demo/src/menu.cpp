#include "menu.hpp"
#include "game.hpp"
#include "file.hpp"

Menu::Menu(Game& game)
    : game(game)
{
    glfwSetInputMode(game.engine.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

    int w, h;
    Resources::Texture::loadTexture("resources/textures/background.png", imageText, &w, &h);
}

void Menu::update()
{

}

void Menu::inputs()
{

}

void Menu::renderUI()
{
    if (isMainMenuOpen)
        mainMenu();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Menu::render()
{

}

void Menu::preparePanel(ImVec2 pos, ImVec2 size, ImVec4 buttonColor, ImVec4 buttonColorActive, ImVec4 windowColor)
{
    ImGui::SetNextWindowPos(pos, 0, ImVec2(0.0, 0.0));
    ImGui::SetNextWindowSize(size);
    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonColorActive);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonColorActive);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, windowColor);
}

void Menu::mainMenu()
{
    bool bQuit = setupMainMenuButtons();
    if (bQuit)
        return;
}

bool Menu::setupMainMenuButtons()
{
    preparePanel({0.f, 0.f}, {float(game.engine.width), float(game.engine.height)}, PURPLE, PINK, GREY);
    ImGui::Begin("Game", &isMainMenuOpen, ImGuiWindowFlags_NoDecoration);
    ImGui::Indent(game.engine.width / 2.5);

    ImGui::SetCursorPosY(0);
    ImGui::SetCursorPosX(0);
    ImGui::Image((void*)(intptr_t)imageText, ImVec2(game.engine.width, game.engine.height));

 
    ImGui::PushFont(game.engine.font);
    ImGui::SetCursorPosY(50.0f);
    ImGui::SetCursorPosX(100.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, PURPLE);
    ImGui::SetWindowFontScale(3.0);
    ImGui::Text("Wild Dogs");
    ImGui::PopStyleColor(1);
    
    ImGui::SetWindowFontScale(1);


    ImGui::SetCursorPosY(game.engine.height / 3);
    if (ImGui::Button("Start", ImVec2(game.engine.width / 5, game.engine.height / 10)))
        game.loadLevel();

    bool bQuit = false;
    ImGui::SetCursorPosY(game.engine.height / 2);
    if (ImGui::Button("Quit", ImVec2(game.engine.width / 5, game.engine.height / 10)))
    {
        bQuit = true;
        game.quitGame();
    }

    ImGui::PopStyleColor(4);
    ImGui::PopFont();
    ImGui::End();

    return bQuit;
}