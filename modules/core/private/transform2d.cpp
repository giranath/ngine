#include "transform2d.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/epsilon.hpp>

namespace ng
{

transform2d::transform2d() noexcept
: translation()
, rotation{0.f}
, scale{1.f, 1.f}
{
}

transform2d::transform2d(const glm::vec2 translation, float rotation, const glm::vec2& scale) noexcept
: translation(translation)
, rotation(rotation)
, scale(scale)
{

}

transform2d::transform2d(glm::mat3x3 matrix) noexcept
: transform2d{}
{
    // Get last column as the translation
    translation = glm::vec2{matrix[2][0], matrix[2][1]};

    // Reset translation in matrix
    matrix[2][0] = 0.f;
    matrix[2][1] = 0.f;

    // Extract scale
    const glm::vec2 scale_x_vector{matrix[0][0], matrix[1][0]};
    const glm::vec2 scale_y_vector{matrix[0][1], matrix[1][1]};

    scale.x = glm::length(scale_x_vector);
    scale.y = glm::length(scale_y_vector);

    const glm::mat2 rotation_matrix{
        matrix[0][0] / scale.x, matrix[0][1] / scale.y,
        matrix[1][0] / scale.x, matrix[1][1] / scale.y
    };

    rotation = std::atan2(rotation_matrix[0][1], rotation_matrix[0][0]);
}

transform2d transform2d::inverse() const noexcept
{
    const glm::mat3 inverse_matrix = glm::inverse(matrix());

    return transform2d{inverse_matrix};
}

glm::mat3x3 transform2d::rotation_matrix() const noexcept
{
    return glm::rotate(glm::mat3{1.f}, rotation);
}

glm::mat3x3 transform2d::translation_matrix() const noexcept
{
    return glm::translate(glm::mat3{1.f}, translation);
}

glm::mat3x3 transform2d::scale_matrix() const noexcept
{
    return glm::scale(glm::mat3{1.f}, scale);
}

glm::mat3x3 transform2d::matrix() const noexcept
{
    return translation_matrix() * scale_matrix() * rotation_matrix(); //scale_matrix() * rotation_matrix() * translation_matrix();
}

glm::vec2 transform2d::transform_point(const glm::vec2& point) const noexcept
{
    return matrix() * glm::vec3{point.x, point.y, 1.0f};
}

glm::vec2 transform2d::transform_vector(const glm::vec2& vector) const noexcept
{
    return matrix() * glm::vec3{vector.x, vector.y, 0.0f};
}

glm::vec2 transform2d::right() const noexcept
{
    return rotation_matrix() * glm::vec3{1.f, 0.f, 0.f};
}

glm::vec2 transform2d::up() const noexcept
{
    return rotation_matrix() * glm::vec3{0.f, 1.f, 0.f};
}

void transform2d::reset() noexcept
{
    translation = glm::vec2{0.f, 0.f};
    rotation = 0.f;
    scale = glm::vec2{1.f, 1.f};
}

transform2d transform2d::operator*(const transform2d& other) const noexcept
{
    const glm::mat3 new_matrix = matrix() * other.matrix();

    return transform2d{new_matrix};
}

transform2d& transform2d::operator*=(const transform2d& other) noexcept
{
    const glm::mat3 new_matrix = matrix() * other.matrix();

    *this = transform2d{new_matrix};

    return *this;
}

bool transform2d::similar(const transform2d& other, float epsilon) const noexcept
{
    const auto translation_equal = glm::epsilonEqual(translation, other.translation, epsilon);
    const auto rotation_equal = glm::epsilonEqual(rotation, other.rotation, epsilon);
    const auto scale_equal = glm::epsilonEqual(scale, other.scale, epsilon);

    const float diff = scale.y - other.scale.y;

    // 0.00000011920929
    // 0.00000011920929

    return translation_equal.x && translation_equal.y
        && rotation_equal
        && scale_equal.x && scale_equal.y;
}

bool transform2d::operator==(const transform2d& other) const noexcept
{
    return similar(other);
}

bool transform2d::operator!=(const transform2d& other) const noexcept
{
    return !similar(other);
}

std::ostream& operator<<(std::ostream& out, const transform2d& transform)
{
    return out << "T=(" << transform.translation.x << ", " << transform.translation.y << ") R="
               << transform.rotation
               << " S=(" << transform.scale.x << ", " << transform.scale.y << ')' << std::endl;
}

}