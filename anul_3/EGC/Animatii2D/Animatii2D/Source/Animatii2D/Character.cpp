#include "Character.h"
#include <iostream>

Character::Character(std::string meshName, std::string texName, std::string spritesheetName, char orientation)
{
    this->meshName = meshName;
    this->texName = texName;
    this->spritesheetName = spritesheetName;
    this->orientation = orientation;


    xmlResult = xmlDoc.load_file((textureLoc + spritesheetName).c_str());

    walkFrames = xmlDoc.child("animations").find_child_by_attribute("name", ANIMATION_WALK.c_str()).select_nodes("frame").size();
    punchFrames = xmlDoc.child("animations").find_child_by_attribute("name", ANIMATION_PUNCH.c_str()).select_nodes("frame").size();
    fallFrames = xmlDoc.child("animations").find_child_by_attribute("name", ANIMATION_FALL.c_str()).select_nodes("frame").size();
    blockFrames = xmlDoc.child("animations").find_child_by_attribute("name", ANIMATION_BLOCK.c_str()).select_nodes("frame").size();

    scale = glm::vec2(3, 3);

    vertices = 
    {
        glm::vec3(0.1f,   0.5f, 0.0f),
        glm::vec3(0.1f,  -0.5f, 0.0f),
        glm::vec3(-0.1f, -0.5f, 0.0f),
        glm::vec3(-0.1f,  0.5f, 0.0f),
    };

    normals =
    {
        glm::vec3(0, 1, 1),
        glm::vec3(1, 0, 1),
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
    };

    indices = 
    {
        0, 1, 3,
        1, 2, 3
    };

    UpdateTexCoords();
}

Character::~Character()
{
}

void Character::MoveLeft(float deltaTime)
{
    scale = glm::vec2(3, 3);
    position -= glm::vec2(1.2f, 0) * deltaTime * ((float)Engine::GetElapsedTime() - moveTime + 1);
    currentAnimation = ANIMATION_WALK;

    if (Engine::GetElapsedTime() - keyPressTime > 0.1f / ((float)Engine::GetElapsedTime() - moveTime + 1))
    {
        if (orientation == 1)
            currentFrame--;
        else
            currentFrame++;

        if (orientation == 1 && currentFrame < 0)
            currentFrame = walkFrames-1;

        if (orientation == -1 && currentFrame >= walkFrames)
            currentFrame = 0;

        UpdateTexCoords();
        keyPressTime = Engine::GetElapsedTime();
    }
}

void Character::MoveRight(float deltaTime)
{
    scale = glm::vec2(3, 3);
    position += glm::vec2(1.2f, 0) * deltaTime * ((float)Engine::GetElapsedTime() - moveTime + 1);
    currentAnimation = ANIMATION_WALK;

    if (Engine::GetElapsedTime() - keyPressTime > 0.1f / ((float)Engine::GetElapsedTime() - moveTime + 1))
    {
        if (orientation == 1)
            currentFrame++;
        else
            currentFrame--;

        if (orientation == 1 && currentFrame >= walkFrames)
            currentFrame = 0;

        if (orientation == -1 && currentFrame < 0)
            currentFrame = walkFrames - 1;

        UpdateTexCoords();
        keyPressTime = Engine::GetElapsedTime();
    }
}

void Character::SetKeyPressTime(float time)
{
    keyPressTime = time;
}

void Character::SetMoveTime(float time)
{
    moveTime = time;
}

void Character::SetOrientation(char orientation)
{
    this->orientation = orientation;
}

char Character::GetOrientation()
{
    return orientation;
}

bool Character::HasPunched()
{
    return _hasPunched;
}

void Character::ResetPunch()
{
    _hasPunched = false;
}

void Character::Update(float deltaTime)
{
    if (currentAnimation == ANIMATION_FALL && Engine::GetElapsedTime() - keyPressTime > 0.1f && currentFrame < fallFrames)
    {
        UpdateTexCoords();

        scale = glm::vec2(3, 3) + glm::vec2(xScale, 0);

        currentFrame++;
        keyPressTime = Engine::GetElapsedTime();
    }

    if (_blockedPunch && currentFrame < blockFrames-1)
    {
        currentFrame = 1;
        UpdateTexCoords();

        keyPressTime = Engine::GetElapsedTime();

        _blockedPunch = false;
    }

    if (_isBlocking && !_blockedPunch && Engine::GetElapsedTime() - keyPressTime > 0.2f)
        currentFrame = 0;
    
}

void Character::GoIdle()
{
    _isBlocking = false;

    currentAnimation = ANIMATION_IDLE;
    currentFrame = 0;
    scale = glm::vec2(3, 3);

    UpdateTexCoords();
}

void Character::Punch()
{
    _isBlocking = false;
    currentAnimation = ANIMATION_PUNCH;

    if (Engine::GetElapsedTime() - keyPressTime > 0.1f)
    {
        UpdateTexCoords();
        keyPressTime = Engine::GetElapsedTime();

        

        currentFrame++;

        if (currentFrame >= punchFrames)
        {
            _hasPunched = true;
            currentFrame = 0;
        }

        if (currentFrame == 0)
        {
            scale += glm::vec2(xScale, 0);
        }
        else
            scale = glm::vec2(3, 3);
    }
}

void Character::Block()
{
    if (!_isBlocking)
        currentFrame = 0;

    currentAnimation = ANIMATION_BLOCK;
    

    _isBlocking = true;

    scale = glm::vec2(3, 3);

    UpdateTexCoords();
}


void Character::ReceivePunch()
{
    if (!_isBlocking)
        Fall();
    else
    {
        _blockedPunch = true;
        
    }
}

void Character::Fall()
{
    _isBlocking = false;
    currentAnimation = ANIMATION_FALL;
    currentFrame = 0;
    
}

void Character::UpdateTexCoords()
{
    currentFrameNode = xmlDoc.child("animations").find_child_by_attribute("name", currentAnimation.c_str()).find_child_by_attribute("number", std::to_string(currentFrame).c_str());
    
    x0 = currentFrameNode.attribute("x0").as_float();
    x1 = currentFrameNode.attribute("x1").as_float();
    x2 = currentFrameNode.attribute("x2").as_float();
    x3 = currentFrameNode.attribute("x3").as_float();

    y0 = currentFrameNode.attribute("y0").as_float();
    y1 = currentFrameNode.attribute("y1").as_float();
    y2 = currentFrameNode.attribute("y2").as_float();
    y3 = currentFrameNode.attribute("y3").as_float();

    xScale = currentFrameNode.attribute("xscale").as_float(0);
    
    

    if (orientation == ORIENTATION_RIGHT)
    {
        texCoords =
        {
            glm::vec2(x0, y0),
            glm::vec2(x1, y1),
            glm::vec2(x2, y2),
            glm::vec2(x3, y3),
        };
    }
    

    if (orientation == ORIENTATION_LEFT)
    {
        texCoords =
        {
            glm::vec2(x3, y3),
            glm::vec2(x2, y2),
            glm::vec2(x1, y1),
            glm::vec2(x0, y0),   
        };
    }
}
