#include "lab_m1/TankWars/tankWars.h"

#include <vector>
#include <iostream>

#include "lab_m1/TankWars/hw-transform2D.h"
#include "lab_m1/TankWars/hw-object2D.h"

using namespace std;
using namespace m1;


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

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    forfecareX = 0;
    forfecareY = 0;

    // Initialize angularStep
    angularStep = 0;
    step = 1;


    for (float i = 0; i < resolution.x; i++) {
        peaks.push_back(sinusoidal_foo(i));
    }
    
    //modelMatrix *= transform2D::Translate(200,200);
    //RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
    Mesh* square1 = hw_object2D::CreateSquare("square1", corner, 1, glm::vec3(0.4, 0.4, 0.5), true);
    AddMeshToList(square1);
    Mesh* tank = hw_object2D::CreateTank("tank", glm::vec3(1, 1, 1), true);
    AddMeshToList(tank);
   
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



void TankWars::Update(float deltaTimeSeconds)
{
    for (int i = 0; i < peaks.size() - 1; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(i * step, peaks[i] + 250);
        modelMatrix *= transform2D::Forfecare(step, peaks[i + 1] - peaks[i]);
        modelMatrix *= transform2D::Scale(step, 500);
        modelMatrix *= transform2D::Translate(0, -1);
        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    }
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate((peaks.size() - 2) * step, peaks[(peaks.size() - 2)] + 250);
    modelMatrix *= transform2D::Forfecare(step, peaks[(peaks.size() - 1)] - peaks[(peaks.size() - 2)]);
    modelMatrix *= transform2D::Scale(step, 500);
    modelMatrix *= transform2D::Translate(0, -1);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);


    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(500, 500);
    RenderMesh2D(meshes["tank"], shaders["VertexColor"], modelMatrix);


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
}


void TankWars::OnKeyPress(int key, int mods)
{
    // Add key press event
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
}
