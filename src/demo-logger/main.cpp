#include <iostream>
#include "pfs/string.hpp"
#include "pfs/safeformat.hpp"
#include "pfs/logger.hpp"

typedef pfs::log<>  log_ns;
typedef pfs::string string_type;
typedef pfs::safeformat fmt;
// typedef pfs::logger::stdout_appender<string_type>    stdout_appender;
// typedef pfs::logger::stderr_appender<string_type>    stderr_appender;
// typedef logger::appender_type                        appender_type;

int main ()
{
    log_ns::logger log;
    log.set_priority(log_ns::priority::trace);
    log.connect(log.add_appender<log_ns::stdout_appender>());
    log.trace(fmt("%s, %s!")("Hello")("World").str());
    string_type s(fmt("%s, %s!")("Hello")("World").str());
    log.trace(fmt("%s")(s).str());

    log_ns::stdout_appender out_appender;
    log_ns::stderr_appender err_appender;

    std::cout << "--All messages will be print with date as ABSOLUTE specifier:\n";

    out_appender.set_pattern("%d{ABSOLUTE} [%p]: %m");
    err_appender.set_pattern("%d{ABSOLUTE} [%p]: %m");
    log.disconnect_all();
    log.set_priority(log_ns::priority::trace);
    log.connect(log_ns::priority::trace, out_appender);
    log.connect(log_ns::priority::debug, out_appender);
    log.connect(log_ns::priority::info, out_appender);
    log.connect(log_ns::priority::warn, err_appender);
    log.connect(log_ns::priority::error, err_appender);
    log.trace("logging trace");
    log.debug("logging debug");
    log.info("logging info");
    log.warn("logging warn");
    log.error("logging error");


    std::cout << "--All messages will be print excluding Trace and Debug with date as DATE specifier:\n";

    out_appender.set_pattern("%d{DATE} [%p]: %m");
    err_appender.set_pattern("%d{DATE} [%p]: %m");
    log.set_priority(log_ns::priority::info);
    log.trace("logging trace");
    log.debug("logging debug");
    log.info("logging info");
    log.warn("logging warn");
    log.error("logging error");


    std::cout << "--Only Warn and Error level messages will be print with date as ISO8601 specifier:\n";

    out_appender.set_pattern(pfs::string("%d{ISO8601} [%p]: %m"));
    err_appender.set_pattern(pfs::string("%d{ISO8601} [%p]: %m"));

    log.set_priority(log_ns::priority::warn);
    log.trace("logging trace");
    log.debug("logging debug");
    log.info("logging info");
    log.warn("logging warn");
    log.error("logging error");

    log.set_priority(log_ns::priority::trace);

    out_appender.set_pattern("%d{%d/%m/%Y %H:%M:%S} [%p]: %m");
    log.trace("This is a message dated with format dd/mm/yyyy hh:mm:ss");

    out_appender.set_pattern("%d{ABSOLUTE} [%p]: {%20.30m}");
    log.trace("This is a truncated message of logging with Trace priority");

    out_appender.set_pattern("%d{ABSOLUTE} [%p]: {%30m}");
    log.trace("Left padding");

    out_appender.set_pattern("%d{ABSOLUTE} [%p]: {%-30m}");
    log.trace("Right padding");

    return EXIT_SUCCESS;
}
