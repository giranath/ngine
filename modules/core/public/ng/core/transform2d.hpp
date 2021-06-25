#ifndef NGINE_CORE_TRANSFORM2D_HPP
#define NGINE_CORE_TRANSFORM2D_HPP

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <ostream>

namespace ng
{

/**
 * Represents a 2d transform
 */
struct transform2d
{
    glm::vec2 translation;
    float rotation;
    glm::vec2 scale;

    /**
     * Construct an identity transform
     */
    transform2d() noexcept;

    /**
     * Construct a transform from it's component
     * @param translation The translation vector
     * @param rotation The rotation angle
     * @param scale The scale vector
     */
    explicit transform2d(const glm::vec2 translation, float rotation = 0.0f, const glm::vec2& scale = glm::vec2{1.f, 1.f}) noexcept;

    /**
     * Decompose a transform matrix into it's components
     * @param matrix The matrix to decompose
     */
    explicit transform2d(glm::mat3x3 matrix) noexcept;

    /**
     * Returns the inverse transform
     * @return The inverse transform
     */
    [[nodiscard]] transform2d inverse() const noexcept;

    /**
     * Returns the rotation matrix of this transform
     * @return The rotation matrix
     */
    [[nodiscard]] glm::mat3x3 rotation_matrix() const noexcept;

    /**
     * Returns the translation matrix
     * @return The translation matrix
     */
    [[nodiscard]] glm::mat3x3 translation_matrix() const noexcept;

    /**
     * Returns the scale matrix
     * @return The scale matrix
     */
    [[nodiscard]] glm::mat3x3 scale_matrix() const noexcept;

    /**
     * Returns the matrix representation of the transform
     * @return The matrix representation of the transform
     */
    [[nodiscard]] glm::mat3x3 matrix() const noexcept;

    /**
     * Transform a point with this transform
     * @param point The point to transform
     * @return the transformed point
     */
    [[nodiscard]] glm::vec2 transform_point(const glm::vec2& point) const noexcept;

    /**
     * Transform a vector with this transform
     * @param vector The vector to transform
     * @return the transformed vector
     */
    [[nodiscard]] glm::vec2 transform_vector(const glm::vec2& vector) const noexcept;

    /**
     * Returns the right vector from this transform
     * @return The right vector
     */
    [[nodiscard]] glm::vec2 right() const noexcept;

    /**
     * Returns the up vector from this transform
     * @return The up vector
     */
    [[nodiscard]] glm::vec2 up() const noexcept;

    /**
     * Reset this transform to it's identity
     */
    void reset() noexcept;

    /**
     * Multiply this transform with another transform
     * @param other The other transform to multiply with this
     * @return the result of the multiplication
     */
    [[nodiscard]] transform2d operator*(const transform2d& other) const noexcept;

    /**
     * Transform this transform with another transform
     * @param other The other transform to multiply with this
     * @return a reference to this that was modified
     */
    transform2d& operator*=(const transform2d& other) noexcept;

    /**
     * Check if this transform is similar enough to other
     * @param other The other transform
     * @param epsilon The epsilon to use to compare transforms
     * @return true when similar or false otherwise
     * @note Because transforms are composed of floats, perfect equality is highly improbable therefore it's often
     *       better to check how similar two transforms are
     */
    [[nodiscard]] bool similar(const transform2d& other, float epsilon = std::numeric_limits<float>::epsilon() /*glm::epsilon<float>()*/) const noexcept;

    bool operator==(const transform2d& other) const noexcept;
    bool operator!=(const transform2d& other) const noexcept;
};

std::ostream& operator<<(std::ostream& out, const transform2d& transform);

}

#endif
