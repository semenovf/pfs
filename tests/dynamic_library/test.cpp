/**
 * @file   test.cpp
 * @author wladt
 * @date   Aug 18, 2017
 *
 * @brief Dynamic library testing
 */
#include <iostream>
#include <pfs/test/test.hpp>
#include <pfs/filesystem.hpp>
#include <pfs/dynamic_library.hpp>

using pfs::dynamic_library;
using pfs::filesystem::path;
using pfs::filesystem::pathlist;

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(4);

	typedef int (*dl_test_fn)(void);

	dynamic_library dl;
	path dlfile = pfs::build_so_filename("shared_object");
	path * dlpath_ptr = 0;

	pathlist searchdirs;

	searchdirs.push_back(path("."));
	searchdirs.push_back(path(".."));

    path dlpath = pfs::filesystem::search_path(dlfile, searchdirs);
    
	if (!dlpath.empty()) {
		dlpath_ptr = & dlfile;
	}

	TEST_FAIL2(dlpath_ptr != 0, "Candidate for dynamic library found");

	pfs::error_code ec;

	bool rc = dl.open(*dlpath_ptr, ec);

	if (!rc) {
		std::cerr << pfs::to_string<typename path::string_type>(ec) << std::endl;
	}

	TEST_FAIL2(rc, "Open/load library");

	dl_test_fn dltest = pfs::void_func_ptr_cast<dl_test_fn>(dl.resolve("dl_only_for_testing_purpose"
			, ec));

	if (ec) {
		std::cerr << pfs::to_string<typename path::string_type>(ec) << std::endl;
	}

	TEST_FAIL2(!ec, "'dl_only_for_testing_purpose': symbol (function pointer) found");

	TEST_OK2(dltest() == 1234, "run plugin/library function");

	return END_TESTS;
}
