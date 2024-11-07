#pragma once

#include "components/simple_scene.h"
#include <vector>


namespace m1
{
    class TankWars : public gfxc::SimpleScene
    {
    public:
        TankWars();
        ~TankWars();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;

        void FrameEnd() override;
        void CreateField();
        void PlaceTanks();
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;
        float go;
        float forfecareX, forfecareY;
        bool left;
        std::vector<float> peaks;
        float step;
        float tank_x;
        float tank_y;
        float enemy_x;
        float enemy_y;
        float elevation;
        float tank_angle, enemy_angle, t_angle, t_enemy_angle;
        float t_position, t_enemy_position;

        // TODO(student): If you need any other class variables, define them here.

    };
}   // namespace m1
