#ifndef TEST_REFERENCE_WRAPPER_HPP
#define TEST_REFERENCE_WRAPPER_HPP

namespace test_reference_wrapper {

template <typename json_type>
void test ()
{
    ADD_TESTS(12);

    typedef pfs::json::reference_wrapper<json_type> ref_type;
    typedef pfs::json::reference_wrapper<json_type const> const_ref_type;
    json_type json; // null value
    //typename json_type::iterator it = json.begin();

    json["Image"]["Width"]               = 800;
    json["Image"]["Height"]              = 600;
    json["Image"]["Title"]               = "View from 15th Floor";
    json["Image"]["Thumbnail"]["Url"]    = "http://www.example.com/image/481989943";
    json["Image"]["Thumbnail"]["Height"] = 125;
    json["Image"]["Thumbnail"]["Width"]  = "100";

    json["Image"]["IDs"].push_back(116);
    json["Image"]["IDs"].push_back(943);
    json["Image"]["IDs"].push_back(234);
    json["Image"]["IDs"].push_back(38793);

    ref_type jref(json);

    TEST_FAIL(jref["Image"]);
    TEST_FAIL(jref["Image"]["Width"]);
    TEST_OK(jref["Image"]["Width"].template get<int>() == 800);
    TEST_OK(jref["Image"]["Width1"].template get<int>(-1) == -1);
    TEST_OK(jref["Image"]["IDs"][3].template get<uint32_t>() == 38793);
    TEST_OK(jref["Image"]["IDs"][4].template get<uint32_t>(1111) == 1111);

    const_ref_type jconstref(json);
    TEST_FAIL(jconstref["Image"]);
    TEST_FAIL(jconstref["Image"]["Width"]);
    TEST_OK(jconstref["Image"]["Width"].template get<int>() == 800);
    TEST_OK(jconstref["Image"]["Width1"].template get<int>(-1) == -1);
    TEST_OK(jconstref["Image"]["IDs"][3].template get<uint32_t>() == 38793);
    TEST_OK(jconstref["Image"]["IDs"][4].template get<uint32_t>(1111) == 1111);
}

} // test_reference_wrapper

#endif // TEST_REFERENCE_WRAPPER_HPP


