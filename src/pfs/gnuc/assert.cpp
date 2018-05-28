#include <cstdio>
#include <cstdlib> // abort()
#include <unistd.h>
#include <execinfo.h>
#include "pfs/assert.hpp"

void pfs_backtrace (char const * file, int line, char const * text)
{
    fprintf(stdout, "***BACKTRACE: (%s[%d]): %s\n", file, line, text ? text : "?");
    void * bt_buffer[PFS_GNUC_BACKTRACE_MAXSIZE];
    int bt_size = backtrace(bt_buffer, PFS_GNUC_BACKTRACE_MAXSIZE);
    fprintf(stdout, "===BEGIN OF BACKTRACE\n");
    backtrace_symbols_fd(bt_buffer, bt_size, PFS_GNUC_BACKTRACE_FD);
    fprintf(stdout, "===END OF BACKTRACE\n");
    fsync(STDOUT_FILENO);
    fsync(STDERR_FILENO);
}

void pfs_assert (char const * file, int line, char const * text)
{
    fprintf(stderr, "***ERROR: (%s[%d]): %s\n", file, line, text ? text : "?");
    void * bt_buffer[PFS_GNUC_BACKTRACE_MAXSIZE];
    int bt_size = backtrace(bt_buffer, PFS_GNUC_BACKTRACE_MAXSIZE);
    fprintf(stderr, "===BEGIN OF BACKTRACE\n");
    backtrace_symbols_fd(bt_buffer, bt_size, PFS_GNUC_ASSERT_BACKTRACE_FD);
    fprintf(stderr, "===END OF BACKTRACE\n");
    fsync(STDOUT_FILENO);
    fsync(STDERR_FILENO);
    abort();
}

void pfs_check_warn (char const * file, int line, char const * text)
{
    fprintf(stderr, "***WARN: (%s[%d]): %s\n", file, line, text ? text : "?");
}

void pfs_check_error (char const * file, int line, char const * text)
{
    fprintf(stderr, "***ERROR: (%s[%d]): %s\n", file, line, text ? text : "?");
}
