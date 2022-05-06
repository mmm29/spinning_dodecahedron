#include <cmath>

#include "graphics_utils.h"

Matrix4 CreateProjectionMatrix(float aspect_ratio, float fov, float near_z, float far_z) {
    const float fov_tan = tanf(fov / 2);
    const float far_near_distance = far_z - near_z;
    const float far_near_over_dist = (-far_z * near_z) / far_near_distance;

    return Matrix4({
                           {1 / (aspect_ratio * fov_tan), 0,           0,                         0},
                           {0,                            1 / fov_tan, 0,                         0},
                           {0,                            0,           far_z / far_near_distance, far_near_over_dist},
                           {0,                            0,           1,                         0}
                   });
}

Matrix4 CreateViewMatrix(const Vector3 &position, const Vector3 &right, const Vector3 &up, const Vector3 &forward) {
    return Matrix4{
            {right[0], up[0], forward[0], -position[0]},
            {right[1], up[1], forward[1], -position[1]},
            {right[2], up[2], forward[2], -position[2]},
            {0,        0,     0,          1},
    };

    Matrix4 result;

    result.at(0, 0) = right[0];
    result.at(1, 0) = right[1];
    result.at(2, 0) = right[2];
    result.at(3, 0) = 0;

    result.at(0, 1) = up[0];
    result.at(1, 1) = up[1];
    result.at(2, 1) = up[2];
    result.at(3, 1) = 0;

    result.at(0, 2) = forward[0];
    result.at(1, 2) = forward[1];
    result.at(2, 2) = forward[2];
    result.at(3, 2) = 0;

    result.at(0, 2) = -position[0];
    result.at(1, 2) = -position[1];
    result.at(2, 2) = -position[2];
    result.at(3, 2) = 1;

    return result;
}

Matrix4 CreateScreenSpaceMatrix(const Vector2 &size) {
    const float half_width = size[0] / 2;
    const float half_height = size[1] / 2;

    return Matrix4({
                           {half_width, 0,            0, half_width},
                           {0,          -half_height, 0, half_height},
                           {0,          0,            1, 0},
                           {0,          0,            0, 1}
                   });
}