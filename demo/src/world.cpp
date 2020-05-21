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

void World::save(Save::Saver& saver) 
{
    // saver.save(nextEntity);
    saver.save(grounds.size());
    saver.save(enemies.size());
}

void World::loadData(Save::Loader& loader) 
{
    // loader.load(nextEntity);

    std::size_t nbGrounds;
    loader.load(nbGrounds);

    for (std::size_t i = 0; i < nbGrounds; i++)
    {
        // std::pair<std::unordered_map<Entity::EntityID, Entity::BasicEntity>::iterator, bool> it = grounds.emplace(Entity::EntityID{i}, Entity::EntityID{i});
        // if (it.second) // if insertion took place
        {
            // game.engine.physicsSystem.loadPhysicComponentItContainer(it.first->second.physicCompIt, it.first->first);
            // game.engine.physicsSystem.loadColliderItContainer(it.first->second.colliderIt, it.first->first);
            // it.first->second.colliderIt = game.engine.physicsSystem.addCollider<Box>(it.first->second);
            // it.first->second.physicCompIt = game.engine.physicsSystem.addPhysicComponent(it.first->second);
            // nextEntity.next();
            addGround({{}});
        }
    }

    std::size_t nbEnemies;
    loader.load(nbEnemies);

    for (std::size_t i = 0; i < nbEnemies; i++)
    {
        // enemies.emplace_back();
        // saveSystem.add(&enemies.back());
        // std::pair<std::unordered_map<Entity::EntityID, Entity::Enemy>::iterator, bool> it = enemies.emplace(Entity::EntityID{i}, Entity::EntityID{i});
        // if (it.second) // if insertion took place
        // {
        //     saveSystem.add(&it.first->second);
            addEnemy({{}});
            // it.first->second.colliderIt = game.engine.physicsSystem.addCollider<Box>(it.first->second);
            // it.first->second.physicCompIt = game.engine.physicsSystem.addPhysicComponent(it.first->second);
            // it.first->second.colliderIt->transform = it.first->second.physicCompIt->collider.transform = &it.first->second.mesh->transform;
            // game.engine.physicsSystem.loadPhysicComponentItContainer(it.first->second.physicCompIt, it.first->first);
            // game.engine.physicsSystem.loadColliderItContainer(it.first->second.colliderIt, it.first->first);
            // nextEntity.next();
        // }
    }
}


World::World(Game& game, bool isLoaded, bool isEditorMode)
    : game(game), isLoaded(isLoaded), isEditorMode(isEditorMode)
{
    bool ret = Resources::Texture::loadTexture("resources/textures/crosshair.png", imageWidth, imageHeight, imageText);
}

void World::setKeys(bool isAzerty)
{
    if (isAzerty)
    {
        player.inputKeys.keys = 
        {
            GLFW_KEY_Z,
            GLFW_KEY_S,
            GLFW_KEY_D,
            GLFW_KEY_Q,
            GLFW_KEY_SPACE
        };

        fpsCamera.inputKeys.keys = 
        {
            GLFW_KEY_Z,
            GLFW_KEY_S,
            GLFW_KEY_D,
            GLFW_KEY_Q,
        };
    }
    else 
    {
        player.inputKeys.keys = 
        {
            GLFW_KEY_W,
            GLFW_KEY_S,
            GLFW_KEY_D,
            GLFW_KEY_A,
            GLFW_KEY_SPACE
        };

        fpsCamera.inputKeys.keys = 
        {
            GLFW_KEY_W,
            GLFW_KEY_S,
            GLFW_KEY_D,
            GLFW_KEY_A,
        };
    }
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

    addGround({{5.f, -30, 0}, {0.f,0,0}, {10,1,20}});
    addGround({{50.f, -33, 0}, {0.f,0,0}, {1,1,1}});
    addGround({{5.f, -35, -20}, {0.f,0,0}, {20,1,40}});

    addGround({{2.f, -28, 50}, {0.f,0,0}, {1,1,2}});

    addEnemy({{5.f, -4, 5}, {0.f,0,0}, {1,1,1}});

    // === Add Player === //
    player.setTransformParent(root);
    player.setResources(game.engine.resourceManager);

    player.addRendering(game.engine.rendererSystem);
    player.addCollisions(game.engine.physicsSystem);
    player.addPhysics(game.engine.physicsSystem);

    player.camera.setup(player.transform);

    player.camera.transform.transform.location.y = 2.f;
    player.camera.transform.UpdateLocalTransformMatrix();
    player.camera.transform.transformMatrixNode->setDirtySelfAndChildren();

    // === Add other cameras === //
    fpsCamera.setup(player.transform);
    tpsCamera.setup(player.transform);
}

void World::load()
{
    setKeys(game.isAzerty);

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

    lightManager.lightsBufferInit(10);
    lightManager.lights.emplace_back();
    lightManager.lights.emplace_back();
    {
        Renderer::LightData& l = lightManager.lights[lightManager.lights.size() - 1].lightData;
        l.location = {20.f, -27.f, 12, 0.0}; 
        l.lightType = 3;
        l.ambient = {0.7, 0.7,0.7,0};
    }

    updateCameraProjection();

    if (isEditorMode)
    {
        controller = &fpsCamera;
        camera = &fpsCamera;
    }
}

World::~World()
{
    for (Entity::Enemy* enemy : enemies)
        if (enemy != nullptr)
            delete enemy;

    for (Entity::Ground* ground : grounds)
        if (ground != nullptr)
            delete ground;


    enemies.clear();
    grounds.clear();
    game.engine.rendererSystem.reset();
    game.engine.physicsSystem.reset(); 
}

void World::inputs()
{
    if (controller != nullptr && !isPauseMenuOpen)
        controller->inputs(game.engine);

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
    player.camera.projection = Core::Maths::Matrix4x4::CreatePerspectiveProjectionMatrix(game.engine.width, game.engine.height, 0.1, 10000, 90.f);
    fpsCamera.projection = player.camera.projection;
}

void World::addGround(const Physics::Transform& transform)
{
    grounds.emplace_back(new Entity::Ground());
    Entity::Ground* ground = grounds.back();

    ground->setResources(game.engine.resourceManager);
    ground->setTransformParent(root);            
    ground->setTransform(transform);

    ground->addRendering(game.engine.rendererSystem);
    ground->addCollisions(game.engine.physicsSystem);
}

void World::addEnemy(const Physics::Transform& transform)
{ 
    enemies.emplace_back(new Entity::Enemy());
    Entity::Enemy* enemy = enemies.back();

    enemy->setResources(game.engine.resourceManager);
    enemy->setTransformParent(root);            
    enemy->setTransform(transform);

    enemy->addRendering(game.engine.rendererSystem);
    enemy->addCollisions(game.engine.physicsSystem);
    enemy->addPhysics(game.engine.physicsSystem);

    enemy->patrolTarget = transform.location;
}

void World::addBullet(const Physics::Transform& transform)
{
    bullets.emplace_back(std::make_unique<Entity::RenderedEntity>());

    std::unique_ptr<Entity::RenderedEntity>& bullet = bullets.back();

    bullet->setResources(game.engine.resourceManager);
    bullet->addRendering(game.engine.rendererSystem);

    bullet->setTransformParent(root);
    bullet->setTransform(transform);
    bullet->timer = game.engine.lastTime + bullet->lifeTime;
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
    Entity::Ground* it = grounds.front();
    it->transform.transform.location.z = std::cos(game.engine.lastTime / 2) * 50;
    it->transform.UpdateLocalTransformMatrix();
    it->transform.transformMatrixNode->setDirtySelfAndChildren();

    player.onPlayerDeath = [this](){ gameOver(); };
    /////////////

    game.engine.physicsSystem.simulatePhysics(game.engine);
}

void World::update()   
{
    if (game.engine.hasWindowSizeChanged)
        updateCameraProjection();

    if (isPauseMenuOpen == false)
    {
        // Update entities
        for (Entity::Enemy* enemy : enemies)
        {
            if (player.transform.transformMatrixNode.isValid())
            {
                enemy->chaseTarget = player.transform.transformMatrixNode->worldData.getTranslationVector();
                enemy->update(game.engine);
            }
        }

        player.tryToJump(game.engine);

        if(glfwGetMouseButton(game.engine.window, GLFW_MOUSE_BUTTON_LEFT))
        {
            Segment3D directionHit = player.shoot();
            SegmentHit hit;
            Physics::CollisionComponentInterface<Box>* touchEntity = nullptr;

            if(game.engine.physicsSystem.raycast(directionHit, hit, touchEntity))
            {
                //test hit enemy
                std::vector<Entity::Enemy*>::iterator it = enemies.begin();
                while (it != enemies.end() && (*it) != touchEntity)
                {
                    it++;
                }
                if (it != enemies.end())
                {
                    //hit: add a box during 2s
                    (*it)->takeDamage(5);
                    addBullet({{hit.collisionPoint}, {0,0,0.f}, {0.05f,0.05f,0.05f}});
                }
                else 
                {
                    //test hit ground/wall
                    std::vector<Entity::Ground*>::iterator it = grounds.begin();
                    while (it != grounds.end() && (*it) != touchEntity)
                    {
                        it++;
                    }
                    if (it != grounds.end())
                    {
                        addBullet({{hit.collisionPoint}, {0,0,0.f}, {0.5f,0.5f,0.5f}});
                    }
                }
            }
        }

        if(bullets.size() > 0)
        {
            std::vector<std::unique_ptr<Entity::RenderedEntity>>::iterator r = bullets.begin();
            while(bullets.size() != 0 && game.engine.lastTime >= r->get()->timer)
            {
                // (*r)->mesh.erase();
                game.engine.rendererSystem.erase((*r)->meshIt);
                r = bullets.erase(r);
            }
               
        }

        // Save game
        if (glfwGetKey(game.engine.window, GLFW_KEY_F5))
        {
            // saveSystem.save(Game::savedFilename);
            isLoadAvailable = Resources::File::doesFileExist(Game::savedFilename);
        }

        updatePhysics();

        // if (isEditorMode)
        //     updateEditorFunctions();
    }
}

void World::gameOver()
{
    if (Resources::File::doesFileExist(game.savedFilename))
    {
        game.loadLevel(false);
    }
    else
    {
        game.loadLevel(false);
    }
    
}

void World::renderUI() 
{
    if (isPauseMenuOpen)
        pauseMenu();
    else
        hud();
        
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void World::render()   
{
    if (camera != nullptr)
    {
        camera->updateViewProjectionMatrix();
        game.engine.rendererSystem.draw(*camera, lightManager);
    }
}

void World::hud()
{
    Menu::preparePanel(ImVec2(0, 0), {game.engine.width, game.engine.height}, {0.f,0.f,0.f,0.f}, {0.f,0.f,0.f,0.f}, {0.f,0.f,0.f,0.f});
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(0.f,0.f,0.f,0.f));

    ImGui::Begin("Pause", &inGame, ImGuiWindowFlags_NoDecoration);
    //ImGui::PushFont(font);
    ImGui::SetCursorPosY(game.engine.height - 30.f);
    assert(player.maxLifePoints != 0.f);
    ImGui::SetWindowFontScale(1.0);
    ImGui::ProgressBar(player.lifePoints / player.maxLifePoints, ImVec2(200,20));

    ImGui::SetCursorPosY(game.engine.height - 30.f);
    ImGui::SetCursorPosX(game.engine.width - 100.f);
    ImGui::SetWindowFontScale(1.5);
    ImGui::Text("%i / %i", player.nbBullet, player.maxNbBullet);

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
    
    ImGui::Begin("Pause", &isPauseMenuOpen, ImGuiWindowFlags_NoTitleBar);
    ImGui::Indent(400 / 2 - 50);
    //ImGui::PushFont(font);
    ImGui::SetWindowFontScale(1.0);
    ImGui::SetCursorPosY(30.0f);
    if (ImGui::Button("Back to game", ImVec2(100, 50)))
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_None);
        glfwSetInputMode(game.engine.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);        
        isPauseMenuOpen = !isPauseMenuOpen;
        inGame = true;
    }

    ImGui::SetCursorPosX(130.0f);
    ImGui::SetCursorPosY(110.0f);
    if (ImGui::Button("Back to main menu", ImVec2(150, 50)))
    {        
        isPauseMenuOpen = !isPauseMenuOpen;
        game.loadMenu();
    }

    ImGui::SetCursorPosY(190.0f);
    if (ImGui::Button("Save", ImVec2(100, 50)))
    {
        // saveSystem.save(Game::savedFilename);
        isLoadAvailable = Resources::File::doesFileExist(Game::savedFilename);
    }

    if (isLoadAvailable)
    {
        ImGui::SetCursorPosY(270.0f);
        if (ImGui::Button("Load", ImVec2(100, 50)))
            game.loadLevel(true);
    }

    ImGui::PopStyleColor(4);
    //ImGui::PopFont();
    ImGui::End();
}

