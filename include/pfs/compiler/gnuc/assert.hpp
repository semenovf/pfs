/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   assert.hpp
 * Author: wladt
 *
 * Created on January 26, 2017, 3:57 PM
 */

#ifndef __PFS_COMPILER_GNUC_ASSERT_HPP__
#define __PFS_COMPILER_GNUC_ASSERT_HPP__

#include <stdio.h>
#include <unistd.h>
#include <execinfo.h>

#define __TFILE__ __FILE__

#ifndef PFS_GNUC_BACKTRACE_SIZE
#	define PFS_GNUC_BACKTRACE_SIZE 20
#endif

// Default is standard error
#ifndef PFS_GNUC_ASSERT_BACKTRACE_FD
#	define PFS_GNUC_ASSERT_BACKTRACE_FD 2
#endif

// Default is standard output
#ifndef PFS_GNUC_BACKTRACE_FD
#	define PFS_GNUC_BACKTRACE_FD    1 
#endif

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

#endif /* __PFS_COMPILER_GNUC_ASSERT_HPP__ */

