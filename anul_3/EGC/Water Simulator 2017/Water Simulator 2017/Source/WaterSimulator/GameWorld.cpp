#include "GameWorld.h"
#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>



GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}




void GameWorld::Init()
{
    // shader pt lumini
    shader = new Shader("Shader");
    shader->AddShader("Source/WaterSimulator/Shaders/VS.glsl", GL_VERTEX_SHADER);
    shader->AddShader("Source/WaterSimulator/Shaders/FS.glsl", GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;


    camera = new Camera();
    camera->Set(glm::vec3(0, 1, 3.5f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    CreateWaterMesh(water);
    
    entities->push_back(water);
    
    waterSphere->setMesh(new Mesh("WaterSphere"));
    waterSphere->getMesh()->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
    entities->push_back(waterSphere);

    

    
}


void GameWorld::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    resolution = window->GetResolution();

}

void GameWorld::Update(float deltaTimeSeconds)
{
    deltaTimeSeconds *= speedCoefficient;
    glm::mat4 modelMatrix;
    
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);


    for (std::list<WaterDroplet*>::iterator i = droplets->begin(); i != droplets->end();)
    {
        if ((*i)->getPosition().y <= 0 && (*i)->getWavesList()->size() < (*i)->getMaxNofBounces() && (*i)->getNofBounces() < (*i)->getMaxNofBounces())
        {
            wave = new Wave();
            wave->setSpawnTime(Engine::GetElapsedTime());
            wave->setPosition((*i)->getPosition());

            wave->setSpeedCoefficient(&speedCoefficient);

            (*i)->getWavesList()->push_back(wave);

            (*i)->setPosition((*i)->getPosition() + glm::vec3(0, 0.5f, 0));
        }

        if ((*i)->canDespawn())
        {
            i = droplets->erase(i);
            std::cout << "Despawned droplet" << std::endl;
        }
        else
            i++;
    }


    for (std::list<Entity*>::iterator i = entities->begin(); i != entities->end(); )
    {
        (*i)->applyGravity(deltaTimeSeconds);


        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, (*i)->getPosition());
        modelMatrix = glm::scale(modelMatrix, (*i)->getScale());
        RenderMesh((*i), shaders["Shader"], modelMatrix);

        if ((*i)->canDespawn())
            i = entities->erase(i);
        else
            i++;
    }

}

void GameWorld::FrameEnd()
{
}

#pragma region MESH_UTILS

void GameWorld::CreateWaterMesh(Water * water)
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(water->GetVertices()[0]) * (water->GetVertices().size()), &(water->GetVertices()[0]), GL_STATIC_DRAW);

    // si IBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(water->GetIndices()[0]) * (water->GetIndices().size()), &(water->GetIndices()[0]), GL_STATIC_DRAW);

    // set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));


    Mesh * mesh = new Mesh("water");
    mesh->SetDrawMode(GL_TRIANGLES);

    mesh->InitFromBuffer(VAO, static_cast<unsigned int>(water->GetIndices().size()));

    water->setMesh(mesh);

}



void GameWorld::RenderMesh(Entity * entity, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!entity || !shader)
        return;
    
    glUseProgram(shader->program);

    // -- projection / view / model uniforms
    glUniformMatrix4fv(shader->loc_view_matrix, 1, false, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, false, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // -- lightning uniforms
    int lightPositionLocation = glGetUniformLocation(shader->program, "light_position");
    glUniform3fv(lightPositionLocation, 1, glm::value_ptr(waterSphere->getPosition()));

    glm::vec3 eyePosition = camera->position;
    int eyePositionLocation = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(eyePositionLocation, 1, glm::value_ptr(eyePosition));

    int shininessLocation = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(shininessLocation, entity->getShininess());
    
    int kdLocation = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(kdLocation, entity->getKd());

    int ksLocation = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(ksLocation, entity->getKs());

    int colorLocation = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(colorLocation, 1, glm::value_ptr(entity->getColor()));


    int isRenderingWater = 0;

    


    if (dynamic_cast<Water*>(entity) != NULL)
    {        
        shader->Use();
        entity->getMesh()->Render();

        isRenderingWater = 1;

        int wLocation = glGetUniformLocation(shader->program, "w");
        glUniform1f(wLocation, water->FREQUENCY);

        int phiLocation = glGetUniformLocation(shader->program, "phi");
        glUniform1f(phiLocation, water->PHASE);

        int isRenderingWaterLocation = glGetUniformLocation(shader->program, "isRenderingWater");
        glUniform1i(isRenderingWaterLocation, isRenderingWater);


        int ALocation = glGetUniformLocation(shader->program, "A");

        for (std::list<WaterDroplet*>::iterator j = droplets->begin(); j != droplets->end(); j++)
        {
            if ((*j)->getWavesList()->size() > 0)
                glUniform1f(ALocation, water->AMPLITUDE);
            else
                glUniform1f(ALocation, 0);


            for (std::list<Wave*>::iterator i = (*j)->getWavesList()->begin(); i != (*j)->getWavesList()->end(); )
            {
                
                int CLocation = glGetUniformLocation(shader->program, "C");
                glUniform2fv(CLocation, 1, glm::value_ptr(glm::vec2((*i)->getPosition().x, (*i)->getPosition().z)));

                int tLocation = glGetUniformLocation(shader->program, "t");


                if (switchSpeed)
                {
                    // adaug offset de timp format din toti timpii inmultiti cu coeficientii lor
                    if ((*i)->getTimeBorder() == 0)
                        (*i)->setTimeBorder((crtTime - (*i)->getSpawnTime()) * lastSpeedCoefficient);
                    else
                        (*i)->setTimeBorder((*i)->getTimeBorder() + (crtTime - lastCrtTime) * lastSpeedCoefficient);
                }
                
                
                glUniform1f(tLocation, (Engine::GetElapsedTime() - MAX((*i)->getSpawnTime(), crtTime)) * speedCoefficient + (*i)->getTimeBorder());

                
                shader->Use();
                entity->getMesh()->Render();

                if ((*i)->canDespawn())
                {
                    i = (*j)->getWavesList()->erase(i);
                    std::cout << "Despawned wave" << std::endl;
                }
                else
                    i++;
            }
        }
        switchSpeed = false;
        
    }

    

    int isRenderingWaterLocation = glGetUniformLocation(shader->program, "isRenderingWater");
    glUniform1i(isRenderingWaterLocation, isRenderingWater);


    // render an object using the specified shader and the specified position
    shader->Use();
    entity->getMesh()->Render();
}


#pragma endregion MESH_UTILS

void GameWorld::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 3.0f;

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        if (window->KeyHold(GLFW_KEY_W))
            camera->MoveForward(cameraSpeed * deltaTime);

        if (window->KeyHold(GLFW_KEY_A))
            camera->TranslateRight(-cameraSpeed * deltaTime);

        if (window->KeyHold(GLFW_KEY_S))
            camera->MoveForward(-cameraSpeed * deltaTime);

        if (window->KeyHold(GLFW_KEY_D))
            camera->TranslateRight(cameraSpeed * deltaTime);

        if (window->KeyHold(GLFW_KEY_Q))
            camera->position -= glm::vec3(0, cameraSpeed, 0) * deltaTime;
        //camera->TranslateUpword(-cameraSpeed * deltaTime);

        if (window->KeyHold(GLFW_KEY_E))
            camera->position += glm::vec3(0, cameraSpeed, 0) * deltaTime;
        //camera->TranslateUpword(cameraSpeed * deltaTime);
    }
    else
    {
        if (window->KeyHold(GLFW_KEY_W))
            waterSphere->setPosition(waterSphere->getPosition() + glm::vec3(0, 0, -cameraSpeed * deltaTime));

        if (window->KeyHold(GLFW_KEY_A))
            waterSphere->setPosition(waterSphere->getPosition() + glm::vec3(-cameraSpeed * deltaTime, 0, 0));

        if (window->KeyHold(GLFW_KEY_S))
            waterSphere->setPosition(waterSphere->getPosition() + glm::vec3(0, 0, cameraSpeed * deltaTime));

        if (window->KeyHold(GLFW_KEY_D))
            waterSphere->setPosition(waterSphere->getPosition() + glm::vec3(cameraSpeed * deltaTime, 0, 0));

        if (window->KeyHold(GLFW_KEY_Q))
            waterSphere->setPosition(waterSphere->getPosition() + glm::vec3(0, -cameraSpeed * deltaTime, 0));

        if (window->KeyHold(GLFW_KEY_E))
            waterSphere->setPosition(waterSphere->getPosition() + glm::vec3(0, cameraSpeed * deltaTime, 0));

        
    }
    
}

void GameWorld::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE)
    {
        waterDroplet = new WaterDroplet();

        waterDroplet->setMesh(new Mesh("WaterDroplet"));

        waterDroplet->getMesh()->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
        waterDroplet->setScale(glm::vec3(0.5, 0.5, 0.5));
        waterDroplet->setMaxNofBounces(5);
        waterDroplet->setSpeedCoefficient(&speedCoefficient);

        waterDroplet->setSpawnTime(Engine::GetElapsedTime());
        waterDroplet->setPosition(waterSphere->getPosition() + glm::vec3(0, -0.2, 0));
        
        entities->push_back(waterDroplet);
        droplets->push_back(waterDroplet);
    }

    if (key == GLFW_KEY_LEFT)
    {

        lastCrtTime = crtTime;
        lastSpeedCoefficient = speedCoefficient;

        speedCoefficient = originalSpeedCoefficient / 4;
        crtTime = Engine::GetElapsedTime();

        switchSpeed = true;
    }

    if (key == GLFW_KEY_RIGHT)
    {

        lastCrtTime = crtTime;
        lastSpeedCoefficient = speedCoefficient;

        speedCoefficient = originalSpeedCoefficient * 4;
        crtTime = Engine::GetElapsedTime();

        switchSpeed = true;
    }

    if (key == GLFW_KEY_DOWN)
    {

        lastCrtTime = crtTime;
        lastSpeedCoefficient = speedCoefficient;

        speedCoefficient = originalSpeedCoefficient;
        crtTime = Engine::GetElapsedTime();

        switchSpeed = true;
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
        camera->RotateFirstPerson_OX(-1 * deltaY * sensivityOY);
        camera->RotateFirstPerson_OY(-1 * deltaX * sensivityOX);
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


