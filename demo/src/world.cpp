#include "world.hpp"

#include "file.hpp"
#include "shader.hpp"
#include "matrix4x4.hpp"
#include "camera.hpp"
#include "game.hpp"

#include "segment3D.hpp"
#include "primitives.hpp"
#include "log.hpp"

#include "physicComponentInterface.hpp"
#include "collisionComponentInterface.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <array>


World::World(Game& game, bool isLoaded, bool isEditorMode)
    : game(game), entityGroup(game.engine), isLoaded(isLoaded), isEditorMode(isEditorMode)
{
    Resources::Texture::loadTexture("resources/textures/crosshair.png", crosshairWidth, crosshairHeight, crosshair);
    Resources::Texture::loadTexture("resources/textures/cross.png", crossWidth, crossHeight, cross);
    Resources::Texture::loadTexture("resources/textures/bullet.png", bulletWidth, bulletHeight, bullet);
}

void World::makeNewLevel()
{
    // ===== Set up Entities ===== //

    Core::Maths::Vec4 pathColor = {0.835f, 0.650f, 0.384f,1};
    Core::Maths::Vec4 grassColor = {0.3f, 0.42f, 0.3f,1.f};
    Core::Maths::Vec4 stoneColor = {0.2f, 0.2f, 0.2f,1.f};

    entityGroup.addGround({{0, -30, 40}, {0.f,0,0}, {10,1,40}}, grassColor);
    entityGroup.addGround({{0, -30, -10}, {0.f,0,0}, {10,5,10}}, stoneColor);
    entityGroup.addGround({{20, -30, 20}, {0.f,0,0}, {10,5,40}}, stoneColor);
    entityGroup.addGround({{-20, -30, 40}, {0.f,0,0}, {10,5,60}}, stoneColor);
    
    entityGroup.addGround({{19, -32, 70}, {0.f,0,-0.2}, {10,1,10}}, grassColor);
    entityGroup.addGround({{45, -34, 70}, {0.f,0,0.f}, {20,1,10}}, grassColor);
    entityGroup.addGround({{74, -32, 70}, {0.f,0,0.2}, {10,1,10}}, grassColor);

    entityGroup.addGround({{25, -30, 90}, {0.f,0,0}, {60,5,10}}, stoneColor);
    entityGroup.addGround({{50, -30, 50}, {0.f,0,0}, {35,5,10}}, stoneColor);
    
    // Boss plateform
    entityGroup.addGround({{103, -30.2, 70}, {0.f,0,0}, {20,1,20}}, grassColor);

    entityGroup.addGround({{127, -30, 70}, {0.f,0,0}, {5,5,20}}, stoneColor);
    entityGroup.addGround({{103, -30, 45}, {0.f,0,0}, {20,5,5}}, stoneColor);
    entityGroup.addGround({{103, -30, 95}, {0.f,0,0}, {20,5,5}}, stoneColor);

    // Path block
    {
        entityGroup.addGround({{0, -30 + 0.1, 40}, {0.f,0,0}, {3.f,1,33.f}}, pathColor);

        entityGroup.addGround({{19, -32 + 0.1, 70}, {0.f,0,-0.2}, {10.f,1,3.f}}, pathColor);
        entityGroup.addGround({{45, -34 + 0.1, 70}, {0.f,0,0.f}, {20.f,1,3.f}}, pathColor);

        entityGroup.addGround({{74, -32 + 0.1, 70}, {0.f,0,0.2}, {10.f,1,3.f}}, pathColor);
        
        // Boss plateform
        entityGroup.addGround({{103, -30.2 + 0.1, 70}, {0.f,0,0}, {12.5,1,12.5}}, pathColor);
    }

    entityGroup.addEnemy({{0.f, -24, 35}, {0.f,0,0}, {1,1,1}});

    entityGroup.addEnemy({{2.f, -24, 45}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{-2.f, -24, 45}, {0.f,0,0}, {1,1,1}});

    entityGroup.addEnemy({{0.f, -24, 50}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{-2.f,-24, 62}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{2.f, -24, 62}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{0.f, -24, 52}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{-2.f,-24, 64}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{2.f, -24, 64}, {0.f,0,0}, {1,1,1}});

    for (unsigned int i = 0; i < 5; i++)
    {
        entityGroup.addEnemy({{46.f,-24, 66 + float(i) * 2.f}, {0.f,0,0}, {1,1,1}});
        entityGroup.addEnemy({{48.f,-24, 66 + float(i) * 2.f}, {0.f,0,0}, {1,1,1}});
        entityGroup.addEnemy({{50.f,-24, 66 + float(i) * 2.f}, {0.f,0,0}, {1,1,1}});
        entityGroup.addEnemy({{52.f,-24, 66 + float(i) * 2.f}, {0.f,0,0}, {1,1,1}});
        entityGroup.addEnemy({{54.f,-24, 66 + float(i) * 2.f}, {0.f,0,0}, {1,1,1}});
    }

    entityGroup.addEnemyBoss({{100.f, -10, 70 }, {0.f,0.f,0}, {5,5,5}});

    // === Add Player === //
    entityGroup.addPlayer({{0,-26,10.0 + 0.0}});

    if (game.isHighQuality)
        setHighQualitySettings();
    else 
        setLowQualitySettings();
}

void World::load()
{
    isLoadAvailable = Resources::File::doesFileExist(Game::savedFilename);

    // lock cursor
    ImGui::SetMouseCursor(ImGuiMouseCursor_None);
    glfwSetInputMode(game.engine.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    makeNewLevel();


    updateCameraProjection();
}

World::~World()
{

}

void World::inputs()
{
    if (entityGroup.controller != nullptr && !isPauseMenuOpen)
        entityGroup.controller->inputs(game.engine);

    if (game.engine.isKeyDown(GLFW_KEY_ESCAPE))
    {
        // lock cursor
        glfwSetInputMode(game.engine.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        isPauseMenuOpen = true;
        inGame = false;
    }
}

void World::updateCameraProjection()
{
    entityGroup.player->camera.projection = Core::Maths::Matrix4x4::CreatePerspectiveProjectionMatrix(game.engine.width, game.engine.height, 0.1, 10000, 90.f);
}

void World::updatePhysics()
{
    game.engine.physicsSystem.simulatePhysics(game.engine);
}

void World::setLowQualitySettings()
{
    game.isHighQuality = false;

    glClearColor(0.5,0.5,0.9, 1.f);

    entityGroup.removeAllDecorations();
    entityGroup.removeAllLights();

    Renderer::LightData light;
    light.lightType = 1;
    light.ambient   = {0.4, 0.4, 0.4, 1};
    light.dif       = {0.6, 0.6, 0.6, 1};
    light.location  = {-1,-1,-1,1};
    entityGroup.addLight(light);

    entityGroup.player->gun.setLowQuality(game.engine.resourceManager);
}

void World::setHighQualitySettings()
{
    game.isHighQuality = true;

    glClearColor(0.1,0.1,0.2,1.f); // dark blue, night sky color

    entityGroup.removeAllLights();

    entityGroup.addTree({{-15, -25, 20}, {0.f,0,0}, {1,1,1}});
    for (uint i = 0; i < 5; i++)
    {
        entityGroup.addTree({{6+std::sin(float(i)*10)*2.f, -30, 5+float(i)*13},     {0.f,std::cos(float(i)),0}, {1,1,1}});
        entityGroup.addTree({{-6-std::sin(10+float(i)*10)*2.f, -30, 5+float(i)*13}, {0.f,std::sin(float(i)),0}, {1,1,1}});
    }
    entityGroup.addRock({{4, -29, 20}, {0.f,0,0}, {1,1,1}});
    entityGroup.addRock({{4.4, -29, 36}, {0.f,0,0}, {1,1,1}});
    entityGroup.addRock({{-5, -29, 60}, {0.f,0,0}, {1,1,1}});
    entityGroup.addRock({{-6, -29, 40}, {0.f,0,0}, {1,1,1}});

    auto addFireflyWithLight = [&](const Core::Maths::Vec3& loc = {0, -26, 0.15}) 
    {
        entityGroup.addFirefly({loc, {0.f,0,0}, {0.4,0.4,0.4}});
        {
            Renderer::LightData data;
            data.location = Core::Maths::Vec4{loc, 1};
            data.ambient  = Core::Maths::Vec4{1, 1, 1, 1};
            entityGroup.addLight(std::move(data));
        }
    };

    addFireflyWithLight({0, -26, 0.15});

    entityGroup.addLight(Core::Maths::Vec3{0, -26, 70});
    entityGroup.addLight(Core::Maths::Vec3{50, -26, 70});
    entityGroup.addLight(Core::Maths::Vec3{100, -20, 70});
    
    for (unsigned int i = 0; i < 5; i++)
    {
        const float size = std::sin(i*3)/5 + 1; 
        entityGroup.addRock({{52 - 7 * float(i), -33, 75 + 2 * std::cos(float(i * 2))}, {size,0,0}, {size,size,size}});
        entityGroup.addRock({{58 - 7 * float(i), -33, 63 + 2 * std::cos(float(i * 7))}, {size,0,0}, {size,size,size}});
    }

    entityGroup.addTree({{87, -29, 78}, {0.f,0.f,0.f}, {1,1,1}});
    entityGroup.addTree({{87, -29, 63}, {0.f,0.f,0.f}, {1,1,1}});

    entityGroup.player->gun.setHighQuality(game.engine.resourceManager);
}

void World::switchQualitySettings()
{
    if (game.isHighQuality)
        setLowQualitySettings();
    else 
        setHighQualitySettings();
}

void World::update()   
{
    if (game.engine.isKeyDown(GLFW_KEY_P))
    {
        if (!isPKeyPressed)
        {
            isPKeyPressed = true;
            switchQualitySettings();
        }
    }
    else 
        isPKeyPressed = false;

    if (game.engine.hasWindowSizeChanged)
        updateCameraProjection();

    if (isPauseMenuOpen == false)
    {
        if(!gWin)
            playTime += game.engine.deltaTime;

        // Update entities
        for (std::unique_ptr<Entity::Enemy>& enemy : entityGroup.enemies)
        {
            enemy->target = entityGroup.player.get();

            if (entityGroup.player->transform.transformMatrixNode.isValid())
            {
                enemy->update(game.engine, playTime);
            }
        }

        entityGroup.player->tryToJump(game.engine);

        if(glfwGetMouseButton(game.engine.window, GLFW_MOUSE_BUTTON_LEFT))
        {
            entityGroup.player->shoot(game.engine.physicsSystem, entityGroup, playTime);
        }

        if(glfwGetMouseButton(game.engine.window, GLFW_MOUSE_BUTTON_RIGHT))
        {
            entityGroup.player->reloadAmmo();
        }

        if (entityGroup.enemies.size() == 0)
        {
            gWin = true;
        }

        if(entityGroup.bullets.size() > 0)
        {
            std::vector<std::unique_ptr<Entity::RenderedEntity>>::iterator r = entityGroup.bullets.begin();
            while(entityGroup.bullets.size() != 0 && game.engine.lastTime >= r->get()->timer)
            {
                game.engine.rendererSystem.erase((*r)->meshIt);
                r = entityGroup.bullets.erase(r);
            }
               
        }

        updatePhysics();

        entityGroup.collectGarbage();
    }
}

void World::gameWin()
{
    Menu::preparePanel(ImVec2(0, 0), {float(game.engine.width), float(game.engine.height)}, {0.f,0.f,0.f,0.f}, {0.f,0.f,0.f,0.f}, {0.5f,0.5f,0.5f,0.5f});

    ImGui::Begin("Pause", &entityGroup.player->gOver, ImGuiWindowFlags_NoDecoration);

    ImGui::SetCursorPosY(game.engine.height / 2);
    ImGui::SetCursorPosX(game.engine.width / 2 - 100.f);
    ImGui::SetWindowFontScale(2.0);    
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImVec4(0.f, 1.f, 0.f, t/3)));
    ImGui::Text("You win in \n %f seconds", playTime);

    ImGui::PopStyleColor(5);
    ImGui::End();

    t += game.engine.deltaTime;
    if (t >= 4)
    {
        entityGroup.player->gOver = false;
        game.loadMenu();
        t = 0;
    } 
}

void World::gameOver()
{
    Menu::preparePanel(ImVec2(0, 0), {float(game.engine.width), float(game.engine.height)}, {0.f,0.f,0.f,0.f}, {0.f,0.f,0.f,0.f}, {0.5f,0.5f,0.5f,0.5f});

    ImGui::Begin("Pause", &entityGroup.player->gOver, ImGuiWindowFlags_NoDecoration);

    ImGui::SetCursorPosY(game.engine.height / 2);
    ImGui::SetCursorPosX(game.engine.width / 2);
    ImGui::SetWindowFontScale(2.0);
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImVec4(1.f, 0.f, 0.f, t/3)));
    ImGui::Text("You Loose");

    ImGui::PopStyleColor(5);
    ImGui::End();

    t += game.engine.deltaTime;
    if (t >= 4)
    {
        entityGroup.player->gOver = false;
        game.loadMenu();
        t = 0;
    } 
}

void World::renderUI() 
{
    if (isPauseMenuOpen)
        pauseMenu();
    else if(entityGroup.player->gOver)
    {
        gameOver();
    }
    else if (gWin)
    {
        gameWin();
    }  
    else
        hud();
        
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void World::render()   
{
    if (entityGroup.camera != nullptr)
    {
        entityGroup.camera->updateViewProjectionMatrix();
        game.engine.rendererSystem.draw(*entityGroup.camera, entityGroup.lightManager);
    }
}

void World::hud()
{
    Menu::preparePanel(ImVec2(0, 0), {float(game.engine.width), float(game.engine.height)}, {0.f,0.f,0.f,0.f}, {0.f,0.f,0.f,0.f}, {0.f,0.f,0.f,0.f});

    ImGui::Begin("Pause", &inGame, ImGuiWindowFlags_NoDecoration);
    ImGui::SetCursorPosY(game.engine.height - 30.f);
    assert(entityGroup.player->maxLifePoints != 0.f);
    ImGui::SetWindowFontScale(1.0);

    // Lifebar   
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    drawList->AddRectFilled(ImVec2(50.f, game.engine.height - 50.f), ImVec2(300.f, game.engine.height - 20.f), ImGui::GetColorU32(ImVec4(0.2f, 0.2f, 0.2f, 0.5f)), 50.f);
    drawList->AddRectFilled(ImVec2(50.f + 1.f, game.engine.height - 50.f + 1.f), ImVec2(50.f + (250.f - 1.f) * (entityGroup.player->lifePoints / entityGroup.player->maxLifePoints), game.engine.height - 20.f - 1.f), ImGui::GetColorU32(ImVec4(1.f - entityGroup.player->lifePoints / entityGroup.player->maxLifePoints, entityGroup.player->lifePoints / entityGroup.player->maxLifePoints, 0.f, 1.f)), 50.f);
    ImGui::SetCursorPosY(game.engine.height - crossHeight/5);
    ImGui::SetCursorPosX(0.f);
    ImGui::Image((void*)(intptr_t)cross, ImVec2(crossWidth/5, crossHeight/5));

    // Nb of bullets
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImVec4(1.f, 0.f, 0.f, 1.f)));
    ImGui::SetCursorPosY(game.engine.height - 50.f);
    ImGui::SetCursorPosX(game.engine.width - 130.f);
    ImGui::SetWindowFontScale(1.5);
    ImGui::Text("%i / %i", entityGroup.player->nbBullet, entityGroup.player->maxNbBullet);
    ImGui::SetCursorPosY(game.engine.height - 10.f - bulletHeight/10);
    ImGui::SetCursorPosX(game.engine.width - 135.f - bulletWidth/10);
    ImGui::Image((void*)(intptr_t)bullet, ImVec2(bulletWidth/10, bulletHeight/10));

    // Crosshair
    ImGui::SetCursorPosY(game.engine.height / 2 - crosshairHeight / 2);
    ImGui::SetCursorPosX(game.engine.width / 2 - crosshairWidth / 2);
    ImGui::Image((void*)(intptr_t)crosshair, ImVec2(crosshairWidth, crosshairHeight));

    // Nb of enemies
    ImGui::SetCursorPosY(0);
    ImGui::SetCursorPosX(0);
    ImGui::Text("%lu enemies remaining", entityGroup.enemies.size());


    ImGui::PopStyleColor(5);
    ImGui::End(); 
}

void World::pauseMenu()
{
    Menu::preparePanel(ImVec2(game.engine.width / 2 - 200, game.engine.height / 2 - 150), {400, 350}, {0.f, 0.f, 0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f, 0.f, 0.f});
    
    ImGui::Begin("Pause", &isPauseMenuOpen, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    ImGui::SetWindowFontScale(1.0);
    ImGui::Indent(400/2 - 100);
    

    ImGui::SetCursorPosY(50.f);
    ImGui::SetWindowFontScale(1.5);
    ImGui::Text("Pause");

    ImGui::SetCursorPosY(125.0f);
    ImGui::SetWindowFontScale(1.0);
    if (ImGui::Button("Back to game", ImVec2(200, 50)))
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_None);
        glfwSetInputMode(game.engine.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);        
        isPauseMenuOpen = !isPauseMenuOpen;
        inGame = true;
    }

    ImGui::SetCursorPosY(225.0f);
    if (ImGui::Button("Back to main menu", ImVec2(200, 50)))
    {        
        isPauseMenuOpen = !isPauseMenuOpen;
        game.loadMenu();
    }

    ImGui::PopStyleColor(4);
    ImGui::End();
}

