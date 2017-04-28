/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */
#include <pfs/test/test.hpp>
#include <pfs/lexical_cast.hpp>
#include <pfs/traits/stdcxx/string.hpp>

#ifdef QT_CORE_LIB
#   include <pfs/foundation/cxx/qt/string.hpp>
#endif

#include "test_string_to_uintmax.hpp"
#include "test_integer.hpp"

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

    {
        typedef pfs::traits::string<std::string> string_type;

        test_string_to_uintmax<string_type>();
//        test_integer<string_type>();
    }
            
#ifdef QT_CORE_LIB
    {
        typedef pfs::traits::string<foundation::qt, QChar> string_type;

        test_string_to_uintmax::test<string_type>();
        test_integer<string_type>();
    }
#endif
	
	return END_TESTS;
}

