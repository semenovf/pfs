/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test.hpp>
#include <pfs.hpp>
#include <pfs/traits/stdcxx/string.hpp>
#include <pfs/traits/stdcxx/string_builder.hpp>
#include <pfs/traits/stdcxx/list.hpp>
#include <pfs/fs/path.hpp>

typedef pfs::fs::traits<std::string, stdcxx::list> traits_type;
typedef pfs::fs::path<traits_type>   path_type;
typedef typename traits_type::string_t string_type;

#include "test_path.hpp"
#include "test_canonical.hpp"

int main (int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

  	test::base();
	test::canonical();
    
	return END_TESTS;
}
