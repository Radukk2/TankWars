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

    projectiles = 0;

    // Initialize angularStep
    angularStep = 0;
    step = 1;

    p1_alive = true;

    num_hits_1 = 0;
    num_hits_2 = 0;

    launch_p1 = false;
    launch_p2 = false;


    for (float i = 0; i < resolution.x; i++) {
        peaks.push_back(sinusoidal_foo(i));
    }
    float start_x = 100;
    float start_x_next = 101;
    tank_x = start_x;
    tank_y = peaks[start_x] + (tank_x - start_x) * (peaks[start_x_next] - peaks[start_x]);
    //cout << tank_x << " " << tank_y;
    float V_x = start_x_next - start_x;
    float V_y = peaks[start_x_next] - peaks[start_x];

    tank_angle = atan2(V_y, V_x);

    start_x = 1000;
    start_x_next = 1001;

    enemy_x = start_x;
    enemy_y = peaks[start_x] + (enemy_x - start_x) * (peaks[start_x_next] - peaks[start_x]);

    V_x = start_x_next - start_x;
    V_y = peaks[start_x_next] - peaks[start_x];

    enemy_angle = atan2(V_y, V_x);

    t_angle = 0;

    t_enemy_angle = 0;

    /*t_position_x = 0;
    t_position_y = 0;



    t_enemy_position_x = 0;
    t_enemy_position_y = 0;*/
    /*projectileSpeed.x = cos(t_angle) * magnitude;
    projectileSpeed.y = sin(t_angle) * magnitude;*/

    /*projectileCoordinates = glm::vec2(0, 0);*/
    //modelMatrix *= transform2D::Translate(200,200);
    //RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
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

void TankWars::PlaceTanks(float deltaTime) {
    //place my_tank
    if (p1_alive) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(tank_x, tank_y + elevation);
        modelMatrix *= transform2D::Rotate(tank_angle);
        RenderMesh2D(meshes["tank"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D::Translate(0, 25);
        modelMatrix *= transform2D::Rotate(t_angle);
        RenderMesh2D(meshes["turret"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D::Translate(25, 0);
        coordinates = modelMatrix;

        if (launch_p1) {
            float speed = 2;
            for (size_t i = 0; i < projectileCoordinates.size(); ++i) {
                projectileCoordinates[i] += projectileSpeed[i] * deltaTime * speed;
                projectileSpeed[i].y -= g * deltaTime * speed;
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(projectileCoordinates[i].x,projectileCoordinates[i].y);
                modelMatrix *= transform2D::Rotate(tank_angle);
                RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);
                /*if (projectileCoordinates[i].y < 0 || projectileCoordinates[i].x > window->GetResolution().x) {
                    projectileCoordinates.erase(projectileCoordinates.begin() + i);
                    projectileSpeed.erase(projectileSpeed.begin() + i);
                    --i;
                }*/
            }

            // Reset launch if no projectiles are left
            if (projectileCoordinates.empty()) {
                launch_p1 = false;
            }
        }

        //lifeBar and life
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(tank_x, tank_y +  elevation + 50);
        RenderMesh2D(meshes["lifeBar"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D::Translate(-25, 0);
        modelMatrix *= transform2D::Scale(1 - 0.1  * num_hits_1, 1);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
        
    }
    //place enemy tank
    if (p2_alive) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(enemy_x, enemy_y + elevation);
        modelMatrix *= transform2D::Rotate(enemy_angle);
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D::Translate(0,25);
        modelMatrix *= transform2D::Rotate(t_enemy_angle + M_PI);
        enemy_coordinates = modelMatrix;
        RenderMesh2D(meshes["turret"], shaders["VertexColor"], modelMatrix);
        if (launch_p2) {
            float speed = 2;
            for (size_t i = 0; i < e_projectileCoordinates.size(); ++i) {
                e_projectileCoordinates[i] += e_projectileSpeed[i] * deltaTime * speed;
                e_projectileSpeed[i].y -= g * deltaTime * speed;
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(e_projectileCoordinates[i].x, e_projectileCoordinates[i].y);
                modelMatrix *= transform2D::Rotate(enemy_angle);
                RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);
                /*if (projectileCoordinates[i].y < 0 || projectileCoordinates[i].x > window->GetResolution().x) {
                    projectileCoordinates.erase(projectileCoordinates.begin() + i);
                    projectileSpeed.erase(projectileSpeed.begin() + i);
                    --i;
                }*/
            }

            // Reset launch if no projectiles are left
            if (e_projectileCoordinates.empty()) {
                launch_p2 = false;
            }
        }
        //lifeBar and life
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(enemy_x, enemy_y + elevation + 50);
        RenderMesh2D(meshes["lifeBar"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D::Translate(-25, 0);
        modelMatrix *= transform2D::Scale(1 - 0.1 * num_hits_2, 1);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
    }

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
    /*for (int i = 0; i < projectileCoordinates.size(); i++) {
        float dx = projectileCoordinates[i].x - enemy_x;
        float dy = projectileCoordinates[i].y - enemy_y;
        float dist = sqrt(dx * dx + dy * dy);
        cout << projectileCoordinates[i].x << " " << projectileCoordinates[i].y << " - " << enemy_x << " " << enemy_y << "\n";
        if (dist < 40)
            EnemyHit();
    }*/
}

void TankWars::Update(float deltaTimeSeconds)
{

    CreateField();
    PlaceTanks(deltaTimeSeconds);
    Hit();
    
    /*for (int i = 0; i < peaks.size() - 1; i++) {
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
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);*/


    /*modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(tank_x, tank_y + elevation);
    modelMatrix *= transform2D::Rotate(tank_angle);
    RenderMesh2D(meshes["tank"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(tank_x + 15, tank_y + elevation + 20);
    modelMatrix *= transform2D::Rotate(tank_angle);
    RenderMesh2D(meshes["turret"], shaders["VertexColor"], modelMatrix);


    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(enemy_x, enemy_y + elevation);
    modelMatrix *= transform2D::Rotate(enemy_angle);
    RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);*/

    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented.
    // Remember, the last matrix in the chain will take effect first!

}


void TankWars::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void TankWars::OnInputUpdate(float deltaTime, int mods)
{
    //tank movement
    float tank_speed = 50;
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
        if (tank_x < 1240) {
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
        if (enemy_x < 1240) {
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
        t_enemy_angle += turret_speed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_DOWN)) {
        t_enemy_angle -= turret_speed * deltaTime;
    }


}


void TankWars::OnKeyPress(int key, int mods)
{

    if (key == GLFW_KEY_SPACE) {
        if (key == GLFW_KEY_SPACE) {
            projectileCoordinates.push_back(glm::vec2(coordinates[2][0] + cos(t_angle), coordinates[2][1] + sin(t_angle)));
            projectileSpeed.push_back(glm::vec2(cos(tank_angle + t_angle) * magnitude, sin(tank_angle + t_angle) * magnitude));
            launch_p1 = true;
        }
    }

    if (key == GLFW_KEY_ENTER) {
        e_projectileCoordinates.push_back(glm::vec2(enemy_coordinates[2][0] + cos(t_enemy_angle), enemy_coordinates[2][1] + sin(t_enemy_angle)));
        e_projectileSpeed.push_back(glm::vec2(cos(enemy_angle + t_enemy_angle) * magnitude, sin(enemy_angle + t_enemy_angle) * magnitude));
        launch_p2 = true;
    }
    if (key == GLFW_KEY_F1) {
        HeroHit();
    }

    if (key == GLFW_KEY_F2) {
        EnemyHit();
    }
    // Add key press event
}


void TankWars::OnKeyRelease(int key, int mods)
{

    // Add key release event
    if (key == GLFW_KEY_ENTER) {
        launch_p2 = false;
    }
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
}
