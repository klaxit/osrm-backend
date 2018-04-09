#include "engine/unpacking_cache.hpp"

#include <boost/test/test_case_template.hpp>
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <iterator>
#include <vector>

BOOST_AUTO_TEST_SUITE(unpacking_cache)

using namespace osrm;
using namespace osrm::engine;

BOOST_AUTO_TEST_CASE(add_edge_and_check_existence)
{
    // Arrange (Setup)
    unsigned timestamp = 1522782542;
    UnpackingCache cache(1, timestamp);

    auto key = std::make_tuple(1, 1, 1);
    auto value = 1;

    // Act
    cache.AddEdge(key, value);

    // Assert
    BOOST_CHECK(cache.IsEdgeInCache(key) == true);
    BOOST_CHECK(cache.IsEdgeInCache(std::make_tuple(2, 2, 2)) == false);

    auto result = cache.GetDuration(key);
    BOOST_CHECK_EQUAL(result, value);
}

BOOST_AUTO_TEST_CASE(cache_invalidation)
{
    // Arrange (Setup)
    unsigned timestamp = 1522782542;
    UnpackingCache cache(1, timestamp);

    auto key1 = std::make_tuple(1, 1, 1);
    auto value1 = 1;

    auto key2 = std::make_tuple(2, 2, 2);
    auto value2 = 2;

    // Act
    cache.AddEdge(key1, value1);
    cache.AddEdge(key2, value2);

    // Assert
    auto result = cache.GetDuration(key1);
    BOOST_CHECK_EQUAL(result, MAXIMAL_EDGE_DURATION);

    result = cache.GetDuration(key2);
    BOOST_CHECK_EQUAL(result, value2);
}

BOOST_AUTO_TEST_CASE(new_data)
{
    // Arrange (Setup)
    unsigned timestamp1 = 1522782542;
    unsigned timestamp2 = 1522782543;

    UnpackingCache cache(1, timestamp1);

    auto key1 = std::make_tuple(1, 2, 3);
    auto value1 = 1;
    auto key2 = std::make_tuple(2, 3, 4);
    auto value2 = 2;

    // Act
    cache.AddEdge(key1, value1);
    cache.Clear(timestamp2);
    cache.AddEdge(key2, value2);

    // Assert
    BOOST_CHECK(cache.IsEdgeInCache(key1) == false);
    BOOST_CHECK(cache.IsEdgeInCache(key2) == true);
    BOOST_CHECK(cache.IsEdgeInCache(std::make_tuple(2, 2, 2)) == false);

    auto result = cache.GetDuration(key2);
    BOOST_CHECK_EQUAL(result, value2);
}

BOOST_AUTO_TEST_SUITE_END()
