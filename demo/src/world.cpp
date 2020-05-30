#include "world.hpp"

#include "file.hpp"
#include "shader.hpp"
#include "matrix4x4.hpp"
#include "camera.hpp"
#include "game.hpp"

#include "segment3D.hpp"
#include "primitives.hpp"
#include "log.hpp"
#include "editorUtility.hpp"

#include "physicComponentInterface.hpp"
#include "collisionComponentInterface.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// void World::save(Save::Saver& saver) 
// {
//     // saver.save(nextEntity);
//     saver.save(grounds.size());
//     saver.save(enemies.size());
// }

// void World::loadData(Save::Loader& loader) 
// {
//     // loader.load(nextEntity);

//     std::size_t nbGrounds;
//     loader.load(nbGrounds);

//     for (std::size_t i = 0; i < nbGrounds; i++)
//     {
//         // std::pair<std::unordered_map<Entity::EntityID, Entity::BasicEntity>::iterator, bool> it = grounds.emplace(Entity::EntityID{i}, Entity::EntityID{i});
//         // if (it.second) // if insertion took place
//         {
//             // game.engine.physicsSystem.loadPhysicComponentItContainer(it.first->second.physicCompIt, it.first->first);
//             // game.engine.physicsSystem.loadColliderItContainer(it.first->second.colliderIt, it.first->first);
//             // it.first->second.colliderIt = game.engine.physicsSystem.addCollider<Box>(it.first->second);
//             // it.first->second.physicCompIt = game.engine.physicsSystem.addPhysicComponent(it.first->second);
//             // nextEntity.next();
//             addGround({{}});
//         }
//     }

//     std::size_t nbEnemies;
//     loader.load(nbEnemies);

//     for (std::size_t i = 0; i < nbEnemies; i++)
//     {
//         // enemies.emplace_back();
//         // saveSystem.add(&enemies.back());
//         // std::pair<std::unordered_map<Entity::EntityID, Entity::Enemy>::iterator, bool> it = enemies.emplace(Entity::EntityID{i}, Entity::EntityID{i});
//         // if (it.second) // if insertion took place
//         // {
//         //     saveSystem.add(&it.first->second);
//             addEnemy({{}});
//             // it.first->second.colliderIt = game.engine.physicsSystem.addCollider<Box>(it.first->second);
//             // it.first->second.physicCompIt = game.engine.physicsSystem.addPhysicComponent(it.first->second);
//             // it.first->second.colliderIt->transform = it.first->second.physicCompIt->collider.transform = &it.first->second.mesh->transform;
//             // game.engine.physicsSystem.loadPhysicComponentItContainer(it.first->second.physicCompIt, it.first->first);
//             // game.engine.physicsSystem.loadColliderItContainer(it.first->second.colliderIt, it.first->first);
//             // nextEntity.next();
//         // }
//     }
// }


World::World(Game& game, bool isLoaded, bool isEditorMode)
    : game(game), entityGroup(game.engine), isLoaded(isLoaded), isEditorMode(isEditorMode)
{
    Resources::Texture::loadTexture("resources/textures/crosshair.png", imageWidth, imageHeight, imageText);
}

void World::loadFromSavefile(const char* savedFilename)
{
    // saveSystem.load(savedFilename);
    // for (Entity::Enemy* enemy : enemies)
    //     enemy->loadLinks(root);
}

void World::makeNewLevel()
{
    // ===== Set up Entities ===== //

    entityGroup.addGround({{0, -30, 40}, {0.f,0,0}, {10,1,40}});
    entityGroup.addGround({{110, -30, 70}, {0.f,0,0}, {100,1,10}});
    entityGroup.addGround({{220, -30, 110}, {0.f,0,0}, {10,1,50}});
    // Boss plateform
    entityGroup.addGround({{220, -30, 160+20}, {0.f,0,0}, {20,1,20}});

    entityGroup.addEnemy({{0.f, 0, 20}, {0.f,0,0}, {1,1,1}});

    entityGroup.addEnemy({{0.f, 0, 60}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{-2.f, 0, 62}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{2.f, 0, 62}, {0.f,0,0}, {1,1,1}});

    entityGroup.addEnemy({{60.f, 0, 70}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{62.f, 0, 70}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{64.f, 0, 70}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{62.f, 0, 72}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{62.f, 0, 72}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{60.f, 0, 72}, {0.f,0,0}, {1,1,1}});

    entityGroup.addEnemy({{180.f, 0, 70}, {0.f,0,0}, {1,1,1}});

    entityGroup.addEnemy({{220.f, 0, 70}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{220.f, 0, 72}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{220.f, 0, 68}, {0.f,0,0}, {1,1,1}});

    entityGroup.addEnemy({{220.f, 0, 120}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{222.f, 0, 120}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{218.f, 0, 120}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{220.f, 0, 122}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{222.f, 0, 122}, {0.f,0,0}, {1,1,1}});
    entityGroup.addEnemy({{218.f, 0, 122}, {0.f,0,0}, {1,1,1}});


    // === Add Player === //
    entityGroup.addPlayer({{0,0,10.0 + 0.0}});

    // for (uint i = 0; i < 100; i++)
    // {
    //     entityGroup.addBullet({});
    // }

    // // === Add other cameras === //
    // fpsCamera.setup(player.transform);
    // tpsCamera.setup(player.transform);
}

void World::load()
{
    isLoadAvailable = Resources::File::doesFileExist(Game::savedFilename);

    // lock cursor
    ImGui::SetMouseCursor(ImGuiMouseCursor_None);
    glfwSetInputMode(game.engine.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // saveSystem.add(this);
    // saveSystem.add(&root);
    // saveSystem.add(&rendererSystem);
    // saveSystem.add(&player);

    if (isLoaded)
        loadFromSavefile(Game::savedFilename);
    else 
        makeNewLevel();

    entityGroup.setKeys(game.isAzerty);

    // lightManager.lightsBufferInit(10);
    // lightManager.lights.emplace_back();
    // lightManager.lights.emplace_back();
    // {
    //     Renderer::LightData& l = lightManager.lights[lightManager.lights.size() - 1].lightData;
    //     l.location = {20.f, -27.f, 12, 0.0}; 
    //     l.lightType = 3;
    //     l.ambient = {0.7, 0.7,0.7,0};
    // }

    updateCameraProjection();

    // if (isEditorMode)
    // {
    //     controller = &fpsCamera;
    //     camera = &fpsCamera;
    // }
}

World::~World()
{
    // game.engine.rendererSystem.reset(); 
    // TODO : remove Physic Comps from physicsSystem
    // game.engine.physicsSystem.reset(); 
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
    // entityGroup.fpsCamera.projection = player.camera.projection;
}

// void World::updateEditorFunctions()
// {
//     if (game.engine.isKeyDown(GLFW_KEY_E))
//     {
//         if (!wasEditorKeyPressed)
//         {
//             editorMode.next();
//             wasEditorKeyPressed = true;
//         }
//     }
//     else if (game.engine.isKeyDown(GLFW_KEY_C))
//     {
//         if (!wasEditorKeyPressed)
//         {
//             editorMode.mode = EditorMode::E_TRANSLATION;
//             wasEditorKeyPressed = true;
//         }
//     }
//     else if (game.engine.isKeyDown(GLFW_KEY_V))
//     {
//         if (!wasEditorKeyPressed)
//         {
//             editorMode.nextRotation();
//             wasEditorKeyPressed = true;
//         }
//     }
//     else if (game.engine.isKeyDown(GLFW_KEY_B))
//     {
//         if (!wasEditorKeyPressed)
//         {
//             editorMode.nextScale();
//             wasEditorKeyPressed = true;
//         }
//     }
//     else if (game.engine.isKeyDown(GLFW_KEY_P))
//     {
//         if (!wasEditorKeyPressed)
//         {
//             addGround({fpsCamera.transform.transformMatrixNode->worldData.getTranslationVector(), {0.f,0,0}, {1,1,1}});
//             wasEditorKeyPressed = true;
//         }
//     }
//     else if (game.engine.isKeyDown(GLFW_KEY_O))
//     {
//         if (!wasEditorKeyPressed)
//         {
//             addEnemy({fpsCamera.transform.transformMatrixNode->worldData.getTranslationVector(), {0.f,0,0}, {1,1,1}});
//             wasEditorKeyPressed = true;
//         }
//     }
//     else 
//     {
//         wasEditorKeyPressed = false;
//     }

//     if (glfwGetMouseButton(game.engine.window, GLFW_MOUSE_BUTTON_LEFT) && editorSelectedEntity != nullptr)
//     {
//         fpsCamera.enableInputs = false;
//         switch (editorMode.mode)
//         {
//         case EditorMode::E_TRANSLATION:
//             EditorUtility::moveEntityWithCursor(fpsCamera, *editorSelectedEntity, game.engine);
//             break;

//         case EditorMode::E_ROTATION_X:
//             EditorUtility::rotateEntityWithCursor(*editorSelectedEntity, game.engine, 0);
//             break;
//         case EditorMode::E_ROTATION_Y:
//             EditorUtility::rotateEntityWithCursor(*editorSelectedEntity, game.engine, 1);
//             break;
//         case EditorMode::E_ROTATION_Z:
//             EditorUtility::rotateEntityWithCursor(*editorSelectedEntity, game.engine, 2);
//             break;

//         case EditorMode::E_SCALE_X:
//             EditorUtility::scaleEntityWithCursor(*editorSelectedEntity, game.engine, 0);
//             break;
//         case EditorMode::E_SCALE_Y:
//             EditorUtility::scaleEntityWithCursor(*editorSelectedEntity, game.engine, 1);
//             break;
//         case EditorMode::E_SCALE_Z:
//             EditorUtility::scaleEntityWithCursor(*editorSelectedEntity, game.engine, 2);
//             break;
        
//         default:
//             break;
//         }
//     }
//     else
//     {
//         Entity::BasicEntity* newSelection = nullptr;
//         SegmentHit hit;
//         hit.t = 2.f;
//         for (Entity::BasicEntity* ground : grounds)
//         {
//             SegmentHit tempHit;
//             if (EditorUtility::isInFrontOfPlayer(fpsCamera, *ground, tempHit))
//             {
//                 if (tempHit.t < hit.t)
//                 {
//                     hit = tempHit;
//                     newSelection = ground;
//                 }
//             }
//         }

//         if (newSelection != nullptr)
//         {
//             if (newSelection != editorSelectedEntity)
//             {
//                 newSelection->mesh.shader = &game.engine.resourceManager.get(E_Shader::E_LIGHTED);

//                 editorSelectedEntity = newSelection;
//                 editorSelectedEntity->mesh.color = {0, 0.4, 0.4, 0.5};
//                 editorSelectedEntity->mesh.shader = &game.engine.resourceManager.get(E_Shader::E_FLAT);
//             }
//         }
//         else 
//         {
//             if (editorSelectedEntity != nullptr)
//             {
//                 editorSelectedEntity->mesh.shader = &game.engine.resourceManager.get(E_Shader::E_LIGHTED);
//             }
//             editorSelectedEntity = nullptr;
//         }

//         fpsCamera.enableInputs = true;
//     } 
// }

void World::updatePhysics()
{
    // === DEMO === //
    // Entity::Ground* it = entityGroup.grounds.front();
    // it->transform.transform.location.z = std::cos(game.engine.lastTime / 2) * 50;
    // it->transform.UpdateLocalTransformMatrix();
    // it->transform.transformMatrixNode->setDirtySelfAndChildren();

    //entityGroup.player->onPlayerDeath = [this](){ gameOver(); };
    /////////////

    game.engine.physicsSystem.simulatePhysics(game.engine);
}

void World::update()   
{
    if (game.engine.hasWindowSizeChanged)
        updateCameraProjection();

    if (isPauseMenuOpen == false)
    {
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
                // (*r)->mesh.erase();
                game.engine.rendererSystem.erase((*r)->meshIt);
                r = entityGroup.bullets.erase(r);
            }
               
        }

        // // Save game
        // if (glfwGetKey(game.engine.window, GLFW_KEY_F5))
        // {
        //     // saveSystem.save(Game::savedFilename);
        //     isLoadAvailable = Resources::File::doesFileExist(Game::savedFilename);
        // }

        updatePhysics();

        // if (isEditorMode)
        //     updateEditorFunctions();

        entityGroup.colletGarbage();
    }
}

void World::gameWin()
{
     Menu::preparePanel(ImVec2(0, 0), {float(game.engine.width), float(game.engine.height)}, {0.f,0.f,0.f,0.f}, {0.f,0.f,0.f,0.f}, {0.f,0.f,0.f,0.f});
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(0.f,0.f,0.f,0.f));

    ImGui::Begin("Pause", &entityGroup.player->gOver, ImGuiWindowFlags_NoDecoration);
    //ImGui::PushFont(font);

    ImGui::SetCursorPosY(game.engine.height / 2);
    ImGui::SetCursorPosX(game.engine.width / 2);
    ImGui::SetWindowFontScale(2.0);
    ImGui::Text("You Won");

    ImGui::PopStyleColor(5);
    //ImGui::PopFont();
    ImGui::End();

    t += game.engine.deltaTime;
    if (t >= 3)
    {
        entityGroup.player->gOver = false;
        game.loadMenu();
        t = 0;
    } 
}

void World::gameOver()
{
    Menu::preparePanel(ImVec2(0, 0), {float(game.engine.width), float(game.engine.height)}, {0.f,0.f,0.f,0.f}, {0.f,0.f,0.f,0.f}, {0.f,0.f,0.f,0.f});
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(0.f,0.f,0.f,0.f));

    ImGui::Begin("Pause", &entityGroup.player->gOver, ImGuiWindowFlags_NoDecoration);
    //ImGui::PushFont(font);

    ImGui::SetCursorPosY(game.engine.height / 2);
    ImGui::SetCursorPosX(game.engine.width / 2);
    ImGui::SetWindowFontScale(2.0);
    ImGui::Text("You Loose");

    ImGui::PopStyleColor(5);
    //ImGui::PopFont();
    ImGui::End();

    t += game.engine.deltaTime;
    if (t >= 3)
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
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(0.f,0.f,0.f,0.f));

    ImGui::Begin("Pause", &inGame, ImGuiWindowFlags_NoDecoration);
    //ImGui::PushFont(font);
    ImGui::SetCursorPosY(game.engine.height - 30.f);
    assert(entityGroup.player->maxLifePoints != 0.f);
    ImGui::SetWindowFontScale(1.0);
    ImGui::ProgressBar(entityGroup.player->lifePoints / entityGroup.player->maxLifePoints, ImVec2(200,20));

    ImGui::SetCursorPosY(game.engine.height - 30.f);
    ImGui::SetCursorPosX(game.engine.width - 130.f);
    ImGui::SetWindowFontScale(1.5);
    ImGui::Text("%i / %i", entityGroup.player->nbBullet, entityGroup.player->maxNbBullet);

    ImGui::SetCursorPosY(game.engine.height / 2 - imageHeight / 2);
    ImGui::SetCursorPosX(game.engine.width / 2 - imageWidth / 2);
    ImGui::Image((void*)(intptr_t)imageText, ImVec2(imageWidth, imageHeight));

    ImGui::PopStyleColor(5);
    //ImGui::PopFont();
    ImGui::End(); 
}

void World::pauseMenu()
{
    Menu::preparePanel(ImVec2(game.engine.width / 2 - 200, game.engine.height / 2 - 150), {400, 350}, PURPLE, PINK, DARKERGREY);
    
    ImGui::Begin("Pause", &isPauseMenuOpen, ImGuiWindowFlags_NoDecoration);
    //ImGui::PushFont(font);
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
    //ImGui::PopFont();
    ImGui::End();
}

