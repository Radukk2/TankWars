#include "hw-object2D.h"

#include <vector>
#include <cmath>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* hw_object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* hw_object2D::CreateTank(
    const std::string& name,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(-45, 10, 0), color), //0
        VertexFormat(glm::vec3(-40, 20, 0), color), //1
        VertexFormat(glm::vec3(40, 20, 0), color),  //2
        VertexFormat(glm::vec3(45, 10, 0), color),  //3
        VertexFormat(glm::vec3(40, 10, 0), color),  //4
        VertexFormat(glm::vec3(20, 0, 0), color),   //5
        VertexFormat(glm::vec3(-20, 0, 0), color),  //6
        VertexFormat(glm::vec3(-40, 10, 0), color), //7
        VertexFormat(glm::vec3(0, 20, 0), color)    //8
    };

    for (int i = 0; i < 210; i++) {
        vertices.push_back(VertexFormat(glm::vec3(15 * cos(i * M_PI / 200), 15 * sin(i * M_PI / 200) + 20, 0), color));
    }
    Mesh* tank = new Mesh(name);

    std::vector<unsigned int> indices = { 0, 3, 1,
                                          1, 3, 2,
                                          7, 6, 5,
                                          7, 5, 4 };
    for (int i = 0; i < 200; i++) {
        indices.push_back(i + 8);
        indices.push_back(8);
        indices.push_back(i + 9);
    }

    if (!fill) {
        tank->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    tank->InitFromData(vertices, indices);
    return tank;
}


Mesh* hw_object2D::CreateTurret(
    const std::string& name,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, 3, 0), color), //0
        VertexFormat(glm::vec3(30, 3, 0), color), //1
        VertexFormat(glm::vec3(30, -3, 0), color),  //2
        VertexFormat(glm::vec3(0, -3, 0), color),  //3

    };
    Mesh* turret = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 2, 1,
                                          0, 3, 2, };
    if (!fill) {
        turret->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }
    turret->InitFromData(vertices, indices);
    return turret;
}

Mesh* hw_object2D::CreateLifeBar(
    const std::string& name,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(-27, 4, 0), color), //0
        VertexFormat(glm::vec3(27, 4, 0), color), //1
        VertexFormat(glm::vec3(27, -4, 0), color),  //2
        VertexFormat(glm::vec3(-27, -4, 0), color),  //3
        VertexFormat(glm::vec3(-25, 2, 0), color),  //4
        VertexFormat(glm::vec3(25, 2, 0), color),   //5
        VertexFormat(glm::vec3(25, -2, 0), color),  //6
        VertexFormat(glm::vec3(-25, -2, 0), color), //7
    };
    Mesh* lifeBar = new Mesh(name);
    std::vector<unsigned int> indices = {
                                        0,4,1,
                                        4,5,1,
                                        1,5,6,
                                        1,6,2,
                                        7,2,6,
                                        7,3,2,
                                        0,3,4,
                                        4,7,3 };

    if (!fill) {
        lifeBar->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }
    lifeBar->InitFromData(vertices, indices);
    return lifeBar;


}

Mesh* hw_object2D::CreateLife(
    const std::string& name,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(-0, 2, 0), color),  //0
        VertexFormat(glm::vec3(50, 2, 0), color),   //1
        VertexFormat(glm::vec3(50, -2, 0), color),  //2
        VertexFormat(glm::vec3(0, -2, 0), color), //3
    };
    Mesh* life = new Mesh(name);
    std::vector<unsigned int> indices = {
                                        0,3,2,
                                        0,2,1};

    if (!fill) {
        life->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }
    life->InitFromData(vertices, indices);
    return life;
}

Mesh* hw_object2D::CreateProjectile(
    const std::string& name,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    vertices.push_back((glm::vec3(0, 0, 0), color));
    for (int i = 0; i < 200; i++) {
        float angle = i * 6 * M_PI / 200;
        vertices.push_back(VertexFormat(glm::vec3(6 * cos(angle), 6 * sin(angle), 0), color));
    }
    Mesh* projectile = new Mesh(name);
    std::vector<unsigned int> indices;

    for (int i = 1; i < 200; i++) {
        indices.push_back(i);
        indices.push_back(0);
        indices.push_back(i + 1);
    }
    indices.push_back(200);
    indices.push_back(0);
    indices.push_back(1);
    if (!fill) {
        projectile->SetDrawMode(GL_LINE_LOOP);
    }

    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }
    projectile->InitFromData(vertices, indices);
    return projectile;
}

Mesh* hw_object2D::CreateTrajectory(
    const std::string& name,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    vertices.push_back((glm::vec3(0, 0, 0), color));
    for (int i = 0; i < 200; i++) {
        float angle = i * 2 * M_PI / 200;
        vertices.push_back(VertexFormat(glm::vec3(2 * cos(angle), 2 * sin(angle), 0), color));
    }
    Mesh* projectile = new Mesh(name);
    std::vector<unsigned int> indices;

    for (int i = 1; i < 200; i++) {
        indices.push_back(i);
        indices.push_back(0);
        indices.push_back(i + 1);
    }
    indices.push_back(200);
    indices.push_back(0);
    indices.push_back(1);
    if (!fill) {
        projectile->SetDrawMode(GL_LINE_LOOP);
    }

    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }
    projectile->InitFromData(vertices, indices);
    return projectile;
}


Mesh* hw_object2D::CreateTrajectoryLine(
    const std::string& name,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(-1, 0, 0), color),  //0
        VertexFormat(glm::vec3(1, 8, 0), color),   //1
        VertexFormat(glm::vec3(-1, 8, 0), color),  //2
        VertexFormat(glm::vec3(1, 0, 0), color), //3
    };
    Mesh* projectile = new Mesh(name);
    std::vector<unsigned int> indices = {
        0, 2, 1,
        0, 3, 2
    };

    if (!fill) {
        projectile->SetDrawMode(GL_LINE_LOOP);
    }

    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }
    projectile->InitFromData(vertices, indices);
    return projectile;
}