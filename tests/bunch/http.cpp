#include "pfs/net/http.hpp"
#include <map>
#include <string>
#include "../catch.hpp"

using namespace pfs::net;

TEST_CASE("HTTP / DIGIT") {
    char digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

    for (char * p = digits
            , * last = p + sizeof(digits) / sizeof(digits[0])
            ; p != last; p++)
        CHECK(proto::is_digit(*p));

    CHECK_FALSE(proto::is_digit('A'));
    CHECK_FALSE(proto::is_digit('a'));
    CHECK_FALSE(proto::is_digit('Z'));
    CHECK_FALSE(proto::is_digit('z'));
}

TEST_CASE("HTTP / Control charactares") {
    for (int i = 0; i <= 31; i++)
        CHECK(proto::is_control(char(i)));

    CHECK_FALSE(proto::is_control(' '));
    CHECK_FALSE(proto::is_control('A'));
    CHECK_FALSE(proto::is_control('a'));
    CHECK_FALSE(proto::is_control('Z'));
    CHECK_FALSE(proto::is_control('z'));
}

TEST_CASE("HTTP / separator charactares") {
    char separators[] = { ')', '<', '>', '@', ',', ';', ':', '\\', '"', '/'
            , '[', ']', '?', '=', '{', '}', ' ', '\t' };

    for (char * p = separators
            , * last = p + sizeof(separators) / sizeof(separators[0])
            ; p != last; p++)
        CHECK(proto::is_separator(*p));

    CHECK_FALSE(proto::is_separator('A'));
    CHECK_FALSE(proto::is_separator('a'));
    CHECK_FALSE(proto::is_separator('Z'));
    CHECK_FALSE(proto::is_separator('z'));
}

TEST_CASE("HTTP / compare") {
    std::string empty_string = "";
    std::string abc_string = "abc";

    CHECK(proto::compare(empty_string.begin(), empty_string.end(), "") == 0);
    CHECK(proto::compare(empty_string.begin(), empty_string.end(), "a") < 0);
    CHECK(proto::compare(abc_string.begin(), abc_string.end(), "") > 0);
    CHECK(proto::compare(abc_string.begin(), abc_string.end(), "abc") == 0);
    CHECK(proto::compare(abc_string.begin(), abc_string.end(), "ab") > 0);
    CHECK(proto::compare(abc_string.begin(), abc_string.end(), "b") < 0);
    CHECK(proto::casecompare(abc_string.begin(), abc_string.end(), "ABC") == 0);
    CHECK(proto::casecompare(abc_string.begin(), abc_string.end(), "AB") > 0);
    CHECK(proto::casecompare(abc_string.begin(), abc_string.end(), "B") < 0);
}

TEST_CASE("HTTP / parse CRLF") {
    std::string crlf1("\n");
    std::string crlf2("\r\n");
    std::string crlf_invalid("Abc");

    std::string::iterator pos = crlf1.begin();
    CHECK(proto::advance_crlf(pos, crlf1.end()));
    CHECK(pos == crlf1.end());

    pos = crlf2.begin();
    CHECK(proto::advance_crlf(pos, crlf2.end()));
    CHECK(pos == crlf2.end());

    pos = crlf_invalid.begin();
    CHECK_FALSE(proto::advance_crlf(pos, crlf_invalid.end()));
    CHECK(pos == crlf_invalid.begin());

}

TEST_CASE("HTTP / parse HTTP version") {
    std::string http_1_0("HTTP/1.0");
    std::string http_1_1("HTTP/1.1");
    std::string http_2_4("HTTP/2.4");
    std::string http_invalid1("TTP/1.0");
    std::string http_invalid2("HTTP / 1.0");

    std::string::iterator pos = http_1_0.begin();
    CHECK(proto::advance_proto_version(pos, http_1_0.end()));

    pos = http_1_1.begin();
    CHECK(proto::advance_proto_version(pos, http_1_1.end()));

    pos = http_2_4.begin();
    CHECK(proto::advance_proto_version(pos, http_2_4.end()));

    pos = http_invalid1.begin();
    CHECK_FALSE(proto::advance_proto_version(pos, http_invalid1.end()));

    pos = http_invalid2.begin();
    CHECK_FALSE(proto::advance_proto_version(pos, http_invalid2.end()));
}

TEST_CASE("HTTP / parse request line") {
    proto::request_line<std::string::iterator> r;

    std::string http_1_0("GET https://github.com/semenovf/pfs HTTP/1.0\r\n");
    std::string::iterator pos = http_1_0.begin();
    REQUIRE(proto::advance_request_line(pos, http_1_0.end(), & r));
    CHECK(std::string(r.method.first, r.method.second) == "GET");
    CHECK(std::string(r.uri.first, r.uri.second) == "https://github.com/semenovf/pfs");
    CHECK(r.proto == proto::HTTP_PROTO);
    CHECK(r.version.major == 1);
    CHECK(r.version.minor == 0);
    CHECK(pos == http_1_0.end());

    std::string http_1_1("POST https://github.com/semenovf/pfs HTTP/1.1\n");
    pos = http_1_1.begin();
    REQUIRE(proto::advance_request_line(pos, http_1_1.end(), & r));
    CHECK(std::string(r.method.first, r.method.second) == "POST");
    CHECK(std::string(r.uri.first, r.uri.second) == "https://github.com/semenovf/pfs");
    CHECK(r.proto == proto::HTTP_PROTO);
    CHECK(r.version.major == 1);
    CHECK(r.version.minor == 1);
    CHECK(pos == http_1_1.end());

    std::string http_invalid("POST https://github.com/semenovf/pfs HTTP/1.1");
    pos = http_invalid.begin();
    REQUIRE_FALSE(proto::advance_request_line(pos, http_invalid.end()));
}

TEST_CASE("HTTP / parse field") {
    proto::request_field<std::string::iterator> r;

    std::string field1("User-Agent: \n");
    std::string::iterator pos = field1.begin();
    REQUIRE(proto::advance_field(pos, field1.end(), & r));
    CHECK(std::string(r.name.first, r.name.second) == "User-Agent");
    CHECK(std::string(r.value.first, r.value.second) == "");
    CHECK(pos == field1.end());

    std::string field2("User-Agent: CERN-LineMode/2.15 libwww/2.17b3\n");
    pos = field2.begin();
    REQUIRE(proto::advance_field(pos, field2.end(), & r));
    CHECK(std::string(r.name.first, r.name.second) == "User-Agent");
    CHECK(std::string(r.value.first, r.value.second) == "CERN-LineMode/2.15 libwww/2.17b3");
    CHECK(pos == field2.end());

    std::string field_invalid("User-Agent \n");
    pos = field_invalid.begin();
    REQUIRE_FALSE(proto::advance_field(pos, field_invalid.end()));
}

TEST_CASE("HTTP / parse header") {
    typedef std::map<std::string, std::string> property_tree;
    property_tree headers;

    std::string request1 = "GET /live.ffm HTTP/1.1\n"
            "User-Agent: Lavf/57.83.100\n"
            "Accept: */*\n"
            "Range: bytes=0-\n"
            "Connection: close\n"
            "Host: localhost:9090\n"
            "Icy-MetaData: 1\n\n";
    std::string::iterator pos = request1.begin();
    REQUIRE(proto::advance_headers(pos, request1.end(), & headers));
    CHECK(headers["#method"] == "GET");
    CHECK(headers["#uri"] == "/live.ffm");
    CHECK(headers["#proto"] == "HTTP");
    CHECK(headers["#version"] == "1.1");
    CHECK(headers["user-agent"] == "Lavf/57.83.100");
    CHECK(headers["accept"] == "*/*");
    CHECK(headers["range"] == "bytes=0-");
    CHECK(headers["connection"] == "close");
    CHECK(headers["host"] == "localhost:9090");
    CHECK(headers["icy-metadata"] == "1");

    headers.clear();

    std::string request2 = "GET /live.ffm HTTP/1.1\n"
            "User-Agent: Lavf/57.83.100\n"
            "Accept: */*\n"
            "Range: bytes=0-\n"
            "Connection: close\n"
            "Host: localhost:9090\n"
            "Icy-MetaData: 1";
    pos = request2.begin();
    CHECK_FALSE(proto::advance_headers(pos, request2.end(), & headers));
}
