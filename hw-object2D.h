#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace hw_object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateTank(const std::string& name, glm::vec3 color, bool fill = false);
    Mesh* CreateTurret(const std::string& name,glm::vec3 color, bool fill);
    Mesh* CreateLifeBar(const std::string& name, glm::vec3 color, bool fill);
    Mesh* CreateLife(const std::string& name, glm::vec3 color, bool fill);
    Mesh* CreateProjectile(const std::string& name, glm::vec3 color, bool fill);
}
