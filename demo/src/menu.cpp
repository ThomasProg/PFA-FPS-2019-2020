#include "menu.hpp"
#include "game.hpp"
#include "file.hpp"

Menu::Menu(Game& game)
    : game(game), isLoadAvailable(Resources::File::doesFileExist(Game::savedFilename))
{

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

    if (option)
        renderPanelOptions();
}

bool Menu::setupMainMenuButtons()
{
    preparePanel({0.f, 0.f}, {float(game.engine.width), float(game.engine.height)}, PURPLE, PINK, GREY);
    ImGui::Begin("Game", &isMainMenuOpen, ImGuiWindowFlags_NoTitleBar);
    ImGui::Indent(game.engine.width / 2.5);

    //////////////////////////////////////
    //ImGui::PushFont(font);
    ImGui::SetCursorPosY(50.0f);
    ImGui::SetCursorPosX(100.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, PURPLE);
    ImGui::SetWindowFontScale(3.0);
    ImGui::Text("Platformer, The game");
    ImGui::PopStyleColor(1);
    ImGui::SetWindowFontScale(1);
    //////////////////////////////////////

    ImGui::SetCursorPosY(game.engine.height / 3.5);
    if (ImGui::Button("New Game", ImVec2(game.engine.width / 5, game.engine.height / 10)))
        game.loadLevel(false);

    if (isLoadAvailable)
    {
        ImGui::SetCursorPosY(game.engine.height / 2.3);
        if (ImGui::Button("Load Game", ImVec2(game.engine.width / 5, game.engine.height / 10)))
        {
            game.loadLevel(true);
        }
    }

    ImGui::SetCursorPosY(game.engine.height / 1.7);
    if (ImGui::Button("Controls", ImVec2(game.engine.width / 5, game.engine.height / 10)))
    {
        option = !option;
        //isMainMenuOpen = !isMainMenuOpen;
    }


    ImGui::SetCursorPosY(game.engine.height / 1.35);
    if (ImGui::Button("Load Level Editor", ImVec2(game.engine.width / 5, game.engine.height / 10)))
    {
        game.loadLevel(true, true);
        //isMainMenuOpen = !isMainMenuOpen;
    }

    bool bQuit = false;
    ImGui::SetCursorPosY(game.engine.height / 1.13);
    if (ImGui::Button("Quit", ImVec2(game.engine.width / 5, game.engine.height / 10)))
    {
        bQuit = true;
        game.quitGame();
    }

    ImGui::PopStyleColor(4);
    //ImGui::PopFont();
    ImGui::End();

    return bQuit;
}

void Menu::renderPanelOptions()
{
    preparePanel({0, 0}, {float(game.engine.width), float(game.engine.height)}, PURPLE, PINK, GREY);
    ImGui::Begin("Options", &option, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    ImGui::SetCursorPosY(50);
    ImGui::SetCursorPosX(100.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, PURPLE);
    ImGui::SetWindowFontScale(3.0);
    ImGui::Text("OPTIONS");
    ImGui::PopStyleColor(1);
    ImGui::SetWindowFontScale(1);

    ImGui::SetCursorPosY(150);
    ImGui::Text("The game is in Qwerty if you want to change it check \"Azerty\"  box here : ");

    ImGui::SetCursorPosY(200);
    ImGui::Checkbox("Azerty", &game.isAzerty);

    ImGui::SetCursorPosY(300);
    if (ImGui::Button("Return main menu", ImVec2(game.engine.width / 5, game.engine.height / 10)))
    {
        option = !option;
    }

    ImGui::PopStyleColor(4);
    //ImGui::PopFont();
    ImGui::End();
}