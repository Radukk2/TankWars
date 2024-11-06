#include "hw-object2D.h"

#include <vector>

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
        VertexFormat(glm::vec3(-50, 10, 0), color),
        VertexFormat(glm::vec3(-40, 20, 0), color),
        VertexFormat(glm::vec3(40, 20, 0), color),
        VertexFormat(glm::vec3(50, 10, 0), color),
        VertexFormat(glm::vec3(40, 10, 0), color),
        VertexFormat(glm::vec3(20, 0, 0), color),
        VertexFormat(glm::vec3(-20, 0, 0), color),
        VertexFormat(glm::vec3(-40, 10, 0), color),
    };
    Mesh* tank = new Mesh(name);

    std::vector<unsigned int> indices = { 0, 3, 1, 
                                          1, 3, 2, 
                                          7, 6, 5,
                                          7, 5, 4};

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
