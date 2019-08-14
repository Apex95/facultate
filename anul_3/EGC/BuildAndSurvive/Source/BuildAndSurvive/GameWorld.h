#pragma once
#include <Component/SimpleScene.h>
#include "Camera.h"
#include "Entity.h"

class GameWorld : public SimpleScene
{
public:
    GameWorld();
    ~GameWorld();

    void Init() override;

    struct ViewportSpace
    {
        ViewportSpace() : x(0), y(0), width(1), height(1) {}
        ViewportSpace(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {}
        int x;
        int y;
        int width;
        int height;
    };

private:
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;

    void RenderMesh(Entity * entity, Shader * shader, const glm::mat4 & modelMatrix);

    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
    void OnWindowResize(int width, int height) override;

    void CreateWaypoints();

    ViewportSpace viewSpace;

protected:
    Camera *camera;
    glm::mat4 projectionMatrix;

    std::string meshPath = RESOURCE_PATH::MODELS + "BuildAndSurvive";
};
