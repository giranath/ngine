#include <catch.hpp>
#include <glm/gtc/epsilon.hpp>
#include <ng/core/transform2d.hpp>

TEST_CASE("An identity transform can be created", "[transform2d]" )
{
    const ng::transform2d identity;

    REQUIRE(identity.translation == glm::vec2{0.f, 0.f});
    REQUIRE(identity.rotation == 0.f);
    REQUIRE(identity.scale == glm::vec2{1.f, 1.f});
}

TEST_CASE("A transform can be converted into a translation matrix", "[transform2d]")
{
    const float rotation = glm::radians(45.f);
    const float scale_x = 1.2f;
    const float scale_y = 1.5f;
    const float translation_x = 10.f;
    const float translation_y = 13.f;

    const ng::transform2d transform{glm::vec2{translation_x, translation_y},
                                    rotation,
                                    glm::vec2{scale_x, scale_y}};

    const glm::mat3 matrix = transform.translation_matrix();

    const glm::mat3 expected_matrix{
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        translation_x, translation_y, 1.f
    };

    REQUIRE(matrix == expected_matrix);
}

TEST_CASE("A transform can be converted into a rotation matrix", "[transform2d]")
{
    const float rotation = glm::radians(45.f);
    const float scale_x = 1.2f;
    const float scale_y = 1.5f;
    const float translation_x = 10.f;
    const float translation_y = 13.f;

    const ng::transform2d transform{glm::vec2{translation_x, translation_y},
                                    rotation,
                                    glm::vec2{scale_x, scale_y}};

    const glm::mat3 matrix = transform.rotation_matrix();

    const glm::mat3 expected_matrix{
            std::cos(rotation), std::sin(rotation), 0.f,
            -std::sin(rotation), std::cos(rotation), 0.f,
            0.f, 0.f, 1.f
    };

    REQUIRE(matrix == expected_matrix);
}

TEST_CASE("A transform can be converted into a scale matrix", "[transform2d]")
{
    const float rotation = glm::radians(45.f);
    const float scale_x = 1.2f;
    const float scale_y = 1.5f;
    const float translation_x = 10.f;
    const float translation_y = 13.f;

    const ng::transform2d transform{glm::vec2{translation_x, translation_y},
                                    rotation,
                                    glm::vec2{scale_x, scale_y}};

    const glm::mat3 matrix = transform.scale_matrix();

    const glm::mat3 expected_matrix{
            scale_x, 0.f, 0.f,
            0.f, scale_y, 0.f,
            0.f, 0.f, 1.f
    };

    REQUIRE(matrix == expected_matrix);
}

TEST_CASE("A transform can be converted into a matrix", "[transform2d]")
{
    const float rotation = glm::radians(45.f);
    const float scale_x = 1.2f;
    const float scale_y = 1.5f;
    const float translation_x = 10.f;
    const float translation_y = 13.f;

    const ng::transform2d transform{glm::vec2{translation_x, translation_y},
                                    rotation,
                                    glm::vec2{scale_x, scale_y}};

    const glm::mat3 matrix = transform.matrix();

    const glm::mat3 expected_matrix = {
            scale_x * std::cos(rotation), scale_y * std::sin(rotation), 0.f,
            -scale_x * std::sin(rotation), scale_y * std::cos(rotation), 0.f,
            translation_x,                 translation_y,                1.f
    };

    REQUIRE(matrix == expected_matrix);
}

TEST_CASE("A transform matrix can be decomposed", "[transform2d]")
{
    const float rotation = glm::radians(45.f);
    const float scale_x = 1.2f;
    const float scale_y = 1.5f;
    const float translation_x = 10.f;
    const float translation_y = 13.f;

    const ng::transform2d transform{glm::vec2{translation_x, translation_y},
                                    rotation,
                                    glm::vec2{scale_x, scale_y}};

    const ng::transform2d decomposed_transform{transform.matrix()};

    REQUIRE(transform.similar(decomposed_transform, std::numeric_limits<float>::epsilon() * 1.1f));
}

TEST_CASE("A transform has a right vector", "[transform2d]")
{
    const float rotation = glm::radians(45.f);
    const float scale_x = 1.2f;
    const float scale_y = 1.5f;
    const float translation_x = 10.f;
    const float translation_y = 13.f;

    const ng::transform2d transform{glm::vec2{translation_x, translation_y},
                                    rotation,
                                    glm::vec2{scale_x, scale_y}};

    const glm::vec2 right = transform.right();

    REQUIRE(right == glm::vec2{std::cos(rotation), std::sin(rotation)});
}

TEST_CASE("A transform has a up vector", "[transform2d]")
{
    const float rotation = glm::radians(45.f);
    const float scale_x = 1.2f;
    const float scale_y = 1.5f;
    const float translation_x = 10.f;
    const float translation_y = 13.f;

    const ng::transform2d transform{glm::vec2{translation_x, translation_y},
                                    rotation,
                                    glm::vec2{scale_x, scale_y}};

    const glm::vec2 up = transform.up();

    // Up is always 90 degrees more than right
    REQUIRE(up == glm::vec2{std::cos(rotation + glm::radians(90.f)), std::sin(rotation + glm::radians(90.f))});
}

TEST_CASE("Transforms can be combined", "[transform2d]")
{
    const ng::transform2d parent{glm::vec2{10.f, 10.f}, glm::radians(45.f), glm::vec2{1.f, 1.f}};
    const ng::transform2d child{glm::vec2{5.f, 10.f}, glm::radians(0.f), glm::vec2{0.5f, 0.5f}};

    const ng::transform2d combination = parent * child;
    const glm::mat3 expected_matrix = parent.matrix() * child.matrix();
    const glm::mat3 combination_matrix = combination.matrix();

    for(int i = 0; i < 3; ++i)
    {
        REQUIRE(glm::all(glm::epsilonEqual(expected_matrix[i], combination_matrix[i], glm::epsilon<float>())));
    }
}

TEST_CASE("Transforms can be inversed", "[transform2d]")
{
    const ng::transform2d transform{glm::vec2{10.f, 10.f}, glm::radians(45.f), glm::vec2{1.f, 1.f}};

    const ng::transform2d inverse = transform.inverse();
    const glm::mat3 expected_matrix = glm::inverse(transform.matrix());
    const glm::mat3 combination_matrix = inverse.matrix();

    for(int i = 0; i < 3; ++i)
    {
        REQUIRE(glm::all(glm::epsilonEqual(expected_matrix[i], combination_matrix[i], glm::epsilon<float>())));
    }
}

TEST_CASE("A transform can transform a point", "[transform2d]")
{
    const glm::vec2 point{10.f, 20.f};

    SECTION("Identity transform won't affect point")
    {
        const ng::transform2d identity;

        const glm::vec2 transformed_point = identity.transform_point(point);

        REQUIRE(transformed_point == point);
    }

    SECTION("Translation transform will affect point")
    {
        const ng::transform2d transform{glm::vec2{10.f, 15.f}};

        const glm::vec2 transformed_point = transform.transform_point(point);

        REQUIRE(transformed_point == glm::vec2{20.f, 35.f});
    }

    SECTION("Full transform will affect point")
    {
        const float rotation = glm::radians(45.f);
        const float scale_x = 1.2f;
        const float scale_y = 1.5f;
        const float translation_x = 10.f;
        const float translation_y = 13.f;

        const ng::transform2d transform{glm::vec2{translation_x, translation_y},
                                        rotation,
                                        glm::vec2{scale_x, scale_y}};

        const glm::vec2 transformed_point = transform.transform_point(point);

        REQUIRE(transformed_point == glm::vec2{1.51471901f, 44.8198051f});
    }
}