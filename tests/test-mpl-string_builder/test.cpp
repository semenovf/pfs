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
#include <pfs/foundation/cxx/stdcxx/string.hpp>
#include <pfs/foundation/cxx/stdcxx/string_builder.hpp>

// Enabled by `qt_enable`
#ifdef QT_CORE_LIB
#   include <pfs/foundation/cxx/qt/string.hpp>
#   include <pfs/foundation/cxx/qt/string_builder.hpp>
#endif

template <typename CharT>
void test_basic ()
{
    ADD_TESTS(2);

    typedef pfs::traits::string<CharT>         string_type;
    typedef pfs::traits::string_builder<CharT> string_builder;
    
    string_builder sb;
    sb.push_back("Hello");
    sb.push_back(',');
    sb.push_back(1, ' ');
    sb.push_back(string_type("World"));
    sb.push_back("! abracadabra", 1);
    sb.push_back(string_type(" Yeh!"));

    TEST_OK(sb.template str<string_type>() == string_type("Hello, World! Yeh!"));
    TEST_OK(sb.template str<string_type>() != string_type("Hello, World! Yeh! Cool!"));
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

