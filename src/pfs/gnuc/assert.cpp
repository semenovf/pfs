/* 
 * File:   assert.cpp
 * Author: wladt
 *
 * Created on January 26, 2017, 3:57 PM
 */

#include <stdio.h>
#include <unistd.h>
#include <cstdlib> // abort()
#include <execinfo.h>
#include "pfs/assert.hpp"

template <>
void pfs_backtrace<char const *> (char const * file, int line, char const * text)
{
    fprintf(stdout, "***BACKTRACE: (%s[%d]): %s\n", file, line, text ? text : "?");
	void * bt_buffer[PFS_GNUC_BACKTRACE_SIZE];
	int bt_size = backtrace(bt_buffer, PFS_GNUC_BACKTRACE_SIZE);
	fprintf(stdout, "===BEGIN OF BACKTRACE\n");
	backtrace_symbols_fd(bt_buffer, bt_size, PFS_GNUC_BACKTRACE_FD);
	fprintf(stdout, "===END OF BACKTRACE\n");
    fsync(STDOUT_FILENO);
    fsync(STDERR_FILENO);
}

template <>
void pfs_assert<char const *> (char const * file, int line, char const * text)
{
	fprintf(stderr, "***ERROR: (%s[%d]): %s\n", file, line, text ? text : "?");
	void * bt_buffer[PFS_GNUC_BACKTRACE_SIZE];
	int bt_size = backtrace(bt_buffer, PFS_GNUC_BACKTRACE_SIZE);
	fprintf(stderr, "===BEGIN OF BACKTRACE\n");
	backtrace_symbols_fd(bt_buffer, bt_size, PFS_GNUC_ASSERT_BACKTRACE_FD);
	fprintf(stderr, "===END OF BACKTRACE\n");
    fsync(STDOUT_FILENO);
    fsync(STDERR_FILENO);
	abort();
}

template <>
void pfs_check_warn<char const *> (char const * file, int line, char const * text)
{
    fprintf(stderr, "***WARN: (%s[%d]): %s\n", file, line, text ? text : "?");
}

template <>
void pfs_check_error<char const *> (char const * file, int line, char const * text)
{
    fprintf(stderr, "***ERROR: (%s[%d]): %s\n", file, line, text ? text : "?");
}
