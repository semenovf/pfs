/**
 * @file   uri.cpp
 * @author wladt
 * @date   Oct 12, 2012 11:18:36 AM
 *
 * @brief URI testing
 */

#include "pfs/test/test.hpp"
#include "pfs/string.hpp"
#include "pfs/net/uri.hpp"
#include "pfs/net/uri_parse.hpp"
#include "pfs/traits/stdcxx/string.hpp"

// Enabled by `qt_enable`
#if PFS_HAVE_QT
#   include "pfs/traits/qt/string.hpp"
#endif

#include "test_grammar.hpp"

namespace stdcxx {

typedef pfs::string<pfs::traits::stdcxx::string> string_type;
typedef pfs::net::uri<string_type> uri;
typedef pfs::net::uri_grammar<uri> uri_grammar;

} // stdcxx

#if PFS_HAVE_QT

namespace qt {

typedef pfs::string<pfs::traits::qt::string> string_type;
typedef pfs::net::uri<string_type> uri;
typedef pfs::net::uri_grammar<uri> uri_grammar;

} // qt

#endif // PFS_HAVE_QT

#if __COMMENT__

static pfs::fsm::test_entry __test_entries[] = {


		/* "!" / "$" / "&" / "'" / "(" / ")"
		       / "*" / "+" / "," / ";" / "=" */
		, { VHEADER(sub_delims_tr)
			, { "!", "$", "&", "'", VNULL }
			, { INULL }}

		, { VHEADER(sub_delims_tr)
			, { "(", ")", "*", "+", VNULL }
			, { INULL }}

		, { VHEADER(sub_delims_tr)
			, { ",", ";", "=", VNULL }
			, {   {-1, ":" }
				, {-1, "~" }
				, {-1, "\\" }
				, INULL }}


		/* unreserved / pct-encoded / sub-delims / ":" / "@" */
		, { VHEADER(pchar_tr)
			, { "A", ":", "&", "%DE", VNULL }
			, {   {-1, "%AR" }
				, {-1, "[" }
				, INULL }}

		/* 1*pchar */
		, { VHEADER(segment_nz_tr)
			, { "ABCDE", "@", "@:", "%DE", VNULL }
			, { { 3, "%BE%AR" }
				, {-1, "}" }
				, {-1, "%AR" }
				, INULL}}


		/* 1*( unreserved / pct-encoded / sub-delims / "@" )
		   		; non-zero-length segment without any colon ":" */
		, { VHEADER(segment_nz_nc_tr)
			, { "@", "@@", "%DE%AD", "$@;", VNULL }
			, { { -1, "{}" }
				, INULL }}

		/* segment-nz-nc *( "/" segment ) */
		, { VHEADER(path_noscheme_tr)
			, { "%DE%AD", "name@domain", "name@domain/%DE%AD%BE%EF", VNULL }
			, { {12, "name@domain/{}" }
				, INULL }}

		/* 1*4HEXDIG */
		, { VHEADER(h16_tr)
			, { "A", "AB", "ABC", "ABCD", VNULL }
			, {   {-1, "WBCD" }
				, { 1, "AWCD" }
				, { 2, "ABWD" }
				, { 3, "ABCW" }
				, INULL }}


		/* dec-octet "." dec-octet "." dec-octet "." dec-octet */
		, { VHEADER(ipv4address_tr)
			, { "192.168.0.1", "255.255.255.0", "127.0.0.1" }
			, {   { -1, "W92.168.0.1" }
				, { -1, "192.168.0" }
				, { -1, "192.168" }
				, { -1, "192" }
				, INULL } }

		/* [ h16 ] "::" 4( h16 ":" ) ls32 */
		, { VHEADER(ipv6address_tr_3)
			, { "::1B:2C:3D:4E:192.168.1.1", "AB::1B:2C:3D:4E:192.168.1.1", VNULL }
			, { { -1, "AR::1B:2C:3D:4E:192.168.1.1" }, INULL } }

		/* *1( h16 ":" ) h16 */
		, { VHEADER(ipv6address_tr_4_1)
			, { "AB", "AB:CD", VNULL }
			, {   {-1, ":AB" }
			    , { 2, "AB:??" }
				, INULL }}

		/* [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32 */
		, { VHEADER(ipv6address_tr_4)
			, {   "::1B:2C:3D:192.168.1.1"
				, "AB::01:23:45:192.168.1.1"
				, "AB:CD::1B:2C:3D:192.168.1.1", VNULL }
			, {   { -1, "AR::1B:2C:3D:4E:192.168.1.1" }
			 	, { -1, "AB:BC::1B:2C:192.168.1.1" }
				, INULL } }


		/* *2( h16 ":" ) h16 */
		, { VHEADER(ipv6address_tr_5_1)
			, {"AB", "AB:CD", "AB:CD:EF", VNULL }
			, {   {-1, "$F" }
			    , { 2, "AB:??" }
				, INULL }}

		/* *3( h16 ":" ) h16 */
		, { VHEADER(ipv6address_tr_6_1)
			, {"AB", "AB:CD", "AB:CD:EF", "AB:CD:EF:01", VNULL }
			, { {-1, "$F" }
				, INULL }}

		/*
									     6( h16 ":" ) ls32
			/                       "::" 5( h16 ":" ) ls32
			/ [               h16 ] "::" 4( h16 ":" ) ls32
			/ [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
			/ [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
			/ [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
			/ [ *4( h16 ":" ) h16 ] "::"              ls32
			/ [ *5( h16 ":" ) h16 ] "::"              h16
			/ [ *6( h16 ":" ) h16 ] "::"
		*/
		, { VHEADER(ipv6address_tr)
			, {   "AB:CD:EF:01:23:45:192.168.1.1"
				, "::CD:EF:01:23:45:192.168.1.1"
				, "AB::CD:EF:01:23:192.168.1.1"
				, VNULL }
			, { INULL }
		}

		/* "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" ) */
		, { VHEADER(ipvfuture_tr)
			, {   "vAB.:"
				, "vAB.::::::::"
				, VNULL }
				, { {-1, "vA."}
				, INULL }}

		/* "[" ( IPv6address / IPvFuture  ) "]" */
		, { VHEADER(ip_literal_tr)
			, {   "[::CD:EF:01:23:45:192.168.1.1]"
				, "[vAB.::::::::]"
				, VNULL }
				, { {-1, "[?]"}
				, INULL }}

		/* *( unreserved / pct-encoded / sub-delims ) */
		, { VHEADER(reg_name_tr)
			, {   ""
				, "%AB"
				, "%AB%CD%EF"
				, "Hello_World."
				, VNULL }
				, { {0, "?AB"}
				, { 3, "%AB%"}
				, INULL }}

		/*  host = IP-literal / IPv4address / reg-name */
		, { VHEADER(host_tr)
			, {   "[::CD:EF:01:23:45:192.168.1.1]"
				, "192.168.1.1"
				, "~domain.com"
				, VNULL }
				, { {0, "?AB"}
				, { 3, "%AB%"}
				, INULL }}

		/* ":" port */
		, { VHEADER(authority_tr_2)
			, { ":2", ":25", ":"
				, VNULL }
				, { {1, ":AB"}
				, {-1, "AB"}
				, INULL }}

		/* [ userinfo "@" ] host [ ":" port ] */
		/* [ authority_tr_1 ] host [ authority_tr_2 ] */
		, { VHEADER(authority_tr)
			, { "192.168.1.1", "192.168.1.1:", "192.168.1.1:25", "user@192.168.1.1"
				, VNULL }
				, { INULL }}

		/*  *( "/" segment ) */
		, { VHEADER(path_abempty_tr)
			, { "/", "/segment", "/path/to", "/path/to/", VNULL }
				, { INULL }}

        /* relative-part = "//" authority path-abempty
		              / path-absolute
		              / path-noscheme
		              / path-empty
		*/
		, { VHEADER(relative_part_tr)
			, { "//user@host", "//user@host/", VNULL }
				, { INULL }}

		/* *( pchar / "/" / "?" ) */
		, { VHEADER(query_tr)
			, { "/?", "/", "?query%20string", VNULL }
				, { INULL }}

		/* relative-part [ "?" query ] [ "#" fragment ] */
		, { VHEADER(relative_ref_tr)
			, { "//user@host/?query%20string", "#fragment%20string"
				, "//user@host/?query%20string#fragment%20string"
				, VNULL }
				, { INULL }}

		/* ALPHA *( ALPHA / DIGIT / "+" / "-" / "." ) */
		, { VHEADER(scheme_tr)
			, { "a", "aA", "A2", "A+"
				, VNULL }
				, {   {-1, "2a" }
					, {-1, "+A" }
					, {-1, ".A" }
					, {-1, "-a" }
				    , INULL }}

		/* scheme ":" hier-part [ "?" query ] [ "#" fragment ] */
		, { VHEADER(uri_tr)
		, { "http://user@host/?query%20string", "http://user@host/#fragment%20string"
			, "ftp://user@host/path/to?query%20string#fragment%20string"
			, VNULL }
			, { INULL }}

};

#endif

//void test_uri_parse()
//{
//	ADD_TESTS(23);
//
//	char uri_string[512];
//
//	string_type scheme("https");
//	string_type userinfo("user");
//	string_type host("192.168.1.1");
//	uint16_t port = 25;
//	string_type path("/path/to");
//	string_type query("query%20string");
//	string_type fragment("fragment%20string");
//
//	sprintf(uri_string
//		, "%s://%s@%s:%u%s?%s#%s"
//		, scheme.c_str()
//		, userinfo.c_str()
//		, host.c_str()
//		, port
//		, path.c_str()
//		, query.c_str()
//		, fragment.c_str());
//
//	pfs::uri uri;
//	TEST_OK(uri.parse(string_type(uri_string)));
//
//	TEST_FAIL(uri.scheme() == scheme);
//	TEST_FAIL(uri.userinfo() == userinfo);
//	TEST_FAIL(uri.host() == host);
//	TEST_FAIL(uri.path() == path);
//	TEST_FAIL(uri.query() == query);
//	TEST_FAIL(uri.fragment() == fragment);
//	TEST_OK(uri.host_is_ip() == true);
//	TEST_OK(uri.port() == port);
//
//	string_type uri_result = pfs::to_string(uri);
//
////	printf("uri_string: %s\n", uri_string);
////	printf("uri_result: %s\n", uri_result.c_str());
//	TEST_OK(uri_result == string_type(uri_string));
//
//	TEST_FAIL(uri.parse(_u8("file:/tmp/text.txt")));
//	TEST_OK(uri.scheme() == _u8("file"));
//	TEST_OK(uri.path() == _u8("/tmp/text.txt"));
//
//	TEST_FAIL(uri.parse(_u8("scheme:relative/path?query#fragment")));
//	TEST_OK(uri.scheme() == _u8("scheme"));
//	TEST_OK(uri.userinfo().empty());
//	TEST_OK(uri.host().empty());
//	TEST_OK(uri.port() == 0);
//	TEST_OK(uri.path() == _u8("relative/path"));
//	TEST_OK(uri.query() == _u8("query"));
//	TEST_OK(uri.fragment() == _u8("fragment"));
//
//	TEST_OK(uri.parse(_u8("http://ru.indeed.com/%D0%A2%D0%B5%D1%81%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D1%89%D0%B8%D0%BA-%D0%92%D0%B5%D0%B1-jobs")));
//	TEST_OK(uri.parse(_u8("https://webcache.googleusercontent.com/search?client=ubuntu&channel=fs&q=cache:Aaap4fYxHwAJ:http://toriava.ru/category/11/47/b1067/b2047/%2BCrystalMedia+%D0%B5%D0%BA%D0%B0%D1%82%D0%B5%D1%80%D0%B8%D0%BD%D0%B1%D1%83%D1%80%D0%B3&oe=utf-8&redir_esc=&hl=ru&ct=clnk")));
//}

int main(int argc, char *argv[])
{
	PFS_UNUSED2(argc, argv);

	BEGIN_TESTS(0);

	test_grammar<stdcxx::uri_grammar>();
    
#if PFS_HAVE_QT
    test_grammar<qt::uri_grammar>();
#endif

	return END_TESTS;
}
