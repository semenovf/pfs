#include <pfs/result_status.hpp>
#include "../catch.hpp"

#if __cplusplus >= 201103L
    using result_status = pfs::result_status<pfs::string>;
#else
    typedef pfs::result_status<pfs::string> result_status;
#endif


TEST_CASE("Constructors and assign operators", "[constr]") {
    result_status::string_type errstr("error");

    // Default constructor
    {
        result_status rs;
        REQUIRE(static_cast<bool>(rs) == true);
        REQUIRE(rs.what().empty());
    }

    {
        result_status rs(errstr);
        REQUIRE(static_cast<bool>(rs) == false);
        REQUIRE(rs.what() == errstr);
    }

    // Copy constructor
    {
        result_status rs_sample(errstr);
        result_status rs(rs_sample);
        REQUIRE(static_cast<bool>(rs) == false);
        REQUIRE(rs.what() == errstr);
    }

    // Copy assignment
    {
        result_status rs_sample(errstr);
        result_status rs;
        REQUIRE(static_cast<bool>(rs) == true);
        rs = rs_sample;
        REQUIRE(static_cast<bool>(rs) == false);
        REQUIRE(rs.what() == errstr);
    }

#if __cplusplus >= 201103L
    {
        result_status::string_type errstr_sample(errstr);
        result_status rs(std::move(errstr_sample));
        REQUIRE(static_cast<bool>(rs) == false);
        REQUIRE(rs.what() == errstr);
        REQUIRE(errstr_sample.empty());
    }

    // Move constructor
    {
        result_status rs_sample(errstr);
        result_status rs(std::move(rs_sample));
        REQUIRE(static_cast<bool>(rs) == false);
        REQUIRE(rs.what() == errstr);
        REQUIRE(rs_sample.what().empty());
    }

    // Move assignment
    {
        result_status rs_sample(errstr);
        result_status rs;
        rs = std::move(rs_sample);
        REQUIRE(static_cast<bool>(rs) == false);
        REQUIRE(rs.what() == errstr);
        REQUIRE(rs_sample.what().empty());
    }
#endif
}
