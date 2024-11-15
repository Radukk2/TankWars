#include "lab_m1/TankWars/tankWars.h"

#include <vector>
#include <iostream>

#include "lab_m1/TankWars/hw-transform2D.h"
#include "lab_m1/TankWars/hw-object2D.h"

using namespace std;
using namespace m1;

#define turret_length 30
#define magnitude 70
#define g 9.8
#define impact 50
#define threshold 2
#define epsilon  1



/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


TankWars::TankWars()
{
}


TankWars::~TankWars()
{
}

float sinusoidal_foo(float x)
{
    return 10 + 36 * sin(x / 115) + 7 * sin(x / 20) + 40 * sin(x / 250) + + 20 * sin(x / 40);
}

void TankWars::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    glClearColor(0.0f, 0.8f, 1.0, 0);
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    elevation = 250;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    forfecareX = 0;
    forfecareY = 0;


    // Initialize angularStep
    angularStep = 0;


    projectiles = 0;
    step = 1;


    num_hits_1 = 0;
    num_hits_2 = 0;

    launch_p1 = false;
    launch_p2 = false;

    //init terrain heights
    for (float i = 0; i < resolution.x; i++) {
        peaks.push_back(sinusoidal_foo(i));
    }
    //initial position my tank
    float start_x = 100;
    float start_x_next = 101;
    tank_x = start_x;
    tank_y = peaks[start_x] + (tank_x - start_x) * (peaks[start_x_next] - peaks[start_x]);
    float V_x = start_x_next - start_x;
    float V_y = peaks[start_x_next] - peaks[start_x];
    tank_angle = atan2(V_y, V_x);
    t_angle = 0;
    trajectory_1 = true;
    p1_alive = true;

    //initial position enemy tank
    start_x = 1000;
    start_x_next = 1001;
    enemy_x = start_x;
    enemy_y = peaks[start_x] + (enemy_x - start_x) * (peaks[start_x_next] - peaks[start_x]);
    V_x = start_x_next - start_x;
    V_y = peaks[start_x_next] - peaks[start_x];
    enemy_angle = atan2(V_y, V_x);
    t_enemy_angle = M_PI;
    trajectory_2 = true;
    p2_alive = true;

    //all meshes
    Mesh* square1 = hw_object2D::CreateSquare("square1", corner, 1, glm::vec3(0.4, 0.4, 0.5), true);
    AddMeshToList(square1);
    Mesh* tank = hw_object2D::CreateTank("tank", glm::vec3(0.5, 1, 0.1), true);
    AddMeshToList(tank);
    Mesh* enemy = hw_object2D::CreateTank("enemy", glm::vec3(1, 0, 0.1), true);
    AddMeshToList(enemy);
    Mesh* myTurret = hw_object2D::CreateTurret("turret", glm::vec3(0, 0, 0), true);
    AddMeshToList(myTurret);
    Mesh* lifeBar = hw_object2D::CreateLifeBar("lifeBar", glm::vec3(1, 1, 1), true);
    AddMeshToList(lifeBar);
    Mesh* life = hw_object2D::CreateLife("life", glm::vec3(1, 0, 0.1), true);
    AddMeshToList(life);
    Mesh* projectile = hw_object2D::CreateProjectile("projectile", glm::vec3(0, 0, 0), true);
    AddMeshToList(projectile);
    Mesh* trajectoryUnit = hw_object2D::CreateTrajectory("trajectory", glm::vec3(1, 1, 1), true);
    AddMeshToList(trajectoryUnit);
    Mesh* trajectoryUnitLine = hw_object2D::CreateTrajectoryLine("trajectoryLine", glm::vec3(1, 1, 1), true);
    AddMeshToList(trajectoryUnitLine);
   
}


void TankWars::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    //glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

}

void TankWars::CreateField() {
    for (int i = 0; i < peaks.size() - 1; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(i * step, peaks[i] + elevation);
        modelMatrix *= transform2D::Forfecare(step, peaks[i + 1] - peaks[i]);
        modelMatrix *= transform2D::Scale(step, 500);
        modelMatrix *= transform2D::Translate(0, -1);
        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    }
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate((peaks.size() - 2) * step, peaks[(peaks.size() - 2)] + elevation);
    modelMatrix *= transform2D::Forfecare(step, peaks[(peaks.size() - 1)] - peaks[(peaks.size() - 2)]);
    modelMatrix *= transform2D::Scale(step, 500);
    modelMatrix *= transform2D::Translate(0, -1);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
}

void TankWars::PlaceHero(float deltaTime, float speed)
{
    if (p1_alive) {
        //tank and turret
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(tank_x, tank_y + elevation);
        modelMatrix *= transform2D::Rotate(tank_angle);
        RenderMesh2D(meshes["tank"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D::Translate(0, 25);
        modelMatrix *= transform2D::Rotate(t_angle - tank_angle);
        RenderMesh2D(meshes["turret"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D::Translate(25, 0);
        coordinates = modelMatrix;
        //trajectory
        float coordX = coordinates[2][0];
        float coordY = coordinates[2][1];
        float speedX = cos(t_angle) * magnitude;
        float speedY = sin(t_angle) * magnitude;
        float timeStep = 0.2f;
        for (int i = 0; i < 200; ++i) {
            if (i > 0) {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(coordX, coordY);
                if (trajectory_1) {
                    modelMatrix *= transform2D::Rotate(atan2(speedY, speedX) + M_PI / 2);
                    RenderMesh2D(meshes["trajectoryLine"], shaders["VertexColor"], modelMatrix);
                }
                else {
                    RenderMesh2D(meshes["trajectory"], shaders["VertexColor"], modelMatrix);
                }
            }
            coordX += speedX * timeStep;
            coordY += speedY * timeStep;
            speedY -= g * timeStep;
        }
    }
    //all of my projectiles
    for (size_t i = 0; i < projectileCoordinates.size(); ++i) {
        projectileCoordinates[i] += projectileSpeed[i] * deltaTime * speed;
        projectileSpeed[i].y -= g * deltaTime * speed;
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(projectileCoordinates[i].x, projectileCoordinates[i].y);
        modelMatrix *= transform2D::Rotate(t_angle);
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);
        if (projectileCoordinates[i].y < 0 || projectileCoordinates[i].x > window->GetResolution().x) {
            projectileCoordinates.erase(projectileCoordinates.begin() + i);
            projectileSpeed.erase(projectileSpeed.begin() + i);
            --i;
        }
    }
    //life and lifeBar
    if (p1_alive) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(tank_x, tank_y + elevation + 50);
        RenderMesh2D(meshes["lifeBar"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D::Translate(-25, 0);
        modelMatrix *= transform2D::Scale(1 - 0.1 * num_hits_1, 1);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
    }
}

void TankWars::PlaceEnemy(float deltaTime, float speed)
{
    if (p2_alive) {
        //tank and turret
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(enemy_x, enemy_y + elevation);
        modelMatrix *= transform2D::Rotate(enemy_angle);
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D::Translate(0, 25);
        modelMatrix *= transform2D::Rotate(t_enemy_angle - enemy_angle);
        RenderMesh2D(meshes["turret"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D::Translate(25, 0);
        enemy_coordinates = modelMatrix;
        //trajectory
        float coordX = enemy_coordinates[2][0];
        float coordY = enemy_coordinates[2][1];
        float speedX = cos(t_enemy_angle) * magnitude;
        float speedY = sin(t_enemy_angle) * magnitude;
        float timeStep = 0.2f;
        for (int i = 0; i < 200; ++i) {
            if (i > 0) {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(coordX, coordY);
                if (trajectory_2) {
                    modelMatrix *= transform2D::Rotate(atan2(speedY, speedX) + M_PI / 2);
                    RenderMesh2D(meshes["trajectoryLine"], shaders["VertexColor"], modelMatrix);
                }
                else {
                    RenderMesh2D(meshes["trajectory"], shaders["VertexColor"], modelMatrix);
                }
            }
            coordX += speedX * timeStep;
            coordY += speedY * timeStep;
            speedY -= g * timeStep;
        }
    }
    //all enemy projectiles
    for (size_t i = 0; i < e_projectileCoordinates.size(); ++i) {
        e_projectileCoordinates[i] += e_projectileSpeed[i] * deltaTime * speed;
        e_projectileSpeed[i].y -= g * deltaTime * speed;
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(e_projectileCoordinates[i].x, e_projectileCoordinates[i].y);
        modelMatrix *= transform2D::Rotate(t_enemy_angle);
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);
        if (e_projectileCoordinates[i].y < 0 || e_projectileCoordinates[i].x > window->GetResolution().x || e_projectileCoordinates[i].x < 0) {
            e_projectileCoordinates.erase(e_projectileCoordinates.begin() + i);
            e_projectileSpeed.erase(e_projectileSpeed.begin() + i);
            --i;
        }
    }
    // life and lifebar
    if (p2_alive) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(enemy_x, enemy_y + elevation + 50);
        RenderMesh2D(meshes["lifeBar"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D::Translate(-25, 0);
        modelMatrix *= transform2D::Scale(1 - 0.1 * num_hits_2, 1);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
    }
}

void TankWars::PlaceTanks(float deltaTime) {
    float speed = 6;
    //place my tank
    PlaceHero(deltaTime, speed);
    //place enemy tank
    PlaceEnemy(deltaTime, speed);
}

void TankWars::EnemyHit() 
{
    num_hits_2++;
    if (num_hits_2 == 10)
        p2_alive = false;
}

void TankWars::HeroHit()
{
    num_hits_1++;
    if (num_hits_1 == 10)
        p1_alive = false;
}

void TankWars::Hit() {
    //my projectile
    for (int i = 0; i < projectileCoordinates.size(); i++) {
        float dx = projectileCoordinates[i].x - enemy_x;
        float dy = projectileCoordinates[i].y - enemy_y - elevation;
        float dist = sqrt(dx * dx + dy * dy);
        if (dist < 30 && p2_alive) {
            EnemyHit();
            projectileCoordinates.erase(projectileCoordinates.begin() + i);
            projectileSpeed.erase(projectileSpeed.begin() + i);
            --i;
        }
    }

    for (int i = 0; i < projectileCoordinates.size(); i++) {
        float dx_self = projectileCoordinates[i].x - tank_x;
        float dy_self = projectileCoordinates[i].y - tank_y - elevation;
        float dist_self = sqrt(dx_self * dx_self + dy_self * dy_self);
        if (dist_self < 30 && p1_alive) {
            HeroHit();
            projectileCoordinates.erase(projectileCoordinates.begin() + i);
            projectileSpeed.erase(projectileSpeed.begin() + i);
            --i;
        }
    }

    //enemy projectile
    for (int i = 0; i < e_projectileCoordinates.size(); i++) {
        float dx_self = e_projectileCoordinates[i].x - tank_x;
        float dy_self = e_projectileCoordinates[i].y - tank_y - elevation;
        float dist_self = sqrt(dx_self * dx_self + dy_self * dy_self);
        if (dist_self < 30 && p1_alive) {
            HeroHit();
            e_projectileCoordinates.erase(e_projectileCoordinates.begin() + i);
            e_projectileSpeed.erase(e_projectileSpeed.begin() + i);
            --i;
        }
    }
    for (int i = 0; i < e_projectileCoordinates.size(); i++) {
        float dx = e_projectileCoordinates[i].x - enemy_x;
        float dy = e_projectileCoordinates[i].y - enemy_y - elevation;
        float dist = sqrt(dx * dx + dy * dy);
        if (dist < 30 && p2_alive) {
            EnemyHit();
            e_projectileCoordinates.erase(e_projectileCoordinates.begin() + i);
            e_projectileSpeed.erase(e_projectileSpeed.begin() + i);
            --i;
        }

    }
}

void TankWars::HeroHitFloor()
{
    for (int j = 0; j < projectileCoordinates.size(); j++) {
        float proj_x = projectileCoordinates[j].x;
        float proj_y = projectileCoordinates[j].y;
        if (proj_x >= 0 && proj_x < peaks.size() - 1) {
            int x1 = static_cast<int>(proj_x);
            float t = (proj_x - x1);
            float height = peaks[x1] * (1 - t) + peaks[x1 + 1] * t;
            if (height + elevation >= proj_y) {
                int y1 = static_cast<int>(proj_y);
                for (int i = -impact; i < impact; i++) {
                    int tank_pos = static_cast<int>(tank_x);
                    int enemy_pos = static_cast<int>(enemy_x);
                    if (x1 - i >= 0 && x1 - i < window->GetResolution().x) {
                        peaks[x1 - i] -= impact * cos(i * M_PI / (2 * impact));
                        if (peaks[x1 - i] < -elevation)
                            peaks[x1 - i] = -elevation;
                        if (x1 - i == tank_pos) {
                            tank_y -= impact * cos(i * M_PI / (2 * impact));
                            if (tank_y < -elevation)
                                tank_y = -elevation;
                        }
                        if (x1 - i == enemy_pos) {
                            enemy_y -= impact * cos(i * M_PI / (2 * impact));
                            if (enemy_y < -elevation)
                                enemy_y = -elevation;
                        }
                    }

                }
                projectileCoordinates.erase(projectileCoordinates.begin() + j);
                projectileSpeed.erase(projectileSpeed.begin() + j);
                --j;
            }
        }
    }
    float V_x = 1;
    float V_y = peaks[tank_x + 1] - peaks[tank_x];
    tank_angle = atan2(V_y, V_x);
}

void TankWars::VillainHitFloor()
{
    for (int j = 0; j < e_projectileCoordinates.size(); j++) {
        float proj_x = e_projectileCoordinates[j].x;
        float proj_y = e_projectileCoordinates[j].y;
        if (proj_x >= 0 && proj_x < peaks.size() - 1) {
            int x1 = static_cast<int>(proj_x);
            float t = (proj_x - x1);
            float height = peaks[x1] * (1 - t) + peaks[x1 + 1] * t;
            if (height + elevation >= proj_y) {
                int y1 = static_cast<int>(proj_y);
                for (int i = -impact; i < impact; i++) {
                    int tank_pos = static_cast<int>(tank_x);
                    int enemy_pos = static_cast<int>(enemy_x);
                    if (x1 - i >= 0 && x1 - i < window->GetResolution().x) {
                        peaks[x1 - i] -= impact * cos(i * M_PI / (2 * impact));
                        if (peaks[x1 - i] < -elevation)
                            peaks[x1 - i] = -elevation;
                        if (x1 - i == tank_pos) {
                            tank_y -= impact * cos(i * M_PI / (2 * impact));
                            if (tank_y < -elevation)
                                tank_y = -elevation;
                        }
                        if (x1 - i == enemy_pos) {
                            enemy_y -= impact * cos(i * M_PI / (2 * impact));
                            if (enemy_y < -elevation)
                                enemy_y = -elevation;
                        }
                    }
                }
                e_projectileCoordinates.erase(e_projectileCoordinates.begin() + j);
                e_projectileSpeed.erase(e_projectileSpeed.begin() + j);
                --j;
            }
        }
    }
    float V_x = 1;
    float V_y = peaks[enemy_x + 1] - peaks[enemy_x];
    enemy_angle = atan2(V_y, V_x);
}

void TankWars::HitFloor()
{
    HeroHitFloor();
    VillainHitFloor();
}

void TankWars::LandSlide(float deltaTimeSeconds)
{
    for (int i = 1; i < peaks.size() - 1; i++) {
        //if (abs(peaks[i] - peaks[i + 1]) > threshold) {
            /*float erosionAmount = epsilon * abs(peaks[i] - peaks[i + 1]);
            if (peaks[i] > peaks[i + 1]) {
                peaks[i] -= erosionAmount * deltaTimeSeconds;
                peaks[i + 1] += erosionAmount * deltaTimeSeconds;
            }
            else {
                peaks[i] += erosionAmount * deltaTimeSeconds;
                peaks[i + 1] -= erosionAmount * deltaTimeSeconds;
            }*/
        if (i == static_cast<int>(tank_x)) {
            tank_y = (peaks[i - 1] + peaks[i + 1]) / 2;
            float V_x = 1;
            float V_y = peaks[tank_x + 1] - peaks[tank_x];
            tank_angle = atan2(V_y, V_x);

        }
        if (i == static_cast<int>(enemy_x) && peaks[i] == enemy_y) {
            enemy_y = (peaks[i - 1] + peaks[i + 1]) / 2;
            float V_x = 1;
            float V_y = peaks[enemy_x + 1] - peaks[enemy_x];
            enemy_angle = atan2(V_y, V_x);

        }
        peaks[i] = (peaks[i - 1] + peaks[i + 1]) / 2;
    }
}


void TankWars::Update(float deltaTimeSeconds)
{

    PlaceTanks(deltaTimeSeconds);
    CreateField();
    Hit();
    HitFloor();
    LandSlide(deltaTimeSeconds);

}

void TankWars::FrameEnd()
{
}

void TankWars::OnInputUpdate(float deltaTime, int mods)
{
    //tank movement
    float tank_speed = 100;
    float turret_speed = 2;
    if (window->KeyHold(GLFW_KEY_A)) {
        if (tank_x > 40) {
            tank_x -= deltaTime * tank_speed;
            tank_y = peaks[tank_x] + (tank_x - tank_x) * (peaks[tank_x + 1] - peaks[tank_x]);
            float V_x = 1;
            float V_y = peaks[tank_x + 1] - peaks[tank_x];
            tank_angle = atan2(V_y, V_x);
        }
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        if (tank_x < window->GetResolution().x - 40)  {
            tank_x += deltaTime * tank_speed;
            tank_y = peaks[tank_x] + (tank_x - tank_x) * (peaks[tank_x + 1] - peaks[tank_x]);
            float V_x = 1;
            float V_y = peaks[tank_x + 1] - peaks[tank_x];
            tank_angle = atan2(V_y, V_x);
        }
    }
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        if (enemy_x > 40) {
            enemy_x -= deltaTime * tank_speed;
            enemy_y = peaks[enemy_x] + (enemy_x - enemy_x) * (peaks[enemy_x + 1] - peaks[enemy_x]);
            float V_x = 1;
            float V_y = peaks[enemy_x + 1] - peaks[enemy_x];
            enemy_angle = atan2(V_y, V_x);
        }
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        if (enemy_x < window->GetResolution().x - 40) {
            enemy_x += deltaTime * tank_speed;
            enemy_y = peaks[enemy_x] + (enemy_x - enemy_x) * (peaks[enemy_x + 1] - peaks[enemy_x]);
            float V_x = 1;
            float V_y = peaks[enemy_x + 1] - peaks[enemy_x];
            enemy_angle = atan2(V_y, V_x);
        }
    }

    //turret movement/
    if (window->KeyHold(GLFW_KEY_W)) {
        t_angle += turret_speed * deltaTime;
    }


    if (window->KeyHold(GLFW_KEY_S)) {
        t_angle -= turret_speed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_UP)) {
        t_enemy_angle -= turret_speed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_DOWN)) {
        t_enemy_angle += turret_speed * deltaTime;
    }


}

void TankWars::OnKeyPress(int key, int mods)
{

    if (key == GLFW_KEY_SPACE && p1_alive) {
        projectileCoordinates.push_back(glm::vec2(coordinates[2][0] + cos(t_angle), coordinates[2][1] + sin(t_angle)));
        projectileSpeed.push_back(glm::vec2(cos(t_angle) * magnitude, sin(t_angle) * magnitude));
        launch_p1 = true;
    }

    if (key == GLFW_KEY_ENTER && p2_alive) {
        e_projectileCoordinates.push_back(glm::vec2(enemy_coordinates[2][0] + cos( t_enemy_angle), enemy_coordinates[2][1] + sin(t_enemy_angle)));
        e_projectileSpeed.push_back(glm::vec2(cos(t_enemy_angle) * magnitude, sin(t_enemy_angle) * magnitude));
        launch_p2 = true;
    }
    // Add key press event
    if (key == GLFW_KEY_F1) {
        trajectory_1 = !trajectory_1;
    }
    if (key == GLFW_KEY_F2) {
        trajectory_2 = !trajectory_2;
    }
}

void TankWars::OnKeyRelease(int key, int mods)
{

    // Add key release event
}

void TankWars::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}

void TankWars::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}

void TankWars::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}

void TankWars::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void TankWars::OnWindowResize(int width, int height)
{
    peaks.clear();
    for (float i = 0; i < width; i++) {
        peaks.push_back(sinusoidal_foo(i));
    }
}
