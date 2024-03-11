#ifndef OPENGL_3D_SCENERY_BSPLINE_H
#define OPENGL_3D_SCENERY_BSPLINE_H

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class BSplineCurve {
private:
    std::vector<glm::vec3> control_points;
    std::vector<float> knots;
    int degree;

public:
    BSplineCurve(const std::vector<glm::vec3>& points, int splineDegree, bool closed = false);

    glm::vec3 calc(float t) const;

    glm::vec3 calc_tangent(float t) const;

private:
    float basis_func(int i, int k, float t) const;

    float basis_func_derivative(int i, int k, float t) const;
};

#endif //OPENGL_3D_SCENERY_BSPLINE_H
