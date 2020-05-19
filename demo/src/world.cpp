#include "world.hpp"

#include "file.hpp"
#include "shader.hpp"
#include "matrix4x4.hpp"
#include "camera.hpp"
#include "game.hpp"

#include "primitives.hpp"
#include "log.hpp"
#include "editorUtility.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void World::save(Save::Saver& saver) 
{
    saver.save(nextEntity);
    saver.save(grounds.size());
    saver.save(enemies.size());
}

void World::loadData(Save::Loader& loader) 
{
    loader.load(nextEntity);

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
    saveSystem.load(savedFilename);
    for (std::pair<const Entity::EntityID, Entity::Enemy>& enemy : enemies)
        enemy.second.loadLinks(root);
}

void World::makeNewLevel()
{
    player.colliderIt = game.engine.physicsSystem.addCollider<Box>(player);
    player.physicCompIt = game.engine.physicsSystem.addPhysicComponent(player);

    addGround({{5.f, -30, 0}, {0.f,0,0}, {20,1,20}});
    addGround({{50.f, -33, 0}, {0.f,0,0}, {20,1,30}});
    addGround({{5.f, -35, -20}, {0.f,0,0}, {30,1,40}});

    addEnemy({{2.f, -4, 0}, {0.f,0,0}, {1,1,1}});
}

void World::load()
{
    setKeys(game.isAzerty);

    isLoadAvailable = Resources::File::doesFileExist(Game::savedFilename);

    // lock cursor
    ImGui::SetMouseCursor(ImGuiMouseCursor_None);
    glfwSetInputMode(game.engine.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    saveSystem.add(this);
    saveSystem.add(&root);
    saveSystem.add(&rendererSystem);
    saveSystem.add(&player);

    if (isLoaded)
        loadFromSavefile(Game::savedFilename);
    else 
        makeNewLevel();

    // ===== Set up Entity ===== //

    player.setup(rendererSystem, 
                &game.engine.resourceManager.get(E_Model::E_DOG), 
                &game.engine.resourceManager.get(E_Shader::E_TEXTURED), 
                &game.engine.resourceManager.get(E_Texture::E_DOG_TEXTURE), 
                root);
    fpsCamera.setup(player.mesh->transform);
    tpsCamera.setup(player.mesh->transform);

    player.colliderIt = game.engine.physicsSystem.addCollider<Box>(player);
    player.physicCompIt = game.engine.physicsSystem.addPhysicComponent(player);
    player.colliderIt->transform = player.physicCompIt->collider.transform = &player.mesh->transform;


    // if (!isLoaded)
    {
        player.physicCompIt-> collider.worldCollider.radius = 1.f;
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
    enemies.clear();
    grounds.clear();
    rendererSystem.reset();
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
    std::pair<std::unordered_map<Entity::EntityID, Entity::BasicEntity>::iterator, bool> groundIt 
            = grounds.emplace(nextEntity, nextEntity);
    if (groundIt.second) // if insertion took place
    {
        nextEntity.next();

        saveSystem.add(&groundIt.first->second);

        Entity::BasicEntity& ground = groundIt.first->second;

        ground.setup(rendererSystem, 
                &game.engine.resourceManager.get(E_Model::E_BOX), 
                &game.engine.resourceManager.get(E_Shader::E_TEXTURED), 
                &game.engine.resourceManager.get(E_Texture::E_GROUND), 
                root);

        ground.mesh->transform.transform = transform;
        ground.mesh->transform.UpdateLocalTransformMatrix();
        ground.mesh->transform.transformMatrixNode->setDirtySelfAndChildren();

        ground.colliderIt = game.engine.physicsSystem.addCollider<Box>(ground);
        ground.colliderIt->transform = &ground.mesh->transform;
    }
}

void World::addEnemy(const Physics::Transform& transform)
{
    std::pair<std::unordered_map<Entity::EntityID, Entity::Enemy>::iterator, bool> enemyIt
            = enemies.emplace(nextEntity, nextEntity);
    if (enemyIt.second) // if insertion took place
    {
        nextEntity.next();

        saveSystem.add(&enemyIt.first->second);

        Entity::Enemy& enemy = enemyIt.first->second;

        enemy.setup(rendererSystem, 
                &game.engine.resourceManager.get(E_Model::E_DOG), 
                &game.engine.resourceManager.get(E_Shader::E_TEXTURED), 
                &game.engine.resourceManager.get(E_Texture::E_DOG_TEXTURE), 
                root);

        enemy.setup2({0,-10,0}, {0.f,0,0});

        enemy.patrolTarget = transform.location;

        enemy.mesh->transform.transform = transform;
        enemy.mesh->transform.UpdateLocalTransformMatrix();
        enemy.mesh->transform.transformMatrixNode->setDirtySelfAndChildren();

        enemy.colliderIt = game.engine.physicsSystem.addCollider<Box>(enemy);
        enemy.physicCompIt = game.engine.physicsSystem.addPhysicComponent(enemy);
        enemy.colliderIt->transform = enemy.physicCompIt->collider.transform = &enemy.mesh->transform;
        enemy.physicCompIt->collider.worldCollider.radius = 1.f;
    }
}

void World::updateEditorFunctions()
{
    if (game.engine.isKeyDown(GLFW_KEY_E))
    {
        if (!wasEditorKeyPressed)
        {
            editorMode.next();
            wasEditorKeyPressed = true;
        }
    }
    else if (game.engine.isKeyDown(GLFW_KEY_C))
    {
        if (!wasEditorKeyPressed)
        {
            editorMode.mode = EditorMode::E_TRANSLATION;
            wasEditorKeyPressed = true;
        }
    }
    else if (game.engine.isKeyDown(GLFW_KEY_V))
    {
        if (!wasEditorKeyPressed)
        {
            editorMode.nextRotation();
            wasEditorKeyPressed = true;
        }
    }
    else if (game.engine.isKeyDown(GLFW_KEY_B))
    {
        if (!wasEditorKeyPressed)
        {
            editorMode.nextScale();
            wasEditorKeyPressed = true;
        }
    }
    else if (game.engine.isKeyDown(GLFW_KEY_P))
    {
        if (!wasEditorKeyPressed)
        {
            addGround({fpsCamera.transform.transformMatrixNode->worldData.getTranslationVector(), {0.f,0,0}, {1,1,1}});
            wasEditorKeyPressed = true;
        }
    }
    else if (game.engine.isKeyDown(GLFW_KEY_O))
    {
        if (!wasEditorKeyPressed)
        {
            addEnemy({fpsCamera.transform.transformMatrixNode->worldData.getTranslationVector(), {0.f,0,0}, {1,1,1}});
            wasEditorKeyPressed = true;
        }
    }
    else 
    {
        wasEditorKeyPressed = false;
    }

    if (glfwGetMouseButton(game.engine.window, GLFW_MOUSE_BUTTON_LEFT) && editorSelectedEntity != nullptr)
    {
        fpsCamera.enableInputs = false;
        switch (editorMode.mode)
        {
        case EditorMode::E_TRANSLATION:
            EditorUtility::moveEntityWithCursor(fpsCamera, *editorSelectedEntity, game.engine);
            break;

        case EditorMode::E_ROTATION_X:
            EditorUtility::rotateEntityWithCursor(*editorSelectedEntity, game.engine, 0);
            break;
        case EditorMode::E_ROTATION_Y:
            EditorUtility::rotateEntityWithCursor(*editorSelectedEntity, game.engine, 1);
            break;
        case EditorMode::E_ROTATION_Z:
            EditorUtility::rotateEntityWithCursor(*editorSelectedEntity, game.engine, 2);
            break;

        case EditorMode::E_SCALE_X:
            EditorUtility::scaleEntityWithCursor(*editorSelectedEntity, game.engine, 0);
            break;
        case EditorMode::E_SCALE_Y:
            EditorUtility::scaleEntityWithCursor(*editorSelectedEntity, game.engine, 1);
            break;
        case EditorMode::E_SCALE_Z:
            EditorUtility::scaleEntityWithCursor(*editorSelectedEntity, game.engine, 2);
            break;
        
        default:
            break;
        }
    }
    else
    {
        Entity::BasicEntity* newSelection = nullptr;
        SegmentHit hit;
        hit.t = 2.f;
        for (std::pair<const Entity::EntityID, Entity::BasicEntity>& ground : grounds)
        {
            SegmentHit tempHit;
            if (EditorUtility::isInFrontOfPlayer(fpsCamera, ground.second, tempHit))
            {
                if (tempHit.t < hit.t)
                {
                    hit = tempHit;
                    newSelection = &ground.second;
                }
            }
        }

        if (newSelection != nullptr)
        {
            if (newSelection != editorSelectedEntity)
            {
                newSelection->mesh->shader = &game.engine.resourceManager.get(E_Shader::E_TEXTURED);

                editorSelectedEntity = newSelection;
                editorSelectedEntity->mesh->color = {0, 0.4, 0.4, 0.5};
                editorSelectedEntity->mesh->shader = &game.engine.resourceManager.get(E_Shader::E_FLAT);
            }
        }
        else 
        {
            if (editorSelectedEntity != nullptr)
            {
                editorSelectedEntity->mesh->shader = &game.engine.resourceManager.get(E_Shader::E_TEXTURED);
            }
            editorSelectedEntity = nullptr;
        }

        fpsCamera.enableInputs = true;
    } 
}

void World::updatePhysics()
{
    //////// CAN BE SET BEFORE
    for (std::pair<const Entity::EntityID, Entity::Enemy>& enemy : enemies)
    {
        enemy.second.colliderIt->isOverlap   = true;
    }

    if (player.mesh.isValid() && player.mesh->transform.transformMatrixNode.isValid())
    {
        player.colliderIt->isOverlap   = true;
    }

    player.onPlayerDeath = [this](){ gameOver(); };
    /////////////

    game.engine.physicsSystem.simulate(collisionsCallbacks, game.engine);
}

void World::update()   
{
    if (game.engine.hasWindowSizeChanged)
        updateCameraProjection();

    if (isPauseMenuOpen == false)
    {
        // Update entities
        for (std::pair<const Entity::EntityID, Entity::Enemy>& enemy : enemies)
        {
            if (player.mesh.isValid() && player.mesh->transform.transformMatrixNode.isValid())
            {
                enemy.second.chaseTarget = player.mesh->transform.transformMatrixNode->worldData.getTranslationVector();
                enemy.second.update(game.engine);
            }
        }

        // Save game
        if (glfwGetKey(game.engine.window, GLFW_KEY_F5))
        {
            saveSystem.save(Game::savedFilename);
            isLoadAvailable = Resources::File::doesFileExist(Game::savedFilename);
        }

        updatePhysics();

        if (isEditorMode)
            updateEditorFunctions();
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
        rendererSystem.draw(*camera);
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
    std::cout << imageText << std::endl;

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
        saveSystem.save(Game::savedFilename);
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

Physics::CollisionComponentInterface* World::getCollisionComponentEntityFromID(const Entity::EntityID& entityID)
{
    if (player == entityID)
        return &player;
    
    std::unordered_map<Entity::EntityID, Entity::Enemy>::iterator it = enemies.find(entityID);
    if (it != enemies.end() && it->first == entityID)
    {
        return &it->second;
    }

    return nullptr;
}


void World::CollisionsCallbacks::onCollisionEnter(Physics::PhysicsSystem::CollisionsCallbacksSentData& collisionData)
{
    if (world.player == collisionData.movingEntityID)
    {
        world.player.onCollisionEnter(collisionData.hit);
    }
}

void World::CollisionsCallbacks::onCollisionExit(const Entity::EntityID& entityID)
{
    Physics::CollisionComponentInterface* ent = world.getCollisionComponentEntityFromID(entityID);
    ent->onCollisionExit();
}

void World::CollisionsCallbacks::onOverlap(const Physics::PhysicsSystem::CollisionsCallbacksSentData& collisionData)
{
    Physics::CollisionComponentInterface* movingComp    = world.getCollisionComponentEntityFromID(collisionData.movingEntityID);
    Physics::CollisionComponentInterface* collisionComp = world.getCollisionComponentEntityFromID(collisionData.encounteredEntityID);

    if (movingComp != nullptr)
    {
        movingComp->onOverlapEnterSelfHit(collisionData.hit);
    }

    if (collisionComp != nullptr)
    {
        collisionComp->onOverlapEnterAnotherHit(collisionData.hit);
    }
}

