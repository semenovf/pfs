#include "pfs/net/http.hpp"
#include <map>
#include <string>
#include "../catch.hpp"

using namespace pfs::net;

std::string header1("GET /path/index.html HTTP/1.0\n"
        "From: user@domain.com\n"
        "User-Agent: HTTPTool/1.0\n\n");

TEST_CASE("HTTP / DIGIT") {
    char digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

    for (char * p = digits
            , * last = p + sizeof(digits) / sizeof(digits[0])
            ; p != last; p++)
        CHECK(http::is_digit(*p));

    CHECK_FALSE(http::is_digit('A'));
    CHECK_FALSE(http::is_digit('a'));
    CHECK_FALSE(http::is_digit('Z'));
    CHECK_FALSE(http::is_digit('z'));
}

TEST_CASE("HTTP / Control charactares") {
    for (int i = 0; i <= 31; i++)
        CHECK(http::is_control(char(i)));

    CHECK_FALSE(http::is_control(' '));
    CHECK_FALSE(http::is_control('A'));
    CHECK_FALSE(http::is_control('a'));
    CHECK_FALSE(http::is_control('Z'));
    CHECK_FALSE(http::is_control('z'));
}

TEST_CASE("HTTP / separator charactares") {
    char separators[] = { ')', '<', '>', '@', ',', ';', ':', '\'', '"', '/'
            , '[', ']', '?', '=', '{', '}', ' ', '\t' };

    for (char * p = separators
            , * last = p + sizeof(separators) / sizeof(separators[0])
            ; p != last; p++)
        CHECK(http::is_separator(*p));

    CHECK_FALSE(http::is_separator('A'));
    CHECK_FALSE(http::is_separator('a'));
    CHECK_FALSE(http::is_separator('Z'));
    CHECK_FALSE(http::is_separator('z'));
}

TEST_CASE("HTTP / parse CRLF") {
    std::string crlf1("\n");
    std::string crlf2("\r\n");
    std::string crlf_invalid("Abc");

    std::string::iterator pos = crlf1.begin();
    CHECK(http::parse_crlf(pos, crlf1.end()));
    CHECK(pos == crlf1.end());

    pos = crlf2.begin();
    CHECK(http::parse_crlf(pos, crlf2.end()));
    CHECK(pos == crlf2.end());

    pos = crlf_invalid.begin();
    CHECK_FALSE(http::parse_crlf(pos, crlf_invalid.end()));
    CHECK(pos == crlf_invalid.begin());

}

TEST_CASE("HTTP / parse HTTP version") {
    std::string http_0_9("HTTP/0.9");
    std::string http_1_0("HTTP/1.0");
    std::string http_1_1("HTTP/1.1");
    std::string http_2_4("HTTP/2.4");
    std::string http_invalid1("TTP/1.0");
    std::string http_invalid2("HTTP / 1.0");

    std::string::iterator pos = http_0_9.begin();
    CHECK(http::parse_http_version(pos, http_0_9.end()));

    pos = http_1_0.begin();
    CHECK(http::parse_http_version(pos, http_1_0.end()));

    pos = http_1_1.begin();
    CHECK(http::parse_http_version(pos, http_1_1.end()));

    pos = http_2_4.begin();
    CHECK(http::parse_http_version(pos, http_2_4.end()));

    pos = http_invalid1.begin();
    CHECK_FALSE(http::parse_http_version(pos, http_invalid1.end()));

    pos = http_invalid2.begin();
    CHECK_FALSE(http::parse_http_version(pos, http_invalid2.end()));
}

TEST_CASE("HTTP / parse request line") {
    http::request_line<std::string::iterator> r;

    std::string http_0_9("GET https://github.com/semenovf/pfs\n");
    std::string::iterator pos = http_0_9.begin();
    CHECK(http::parse_request_line(pos, http_0_9.end(), r));
    CHECK(std::string(r.method[0], r.method[1]) == "GET");
    CHECK(std::string(r.uri[0], r.uri[1]) == "https://github.com/semenovf/pfs");
    CHECK(std::string(r.version[0], r.version[1]).empty());
    CHECK(pos == http_0_9.end());

    std::string http_1_0("GET https://github.com/semenovf/pfs HTTP/1.0\r\n");
    pos = http_1_0.begin();
    CHECK(http::parse_request_line(pos, http_1_0.end(), r));
    CHECK(std::string(r.method[0], r.method[1]) == "GET");
    CHECK(std::string(r.uri[0], r.uri[1]) == "https://github.com/semenovf/pfs");
    CHECK(std::string(r.version[0], r.version[1]) == "HTTP/1.0");
    CHECK(pos == http_1_0.end());

    std::string http_1_1("POST https://github.com/semenovf/pfs HTTP/1.1\n");
    pos = http_1_1.begin();
    CHECK(http::parse_request_line(pos, http_1_1.end(), r));
    CHECK(std::string(r.method[0], r.method[1]) == "POST");
    CHECK(std::string(r.uri[0], r.uri[1]) == "https://github.com/semenovf/pfs");
    CHECK(std::string(r.version[0], r.version[1]) == "HTTP/1.1");
    CHECK(pos == http_1_1.end());

    std::string http_invalid("POST https://github.com/semenovf/pfs HTTP/1.1");
    pos = http_invalid.begin();
    CHECK_FALSE(http::parse_request_line(pos, http_invalid.end(), r));
}

TEST_CASE("HTTP / parse field") {
    http::request_field<std::string::iterator> r;

    std::string field1("User-Agent: \n");
    std::string::iterator pos = field1.begin();
    CHECK(http::parse_field(pos, field1.end(), r));
    CHECK(std::string(r.key[0], r.key[1]) == "User-Agent");
    CHECK(std::string(r.value[0], r.value[1]) == "");
    CHECK(pos == field1.end());

    std::string field2("User-Agent: CERN-LineMode/2.15 libwww/2.17b3\n");
    pos = field2.begin();
    CHECK(http::parse_field(pos, field2.end(), r));
    CHECK(std::string(r.key[0], r.key[1]) == "User-Agent");
    CHECK(std::string(r.value[0], r.value[1]) == "CERN-LineMode/2.15 libwww/2.17b3");
    CHECK(pos == field2.end());

    std::string field_invalid("User-Agent \n");
    pos = field_invalid.begin();
    CHECK_FALSE(http::parse_field(pos, field_invalid.end(), r));
}

TEST_CASE("HTTP / parse header") {
    typedef std::map<std::string, std::string> property_tree;
    property_tree header;

    std::string request1 = "GET /live.ffm HTTP/1.1\n"
            "User-Agent: Lavf/57.83.100\n"
            "Accept: */*\n"
            "Range: bytes=0-\n"
            "Connection: close\n"
            "Host: localhost:9090\n"
            "Icy-MetaData: 1\n\n";
    std::string::iterator pos = request1.begin();
    CHECK(http::parse_request(header, pos, request1.end()));
    CHECK(header["#method"] == "GET");
    CHECK(header["#uri"] == "/live.ffm");
    CHECK(header["#version"] == "HTTP/1.1");
    CHECK(header["User-Agent"] == "Lavf/57.83.100");
    CHECK(header["Accept"] == "*/*");
    CHECK(header["Range"] == "bytes=0-");
    CHECK(header["Connection"] == "close");
    CHECK(header["Host"] == "localhost:9090");
    CHECK(header["Icy-MetaData"] == "1");

    header.clear();

    std::string request2 = "GET /live.ffm HTTP/1.1\n"
            "User-Agent: Lavf/57.83.100\n"
            "Accept: */*\n"
            "Range: bytes=0-\n"
            "Connection: close\n"
            "Host: localhost:9090\n"
            "Icy-MetaData: 1";
    pos = request2.begin();
    CHECK_FALSE(http::parse_request(header, pos, request2.end()));
    CHECK(header.empty()); // No changes
}
