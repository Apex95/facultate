#pragma once

#include "GameObject.h"
#include "pugixml.hpp"

class Character : public GameObject
{
public:
    Character(std::string meshName = "mesh", std::string texName = "tex", std::string spritesheetName = "spritesheet", char orientation = 0);
    ~Character();

    void MoveLeft(float deltaTime);
    void MoveRight(float deltaTime);
    void GoIdle();
    void Punch();
    void Block();

    void ReceivePunch();
    void Fall();

    void SetKeyPressTime(float time);
    void SetMoveTime(float time);

    void SetOrientation(char orientation);
    char GetOrientation();
    
    bool HasPunched();
    void ResetPunch();

    void Update(float deltaTime) override;

protected:
private:
    std::string spritesheetName;
    pugi::xml_document xmlDoc;
    pugi::xml_parse_result xmlResult;

    const std::string textureLoc = "Resources/Textures/";

    // -- orientare
    char orientation;
    const char ORIENTATION_LEFT = -1;
    const char ORIENTATION_RIGHT = 1;

    // -- constante animatii
    const std::string ANIMATION_IDLE = "idle";
    const std::string ANIMATION_WALK = "walk";
    const std::string ANIMATION_PUNCH = "punch";
    const std::string ANIMATION_BLOCK = "block";
    const std::string ANIMATION_FALL = "fall";

    std::string currentAnimation = ANIMATION_IDLE;
    int currentFrame = 0;
    pugi::xml_node currentFrameNode;

    // -- numar de cadre
    int walkFrames = 0;
    int punchFrames = 0;
    int fallFrames = 0;
    int blockFrames = 0;

    bool _hasPunched;
    bool _isBlocking;
    bool _blockedPunch;

    float x0, x1, x2, x3, y0, y1, y2, y3;
    float xScale = 0;

    float keyPressTime, moveTime;

    void UpdateTexCoords();
};