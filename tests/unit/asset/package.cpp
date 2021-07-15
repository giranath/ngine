#include <catch.hpp>
#include <ng/asset/asset_package.hpp>
#include <sstream>

using namespace ng::literals;

TEST_CASE("A package can be deserialized from a stream", "[asset_package]")
{
    SECTION("an invalid stream cannot be deserialized")
    {
        SECTION("invalid magic header is correctly handled")
        {
            const std::vector<uint8_t> package_stream_bytes{
                    // Header
                    'N', 'Z', 'P', 'S', 'G', 0,

                    // Table Entry
                    //==============================================================================================================
                    // Count
                    1, 0, 0, 0, 0, 0, 0, 0,
                    // Name
                    9, 0, 0, 0, 0, 0, 0, 0, 'm', 'o', 'c', 'k', 'a', 's', 's', 'e', 't',
                    // Asset type
                    7, 0, 0, 0, 0, 0, 0, 0, 'n', 'g', '.', 'm', 'o', 'c', 'k',

                    // Version
                    0,

                    // Size
                    10, 0, 0, 0, 0, 0, 0, 0,

                    // Offset
                    1, 0, 0, 0, 0, 0, 0, 0,

                    // Data
                    11, 0, 0, 0, 0, 0, 0, 0,
                    77, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
            };

            std::string data_str(reinterpret_cast<const char*>(package_stream_bytes.data()), package_stream_bytes.size());
            std::istringstream stream(data_str);

            ng::asset_package package;
            REQUIRE_FALSE(package.try_load(stream));

            REQUIRE(stream.fail());
        }
    }

    SECTION("a valid stream can be deserialized")
    {
        const std::vector<uint8_t> package_stream_bytes{
                // Header
                'N', 'G', 'P', 'K', 'G', 0,

                // Table Entry
                //==============================================================================================================
                // Count
                1, 0, 0, 0, 0, 0, 0, 0,
                // Name
                9, 0, 0, 0, 0, 0, 0, 0, 'm', 'o', 'c', 'k', 'a', 's', 's', 'e', 't',
                // Asset type
                7, 0, 0, 0, 0, 0, 0, 0, 'n', 'g', '.', 'm', 'o', 'c', 'k',

                // Version
                0,

                // Size
                10, 0, 0, 0, 0, 0, 0, 0,

                // Offset
                1, 0, 0, 0, 0, 0, 0, 0,

                // Data
                11, 0, 0, 0, 0, 0, 0, 0,
                77, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
        };

        std::string data_str(reinterpret_cast<const char*>(package_stream_bytes.data()), package_stream_bytes.size());
        std::istringstream stream(data_str);

        ng::asset_package package;
        REQUIRE(package.try_load(stream));

        // All stream should have been consumed
        REQUIRE(stream);

        // Ignore next character (this should put the stream into eof)
        stream.ignore();
        REQUIRE(stream.eof());

        REQUIRE(package.size() == 1);
        REQUIRE(package.contains("mockasset"_name));
        REQUIRE(package[0].size() == 10);
        REQUIRE(package[0].name() == "mockasset"_name);
        REQUIRE(package[0].type() == "ng.mock"_name);
        REQUIRE(package[0].version() == 0);

        const std::vector<uint8_t> expected_data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        REQUIRE(std::equal(package[0].begin(), package[0].end(), expected_data.begin()));
    }
}