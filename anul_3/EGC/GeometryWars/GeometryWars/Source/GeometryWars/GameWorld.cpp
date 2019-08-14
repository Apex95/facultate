#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "GameWorld.h"
#include "Player.h"
#include "Projectile.h"
#include "SimpleEnemy.h"
#include "Shield.h"
#include "BonusItem.h"
#include "SpecialProjectile.h"

using namespace std;

#define DIST(x1, x2, y1, y2) (sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2)))

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}

list<GameObject*> *gameObjects = new list<GameObject*>;
list<GameObject*> *interfaceObjects = new list<GameObject*>;

Player * crtPlayer;
Shield * shield;

double enemiesTimer = 0;
double enemiesInterval = 2;
float red = 0.0f;

double bonusTimer = 0;
double bonusInterval = 3;

bool isGameEnding = false;
int nOfSpecialProjectiles = 0;

int score = 0;

// publice ca sa ma asigur ca jucatorul nu iese din zona de joc
float tx, ty;
float sx, sy, smin;

// creeaza interfata pt nr de vieti
void GameWorld::CreateInterface()
{
    interfaceObjects->clear();

    for (int i = 0; i < crtPlayer->GetLives(); i++)
    {
        Projectile* life = new Projectile(); // aceeasi mesha - ca sa nu mai fac inca o clasa
        life->SetX(logicSpace.width - i * 0.2f);
        life->SetY(logicSpace.height - 1.0f);

        interfaceObjects->push_back(life);

        CreateMesh(life);
    }
}

void GameWorld::ShowScore()
{
    cout << "Scor: " << ++score << endl;
}

void GameWorld::Init()
{
    // pozitionare camera
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    cameraInput->SetActive(false);
    
    logicSpace.x = 0;		
    logicSpace.y = 0;		
    logicSpace.width = 10;	
    logicSpace.height = 10;

    crtPlayer = new Player();
    crtPlayer->SetLives(3);

    
    gameObjects->push_back(crtPlayer);
    CreateMesh(crtPlayer);

    shield = new Shield();
    shield->BindTo(crtPlayer);
    crtPlayer->ShieldActive(false);

    CreateMesh(shield);

   

}

void GameWorld::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);
    
    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    //glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    camera->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    camera->Update();
}



void GameWorld::FrameStart()
{
    // sets the clear color for the color buffer
    glClearColor(red, 0, 0, 1);

    // clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GameWorld::Update(float deltaTimeSeconds)
{   
    CreateInterface();

    // daca se termina jocul, incep sa fac tranzitia
    if (red + 0.5f * deltaTimeSeconds < 1.0f && isGameEnding)
        red += 0.5f * deltaTimeSeconds;

    glm::ivec2 resolution = window->GetResolution();

    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    viewMatrix = GetViewMatrix();

#pragma region Spawnare_bonusuri

    if (abs(Engine::GetElapsedTime() - bonusTimer) > bonusInterval && !isGameEnding)
    {
        BonusItem *b = new BonusItem();
        b->SetX(rand()/(float)RAND_MAX * logicSpace.width);
        b->SetY(rand() / (float)RAND_MAX * logicSpace.height);

        gameObjects->push_back(b);

        int _rand = rand() % 3;

        if (_rand == 0)
            b->SetBonusType(BonusItem::BONUS_LIVES);
        else
            if (_rand == 1)
                b->SetBonusType(BonusItem::BONUS_SHIELD);
            else
                b->SetBonusType(BonusItem::BONUS_PROJECTILES);

        CreateMesh(b);

        

        bonusTimer = Engine::GetElapsedTime();
    }
#pragma endregion Spawnare_bonusuri


#pragma region Spawnare_inamici

    // spawning the bad guys
    if (abs(Engine::GetElapsedTime() - enemiesTimer) > enemiesInterval && !isGameEnding)
    {
        SimpleEnemy * e = new SimpleEnemy();

        int randomAngle = rand();

        e->SetX(logicSpace.width / 2.0f + logicSpace.width / 2.0f * cos(randomAngle * (float)(M_PI / 180)) * 1.5f);
        e->SetY(logicSpace.height / 2.0f + logicSpace.height / 2.0f * sin(randomAngle * (float)(M_PI / 180)) * 1.5f);

        e->SetSpeed((float)(rand() % 3 + 2));

        // sansa sa fie inamic galben (2 vieti)
        if (rand() % 5 + 1 ==  3)
            e->Enhance();

        gameObjects->push_back(e);
        CreateMesh(e);
        
        enemiesTimer = Engine::GetElapsedTime();

        if (enemiesInterval > 0.5)
            enemiesInterval -= 0.1;
    }
#pragma endregion Spawnare_inamici


    int _skip = 0;
    int _lives = 0;
    BonusItem * _bonusItem = NULL;
    Projectile * _projectile = NULL;

#pragma region Rendering_Obiecte
    for (list<GameObject*>::iterator i = gameObjects->begin(); i != gameObjects->end(); i++)
    {
        _skip = 0;

        if (!isGameEnding)
            (*i)->Update(deltaTimeSeconds);

#pragma region Coliziuni_Proiectil-Inamic
        
        // daca e proiectil, avansez si verific coliziunea cu o nava inamica
        if (dynamic_cast<Projectile*>(*i) != NULL)
        {
            _projectile = (Projectile*)(*i);

            if (abs(Engine::GetElapsedTime() - _projectile->GetSpawnTime() > _projectile->GetTimeToLive()))
            {
                i = gameObjects->erase(i);
                i--;

                continue;
            }

            for (list<GameObject*>::iterator j = gameObjects->begin(); j != gameObjects->end(); j++)
            {
                if (i == j || !dynamic_cast<SimpleEnemy*>(*j))
                    continue;
                
                // colizune proiectil-inamic
                if (DIST((*i)->GetX(), (*j)->GetX(), (*i)->GetY(), (*j)->GetY()) < (*i)->GetCollisionRadius() + (*j)->GetCollisionRadius())
                {
                    _lives = (*j)->GetLives();

                    if (_lives - 1 > 0)
                        (*j)->SetLives(_lives - 1);
                    else
                    {
                        ShowScore();
                        j = gameObjects->erase(j);
                        j--;
                    }

                    _lives = (*i)->GetLives();

                    if (_lives - 1 > 0)
                        (*i)->SetLives(_lives - 1);
                    else
                    {
                        // preiau urmatorul element si revin la cel anterior
                        // astfel incat la incrementarea de la for sa ajunga la pozitia corecta
                        i = gameObjects->erase(i);
                        i--;

                        _skip = 1;

                        break;
                    }
                }
            }
        }

#pragma endregion Coliziuni_Proiectil-Inamic

        if (_skip) continue;

#pragma region Coliziuni_Player
        // daca e bonus item
        if (dynamic_cast<BonusItem*>(*i) != NULL)
        {
            if (isGameEnding)
                (*i)->SetSpeed(0);

            _bonusItem = (BonusItem*)(*i);

            if (DIST((*i)->GetX(), crtPlayer->GetX(), (*i)->GetY(), crtPlayer->GetY()) < (*i)->GetCollisionRadius() + crtPlayer->GetCollisionRadius())
            {
                // -- aplicare bonusuri --

                if (_bonusItem->GetBonusType() == BonusItem::BONUS_SHIELD)
                {
                    // aici activare scut
                    crtPlayer->ShieldActive(true);
                    gameObjects->push_back(shield);

                    i = gameObjects->erase(i);
                    i--;
                    
                    continue;
                }

                if (_bonusItem->GetBonusType() == BonusItem::BONUS_LIVES)
                {
                    crtPlayer->SetLives(crtPlayer->GetLives() + 1);
                    i = gameObjects->erase(i);
                    i--;

                    continue;
                }

                if (_bonusItem->GetBonusType() == BonusItem::BONUS_PROJECTILES)
                {
                    nOfSpecialProjectiles = 4;
                    i = gameObjects->erase(i);
                    i--;

                    continue;
                }

            }
        }


        // daca e inamic simplu verific coliziunea cu jucatorul
        if (dynamic_cast<SimpleEnemy*>(*i) != NULL)
        {
            if (isGameEnding)
                (*i)->SetSpeed(0);

            // daca e inamic adaug sansa sa "evolueze" la versiunea cu 2 vieti
            if (rand() % 50 + 1 == 1 && Engine::GetElapsedTime() - enemiesTimer > enemiesInterval /1.1 && !((SimpleEnemy*)(*i))->IsWeakened() && !isGameEnding)
                ((SimpleEnemy*)(*i))->Enhance();

            ((SimpleEnemy*)(*i))->AdjustAngle(crtPlayer->GetX(), crtPlayer->GetY());

            // intai verific daca jucatorul are scut
            if (crtPlayer->HasActiveShield())
            {
                // fac coliziunea cu scutul
                if (DIST((*i)->GetX(), crtPlayer->GetX(), (*i)->GetY(), crtPlayer->GetY()) < (*i)->GetCollisionRadius() + shield->GetCollisionRadius())
                {
                    i = gameObjects->erase(i);
                    i--;
                    
                    ShowScore(); 

                    gameObjects->remove(shield);
                    i = gameObjects->begin();

                    crtPlayer->ShieldActive(false);

                    continue;
                }

            }
            

            // coliziune inamic-jucator
            if (DIST((*i)->GetX(), crtPlayer->GetX(), (*i)->GetY(), crtPlayer->GetY()) < (*i)->GetCollisionRadius() + crtPlayer->GetCollisionRadius())
            {
                i = gameObjects->erase(i);
                i--;

                ShowScore();

                crtPlayer->SetLives(crtPlayer->GetLives() - 1);

                if (crtPlayer->GetLives() == 0)
                    isGameEnding = true;

                continue;
            }
        }

        if (_skip) continue;
#pragma endregion Coliziuni_Player
        
        
        modelMatrix = viewMatrix * Transform2D::Translate((*i)->GetX(), (*i)->GetY());
        modelMatrix *= Transform2D::Rotate((*i)->GetAngle());
        modelMatrix *= Transform2D::Scale((*i)->GetScaleX(), (*i)->GetScaleY());

        RenderMesh2D(meshes[(*i)->GetName()], modelMatrix, (*i)->GetColor());
    }
#pragma endregion Rendering_Obiecte

#pragma region Desenare_interfata
    for (list<GameObject*>::iterator i = interfaceObjects->begin(); i != interfaceObjects->end(); i++)
    {
        modelMatrix = viewMatrix * Transform2D::Translate((*i)->GetX(), (*i)->GetY());
        modelMatrix *= Transform2D::Rotate((*i)->GetAngle());
        modelMatrix *= Transform2D::Scale((*i)->GetScaleX(), (*i)->GetScaleY());

        RenderMesh2D(meshes[(*i)->GetName()], modelMatrix, (*i)->GetColor());
    }
#pragma endregion Desenare_interfata
}

void GameWorld::FrameEnd()
{
}

void GameWorld::OnInputUpdate(float deltaTime, int mods)
{
    if (isGameEnding)
        return;

    if (window->KeyHold(GLFW_KEY_W) && smin * (crtPlayer->GetY() + deltaTime * crtPlayer->GetSpeed()) + ty < viewSpace.height)
        crtPlayer->SetY(crtPlayer->GetY() + deltaTime * crtPlayer->GetSpeed());

    if (window->KeyHold(GLFW_KEY_S) && smin * (crtPlayer->GetY() - deltaTime * crtPlayer->GetSpeed()) + ty > 0)
        crtPlayer->SetY(crtPlayer->GetY() - deltaTime * crtPlayer->GetSpeed());

    if (window->KeyHold(GLFW_KEY_A) && smin * (crtPlayer->GetX() - deltaTime * crtPlayer->GetSpeed()) + tx > 0)
        crtPlayer->SetX(crtPlayer->GetX() - deltaTime * crtPlayer->GetSpeed());


    if (window->KeyHold(GLFW_KEY_D) && smin * (crtPlayer->GetX() + deltaTime * crtPlayer->GetSpeed()) + tx < viewSpace.width)
        crtPlayer->SetX(crtPlayer->GetX() + deltaTime * crtPlayer->GetSpeed());
}


void GameWorld::OnKeyPress(int key, int mods)
{   
}

void GameWorld::OnKeyRelease(int key, int mods)
{
}

void GameWorld::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (isGameEnding)
        return;

    float sx, sy, smin;

    glm::vec3 mouseVector = glm::vec3(mouseX, -mouseY, 1);
    mouseVector = Transform2D::Translate(0, (float)viewSpace.height) * mouseVector;

    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;

    smin = sx > sy ? sy : sx;
    mouseVector = Transform2D::Scale(1/smin, 1/smin) * mouseVector;
    
    // obtin un spatiu logic ce sigur are una din laturi egala cu 
    // o latura a spatiului vizual si incadrat in partea stanga
    // => trebuie centrat cu cateva translatii
    float tx = viewSpace.x / smin - logicSpace.x + (viewSpace.width / smin - logicSpace.width) / 2;
    float ty = viewSpace.y / smin - logicSpace.y + (viewSpace.height / smin - logicSpace.height) / 2;


    mouseVector = Transform2D::Translate(-tx, -ty) * mouseVector;


    float mouseX2 = mouseVector.x;
    float mouseY2 = mouseVector.y;


    float angle = atan2(mouseY2 - crtPlayer->GetY(), mouseX2 - crtPlayer->GetX()) - (float)(M_PI / 2);
    crtPlayer->SetAngle(angle);

}

void GameWorld::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (isGameEnding)
        return;

    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT))
    {
        Projectile * p;

        if (nOfSpecialProjectiles > 0)
        {
            p = new SpecialProjectile();
            nOfSpecialProjectiles--;
        }
        else
            p = new Projectile();

        p->SetSpawnTime((float)(Engine::GetElapsedTime()));

        gameObjects->push_back(p);

        p->SetX(crtPlayer->GetX());
        p->SetY(crtPlayer->GetY());
        p->SetAngle(crtPlayer->GetAngle());

        CreateMesh(p);
    }
}

void GameWorld::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void GameWorld::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void GameWorld::CreateMesh(GameObject * object)
{
     // VAO
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // IBO
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);


    // trimitere date catre VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(object->GetVertices()[0]) * (object->GetVertices().size()), &(object->GetVertices()[0]), GL_STATIC_DRAW);

    // si IBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(object->GetIndices()[0]) * (object->GetIndices().size()), &(object->GetIndices()[0]), GL_STATIC_DRAW);
    
    // set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

    meshes[object->GetName()] = new Mesh(object->GetName());
    meshes[object->GetName()]->SetDrawMode(object->GetDrawPrimitive());
    meshes[object->GetName()]->InitFromBuffer(VAO, static_cast<unsigned short>(object->GetIndices().size()));

    
}


glm::mat3 GameWorld::GetViewMatrix()
{
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;

    if (sx < sy)
        smin = sx;
    else
        smin = sy;

    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;


    return glm::transpose(glm::mat3(smin, 0.0f, tx,
                                    0.0f, smin, ty,
                                    0.0f, 0.0f, 1.0f));
}


