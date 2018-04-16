#pragma once
#include <pfs/fsm/fsm.hpp>
#include <pfs/lexical_cast.hpp>

namespace pfs {
namespace net {

/*
   RFC 3986: Uniform Resource Identifier (URI)
  -----------------------------------------------------------------------
   URI           = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

   hier-part     = "//" authority path-abempty
                 / path-absolute
                 / path-rootless
                 / path-empty

   URI-reference = URI / relative-ref

   absolute-URI  = scheme ":" hier-part [ "?" query ]

   relative-ref  = relative-part [ "?" query ] [ "#" fragment ]

   relative-part = "//" authority path-abempty
                 / path-absolute
                 / path-noscheme
                 / path-empty

   scheme        = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )

   authority     = [ userinfo "@" ] host [ ":" port ]
   userinfo      = *( unreserved / pct-encoded / sub-delims / ":" )
   host          = IP-literal / IPv4address / reg-name
   port          = *DIGIT

   IP-literal    = "[" ( IPv6address / IPvFuture  ) "]"

   IPvFuture     = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )

   IPv6address   =                            6( h16 ":" ) ls32
                 /                       "::" 5( h16 ":" ) ls32
                 / [               h16 ] "::" 4( h16 ":" ) ls32
                 / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
                 / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
                 / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
                 / [ *4( h16 ":" ) h16 ] "::"              ls32
                 / [ *5( h16 ":" ) h16 ] "::"              h16
                 / [ *6( h16 ":" ) h16 ] "::"

   h16           = 1*4HEXDIG
   ls32          = ( h16 ":" h16 ) / IPv4address
   IPv4address   = dec-octet "." dec-octet "." dec-octet "." dec-octet

   dec-octet     = DIGIT                 ; 0-9
                 / %x31-39 DIGIT         ; 10-99
                 / "1" 2DIGIT            ; 100-199
                 / "2" %x30-34 DIGIT     ; 200-249
                 / "25" %x30-35          ; 250-255

   reg-name      = *( unreserved / pct-encoded / sub-delims )

   path          = path-abempty    ; begins with "/" or is empty
                 / path-absolute   ; begins with "/" but not "//"
                 / path-noscheme   ; begins with a non-colon segment
                 / path-rootless   ; begins with a segment
                 / path-empty      ; zero characters

   slash-segment = "/" segment

   path-abempty  = *slash-segment
   path-absolute = "/" [ segment-nz *slash-segment ]
   path-noscheme = segment-nz-nc *slash-segment
   path-rootless = segment-nz *slash-segment
   path-empty    = 0<pchar>

   segment       = *pchar
   segment-nz    = 1*pchar
   segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" )
                 ; non-zero-length segment without any colon ":"

   pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"

   query         = *( pchar / "/" / "?" )

   fragment      = *( pchar / "/" / "?" )

   pct-encoded   = "%" HEXDIG HEXDIG

   unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
   reserved      = gen-delims / sub-delims
   gen-delims    = ":" / "/" / "?" / "#" / "[" / "]" / "@"
   sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
                 / "*" / "+" / "," / ";" / "="
  */

template <typename UriType>
struct uri_grammar
{
    typedef typename UriType::string_type        string_type;
    typedef typename string_type::const_iterator iterator;
    typedef fsm::fsm<iterator>                   fsm_type;
    typedef typename fsm_type::transition_type   transition_type;
    typedef typename fsm_type::char_type         value_type;
    typedef typename UriType::data_rep           parse_context;

    uri_grammar ();

    static bool set_scheme (iterator first, iterator last, void * context, void * /*action_args*/)
    {
        if (context) {
            parse_context * ctx = static_cast<parse_context *>(context);
            ctx->scheme = string_type(first, last);
        }
        return true;
    }

    static bool set_query (iterator first, iterator last, void * context, void * /*action_args*/)
    {
        if (context) {
            parse_context * ctx = static_cast<parse_context *>(context);
            ctx->query = string_type(first, last);
        }
        return true;
    }

    static bool set_fragment (iterator first, iterator last, void * context, void * /*action_args*/)
    {
        if (context) {
            parse_context * ctx = static_cast<parse_context *>(context);
            ctx->fragment = string_type(first, last);
        }
        return true;
    }

    static bool set_path (iterator first, iterator last, void * context, void * /*action_args*/)
    {
        if (context) {
            parse_context * ctx = static_cast<parse_context *>(context);
            ctx->path = string_type(first, last);
        }
        return true;
    }

    static bool set_userinfo (iterator first, iterator last, void * context, void * /*action_args*/)
    {
        if (context) {
            parse_context * ctx = static_cast<parse_context *>(context);
            ctx->userinfo = string_type(first, last);
        }
        return true;
    }

    static bool unset_userinfo (iterator /*first*/, iterator /*last*/, void * context, void * /*action_args*/)
    {
        if (context) {
            parse_context * ctx = static_cast<parse_context *>(context);
            ctx->userinfo.clear();
        }
        return true;
    }

    static bool set_host (iterator first, iterator last, void * context, void * /*action_args*/)
    {
        if (context) {
            parse_context * ctx = static_cast<parse_context *>(context);
            ctx->host = string_type(first, last);
        }
        return true;
    }

    static bool set_port (iterator first, iterator last, void * context, void * /*action_args*/)
    {
        if (!context) return true;

        parse_context * ctx = static_cast<parse_context *>(context);
        string_type digits(first, last);

        try {
            ctx->port = lexical_cast<uint16_t, string_type>(digits, 10);
        } catch (bad_lexical_cast ex) {
            return false;
        }

        return true;
    }

    static bool set_raw_host (iterator /*first*/, iterator /*last*/, void * context, void * /*action_args*/)
    {
        if (context) {
            parse_context * ctx = static_cast<parse_context *>(context);
            ctx->is_raw_host = true;
        }

        return true;
    }

#if PFS_TEST
    transition_type const * p_unreserved_tr;
    transition_type const * p_pct_encoded_tr;
    transition_type const * p_pchar_tr;
    transition_type const * p_segment_nz_tr;
    transition_type const * p_segment_tr;
    transition_type const * p_slash_segment_tr;
    transition_type const * p_segment_nz_slash_tr;
    transition_type const * p_path_absolute_tr;
    transition_type const * p_segment_nc_tr;
    transition_type const * p_segment_nz_nc_tr;
    transition_type const * p_path_noscheme_tr;
    transition_type const * p_path_empty_tr;
    transition_type const * p_h16_tr;
    transition_type const * p_dec_octet_tr_4;
    transition_type const * p_dec_octet_tr_3;
    transition_type const * p_dec_octet_tr_2;
    transition_type const * p_dec_octet_tr_1;
    transition_type const * p_dec_octet_tr;
    transition_type const * p_ipv4address_tr;
    transition_type const * p_ls32_tr;
    transition_type const * p_h16_semi_tr;
    transition_type const * p_ipv6address_tr_4_1;
    transition_type const * p_ipv6address_tr_5_1;
    transition_type const * p_ipv6address_tr_6_1;
    transition_type const * p_ipv6address_tr_7_1;
    transition_type const * p_ipv6address_tr_8_1;
    transition_type const * p_ipv6address_tr_9_1;
    transition_type const * p_ipv6address_tr_1;
    transition_type const * p_ipv6address_tr_2;
    transition_type const * p_ipv6address_tr_3;
    transition_type const * p_ipv6address_tr_4;
    transition_type const * p_ipv6address_tr_5;
    transition_type const * p_ipv6address_tr_6;
    transition_type const * p_ipv6address_tr_7;
    transition_type const * p_ipv6address_tr_8;
    transition_type const * p_ipv6address_tr_9;
    transition_type const * p_ipv6address_tr;
    transition_type const * p_ipvfuture_tail_tr;
    transition_type const * p_ipvfuture_tr;
    transition_type const * p_ip_literal_entry_tr;
    transition_type const * p_ip_literal_tr;
    transition_type const * p_reg_name_entry_tr;
    transition_type const * p_reg_name_tr;
    transition_type const * p_host_tr;
    transition_type const * p_port_tr;
    transition_type const * p_authority_tr_2;
    transition_type const * p_userinfo_tr;
    transition_type const * p_authority_tr_1;
    transition_type const * p_authority_tr;
    transition_type const * p_path_abempty_tr;
    transition_type const * p_relative_part_tr;
    transition_type const * p_query_chars_tr;
    transition_type const * p_query_tr;
    transition_type const * p_relative_ref_tr_1;
    transition_type const * p_fragment_tr;
    transition_type const * p_relative_ref_tr_2;
    transition_type const * p_relative_ref_tr;
    transition_type const * p_scheme_chars_tr;
    transition_type const * p_scheme_tr;
    transition_type const * p_path_rootless_tr;
    transition_type const * p_hier_part_tr;
    transition_type const * p_uri_tr_1;
    transition_type const * p_uri_tr_2;
#endif
    transition_type const * p_uri_tr;
    transition_type const * p_uri_reference_tr;
};

template <typename UriType>
uri_grammar<UriType>::uri_grammar ()
{
#undef FSM_SEQ
#undef FSM_ONE_OF
#undef FSM_TR
#undef FSM_OPT_TR
#undef FSM_RPT_TR
#undef FSM_RPT_ONE_OF
#undef FSM_NOTHING
#define FSM_SEQ(x)            fsm_type::seq(x.begin(), x.end())
#define FSM_ONE_OF(x)         fsm_type::one_of(x.begin(), x.end())
#define FSM_TR(x)             fsm_type::tr(x)
#define FSM_OPT_TR(x)         fsm_type::opt_tr(x)
#define FSM_RPT_TR(x,a,b)     fsm_type::rpt_tr(x,a,b)
#define FSM_RPT_ONE_OF(x,a,b) fsm_type::rpt_one_of(x.begin(),x.end(),a,b)
#define FSM_NOTHING           fsm_type::nothing()

    static string_type const ALPHA("ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz");
    static string_type const DIGIT("0123456789");

    static string_type const HEXDIGIT("0123456789ABCDEFabcdef");
    static string_type const SLASH("/");
    static string_type const COLON(":");
    static string_type const DCOLON("::");
    static string_type const AT_SIGN("@");
    static string_type const PERIOD(".");
    static string_type const SUB_DELIMS("!$&'()*+,;=");


    /* ALPHA / DIGIT / "-" / "." / "_" / "~" */
    static string_type const UNRESERVED("-._~");

    static transition_type const unreserved_tr[] = {
          {-1, 1, FSM_ONE_OF(ALPHA)     , fsm_type::accept, 0, 0 }
        , {-1, 2, FSM_ONE_OF(DIGIT)     , fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_ONE_OF(UNRESERVED), fsm_type::accept, 0, 0 }
    };

    static string_type const PERCENT_CHAR("%");

    /* "%" HEXDIG HEXDIG */
    static transition_type const pct_encoded_tr[] = {
          { 1,-1, FSM_SEQ(PERCENT_CHAR), fsm_type::normal, 0, 0 }
        , { 2,-1, FSM_ONE_OF(HEXDIGIT) , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_ONE_OF(HEXDIGIT) , fsm_type::accept, 0, 0 }
    };

    /* unreserved / pct-encoded / sub-delims / ":" / "@" */
    static string_type const PCHAR(":@");

    static transition_type const pchar_tr[] = {
          {-1, 1, FSM_TR(unreserved_tr) , fsm_type::accept, 0, 0 }
        , {-1, 2, FSM_TR(pct_encoded_tr), fsm_type::accept, 0, 0 }
        , {-1, 3, FSM_ONE_OF(SUB_DELIMS), fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_ONE_OF(PCHAR)     , fsm_type::accept, 0, 0 }
    };

    /* 1*pchar */
    static transition_type const segment_nz_tr[] = {
          { 1,-1, FSM_TR(pchar_tr),fsm_type::accept, 0, 0 }
        , { 1,-1, FSM_TR(pchar_tr),fsm_type::accept, 0, 0 }
    };

    /* *pchar */
    static transition_type const segment_tr[] = {
          { 0, 1, FSM_TR(pchar_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_NOTHING     , fsm_type::accept, 0, 0 }
    };

    /* "/" segment */
    static transition_type const slash_segment_tr[] = {
          { 1,-1, FSM_SEQ(SLASH)    , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(segment_tr), fsm_type::accept, 0, 0 }
    };

    /* segment-nz *slash_segment */
    static transition_type const segment_nz_slash_tr[] = {
          { 1,-1, FSM_TR(segment_nz_tr)              , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_RPT_TR(slash_segment_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /* "/" [ segment-nz *slash_segment ] */
    static transition_type const path_absolute_tr[] = {
          { 1,-1, FSM_SEQ(SLASH)                  , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_OPT_TR(segment_nz_slash_tr) , fsm_type::accept, 0, 0 }
    };

    /* 1*( unreserved / pct-encoded / sub-delims / "@" )
            ; non-zero-length segment without any colon ":"
    */
    static transition_type const segment_nc_tr[] = {
          {-1, 1, FSM_TR(unreserved_tr) , fsm_type::accept, 0, 0 }
        , {-1, 2, FSM_TR(pct_encoded_tr), fsm_type::accept, 0, 0 }
        , {-1, 3, FSM_ONE_OF(SUB_DELIMS), fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_ONE_OF(AT_SIGN)   , fsm_type::accept, 0, 0 }
    };

    static transition_type const segment_nz_nc_tr[] = {
        {-1,-1, FSM_RPT_TR(segment_nc_tr, 1, -1), fsm_type::accept, 0, 0 }
    };

    /* segment-nz-nc *slash-segment */
    static transition_type const path_noscheme_tr[] = {
          { 1,-1, FSM_TR(segment_nz_nc_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_RPT_TR(slash_segment_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /* 0<pchar> */
    static transition_type const path_empty_tr[] = {
        {-1,-1, FSM_NOTHING, fsm_type::accept, 0, 0 }
    };

    /* 1*4HEXDIG */
    static transition_type const h16_tr[] = {
          { 1,-1, FSM_ONE_OF(HEXDIGIT), fsm_type::accept, 0, 0 }
        , { 2,-1, FSM_ONE_OF(HEXDIGIT), fsm_type::accept, 0, 0 }
        , { 3,-1, FSM_ONE_OF(HEXDIGIT), fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_ONE_OF(HEXDIGIT), fsm_type::accept, 0, 0 }
    };

    /* "25" %x30-35        ; 250-255 */
    static string_type const DEC_OCTET_25("25");
    static string_type const DEC_OCTET_012345("012345");
    static transition_type const dec_octet_tr_4[] = {
          { 1,-1, FSM_SEQ(DEC_OCTET_25)       , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_ONE_OF(DEC_OCTET_012345), fsm_type::accept, 0, 0 }
    };

    /* "2" %x30-34 DIGIT   ; 200-249 */
    static string_type const DIGIT_2("2");
    static string_type const DEC_OCTET_01234("01234");
    static transition_type const dec_octet_tr_3[] = {
          { 1,-1, FSM_SEQ(DIGIT_2)           , fsm_type::normal, 0, 0 }
        , { 2,-1, FSM_ONE_OF(DEC_OCTET_01234), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_ONE_OF(DIGIT)          , fsm_type::accept, 0, 0 }
    };

    /* "1" 2DIGIT  ; 100-199 */
    static string_type const DIGIT_1("1");
    static transition_type const dec_octet_tr_2[] = {
          { 1,-1, FSM_SEQ(DIGIT_1) , fsm_type::normal, 0, 0 }
        , { 2,-1, FSM_ONE_OF(DIGIT), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_ONE_OF(DIGIT), fsm_type::accept, 0, 0 }
    };

    /* %x31-39 DIGIT       ; 10-99*/
    static string_type const DIGIT_EX_ZERO("123456789");
    static transition_type const dec_octet_tr_1[] = {
          { 1,-1, FSM_ONE_OF(DIGIT_EX_ZERO), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_ONE_OF(DIGIT)        , fsm_type::accept, 0, 0 }
    };

    /*
      DIGIT                 ; 0-9
      / %x31-39 DIGIT       ; 10-99
      / "1" 2DIGIT          ; 100-199
      / "2" %x30-34 DIGIT   ; 200-249
      / "25" %x30-35        ; 250-255
    */
    static transition_type const dec_octet_tr[] = {
          {-1, 1, FSM_TR(dec_octet_tr_4), fsm_type::accept, 0, 0 } /* 250 - 255 */
        , {-1, 2, FSM_TR(dec_octet_tr_3), fsm_type::accept, 0, 0 } /* 200 - 249 */
        , {-1, 3, FSM_TR(dec_octet_tr_2), fsm_type::accept, 0, 0 } /* 100 - 199 */
        , {-1, 4, FSM_TR(dec_octet_tr_1), fsm_type::accept, 0, 0 } /* 10 - 99 */
        , {-1,-1, FSM_ONE_OF(DIGIT)     , fsm_type::accept, 0, 0 } /* 0 - 9 */
    };

    /* dec-octet "." dec-octet "." dec-octet "." dec-octet */
    static transition_type const ipv4address_tr[] = {
          { 1,-1, FSM_TR(dec_octet_tr), fsm_type::normal, 0, 0 }
        , { 2,-1, FSM_SEQ(PERIOD)     , fsm_type::normal, 0, 0 }
        , { 3,-1, FSM_TR(dec_octet_tr), fsm_type::normal, 0, 0 }
        , { 4,-1, FSM_SEQ(PERIOD)     , fsm_type::normal, 0, 0 }
        , { 5,-1, FSM_TR(dec_octet_tr), fsm_type::normal, 0, 0 }
        , { 6,-1, FSM_SEQ(PERIOD)     , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(dec_octet_tr), fsm_type::accept, 0, 0 }
    };

    /* ( h16 ":" h16 ) / IPv4address */
    static transition_type const ls32_tr[] = {
          {-1, 1, FSM_TR(ipv4address_tr), fsm_type::accept, 0, 0 }
        , { 2,-1, FSM_TR(h16_tr)        , fsm_type::normal, 0, 0 }
        , { 3,-1, FSM_SEQ(COLON)        , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(h16_tr)        , fsm_type::accept, 0, 0 }
    };

    /* h16 ":" */
    static transition_type const h16_semi_tr[] = {
          { 1,-1, FSM_TR(h16_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_SEQ(COLON), fsm_type::accept, 0, 0 }
    };

    /* *1( h16 ":" ) h16 */
    static transition_type const ipv6address_tr_4_1[] = {
          { 1,-1, FSM_RPT_TR(h16_semi_tr, 0, 1), fsm_type::normal, 0, 0 }
        , {-1, 2, FSM_TR(h16_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_TR(h16_tr), fsm_type::accept, 0, 0 }
    };

    /* *2( h16 ":" ) h16 */
    static transition_type const ipv6address_tr_5_1[] = {
          { 1, 2, FSM_RPT_TR(h16_semi_tr, 0, 2), fsm_type::normal, 0, 0 }
        , {-1, 2, FSM_TR(h16_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_TR(h16_tr), fsm_type::accept, 0, 0 }
    };

    /* *3( h16 ":" ) h16 */
    static transition_type const ipv6address_tr_6_1[] = {
          { 1, 2, FSM_RPT_TR(h16_semi_tr, 0, 3), fsm_type::normal, 0, 0 }
        , {-1, 2, FSM_TR(h16_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_TR(h16_tr), fsm_type::accept, 0, 0 }
    };

    /* *4( h16 ":" ) h16 */
    static transition_type const ipv6address_tr_7_1[] = {
          { 1, 2, FSM_RPT_TR(h16_semi_tr, 0, 4), fsm_type::normal, 0, 0 }
        , {-1, 2, FSM_TR(h16_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_TR(h16_tr), fsm_type::accept, 0, 0 }
    };

    /* *5( h16 ":" ) h16 */
    static transition_type const ipv6address_tr_8_1[] = {
          { 1, 2, FSM_RPT_TR(h16_semi_tr, 0, 5), fsm_type::normal, 0, 0 }
        , {-1, 2, FSM_TR(h16_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_TR(h16_tr), fsm_type::accept, 0, 0 }
    };

    /* *6( h16 ":" ) h16 */
    static transition_type const ipv6address_tr_9_1[] = {
          { 1, 2, FSM_RPT_TR(h16_semi_tr, 0, 6), fsm_type::normal, 0, 0 }
        , {-1, 2, FSM_TR(h16_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_TR(h16_tr), fsm_type::accept, 0, 0 }
    };

    /* 6( h16 ":" ) ls32 */
    static transition_type const ipv6address_tr_1[] = {
          { 1,-1, FSM_RPT_TR(h16_semi_tr, 6, 6), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(ls32_tr), fsm_type::accept, 0, 0 }
    };

    /* "::" 5( h16 ":" ) ls32 */
    static transition_type const ipv6address_tr_2[] = {
          { 1,-1, FSM_SEQ(DCOLON), fsm_type::normal, 0, 0 }
        , { 2,-1, FSM_RPT_TR(h16_semi_tr, 5, 5), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(ls32_tr)              , fsm_type::accept, 0, 0 }
    };

    /* [ h16 ] "::" 4( h16 ":" ) ls32 */
    static transition_type const ipv6address_tr_3[] = {
          { 1, 1, FSM_TR(h16_tr), fsm_type::normal, 0, 0 }
        , { 2,-1, FSM_SEQ(DCOLON), fsm_type::normal, 0, 0 }
        , { 3,-1, FSM_RPT_TR(h16_semi_tr, 4, 4), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(ls32_tr)              , fsm_type::accept, 0, 0 }
    };

    /* [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32 */
    static transition_type const ipv6address_tr_4[] = {
          { 1, 1, FSM_TR(ipv6address_tr_4_1), fsm_type::normal, 0, 0 }

        , { 2,-1, FSM_SEQ(DCOLON)    , fsm_type::normal, 0, 0 }
        , { 3,-1, FSM_TR(h16_semi_tr), fsm_type::normal, 0, 0 }
        , { 4,-1, FSM_TR(h16_semi_tr), fsm_type::normal, 0, 0 }
        , { 5,-1, FSM_TR(h16_semi_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(ls32_tr)    , fsm_type::accept, 0, 0 }
    };

    /* [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32 */
    static transition_type const ipv6address_tr_5[] = {
          { 1, 2, FSM_TR(ipv6address_tr_5_1), fsm_type::normal, 0, 0 }
        , { 2, 2, FSM_TR(ipv6address_tr_5_1), fsm_type::normal, 0, 0 }

        , { 3,-1, FSM_SEQ(DCOLON)    , fsm_type::normal, 0, 0 }
        , { 4,-1, FSM_TR(h16_semi_tr), fsm_type::normal, 0, 0 }
        , { 5,-1, FSM_TR(h16_semi_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(ls32_tr)    , fsm_type::accept, 0, 0 }
    };

    /* [ *3( h16 ":" ) h16 ] "::" h16 ":"   ls32 */
    static transition_type const ipv6address_tr_6[] = {
          { 1, 3, FSM_TR(ipv6address_tr_6_1), fsm_type::normal, 0, 0 }
        , { 2, 3, FSM_TR(ipv6address_tr_6_1), fsm_type::normal, 0, 0 }
        , { 3, 3, FSM_TR(ipv6address_tr_6_1), fsm_type::normal, 0, 0 }

        , { 4,-1, FSM_SEQ(DCOLON)    , fsm_type::normal, 0, 0 }
        , { 5,-1, FSM_TR(h16_semi_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(ls32_tr)    , fsm_type::accept, 0, 0 }
    };


    /* [ *4( h16 ":" ) h16 ] "::" ls32 */
    static transition_type const ipv6address_tr_7[] = {
          { 1, 4, FSM_TR(ipv6address_tr_7_1), fsm_type::normal, 0, 0 }
        , { 2, 4, FSM_TR(ipv6address_tr_7_1), fsm_type::normal, 0, 0 }
        , { 3, 4, FSM_TR(ipv6address_tr_7_1), fsm_type::normal, 0, 0 }
        , { 4, 4, FSM_TR(ipv6address_tr_7_1), fsm_type::normal, 0, 0 }

        , { 5,-1, FSM_SEQ(DCOLON) , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(ls32_tr) , fsm_type::accept, 0, 0 }
    };

    /* [ *5( h16 ":" ) h16 ] "::"              h16 */
    static transition_type const ipv6address_tr_8[] = {
          { 1, 5, FSM_TR(ipv6address_tr_8_1), fsm_type::normal, 0, 0 }
        , { 2, 5, FSM_TR(ipv6address_tr_8_1), fsm_type::normal, 0, 0 }
        , { 3, 5, FSM_TR(ipv6address_tr_8_1), fsm_type::normal, 0, 0 }
        , { 4, 5, FSM_TR(ipv6address_tr_8_1), fsm_type::normal, 0, 0 }
        , { 5, 5, FSM_TR(ipv6address_tr_8_1), fsm_type::normal, 0, 0 }

        , { 6,-1, FSM_SEQ(DCOLON) , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(h16_tr)  , fsm_type::accept, 0, 0 }
    };


    /* [ *6( h16 ":" ) h16 ] "::" */
    static transition_type const ipv6address_tr_9[] = {
          { 1, 6, FSM_TR(ipv6address_tr_9_1), fsm_type::normal, 0, 0 }
        , { 2, 6, FSM_TR(ipv6address_tr_9_1), fsm_type::normal, 0, 0 }
        , { 3, 6, FSM_TR(ipv6address_tr_9_1), fsm_type::normal, 0, 0 }
        , { 4, 6, FSM_TR(ipv6address_tr_9_1), fsm_type::normal, 0, 0 }
        , { 5, 6, FSM_TR(ipv6address_tr_9_1), fsm_type::normal, 0, 0 }
        , { 6, 6, FSM_TR(ipv6address_tr_9_1), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_SEQ(DCOLON)           , fsm_type::accept, 0, 0 }
    };


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
    static transition_type const ipv6address_tr[] = {
          {-1, 1, FSM_TR(ipv6address_tr_1), fsm_type::accept, 0, 0 }
        , {-1, 2, FSM_TR(ipv6address_tr_2), fsm_type::accept, 0, 0 }
        , {-1, 3, FSM_TR(ipv6address_tr_3), fsm_type::accept, 0, 0 }
        , {-1, 4, FSM_TR(ipv6address_tr_4), fsm_type::accept, 0, 0 }
        , {-1, 5, FSM_TR(ipv6address_tr_5), fsm_type::accept, 0, 0 }
        , {-1, 6, FSM_TR(ipv6address_tr_6), fsm_type::accept, 0, 0 }
        , {-1, 7, FSM_TR(ipv6address_tr_7), fsm_type::accept, 0, 0 }
        , {-1, 8, FSM_TR(ipv6address_tr_8), fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_TR(ipv6address_tr_9), fsm_type::accept, 0, 0 }
    };

    /* "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" ) */
    static transition_type const ipvfuture_tail_tr[] = {
          {-1, 1, FSM_TR(unreserved_tr) , fsm_type::accept, 0, 0 }
        , {-1, 2, FSM_ONE_OF(SUB_DELIMS), fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_SEQ(COLON)        , fsm_type::accept, 0, 0 }
    };

    static string_type const LETTER_SMALL_V("v");
    static transition_type const ipvfuture_tr[] = {
          { 1,-1, FSM_SEQ(LETTER_SMALL_V)        , fsm_type::normal, 0, 0 }
        , { 2,-1, FSM_RPT_ONE_OF(HEXDIGIT, 1, -1), fsm_type::normal, 0, 0 }
        , { 3,-1, FSM_SEQ(PERIOD)                , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_RPT_TR(ipvfuture_tail_tr, 1, -1), fsm_type::accept, 0, 0 }
    };

    /* "[" ( IPv6address / IPvFuture  ) "]" */
    static transition_type const ip_literal_entry_tr[] = {
          {-1, 1, FSM_TR(ipv6address_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_TR(ipvfuture_tr)  , fsm_type::accept, 0, 0 }
    };

    static string_type const LEFT_SQUARE_BRACKET("[");
    static string_type const RIGHT_SQUARE_BRACKET("]");
    static transition_type const ip_literal_tr[] = {
          { 1,-1, FSM_SEQ(LEFT_SQUARE_BRACKET) , fsm_type::normal, 0, 0 }
        , { 2,-1, FSM_TR(ip_literal_entry_tr)  , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_SEQ(RIGHT_SQUARE_BRACKET), fsm_type::accept, 0, 0 }
    };

    /* *( unreserved / pct-encoded / sub-delims ) */
    static transition_type const reg_name_entry_tr[] = {
          {-1, 1, FSM_TR(unreserved_tr) , fsm_type::accept, 0, 0 }
        , {-1, 2, FSM_TR(pct_encoded_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_ONE_OF(SUB_DELIMS), fsm_type::accept, 0, 0 }
    };
    static transition_type const reg_name_tr[] = {
          {-1,-1, FSM_RPT_TR(reg_name_entry_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /*  host = IP-literal / IPv4address / reg-name */
    static transition_type const host_tr[] = {
          {-1, 1, FSM_TR(ip_literal_tr) , fsm_type::accept, set_raw_host, 0}
        , {-1, 2, FSM_TR(ipv4address_tr), fsm_type::accept, set_raw_host, 0}

        /* OBSOLETE NOTE: this poses a parsing problem - all IPv4 addresses are valid
        reg_name as well. Fix this by doing the reg_name_tr now,
        then on match, do a function to check if its an IPv4
        address */
        , {-1,-1, FSM_TR(reg_name_tr), fsm_type::accept, 0, 0 }
    };

    /* *DIGIT */
    static transition_type const port_tr[] = {
          {-1,-1, FSM_RPT_ONE_OF(DIGIT, 0, -1), fsm_type::accept, 0, 0 }
    };

    /* ":" port */
    static transition_type const authority_tr_2[] = {
          { 1,-1, FSM_SEQ(COLON) , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(port_tr), fsm_type::accept, set_port, 0}
    };

    /* *( unreserved / pct-encoded / sub-delims / ":" ) */
    static transition_type const userinfo_tr[] = {
          { 0, 1, FSM_TR(unreserved_tr) , fsm_type::accept, 0, 0 }
        , { 0, 2, FSM_TR(pct_encoded_tr), fsm_type::accept, 0, 0 }
        , { 0, 3, FSM_ONE_OF(SUB_DELIMS), fsm_type::accept, 0, 0 }
        , { 0, 4, FSM_SEQ(COLON)        , fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_NOTHING           , fsm_type::accept, 0, 0 }
    };

    /* userinfo "@" */
    static transition_type const authority_tr_1[] = {
          { 1,-1, FSM_TR(userinfo_tr), fsm_type::normal, set_userinfo, 0 }
        , {-1,-1, FSM_SEQ(AT_SIGN)   , fsm_type::accept, 0, 0 }
    };

    /* [ userinfo "@" ] host [ ":" port ] */
    /* [ authority_tr_1 ] host [ authority_tr_2 ] */
    static transition_type const authority_tr[] = {
          { 1, 3, FSM_OPT_TR(authority_tr_1) , fsm_type::normal, 0, 0 }
        , { 2, 3, FSM_TR(host_tr)            , fsm_type::normal, set_host, 0 }
        , {-1, 3, FSM_OPT_TR(authority_tr_2) , fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_NOTHING                , fsm_type::reject, unset_userinfo, 0 }
    };

    /*  *( "/" segment ) */
    static transition_type const path_abempty_tr[] = {
          {-1,-1, FSM_RPT_TR(slash_segment_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /*
    relative-part = "//" authority path-abempty
                  / path-absolute
                  / path-noscheme`
                  / path-empty
    */
    static string_type const DSLASH("//");
    static transition_type const relative_part_tr[] = {
          { 1, 3, FSM_SEQ(DSLASH)         , fsm_type::normal, 0, 0 }
        , { 2, 3, FSM_TR(authority_tr)    , fsm_type::normal, set_path, 0 }
        , {-1, 3, FSM_TR(path_abempty_tr) , fsm_type::accept, set_path, 0 }

        , {-1, 4, FSM_TR(path_absolute_tr), fsm_type::accept, set_path, 0 }
        , {-1, 5, FSM_TR(path_noscheme_tr), fsm_type::accept, set_path, 0 }
        , {-1,-1, FSM_TR(path_empty_tr)   , fsm_type::accept, set_path, 0 }
    };

    /* *( pchar / "/" / "?" ) */
    static string_type const QUERY_CHARS("/?");
    static transition_type const query_chars_tr[] = {
          {-1, 1, FSM_TR(pchar_tr)       , fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_ONE_OF(QUERY_CHARS), fsm_type::accept, 0, 0 }
    };
    static transition_type const query_tr[] = {
        {-1,-1, FSM_RPT_TR(query_chars_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /* "?" query */
    static string_type const QUESTION_MARK("?");
    static transition_type const relative_ref_tr_1[] = {
          { 1,-1, FSM_SEQ(QUESTION_MARK), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(query_tr)      , fsm_type::accept, set_query, 0 }
    };

    /* *( pchar / "/" / "?" ) */
    static transition_type const fragment_tr[] = {
        {-1,-1, FSM_RPT_TR(query_chars_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /* "#" fragment */
    static string_type const NUMBER_SIGN("#");
    static transition_type const relative_ref_tr_2[] = {
          { 1,-1, FSM_SEQ(NUMBER_SIGN), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(fragment_tr) , fsm_type::accept, set_fragment, 0}
    };

    /* relative-part [ "?" query ] [ "#" fragment ] */
    static transition_type const relative_ref_tr[] = {
          { 1,-1, FSM_TR(relative_part_tr)           , fsm_type::normal, 0, 0 }
        , { 2,-1, FSM_RPT_TR(relative_ref_tr_1, 0, 1), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_RPT_TR(relative_ref_tr_2, 0, 1), fsm_type::accept, 0, 0 }
    };

    /* ALPHA *( ALPHA / DIGIT / "+" / "-" / "." ) */
    static string_type const SCHEME_CHARS("+-.");
    static transition_type const scheme_chars_tr[] = {
          {-1, 1, FSM_ONE_OF(ALPHA)       , fsm_type::accept, 0, 0 }
        , {-1, 2, FSM_ONE_OF(DIGIT)       , fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_ONE_OF(SCHEME_CHARS), fsm_type::accept, 0, 0 }
    };
    static transition_type const scheme_tr[] = {
          { 1,-1, FSM_ONE_OF(ALPHA)                 , fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_RPT_TR(scheme_chars_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /* segment-nz *( "/" segment ) */
    static transition_type const path_rootless_tr[] = {
          { 1,-1, FSM_TR(segment_nz_tr)              , fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_RPT_TR(slash_segment_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /*
        "//" authority path-abempty
             / path-absolute
             / path-rootless
             / path-empty
    */
    static transition_type const hier_part_tr[] = {
          { 1, 3, FSM_SEQ(DSLASH)         , fsm_type::normal, 0, 0 }
        , { 2, 3, FSM_TR(authority_tr)    , fsm_type::normal, set_path, 0 }
        , {-1, 3, FSM_TR(path_abempty_tr) , fsm_type::accept, set_path, 0 }

        , {-1, 4, FSM_TR(path_absolute_tr), fsm_type::accept, set_path, 0 }
        , {-1, 5, FSM_TR(path_rootless_tr), fsm_type::accept, set_path, 0 }
        , {-1,-1, FSM_TR(path_empty_tr)   , fsm_type::accept, set_path, 0 }
    };


    /* "?" query */
    static transition_type const uri_tr_1[] = {
          { 1,-1, FSM_SEQ(QUESTION_MARK), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(query_tr)      , fsm_type::accept, set_query, 0 }
    };

    /* "#" fragment */
    static transition_type const uri_tr_2[] = {
          { 1,-1, FSM_SEQ(NUMBER_SIGN), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_TR(fragment_tr) , fsm_type::accept, set_fragment, 0 }
    };


    /* scheme ":" hier-part [ "?" query ] [ "#" fragment ] */
    static transition_type const uri_tr[] = {
          { 1,-1, FSM_TR(scheme_tr)         , fsm_type::normal, set_scheme, 0 }
        , { 2,-1, FSM_SEQ(COLON)            , fsm_type::normal, 0, 0 }
        , { 3,-1, FSM_TR(hier_part_tr)      , fsm_type::normal, 0, 0 }
        , { 4,-1, FSM_RPT_TR(uri_tr_1, 0, 1), fsm_type::normal, 0, 0 }
        , {-1,-1, FSM_RPT_TR(uri_tr_2, 0, 1), fsm_type::accept, 0, 0 }
    };

    /* URI / relative-ref */
    static transition_type const uri_reference_tr[] = {
          {-1, 1, FSM_TR(uri_tr)         , fsm_type::accept, 0, 0 }
        , {-1,-1, FSM_TR(relative_ref_tr), fsm_type::accept, 0, 0 }
    };

#if PFS_TEST
    p_unreserved_tr  = unreserved_tr;
    p_pct_encoded_tr = pct_encoded_tr;
    p_pchar_tr       = pchar_tr;
//    p_segment_nz_tr;
//    p_segment_tr;
//    p_slash_segment_tr;
//    p_segment_nz_slash_tr;
//    p_path_absolute_tr;
//    p_segment_nc_tr;
//    p_segment_nz_nc_tr;
//    p_path_noscheme_tr;
//    p_path_empty_tr;
//    p_h16_tr;
//    p_dec_octet_tr_4;
//    p_dec_octet_tr_3;
//    p_dec_octet_tr_2;
//    p_dec_octet_tr_1;
//    p_dec_octet_tr;
//    p_ipv4address_tr;
//    p_ls32_tr;
//    p_h16_semi_tr;
//    p_ipv6address_tr_4_1;
//    p_ipv6address_tr_5_1;
//    p_ipv6address_tr_6_1;
//    p_ipv6address_tr_7_1;
//    p_ipv6address_tr_8_1;
//    p_ipv6address_tr_9_1;
//    p_ipv6address_tr_1;
//    p_ipv6address_tr_2;
//    p_ipv6address_tr_3;
//    p_ipv6address_tr_4;
//    p_ipv6address_tr_5;
//    p_ipv6address_tr_6;
//    p_ipv6address_tr_7;
//    p_ipv6address_tr_8;
//    p_ipv6address_tr_9;
//    p_ipv6address_tr;
//    p_ipvfuture_tail_tr;
//    p_ipvfuture_tr;
//    p_ip_literal_entry_tr;
//    p_ip_literal_tr;
//    p_reg_name_entry_tr;
//    p_reg_name_tr;
//    p_host_tr;
//    p_port_tr;
//    p_authority_tr_2;
//    p_userinfo_tr;
//    p_authority_tr_1;
//    p_authority_tr;
//    p_path_abempty_tr;
//    p_relative_part_tr;
//    p_query_chars_tr;
//    p_query_tr;
//    p_relative_ref_tr_1;
//    p_fragment_tr;
//    p_relative_ref_tr_2;
//    p_relative_ref_tr;
//    p_scheme_chars_tr;
//    p_scheme_tr;
//    p_path_rootless_tr;
//    p_hier_part_tr;
//    p_uri_tr_1;
//    p_uri_tr_2;
//    p_authority_tr     = authority_tr;
#endif
    p_uri_tr           = uri_tr;
    p_uri_reference_tr = uri_reference_tr;
}

/**
 * @brief  Parse the given string as URI.
 * @details Implementation of RFC 3986 (http://tools.ietf.org/html/rfc3986)
 *
 * @param uri string representation of URI.
 */
template <typename StringType>
bool uri<StringType>::parse (typename string_type::const_iterator first
            , typename string_type::const_iterator last)
{
    typedef uri_grammar<uri<StringType> >   grammar_type;
    typedef typename grammar_type::fsm_type fsm_type;

    // Initialize grammar's static members
    static grammar_type grammar;

    _d.clear();

    fsm_type fsm(grammar.p_uri_tr, & _d);
    typename fsm_type::result_type r = fsm.exec(0, first, last);

    if (r.first && r.second == last)
        return true;

    return false;
}

}} // pfs::net
