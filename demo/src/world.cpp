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
        std::pair<std::unordered_map<Entity::EntityID, Entity::BasicEntity>::iterator, bool> it = grounds.emplace(Entity::EntityID{i}, Entity::EntityID{i});
        if (it.second) // if insertion took place
        {
            saveSystem.add(&it.first->second);
            nextEntity.next();
        }
    }

    std::size_t nbEnemies;
    loader.load(nbEnemies);

    for (std::size_t i = 0; i < nbEnemies; i++)
    {
        // enemies.emplace_back();
        // saveSystem.add(&enemies.back());
        std::pair<std::unordered_map<Entity::EntityID, Entity::Enemy>::iterator, bool> it = enemies.emplace(Entity::EntityID{i}, Entity::EntityID{i});
        if (it.second) // if insertion took place
        {
            saveSystem.add(&it.first->second);
            nextEntity.next();
        }
    }
}


World::World(Game& game, bool isLoaded, bool isEditorMode)
    : game(game), isEditorMode(isEditorMode)
{
    if (game.isAzerty)
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

    isLoadAvailable = Resources::File::doesFileExist(Game::savedFilename);

    // lock cursor
    ImGui::SetMouseCursor(ImGuiMouseCursor_None);
    glfwSetInputMode(game.engine.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    saveSystem.add(this);
    saveSystem.add(&root);
    saveSystem.add(&rendererSystem);
    saveSystem.add(&player);


    if (isLoaded)
    {
        saveSystem.load(Game::savedFilename);
        for (std::pair<const Entity::EntityID, Entity::Enemy>& enemy : enemies)
            enemy.second.loadLinks(root);
    }
    else 
    {
        for (unsigned int i = 0; i < 3; i++)
        {
            std::pair<std::unordered_map<Entity::EntityID, Entity::BasicEntity>::iterator, bool> newGroundIt 
                = grounds.emplace(nextEntity, nextEntity);
            
            if (newGroundIt.second) // if insertion took place
            {
                nextEntity.next();
                saveSystem.add(&newGroundIt.first->second);
            }
        }

        std::pair<std::unordered_map<Entity::EntityID, Entity::Enemy>::iterator, bool> newEnemyIt 
            = enemies.emplace(nextEntity, nextEntity);

        if (newEnemyIt.second) // if insertion took place
        {
            nextEntity.next();
            saveSystem.add(&newEnemyIt.first->second);
        }
    }

    // ===== Set up Entity ===== //
    player.setup(rendererSystem, 
                &game.engine.resourceManager.get(E_Model::E_DOG), 
                &game.engine.resourceManager.get(E_Shader::E_TEXTURED), 
                &game.engine.resourceManager.get(E_Texture::E_DOG_TEXTURE), 
                root);

    auto lambda = [&](Entity::BasicEntity& ent, Core::Maths::Vec3 location = {0.f,0,0}, Core::Maths::Vec3 scale = {1,1,1}, float zRot = 0)
    {
        ent.setup(rendererSystem, 
                &game.engine.resourceManager.get(E_Model::E_BOX), 
                &game.engine.resourceManager.get(E_Shader::E_TEXTURED), 
                &game.engine.resourceManager.get(E_Texture::E_GROUND), 
                root);

        // ent.mesh->transform.transform.rotation.x = M_PI / 2 * 0.90;
        if (!isLoaded)
        {
            ent.mesh->transform.transform.location = location;
            ent.mesh->transform.transform.scale = scale;
            ent.mesh->transform.transform.rotation.z = -zRot;
            ent.mesh->transform.transform.rotation.y = zRot;
            ent.mesh->color = Core::Maths::Vec4(0.7,0.7,0.7,1);
        }
        ent.mesh->transform.UpdateLocalTransformMatrix();
        ent.mesh->transform.transformMatrixNode->setDirtySelfAndChildren();
        ent.colliderIt = game.engine.physicsSystem.addComponentTo(ent);
        // ent.colliderIt->second.aabb.size = Core::Maths::Vec3{1.f/0.5f,1.f/0.5f,1.f/0.5f};
    };

    if (!isLoaded) 
    {
        std::unordered_map<Entity::EntityID, Entity::BasicEntity>::iterator it = grounds.begin();

        lambda((*it++).second, Core::Maths::Vec3(22+10, -100, 0+10), Core::Maths::Vec3(10,20, 9), M_PI / 2 * 0.5);
        lambda((*it++).second, Core::Maths::Vec3(-5, -20, 6-6), Core::Maths::Vec3(20, 4, 30), 0.f);//M_PI / 2 * 0.5);
        lambda((*it++).second, Core::Maths::Vec3(130+10, -40, 0), Core::Maths::Vec3(25, 4, 9));
    }
    else 
    {
        for (std::pair<const Entity::EntityID, Entity::BasicEntity>& ground : grounds)
        {
            lambda(ground.second);
        }
    }
    
    for (std::pair<const Entity::EntityID, Entity::Enemy>& enemy : enemies)
    {
        enemy.second.setup(rendererSystem, 
                    &game.engine.resourceManager.get(E_Model::E_DOG), 
                    &game.engine.resourceManager.get(E_Shader::E_TEXTURED), 
                    &game.engine.resourceManager.get(E_Texture::E_DOG_TEXTURE), 
                    root);

        enemy.second.physicComponent.collider.worldCollider.radius = 1.f;
    }


    fpsCamera.setup(player.mesh->transform);
    tpsCamera.setup(player.mesh->transform);

    // if (!isLoaded)
    {
        player.physicComponent.collider.worldCollider.radius = 1.f;
    }

    // if  (!isLoaded)
    //     enemies.cbegin().mesh->transform.transform.location.x = 6.f;

    for (std::pair<const Entity::EntityID, Entity::Enemy>& enemy : enemies)
    {
        enemy.second.setup2({0,-10,0}, {0.f,0,0});
        enemy.second.colliderIt = game.engine.physicsSystem.addComponentTo(enemy.second);
        enemy.second.mesh->transform.transform.location.x = 6.f;
    }

    player.colliderIt = game.engine.physicsSystem.addComponentTo(player);

    updateCameraProjection();

    if (isEditorMode)
    {
        controller = &fpsCamera;
        camera = &fpsCamera;
    }
}

World::~World()
{
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
    player.camera.projection = Core::Maths::Matrix4x4::CreatePerspectiveProjectionMatrix(game.engine.width, game.engine.height, 0.1, 10000, 45.f);
    fpsCamera.projection = player.camera.projection;
}

void World::addGround(const Core::Maths::Vec3& v)
{
    std::pair<std::unordered_map<Entity::EntityID, Entity::BasicEntity>::iterator, bool> groundIt 
            = grounds.emplace(nextEntity, nextEntity);
    if (groundIt.second) // if insertion took place
    {
        nextEntity.next();

        saveSystem.add(&groundIt.first->second);

        groundIt.first->second.setup(rendererSystem, 
                &game.engine.resourceManager.get(E_Model::E_BOX), 
                &game.engine.resourceManager.get(E_Shader::E_TEXTURED), 
                &game.engine.resourceManager.get(E_Texture::E_GROUND), 
                root);

        groundIt.first->second.mesh->transform.transform.location = v;
        groundIt.first->second.mesh->transform.transform.scale = {3.f,3,3};

        groundIt.first->second.mesh->transform.UpdateLocalTransformMatrix();
        groundIt.first->second.mesh->transform.transformMatrixNode->setDirtySelfAndChildren();

        groundIt.first->second.colliderIt = game.engine.physicsSystem.addComponentTo(groundIt.first->second);
    }
}

void World::addEnemy(const Core::Maths::Vec3& v)
{
    std::pair<std::unordered_map<Entity::EntityID, Entity::Enemy>::iterator, bool> enemyIt
            = enemies.emplace(nextEntity, nextEntity);
    if (enemyIt.second) // if insertion took place
    {
        nextEntity.next();

        saveSystem.add(&enemyIt.first->second);

        enemyIt.first->second.setup(rendererSystem, 
                &game.engine.resourceManager.get(E_Model::E_DOG), 
                &game.engine.resourceManager.get(E_Shader::E_TEXTURED), 
                &game.engine.resourceManager.get(E_Texture::E_DOG_TEXTURE), 
                root);

        enemyIt.first->second.patrolTarget = v;

        enemyIt.first->second.mesh->transform.transform.location = v;
        enemyIt.first->second.mesh->transform.transform.scale = {1.f,1,1};

        enemyIt.first->second.mesh->transform.UpdateLocalTransformMatrix();
        enemyIt.first->second.mesh->transform.transformMatrixNode->setDirtySelfAndChildren();
        enemyIt.first->second.colliderIt = game.engine.physicsSystem.addComponentTo(enemyIt.first->second);
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
            addGround(fpsCamera.transform.transformMatrixNode->worldData.getTranslationVector());
            wasEditorKeyPressed = true;
        }
    }
    else if (game.engine.isKeyDown(GLFW_KEY_O))
    {
        if (!wasEditorKeyPressed)
        {
            addEnemy(fpsCamera.transform.transformMatrixNode->worldData.getTranslationVector());
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
    for (std::pair<const Entity::EntityID, Entity::BasicEntity>& ground : grounds)
    {
        if (ground.second.mesh.isValid() && ground.second.mesh->transform.transformMatrixNode.isValid())
            ground.second.colliderIt->second.worldCollider.transform = ground.second.mesh->transform.transformMatrixNode->worldData;
    }

    for (std::pair<const Entity::EntityID, Entity::Enemy>& enemy : enemies)
    {
        if (enemy.second.mesh.isValid() && enemy.second.mesh->transform.transformMatrixNode.isValid())
            enemy.second.colliderIt->second.worldCollider.transform = enemy.second.mesh->transform.transformMatrixNode->worldData;
            
        enemy.second.colliderIt->second.isOverlap   = true;
    }

    if (player.mesh.isValid() && player.mesh->transform.transformMatrixNode.isValid())
    {
        player.colliderIt->second.worldCollider.transform = player.mesh->transform.transformMatrixNode->worldData;
        player.colliderIt->second.isOverlap   = true;
    }

    player.onPlayerDeath = [this](){ gameOver(); };

    if (player.mesh.isValid())
        player.physicComponent.collider.worldCollider.center = player.mesh->transform.transform.location;

    for (std::pair<const Entity::EntityID, Entity::Enemy>& enemy : enemies)
        if (enemy.second.mesh.isValid())
            enemy.second.physicComponent.collider.worldCollider.center = enemy.second.mesh->transform.transform.location;

    Physics::PhysicsSystem::PhysicsAdditionalData playerIgnoreData;
    playerIgnoreData.ignoredEntities.emplace(player);
    if (player.mesh.isValid())
    {
        player.mesh->transform.transform.location = game.engine.physicsSystem.simulatePhysics(player.physicComponent, 
                                                                                              player.mesh->transform.transform.location, 
                                                                                              playerIgnoreData, 
                                                                                              collisionsCallbacks, 
                                                                                              player);
        nextEntity.next();
    }
    game.engine.physicsSystem.simulateGravity(player.physicComponent, game.engine);

    if (player.mesh.isValid() && player.mesh->transform.transformMatrixNode.isValid()) 
    {
        player.mesh->transform.UpdateLocalTransformMatrix();
        player.mesh->transform.transformMatrixNode->cleanUpdate();
    }

    for (std::pair<const Entity::EntityID, Entity::Enemy>& enemy : enemies)
    {
        Physics::PhysicsSystem::PhysicsAdditionalData enemyIgnoreData;
        enemyIgnoreData.ignoredEntities.emplace(enemy.second);
        if (enemy.second.mesh.isValid())
        {
            enemy.second.mesh->transform.transform.location = game.engine.physicsSystem.simulatePhysics(enemy.second.physicComponent, 
                                                                                                        enemy.second.mesh->transform.transform.location, 
                                                                                                        enemyIgnoreData, 
                                                                                                        collisionsCallbacks, 
                                                                                                        enemy.first);
            nextEntity.next();
        }
        game.engine.physicsSystem.simulateGravity(enemy.second.physicComponent, game.engine);

        if (enemy.second.mesh.isValid() && enemy.second.mesh->transform.transformMatrixNode.isValid())
        {
            enemy.second.mesh->transform.UpdateLocalTransformMatrix();
            enemy.second.mesh->transform.transformMatrixNode->cleanUpdate();
        }
    }
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
        game.loadLevel(true);
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

void World::CollisionsCallbacks::onOverlap(Physics::PhysicsSystem::CollisionsCallbacksSentData& collisionData)
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

