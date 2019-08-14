#include "GameWorld.h"
#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

#include "Player.h"
#include "Enemy_Morph.h"
#include "Tower.h"
#include "MapIcon.h"

#include "MaterialsDB.h"

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}

glm::ivec2 resolution;

std::list<Entity*> *entities = new std::list<Entity*>;
std::list<Entity*> *enemies = new std::list<Entity*>;

Player * crtPlayer;

Enemy_Morph * _enemy;
Tower *_tower;

MapIcon *enemyIcon, *playerIcon, *towerIcon;

Shader *shader = NULL;

std::list<glm::vec3> *waypoints = new std::list<glm::vec3>;

double lastSpawnTime = 0;
int enemyInterval = 5;
int crtWave = 0;

void GameWorld::CreateWaypoints()
{
    waypoints->push_back(glm::vec3(-12, 0, -12));
    waypoints->push_back(glm::vec3(12, 0, -12));
    waypoints->push_back(glm::vec3(-12, 0, 12));
    waypoints->push_back(glm::vec3(12, 0, 12));
}


void GameWorld::Init()
{
    CreateWaypoints();

    // shader pt lumini
    shader = new Shader("Shader");
    shader->AddShader("Source/BuildAndSurvive/Shaders/Light_VS.glsl", GL_VERTEX_SHADER);
    shader->AddShader("Source/BuildAndSurvive/Shaders/Light_FS.glsl", GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;


    camera = new Camera();
    camera->Set(glm::vec3(0, 0, 3.5f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    crtPlayer = new Player();
    crtPlayer->SetEntities(entities);
    crtPlayer->SetEnemies(enemies);

    entities->push_back(crtPlayer);


    // -- creez 3 turnuri pt aparare
    _tower = new Tower();
    _tower->SetPosition(glm::vec3(-3, 0, -3));
    _tower->SetEnemies(enemies);
    _tower->SetEntities(entities);

    entities->push_back(_tower);

    _tower = new Tower();
    _tower->SetPosition(glm::vec3(3, 0, 3));
    _tower->SetEnemies(enemies);
    _tower->SetEntities(entities);

    entities->push_back(_tower);

    _tower = new Tower();
    _tower->SetPosition(glm::vec3(-3, 0, 6));
    _tower->SetEnemies(enemies);
    _tower->SetEntities(entities);

    entities->push_back(_tower);

    // -- iconuri pt minimap
    enemyIcon = new MapIcon();
    enemyIcon->SetMaterialType(MaterialTypes::MATERIAL_RED_PLASTIC);

    playerIcon = new MapIcon();
    playerIcon->SetMaterialType(MaterialTypes::MATERIAL_BLUE_WATER);

    towerIcon = new MapIcon();
    towerIcon->SetMaterialType(MaterialTypes::MATERIAL_GREEN_FLESH);

}

void GameWorld::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    resolution = window->GetResolution();
 
}

void GameWorld::Update(float deltaTimeSeconds)
{
    glm::mat4 modelMatrix;
    
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    // daca playerul moare, revin la 3rd person, echipez pistol
    if (crtPlayer->IsDying() && crtPlayer->GetWeaponType() == WeaponTypes::WEAPON_RIFLE)
    {
        camera->position -= camera->distanceToTarget * glm::normalize(camera->forward) + 2.0f * crtPlayer->GetCenterOffset();
        crtPlayer->SetWeaponType(WeaponTypes::WEAPON_PISTOL);
        crtPlayer->SetPosition(camera->GetTargetPosition());
    }


    glm::vec3 originalPosition = camera->position;
    glm::vec3 originalUp = camera->up;
    glm::vec3 originalForward = camera->forward;
    glm::vec3 originalRight = camera->right;

    // -- jocul propriu zis
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    if (Engine::GetElapsedTime() - lastSpawnTime > enemyInterval)
    {
        // spawnez un inamic
        _enemy = new Enemy_Morph();
        _enemy->SetWaypoints(waypoints);
        _enemy->SetPosition(*(waypoints->begin()));
        _enemy->SetHP((int)(_enemy->GetHP() + lastSpawnTime));
        _enemy->SetScale(_enemy->GetScale() + glm::vec3((float)crtWave) / 4000.0f);
        _enemy->SetPlayer(crtPlayer);

        entities->push_back(_enemy);
        enemies->push_back(_enemy);

        crtWave++;

        if (enemyInterval - crtWave / 5 > 1)
            enemyInterval -= crtWave / 5;

        lastSpawnTime = Engine::GetElapsedTime();
    }

    for (std::list<Entity*>::iterator i = entities->begin(); i != entities->end(); i++)
    {
        // nu afisez jucatorul daca am zoom de la arma
        if ((*i) == crtPlayer && crtPlayer->GetWeaponType() == WeaponTypes::WEAPON_RIFLE)
            continue;

        (*i)->Update(deltaTimeSeconds);

        if ((*i)->CanDespawn())
        {
            if (dynamic_cast<Enemy_Morph*>(*i) != NULL)
                enemies->remove(*i);

            i = entities->erase(i);
            i--;

            continue;
        }

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, (*i)->GetPosition());
        modelMatrix = glm::rotate(modelMatrix, (*i)->GetAngleOX(), glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, (*i)->GetAngleOY(), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, (*i)->GetAngleOZ(), glm::vec3(0, 0, 1));
        modelMatrix = glm::scale(modelMatrix, (*i)->GetScale());

        RenderMesh((*i), shaders["Shader"], modelMatrix);
    }

    // vietile jucatorului
    for (std::list<Life*>::iterator i = crtPlayer->GetLives()->begin(); i != crtPlayer->GetLives()->end(); i++)
    {
        if (crtPlayer->GetNOfLives() < crtPlayer->GetLives()->size())
        {
            i = crtPlayer->GetLives()->erase(i);

            break;
        }


        modelMatrix = glm::mat4(1);

        glm::vec3 translationVector = crtPlayer->GetLivesFloatingRadius() * (*i)->GetTranslationVector();

        // pt fiecare viata folosesc un singur unghi - il retin in angleOX (playerul nu face rotatii pe OX deci valoarea originala nu e necesara)
        (*i)->SetAngleOX((*i)->GetAngleOX() + 1.2f * deltaTimeSeconds);
        (*i)->SetPosition(crtPlayer->GetPosition() + translationVector + crtPlayer->GetCenterOffset());

        modelMatrix = glm::translate(modelMatrix, (*i)->GetPosition());
        modelMatrix = glm::translate(modelMatrix, -translationVector);

        // rotatie pe OY pt a ajusta pozitia atunci cand se roteste caracterul jucatorului
        modelMatrix = glm::rotate(modelMatrix, crtPlayer->GetAngleOY(), glm::vec3(0, 1, 0));

        // rotatia automata in jurul caracterului
        modelMatrix = glm::rotate(modelMatrix, (*i)->GetAngleOX(), (*i)->GetRotationVector());

        modelMatrix = glm::translate(modelMatrix, translationVector);

        modelMatrix = glm::scale(modelMatrix, (*i)->GetScale());



        RenderMesh((*i), shaders["Shader"], modelMatrix);
    }
    


    DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);

    camera->Set(glm::vec3(0, 10, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
    projectionMatrix = glm::ortho((float)-viewSpace.width/50, (float)viewSpace.width/50, -viewSpace.height/50 * window->props.aspectRatio, viewSpace.height/50 * window->props.aspectRatio, (float)-20, (float)200.0f);
    
    // -- minimap
    viewSpace = ViewportSpace((int)(resolution.x * 0.65), 0, (int)(resolution.x * 0.3), (int)(resolution.y * 0.4));
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);
    
    for (std::list<Entity*>::iterator i = entities->begin(); i != entities->end(); i++)
    {   
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, (*i)->GetPosition());
        modelMatrix = glm::rotate(modelMatrix, (*i)->GetAngleOX(), glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, (*i)->GetAngleOY(), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, (*i)->GetAngleOZ(), glm::vec3(0, 0, 1));
 

        // jucator ?
        if ((*i) == crtPlayer)
        {
            modelMatrix = glm::scale(modelMatrix, playerIcon->GetScale());
            RenderMesh(playerIcon, shaders["Shader"], modelMatrix);

            continue;
        }

        // inamic?
        if (dynamic_cast<Enemy_Morph*>(*i) != NULL)
        {
            modelMatrix = glm::scale(modelMatrix, enemyIcon->GetScale());
            RenderMesh(enemyIcon, shaders["Shader"], modelMatrix);

            continue;
        }
        // turn
        if (dynamic_cast<Tower*>(*i) != NULL)
        {
            modelMatrix = glm::scale(modelMatrix, towerIcon->GetScale());
            RenderMesh(towerIcon, shaders["Shader"], modelMatrix);

            continue;
        }
    }

    DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);

    // refac setarile camerei
    camera->position = originalPosition;
    camera->right = originalRight;
    camera->forward = originalForward;
    camera->up = originalUp;

}

void GameWorld::FrameEnd()
{
}

void GameWorld::RenderMesh(Entity * entity, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!entity || !shader)
        return;

    glUseProgram(shader->program);

    // -- light position uniform
    glm::vec3 lightPosition = glm::vec3(1, 5, 1);
    int lightPositionLocation = glGetUniformLocation(shader->program, "light_position");
    glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition));
    
    // -- camera position uniform
    glm::vec3 eyePosition = camera->position;
    int eyePositionLocation = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(eyePositionLocation, 1, glm::value_ptr(eyePosition));

    int shininessLocation = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(shininessLocation, materialsList[entity->GetMaterialType()].shininess);

    int kdLocation = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(kdLocation, materialsList[entity->GetMaterialType()].kd);

    int ksLocation = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(ksLocation, materialsList[entity->GetMaterialType()].ks);

    int colorLocation = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(colorLocation, 1, glm::value_ptr(materialsList[entity->GetMaterialType()].color));


    // render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, false, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, false, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // incerc mesh-ul daca nu exista deja
    if (!meshes[entity->GetMeshName()])
    {
        meshes[entity->GetMeshName()] = new Mesh(entity->GetMeshName());
        meshes[entity->GetMeshName()]->LoadMesh(meshPath, entity->GetMeshName());
    }

    meshes[entity->GetMeshName()]->Render();
}

void GameWorld::OnInputUpdate(float deltaTime, int mods)
{
    // fara inputuri daca jucatorul nu mai are vieti
    if (crtPlayer->IsDying())
        return;

    float cameraSpeed = 3.0f;

    if (window->KeyHold(GLFW_KEY_W))
        camera->MoveForward(cameraSpeed * deltaTime);
     
    if (window->KeyHold(GLFW_KEY_A))
        camera->TranslateRight(-cameraSpeed * deltaTime);

    if (window->KeyHold(GLFW_KEY_S))
        camera->MoveForward(-cameraSpeed * deltaTime);

    if (window->KeyHold(GLFW_KEY_D))
        camera->TranslateRight(cameraSpeed * deltaTime);

    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT))
        crtPlayer->FireWeapon();

    if (crtPlayer->GetWeaponType() == WeaponTypes::WEAPON_RIFLE)
        crtPlayer->SetPosition(camera->position);
    else
        crtPlayer->SetPosition(camera->GetTargetPosition());

}

void GameWorld::OnKeyPress(int key, int mods)
{
    // nu prea mai poate schimba arme
    if (crtPlayer->IsDying())
        return;

    if (key == GLFW_KEY_1)
    {
        if (crtPlayer->GetWeaponType() == WeaponTypes::WEAPON_RIFLE)
            camera->position -= camera->distanceToTarget * glm::normalize(camera->forward) + 2.0f * crtPlayer->GetCenterOffset();

        crtPlayer->SetWeaponType(WeaponTypes::WEAPON_PISTOL);
    }
    if (key == GLFW_KEY_2)
    {
        if (crtPlayer->GetWeaponType() != WeaponTypes::WEAPON_RIFLE)
            camera->position += camera->distanceToTarget * glm::normalize(camera->forward) + 2.0f * crtPlayer->GetCenterOffset();

        crtPlayer->SetWeaponType(WeaponTypes::WEAPON_RIFLE);
    }
    if (key == GLFW_KEY_3)
    {
        if (crtPlayer->GetWeaponType() == WeaponTypes::WEAPON_RIFLE)
            camera->position -= camera->distanceToTarget * glm::normalize(camera->forward) + 2.0f * crtPlayer->GetCenterOffset();

        crtPlayer->SetWeaponType(WeaponTypes::WEAPON_GRENADE_LAUNCHER);
    }
}

void GameWorld::OnKeyRelease(int key, int mods)
{
}

void GameWorld::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        if (crtPlayer->GetWeaponType() == WeaponTypes::WEAPON_RIFLE)
        {
            camera->RotateFirstPerson_OX(-1 * deltaY * sensivityOY);
            camera->RotateFirstPerson_OY(-1 * deltaX * sensivityOX);
        }
        else
        {
            camera->RotateThirdPerson_OX(-1 * deltaY * sensivityOY);
            camera->RotateThirdPerson_OY(-1 * deltaX * sensivityOX);
        }

        if (!crtPlayer->IsDying())
        {
            crtPlayer->SetAngleOY(crtPlayer->GetAngleOY() + -1 * deltaX * sensivityOY);

            crtPlayer->SetOrientation(camera->forward);
        }
    }

    
}

void GameWorld::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void GameWorld::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void GameWorld::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void GameWorld::OnWindowResize(int width, int height)
{
}


