#include <iostream>
#include "pfs/string.hpp"
#include "pfs/traits/stdcxx/string.hpp"
#include "pfs/safeformat.hpp"
#include "pfs/logger.hpp"

typedef pfs::string<pfs::traits::stdcxx::string>     string_type;
typedef pfs::safeformat<string_type>                 fmt;
typedef pfs::logger::logger<string_type>             logger;
typedef pfs::logger::stdout_appender<string_type>    stdout_appender;
typedef pfs::logger::stderr_appender<string_type>    stderr_appender;
typedef logger::appender_type                        appender_type;

int main (int, char * [])
{
    logger log;
    log.set_priority(pfs::logger::priority::trace);
    log.connect(log.add_appender<stdout_appender>());
    log.trace(fmt("%s, %s!")("Hello")("World").str());
    string_type s(fmt("%s, %s!")("Hello")("World").str());
    log.trace(fmt("%s")(s).str());

    stdout_appender out_appender;
    stderr_appender err_appender;
    out_appender.set_pattern("%d{ABSOLUTE} [%p]: %m");
    err_appender.set_pattern("%d{ABSOLUTE} [%p]: %m");

    std::cout << "--All messages will be print with date as ABSOLUTE specifier:\n";

    log.disconnect_all();
    log.set_priority(pfs::logger::priority::trace);
    log.connect(pfs::logger::priority::trace, out_appender);
    log.connect(pfs::logger::priority::debug, out_appender);
    log.connect(pfs::logger::priority::info, out_appender);
    log.connect(pfs::logger::priority::warn, err_appender);
    log.connect(pfs::logger::priority::error, err_appender);

    log.trace("logging trace");
    log.debug("logging debug");
    log.info("logging info");
    log.warn("logging warn");
    log.error("logging error");
    
    out_appender.set_pattern("%d{DATE} [%p]: %m");
    err_appender.set_pattern("%d{DATE} [%p]: %m");

    std::cout << "--All messages will be print excluding Trace and Debug with date as DATE specifier:\n";
    
    log.set_priority(pfs::logger::priority::info);
    log.trace("logging trace");
    log.debug("logging debug");
    log.info("logging info");
    log.warn("logging warn");
    log.error("logging error");

#if __COMMENT__

    stdout_appender.set_pattern(pfs::string("%d{ISO8601} [%p]: %m"));
    stderr_appender.set_pattern(pfs::string("%d{ISO8601} [%p]: %m"));
    printf("--Only Warn and Error level messages will be print with date as ISO8601 specifier:\n");
    log::set_priority(log::warn_priority);
    trace("logging trace");
    debug("logging debug");
    info("logging info");
    warn("logging warn");
    error("logging error");

    log::set_priority(log::trace_priority);

    stdout_appender.set_pattern(_l1("%d{%d/%m/%Y %H:%M:%S} [%p]: %m"));
    trace("This is a message dated with format dd/mm/yyyy hh:mm:ss");
    stdout_appender.set_pattern(_l1("%d{ABSOLUTE} [%p]: {%20.30m}"));
    trace("This is a truncated message of logging with Trace priority");
    stdout_appender.set_pattern(_l1("%d{ABSOLUTE} [%p]: {%30m}"));
    trace("Left padding");
    stdout_appender.set_pattern(_l1("%d{ABSOLUTE} [%p]: {%-30m}"));
    trace("Right padding");
#endif

    return EXIT_SUCCESS;
}
