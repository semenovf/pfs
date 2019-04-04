#include <pfs/result_code.hpp>
#include "../catch.hpp"

#if __cplusplus >= 201103L
    using result_code = pfs::result_code<int, pfs::string>;
#else
    typedef pfs::result_code<int, pfs::string> result_code;
#endif


TEST_CASE("Constructors and assign operators", "[constr]") {

    int errcode = 2;
    result_code::string_type errstr("error");

    // Default constructor
    {
        result_code rc;
        REQUIRE(static_cast<int>(rc) == 0);
        REQUIRE(rc.what().empty());
    }

    {
        result_code rc(errcode, errstr);
        REQUIRE(static_cast<int>(rc) == errcode);
        REQUIRE(rc.what() == errstr);
    }

    // Copy constructor
    {
        result_code rs_sample(errcode, errstr);
        result_code rc(rs_sample);
        REQUIRE(static_cast<int>(rc) == errcode);
        REQUIRE(rc.what() == errstr);
    }

    // Copy assignment
    {
        result_code rc_sample(errcode, errstr);
        result_code rc;
        rc = rc_sample;
        REQUIRE(static_cast<int>(rc) == errcode);
        REQUIRE(rc.what() == errstr);
    }

#if __cplusplus >= 201103L
    // Move constructor
    {
        result_code rc_sample(errcode, errstr);
        result_code rc(std::move(rc_sample));
        REQUIRE(static_cast<int>(rc) == errcode);
        REQUIRE(rc.what() == errstr);
        REQUIRE(rc_sample.what().empty());
    }

    // Move assignment
    {
        result_code rc_sample(errcode, errstr);
        result_code rc;
        rc = std::move(rc_sample);
        REQUIRE(static_cast<int>(rc) == errcode);
        REQUIRE(rc.what() == errstr);
        REQUIRE(rc_sample.what().empty());
    }
#endif
}

