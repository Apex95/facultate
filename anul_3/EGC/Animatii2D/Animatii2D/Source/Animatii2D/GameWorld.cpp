#include "GameWorld.h"

#include <vector>
#include <string>
#include <iostream>
#include <list>
#include <Core/Engine.h>

#include "GameObject.h"
#include "Character.h"




using namespace std;


GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}


std::list<GameObject *> gameObjects;
const string textureLoc = "Resources/Textures/";

Character character1("character1", "fight.png", "Character1.xml", 1);
Character character2("character2", "fight.png", "Character2.xml", -1);

void GameWorld::Init()
{
    GetSceneCamera()->SetPosition(glm::vec3(0, 1, 2));
    GetSceneCamera()->SetOrthographic(4, 4, 0, 100);
    
    
    character1.SetPosition(glm::vec2(-1, 0));
    character2.SetPosition(glm::vec2(1, 0));

    gameObjects.push_back(&character1);
    gameObjects.push_back(&character2);
    

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("ShaderLab9");
        shader->AddShader("Source/Animatii2D/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
        shader->AddShader("Source/Animatii2D/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}

void GameWorld::FrameStart()
{
    // clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void GameWorld::Update(float deltaTimeSeconds)
{
    AdjustCharactersOrientation();


    for (std::list<GameObject*>::iterator i = gameObjects.begin(); i != gameObjects.end(); i++)
    {
        (*i)->Update(deltaTimeSeconds);

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3((*i)->GetPosition(), 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3((*i)->GetScale(), 1));

        // refac mesha de fiecare data...sansa sa se modifice textura
        Mesh* mesh = new Mesh((*i)->GetMeshName());
        mesh->InitFromData((*i)->GetVertices(), (*i)->GetNormals(), (*i)->GetTexCoords(), (*i)->GetIndices());
        meshes[mesh->GetMeshID()] = mesh;
        

        // si textura doar daca nu exista
        if (mapTextures[(*i)->GetTexName()] == NULL)
        {
            Texture2D* texture = new Texture2D();
            texture->Load2D((textureLoc + (*i)->GetTexName()).c_str(), GL_REPEAT);
            mapTextures[(*i)->GetTexName()] = texture;
        }

        RenderSimpleMesh(meshes[(*i)->GetMeshName()], shaders["ShaderLab9"], modelMatrix, mapTextures[(*i)->GetTexName()]);
    }
}

void GameWorld::FrameEnd()
{
}

void GameWorld::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));



    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    if (texture2)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);

    }


    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

bool character1PressedKey = false;
bool character2PressedKey = false;

bool character1Punched = false;
bool character2Punched = false;

void GameWorld::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 2;

    character1PressedKey = false;
    character2PressedKey = false;


    // -- character 1
    if (window->KeyHold(GLFW_KEY_A) && !character1PressedKey)
    {
        character1.MoveLeft(deltaTime);
        character1PressedKey = true;
    }

    if (window->KeyHold(GLFW_KEY_D) && !character1PressedKey)
    {
        character1.MoveRight(deltaTime);
        character1PressedKey = true;
    }

    if (window->KeyHold(GLFW_KEY_B) && !character1PressedKey)
    {
        character1.Block();
        character1PressedKey = true;
    }


    // -- character 2
    if (window->KeyHold(GLFW_KEY_LEFT) && !character2PressedKey)
    {
        character2.MoveLeft(deltaTime);
        character2PressedKey = true;
    }

    if (window->KeyHold(GLFW_KEY_RIGHT) && !character2PressedKey)
    {
        character2.MoveRight(deltaTime);
        character2PressedKey = true;
    }

    if (window->KeyHold(GLFW_KEY_O) && !character2PressedKey)
    {
        character2.Block();
        character2PressedKey = true;
    }

    if (window->KeyHold(GLFW_KEY_SPACE) && !character1PressedKey && !character1Punched)
    {
        character1Punched = character1.HasPunched();

        character1.Punch();
        character1PressedKey = true;

        if (character1Punched && CharactersCollide())
        {
            character2.ReceivePunch();
        }
    }

    if (window->KeyHold(GLFW_KEY_P) && !character2PressedKey && !character2Punched)
    {
        character2.Punch();
        character2PressedKey = true;

        character2Punched = character2.HasPunched();

        if (character2Punched && CharactersCollide())
        {
            character1.ReceivePunch();
        }
    }
}

void GameWorld::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_A || key == GLFW_KEY_D)
    {
        character1.SetMoveTime(Engine::GetElapsedTime());

        if (key == GLFW_KEY_A && character1.GetOrientation() == 1 && !autoOrientation)
            character1.SetOrientation(-1);

        if (key == GLFW_KEY_D && character1.GetOrientation() == -1 && !autoOrientation)
            character1.SetOrientation(1);
    }

    if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT)
    {
        character2.SetMoveTime(Engine::GetElapsedTime());

        if (key == GLFW_KEY_LEFT && character2.GetOrientation() == 1 && !autoOrientation)
            character2.SetOrientation(-1);

        if (key == GLFW_KEY_RIGHT && character2.GetOrientation() == -1 && !autoOrientation)
            character2.SetOrientation(1);
    }

    if (key == GLFW_KEY_Z)
    {
        autoOrientation = !autoOrientation;
        std::cout << "Auto-Orientation: " << autoOrientation << std::endl;
    }
}

void GameWorld::OnKeyRelease(int key, int mods)
{
    if (key == GLFW_KEY_A || key == GLFW_KEY_D || key == GLFW_KEY_SPACE || key == GLFW_KEY_B)
    {
        character1.GoIdle();

        if (key == GLFW_KEY_SPACE)
        {
            character1.ResetPunch();
            character1Punched = false;
        }
    }

    if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT || key == GLFW_KEY_P || key == GLFW_KEY_O)
    {
        character2.GoIdle();

        if (key == GLFW_KEY_P)
        {
            character2.ResetPunch();
            character2Punched = false;
        }
    }
}

void GameWorld::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // add mouse move event
}

void GameWorld::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // add mouse button press event
}

void GameWorld::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // add mouse button release event
}

void GameWorld::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void GameWorld::OnWindowResize(int width, int height)
{
}

void GameWorld::AdjustCharactersOrientation()
{
    if (!autoOrientation)
        return;

    if (character1.GetPosition().x + 0.1f > character2.GetPosition().x + 0.1f)
    {
        character1.SetOrientation(-1);
        character2.SetOrientation(1);

        return;
    }

    if (character1.GetPosition().x + 0.1f < character2.GetPosition().x + 0.1f)
    {
        character1.SetOrientation(1);
        character2.SetOrientation(-1);
    }
    
}

bool GameWorld::CharactersCollide()
{
    return abs(character1.GetPosition().x + 0.1f - (character2.GetPosition().x + 0.1f)) < 0.4f;
}
