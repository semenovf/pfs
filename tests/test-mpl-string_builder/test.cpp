/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs.hpp>
#include <pfs/test/test.hpp>
#include <cstring>
#include <pfs/foundation/stdcxx/string.hpp>
#include <pfs/foundation/stdcxx/string_builder.hpp>

// Enabled by `qt_enable`
#ifdef QT_CORE_LIB
#   include <pfs/foundation/qt/string.hpp>
#   include <pfs/foundation/qt/string_builder.hpp>
#endif

template <typename CharT>
void test_basic ()
{
    ADD_TESTS(3);

    typedef pfs::traits::string<CharT>         string_type;
    typedef pfs::traits::string_builder<CharT> string_builder;
    
    string_builder sb;
    sb.push_back("Hello");
    sb.push_back(',');
    sb.push_back(' ');
    sb.push_back(std::string("World"));
    sb.push_back('!');
    sb.push_back(string_type(" Yeh!"));

	ADD_TESTS(3);
    
    TEST_OK(sb.template str<std::string>() == "Hello, World! Yeh!");
    TEST_OK(sb.template str<string_type>() == string_type("Hello, World! Yeh!"));
    TEST_OK(std::strcmp(sb.template str<char const *>(), "Hello, World! Yeh!") == 0);
}

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);
  
    test_basic<char>();
    
#ifdef QT_CORE_LIB
    test_basic<QChar>();
#endif
    
	return END_TESTS;
}

