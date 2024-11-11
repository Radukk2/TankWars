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
        void PlaceTanks(float deltaTime);
        void EnemyHit();
        void HeroHit();
        void Hit();
        void HitFloor();
        void LandSlide(float deltaTimeSeconds);
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
        glm::mat3 modelMatrix, coordinates,  enemy_coordinates;
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
        float t_position_x, t_position_y;
        float t_enemy_position_x, t_enemy_position_y;
        bool p1_alive, p2_alive, launch_p1, launch_p2;
        int num_hits_1, num_hits_2;
        std::vector<glm::vec2> projectileSpeed, projectileCoordinates;
        std::vector<glm::vec2> e_projectileSpeed, e_projectileCoordinates;
        int projectiles, inAir;
        // TODO(student): If you need any other class variables, define them here.

    };
}   // namespace m1
