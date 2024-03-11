#include "bspline.h"

BSplineCurve::BSplineCurve(const std::vector<glm::vec3> &points, int degree, bool closed)  : control_points(points), degree(degree) {
    if (points.size() < degree + 1) {
        std::cerr << "[BSplineCurve]: Not enough control points for the specified spline degree." << std::endl;
        std::terminate();
    }

    if (closed) {
        for (int i = 0; i < degree; ++i) {
            control_points.push_back(control_points[i]);
        }
    }

    int m = control_points.size() + degree;
    knots.reserve(m + 1);
    for (int i = 0; i < m + 1; ++i) {
        knots.push_back(static_cast<float>(i) / static_cast<float>(m));
    }

}

glm::vec3 BSplineCurve::calc_tangent(float t) const {
    // Ensure t is within [0, 1]
    t = glm::clamp(t, 0.0f, 1.0f);

    // The curve's domain is [a, b]
    float a = knots[degree];
    float b = knots[knots.size() - degree - 1];
    t = (b - a) * t + a;

    glm::vec3 result(0.0f);
    int n = control_points.size() - 1;

    for (int i = 0; i <= n; ++i) {
        result += control_points[i] * basis_func_derivative(i, degree, t);
    }

    return result;
}

glm::vec3 BSplineCurve::calc(float t) const  {
    // Ensure t is within [0, 1]
    t = glm::clamp(t, 0.0f, 1.0f);

    // The curve's domain is [a, b]
    float a = knots[degree];
    float b = knots[knots.size() - degree - 1];
    t = (b - a) * t + a;

    glm::vec3 result(0.0f);
    int n = control_points.size() - 1;

    for (int i = 0; i <= n; ++i) {
        result += control_points[i] * basis_func(i, degree, t);
    }

    return result;
}

float BSplineCurve::basis_func_derivative(int i, int k, float t) const {

    // Implement the derivative of the B-spline basis function
    // You can use this formula: N'_{i, k}(t) = k * (N_{i, k-1}(t) - N_{i+1, k-1}(t)) / (t_{i+k-1} - t_i)
    if (k == 0) {
        return 0.0f;
    }

    float left = k * basis_func(i, k - 1, t);
    float right = k * basis_func(i + 1, k - 1, t);

    return (left - right) / (float)(k);
}

float BSplineCurve::basis_func(int i, int k, float t) const {
    // Implement the B-spline basis function
    // You can use this formula: N_{i, k}(t) = N_{i, k-1}(t) * (t - t_i) / (t_{i+k-1} - t_i)
    if (k == 0) {
        return (t >= knots[i] && t < knots[i + 1]) ? 1.0f : 0.0f;
    }

    float denom1 = knots[i + k] - knots[i];
    float denom2 = (i + k + 1 < knots.size()) ? (knots[i + k + 1] - knots[i + 1]) : 1.0f;

    float left = 0.0f;
    float right = 0.0f;

    if (denom1 != 0.0f) {
        left = (t - knots[i]) / denom1 * basis_func(i, k - 1, t);
    }

    if (denom2 != 0.0f) {
        right = (knots[i + k + 1] - t) / denom2 * basis_func(i + 1, k - 1, t);
    }

    return left + right;
}

