#define BOOST_TEST_DYN_LINK
// clang-format off
#include <boost/test/unit_test.hpp>
// clang-format on

#include <nuschl/readfile.hpp>

BOOST_AUTO_TEST_SUITE(Readfile)
BOOST_AUTO_TEST_CASE(error) {
    BOOST_CHECK_THROW(nuschl::parsing::read_file("adsfjads;fgadsif"),
                      std::runtime_error);
    std::string f("adsfjads;fgadsif");
    BOOST_CHECK_THROW(nuschl::parsing::read_file(f), std::runtime_error);
}
BOOST_AUTO_TEST_SUITE_END()
