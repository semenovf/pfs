/*
 * uri-rfc3986.hpp
 *
 *  Created on: 12.10.2012
 *  Modified on: 10.03.2017
 */


#ifndef __PFS_NET_URI_RFC3986_HPP__
#define __PFS_NET_URI_RFC3986_HPP__

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

template <typename UriT>
struct uri_grammar
{
    typedef typename UriT::string_type           string_type;
    typedef typename UriT::data_rep              user_context;
    typedef fsm::fsm<string_type>                fsm_type;
    typedef typename fsm_type::transition_type   transition_type;
    typedef typename string_type::const_iterator const_iterator;
    
    uri_grammar ();
    
    static bool set_scheme (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args);
    
    static bool set_query (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args);
    
    static bool set_fragment (const_iterator begin
            , const_iterator end
            , void * context, void * action_args);
    
    static bool set_path (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args);
    
    static bool set_userinfo (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args);
    
    static bool unset_userinfo (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args);
    
    static bool set_host (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args);
    
    static bool set_port (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args);
    
    static bool set_raw_host (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args);

#if __PFS_TEST__
    static string_type const * p_alpha;
    static string_type const * p_digit;
    static string_type const * p_hexdigit; // DIGIT / "A" / "B" / "C" / "D" / "E" / "F"

    static transition_type const * p_digit_tr;
    static transition_type const * p_hexdig_tr;
    static transition_type const * p_unreserved_tr;
    static transition_type const * p_pct_encoded_tr;
    static transition_type const * p_sub_delims_tr;
    static transition_type const * p_pchar_tr;
    static transition_type const * p_segment_nz_tr;
    static transition_type const * p_segment_tr;
    static transition_type const * p_slash_segment_tr;
    static transition_type const * p_segment_nz_slash_tr;
    static transition_type const * p_path_absolute_tr;
    static transition_type const * p_segment_nc_tr;
    static transition_type const * p_segment_nz_nc_tr;
    static transition_type const * p_path_noscheme_tr;
    static transition_type const * p_path_empty_tr;
    static transition_type const * p_h16_tr;
    static transition_type const * p_dec_octet_tr_4;
    static transition_type const * p_dec_octet_tr_3;
    static transition_type const * p_dec_octet_tr_2;
    static transition_type const * p_dec_octet_tr_1;
    static transition_type const * p_dec_octet_tr;
    static transition_type const * p_ipv4address_tr;
    static transition_type const * p_ls32_tr;
    static transition_type const * p_h16_semi_tr;
    static transition_type const * p_ipv6address_tr_4_1;
    static transition_type const * p_ipv6address_tr_5_1;
    static transition_type const * p_ipv6address_tr_6_1;
    static transition_type const * p_ipv6address_tr_7_1;
    static transition_type const * p_ipv6address_tr_8_1;
    static transition_type const * p_ipv6address_tr_9_1;
    static transition_type const * p_ipv6address_tr_1;
    static transition_type const * p_ipv6address_tr_2;
    static transition_type const * p_ipv6address_tr_3;
    static transition_type const * p_ipv6address_tr_4;
    static transition_type const * p_ipv6address_tr_5;
    static transition_type const * p_ipv6address_tr_6;
    static transition_type const * p_ipv6address_tr_7;
    static transition_type const * p_ipv6address_tr_8;
    static transition_type const * p_ipv6address_tr_9;
    static transition_type const * p_ipv6address_tr;
    static transition_type const * p_ipvfuture_tail_tr;
    static transition_type const * p_ipvfuture_tr;
    static transition_type const * p_ip_literal_entry_tr;
    static transition_type const * p_ip_literal_tr;
    static transition_type const * p_reg_name_entry_tr;
    static transition_type const * p_reg_name_tr;
    static transition_type const * p_host_tr;
    static transition_type const * p_port_tr;
    static transition_type const * p_authority_tr_2;
    static transition_type const * p_userinfo_tr;
    static transition_type const * p_authority_tr_1;
    static transition_type const * p_authority_tr;
    static transition_type const * p_path_abempty_tr;
    static transition_type const * p_relative_part_tr;
    static transition_type const * p_query_chars_tr;
    static transition_type const * p_query_tr;
    static transition_type const * p_relative_ref_tr_1;
    static transition_type const * p_fragment_tr;
    static transition_type const * p_relative_ref_tr_2;
    static transition_type const * p_relative_ref_tr;
    static transition_type const * p_scheme_chars_tr;
    static transition_type const * p_scheme_tr;
    static transition_type const * p_path_rootless_tr;
    static transition_type const * p_hier_part_tr;
    static transition_type const * p_uri_tr_1;
    static transition_type const * p_uri_tr_2;
#endif    
    static transition_type const * p_uri_tr;
    static transition_type const * p_uri_reference_tr;
};

#if __PFS_TEST__
template <typename UriT>
typename uri_grammar<UriT>::transition_type const * uri_grammar<UriT>::p_authority_tr = 0;
#endif

template <typename UriT>
typename uri_grammar<UriT>::transition_type const * uri_grammar<UriT>::p_uri_tr = 0;

template <typename UriT>
typename uri_grammar<UriT>::transition_type const * uri_grammar<UriT>::p_uri_reference_tr = 0;

template <typename UriT>
uri_grammar<UriT>::uri_grammar ()
{
    static string_type const alpha("ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz");
    static string_type const digit("0123456789");

    static string_type const hexdigit("0123456789ABCDEFabcdef");

    static transition_type const digit_tr[] = {
        {-1,-1, fsm_type::one_of(digit), fsm_type::accept, 0, 0 }
    };

    static transition_type const hexdig_tr[] = {
        {-1,-1, fsm_type::one_of(hexdigit), fsm_type::accept, 0, 0 }
    };

    /* ALPHA / DIGIT / "-" / "." / "_" / "~" */
    static transition_type const unreserved_tr[] = {
          {-1, 1, fsm_type::one_of(alpha), fsm_type::accept, 0, 0 }
        , {-1, 2, fsm_type::one_of(digit), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::one_of(string_type("-._~")), fsm_type::accept, 0, 0 }
    };

    /* "%" HEXDIG HEXDIG */
    static transition_type const pct_encoded_tr[] = {
          { 1,-1, fsm_type::seq(string_type("%")) , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::one_of(hexdigit), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::one_of(hexdigit), fsm_type::accept, 0, 0 }
    };

    /* "!" / "$" / "&" / "'" / "(" / ")"
           / "*" / "+" / "," / ";" / "="
    */
    static transition_type const sub_delims_tr[] = {
        {-1,-1, fsm_type::one_of(string_type("!$&'()*+,;=")), fsm_type::accept, 0, 0 }
    };

    /* unreserved / pct-encoded / sub-delims / ":" / "@" */
    static transition_type const pchar_tr[] = {
          {-1, 1, fsm_type::tr(unreserved_tr) , fsm_type::accept, 0, 0 }
        , {-1, 2, fsm_type::tr(pct_encoded_tr), fsm_type::accept, 0, 0 }
        , {-1, 3, fsm_type::tr(sub_delims_tr) , fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::one_of(string_type(":@")), fsm_type::accept, 0, 0 }
    };


    /* 1*pchar */
    static transition_type const segment_nz_tr[] = {
          { 1,-1, fsm_type::tr(pchar_tr),fsm_type::accept, 0, 0 }
        , { 1,-1, fsm_type::tr(pchar_tr),fsm_type::accept, 0, 0 }
    };

    /* *pchar */
    static transition_type const segment_tr[] = {
          { 0, 1, fsm_type::tr(pchar_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::nothing()   , fsm_type::accept, 0, 0 }
    };

    /* "/" segment */
    static transition_type const slash_segment_tr[] = {
          { 1,-1, fsm_type::seq(string_type("/")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(segment_tr)          , fsm_type::accept, 0, 0 }
    };

    /* segment-nz *slash_segment */
    static transition_type const segment_nz_slash_tr[] = {
          { 1,-1, fsm_type::tr(segment_nz_tr)              , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_tr(slash_segment_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /* "/" [ segment-nz *slash_segment ] */
    static transition_type const path_absolute_tr[] = {
          { 1,-1, fsm_type::seq(string_type("/"))    , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::opt_tr(segment_nz_slash_tr) , fsm_type::accept, 0, 0 }
    };

    /* 1*( unreserved / pct-encoded / sub-delims / "@" )
            ; non-zero-length segment without any colon ":"
    */
    static transition_type const segment_nc_tr[] = {
          {-1, 1, fsm_type::tr(unreserved_tr)       , fsm_type::accept, 0, 0 }
        , {-1, 2, fsm_type::tr(pct_encoded_tr)      , fsm_type::accept, 0, 0 }
        , {-1, 3, fsm_type::tr(sub_delims_tr)       , fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::one_of(string_type("@")), fsm_type::accept, 0, 0 }
    };

    static transition_type const segment_nz_nc_tr[] = {
        {-1,-1, fsm_type::rpt_tr(segment_nc_tr, 1, -1), fsm_type::accept, 0, 0 }
    };

    /* segment-nz-nc *slash-segment */
    static transition_type const path_noscheme_tr[] = {
          { 1,-1, fsm_type::tr(segment_nz_nc_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::rpt_tr(slash_segment_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /* 0<pchar> */
    static transition_type const path_empty_tr[] = {
        {-1,-1, fsm_type::nothing(), fsm_type::accept, 0, 0 }
    };

    /* 1*4HEXDIG */
    static transition_type const h16_tr[] = {
          { 1,-1, fsm_type::one_of(hexdigit), fsm_type::accept, 0, 0 }
        , { 2,-1, fsm_type::one_of(hexdigit), fsm_type::accept, 0, 0 }
        , { 3,-1, fsm_type::one_of(hexdigit), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::one_of(hexdigit), fsm_type::accept, 0, 0 }
    };

    /* "25" %x30-35        ; 250-255 */
    static transition_type const dec_octet_tr_4[] = {
          { 1,-1, fsm_type::seq(string_type("25"))    , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::one_of(string_type("012345")), fsm_type::accept, 0, 0 }
    };

    /* "2" %x30-34 DIGIT   ; 200-249 */
    static transition_type const dec_octet_tr_3[] = {
          { 1,-1, fsm_type::seq(string_type("2"))         , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::one_of(string_type("01234"))  , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::one_of(digit), fsm_type::accept, 0, 0 }
    };

    /* "1" 2DIGIT  ; 100-199 */
    static transition_type const dec_octet_tr_2[] = {
          { 1,-1, fsm_type::seq(string_type("1")), fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::one_of(digit)      , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::one_of(digit)      , fsm_type::accept, 0, 0 }
    };

    /* %x31-39 DIGIT       ; 10-99*/
    static transition_type const dec_octet_tr_1[] = {
          { 1,-1, fsm_type::one_of(string_type("123456789")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::one_of(digit), fsm_type::accept, 0, 0 }
    };

    /*
      DIGIT                 ; 0-9
      / %x31-39 DIGIT       ; 10-99
      / "1" 2DIGIT          ; 100-199
      / "2" %x30-34 DIGIT   ; 200-249
      / "25" %x30-35        ; 250-255
    */
    static transition_type const dec_octet_tr[] = {
          {-1, 1, fsm_type::tr(dec_octet_tr_4), fsm_type::accept, 0, 0 } /* 250 - 255 */
        , {-1, 2, fsm_type::tr(dec_octet_tr_3), fsm_type::accept, 0, 0 } /* 200 - 249 */
        , {-1, 3, fsm_type::tr(dec_octet_tr_2), fsm_type::accept, 0, 0 } /* 100 - 199 */
        , {-1, 4, fsm_type::tr(dec_octet_tr_1), fsm_type::accept, 0, 0 } /* 10 - 99 */
        , {-1,-1, fsm_type::one_of(digit), fsm_type::accept, 0, 0 } /* 0 - 9 */
    };

    /* dec-octet "." dec-octet "." dec-octet "." dec-octet */
    static transition_type const ipv4address_tr[] = {
          { 1,-1, fsm_type::tr(dec_octet_tr)     , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::seq(string_type(".")), fsm_type::normal, 0, 0 }
        , { 3,-1, fsm_type::tr(dec_octet_tr)     , fsm_type::normal, 0, 0 }
        , { 4,-1, fsm_type::seq(string_type(".")), fsm_type::normal, 0, 0 }
        , { 5,-1, fsm_type::tr(dec_octet_tr)     , fsm_type::normal, 0, 0 }
        , { 6,-1, fsm_type::seq(string_type(".")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(dec_octet_tr)     , fsm_type::accept, 0, 0 }
    };

    /* ( h16 ":" h16 ) / IPv4address */
    static transition_type const ls32_tr[] = {
          {-1, 1, fsm_type::tr(ipv4address_tr), fsm_type::accept, 0, 0 }
        , { 2,-1, fsm_type::tr(h16_tr)        , fsm_type::normal, 0, 0 }
        , { 3,-1, fsm_type::seq(string_type(":")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(h16_tr)        , fsm_type::accept, 0, 0 }
    };

    /* h16 ":" */
    static transition_type const h16_semi_tr[] = {
          { 1,-1, fsm_type::tr(h16_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::seq(string_type(":")), fsm_type::accept, 0, 0 }
    };

    /* *1( h16 ":" ) h16 */
    static transition_type const ipv6address_tr_4_1[] = {
          { 1,-1, fsm_type::rpt_tr(h16_semi_tr, 0, 1), fsm_type::normal, 0, 0 }
        , {-1, 2, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
    };

    /* *2( h16 ":" ) h16 */
    static transition_type const ipv6address_tr_5_1[] = {
          { 1, 2, fsm_type::rpt_tr(h16_semi_tr, 0, 2), fsm_type::normal, 0, 0 }
        , {-1, 2, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
    };

    /* *3( h16 ":" ) h16 */
    static transition_type const ipv6address_tr_6_1[] = {
          { 1, 2, fsm_type::rpt_tr(h16_semi_tr, 0, 3), fsm_type::normal, 0, 0 }
        , {-1, 2, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
    };

    /* *4( h16 ":" ) h16 */
    static transition_type const ipv6address_tr_7_1[] = {
          { 1, 2, fsm_type::rpt_tr(h16_semi_tr, 0, 4), fsm_type::normal, 0, 0 }
        , {-1, 2, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
    };

    /* *5( h16 ":" ) h16 */
    static transition_type const ipv6address_tr_8_1[] = {
          { 1, 2, fsm_type::rpt_tr(h16_semi_tr, 0, 5), fsm_type::normal, 0, 0 }
        , {-1, 2, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
    };

    /* *6( h16 ":" ) h16 */
    static transition_type const ipv6address_tr_9_1[] = {
          { 1, 2, fsm_type::rpt_tr(h16_semi_tr, 0, 6), fsm_type::normal, 0, 0 }
        , {-1, 2, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
    };

    /* 6( h16 ":" ) ls32 */
    static transition_type const ipv6address_tr_1[] = {
          { 1,-1, fsm_type::rpt_tr(h16_semi_tr, 6, 6), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(ls32_tr), fsm_type::accept, 0, 0 }
    };

    /* "::" 5( h16 ":" ) ls32 */
    static transition_type const ipv6address_tr_2[] = {
          { 1,-1, fsm_type::seq(string_type("::")), fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::rpt_tr(h16_semi_tr, 5, 5), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(ls32_tr)              , fsm_type::accept, 0, 0 }
    };

    /* [ h16 ] "::" 4( h16 ":" ) ls32 */
    static transition_type const ipv6address_tr_3[] = {
          { 1, 1, fsm_type::tr(h16_tr), fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::seq(string_type("::")), fsm_type::normal, 0, 0 }
        , { 3,-1, fsm_type::rpt_tr(h16_semi_tr, 4, 4), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(ls32_tr)              , fsm_type::accept, 0, 0 }
    };

    /* [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32 */
    static transition_type const ipv6address_tr_4[] = {
          { 1, 1, fsm_type::tr(ipv6address_tr_4_1), fsm_type::normal, 0, 0 }

        , { 2,-1, fsm_type::seq(string_type("::")), fsm_type::normal, 0, 0 }
        , { 3,-1, fsm_type::tr(h16_semi_tr), fsm_type::normal, 0, 0 }
        , { 4,-1, fsm_type::tr(h16_semi_tr), fsm_type::normal, 0, 0 }
        , { 5,-1, fsm_type::tr(h16_semi_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(ls32_tr)    , fsm_type::accept, 0, 0 }
    };

    /* [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32 */
    static transition_type const ipv6address_tr_5[] = {
          { 1, 2, fsm_type::tr(ipv6address_tr_5_1), fsm_type::normal, 0, 0 }
        , { 2, 2, fsm_type::tr(ipv6address_tr_5_1), fsm_type::normal, 0, 0 }

        , { 3,-1, fsm_type::seq(string_type("::")), fsm_type::normal, 0, 0 }
        , { 4,-1, fsm_type::tr(h16_semi_tr), fsm_type::normal, 0, 0 }
        , { 5,-1, fsm_type::tr(h16_semi_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(ls32_tr)    , fsm_type::accept, 0, 0 }
    };

    /* [ *3( h16 ":" ) h16 ] "::" h16 ":"   ls32 */
    static transition_type const ipv6address_tr_6[] = {
          { 1, 3, fsm_type::tr(ipv6address_tr_6_1), fsm_type::normal, 0, 0 }
        , { 2, 3, fsm_type::tr(ipv6address_tr_6_1), fsm_type::normal, 0, 0 }
        , { 3, 3, fsm_type::tr(ipv6address_tr_6_1), fsm_type::normal, 0, 0 }

        , { 4,-1, fsm_type::seq(string_type("::")), fsm_type::normal, 0, 0 }
        , { 5,-1, fsm_type::tr(h16_semi_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(ls32_tr)    , fsm_type::accept, 0, 0 }
    };


    /* [ *4( h16 ":" ) h16 ] "::" ls32 */
    static transition_type const ipv6address_tr_7[] = {
          { 1, 4, fsm_type::tr(ipv6address_tr_7_1), fsm_type::normal, 0, 0 }
        , { 2, 4, fsm_type::tr(ipv6address_tr_7_1), fsm_type::normal, 0, 0 }
        , { 3, 4, fsm_type::tr(ipv6address_tr_7_1), fsm_type::normal, 0, 0 }
        , { 4, 4, fsm_type::tr(ipv6address_tr_7_1), fsm_type::normal, 0, 0 }

        , { 5,-1, fsm_type::seq(string_type("::")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(ls32_tr) , fsm_type::accept, 0, 0 }
    };

    /* [ *5( h16 ":" ) h16 ] "::"              h16 */
    static transition_type const ipv6address_tr_8[] = {
          { 1, 5, fsm_type::tr(ipv6address_tr_8_1), fsm_type::normal, 0, 0 }
        , { 2, 5, fsm_type::tr(ipv6address_tr_8_1), fsm_type::normal, 0, 0 }
        , { 3, 5, fsm_type::tr(ipv6address_tr_8_1), fsm_type::normal, 0, 0 }
        , { 4, 5, fsm_type::tr(ipv6address_tr_8_1), fsm_type::normal, 0, 0 }
        , { 5, 5, fsm_type::tr(ipv6address_tr_8_1), fsm_type::normal, 0, 0 }

        , { 6,-1, fsm_type::seq(string_type("::")) , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(h16_tr)   , fsm_type::accept, 0, 0 }
    };


    /* [ *6( h16 ":" ) h16 ] "::" */
    static transition_type const ipv6address_tr_9[] = {
          { 1, 6, fsm_type::tr(ipv6address_tr_9_1), fsm_type::normal, 0, 0 }
        , { 2, 6, fsm_type::tr(ipv6address_tr_9_1), fsm_type::normal, 0, 0 }
        , { 3, 6, fsm_type::tr(ipv6address_tr_9_1), fsm_type::normal, 0, 0 }
        , { 4, 6, fsm_type::tr(ipv6address_tr_9_1), fsm_type::normal, 0, 0 }
        , { 5, 6, fsm_type::tr(ipv6address_tr_9_1), fsm_type::normal, 0, 0 }
        , { 6, 6, fsm_type::tr(ipv6address_tr_9_1), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::seq(string_type("::"))          , fsm_type::accept, 0, 0 }
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
          {-1, 1, fsm_type::tr(ipv6address_tr_1), fsm_type::accept, 0, 0 }
        , {-1, 2, fsm_type::tr(ipv6address_tr_2), fsm_type::accept, 0, 0 }
        , {-1, 3, fsm_type::tr(ipv6address_tr_3), fsm_type::accept, 0, 0 }
        , {-1, 4, fsm_type::tr(ipv6address_tr_4), fsm_type::accept, 0, 0 }
        , {-1, 5, fsm_type::tr(ipv6address_tr_5), fsm_type::accept, 0, 0 }
        , {-1, 6, fsm_type::tr(ipv6address_tr_6), fsm_type::accept, 0, 0 }
        , {-1, 7, fsm_type::tr(ipv6address_tr_7), fsm_type::accept, 0, 0 }
        , {-1, 8, fsm_type::tr(ipv6address_tr_8), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(ipv6address_tr_9), fsm_type::accept, 0, 0 }
    };

    /* "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" ) */
    static transition_type const ipvfuture_tail_tr[] = {
          {-1, 1, fsm_type::tr(unreserved_tr)    , fsm_type::accept, 0, 0 }
        , {-1, 2, fsm_type::tr(sub_delims_tr)    , fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::seq(string_type(":")), fsm_type::accept, 0, 0 }
    };

    static transition_type const ipvfuture_tr[] = {
          { 1,-1, fsm_type::seq(string_type("v"))           , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::rpt_tr(hexdig_tr, 1, -1)        , fsm_type::normal, 0, 0 }
        , { 3,-1, fsm_type::seq(string_type("."))           , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_tr(ipvfuture_tail_tr, 1, -1), fsm_type::accept, 0, 0 }
    };

    /* "[" ( IPv6address / IPvFuture  ) "]" */
    static transition_type const ip_literal_entry_tr[] = {
          {-1, 1, fsm_type::tr(ipv6address_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(ipvfuture_tr)  , fsm_type::accept, 0, 0 }
    };
    static transition_type const ip_literal_tr[] = {
          { 1,-1, fsm_type::seq(string_type("["))  , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::tr(ip_literal_entry_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::seq(string_type("]"))  , fsm_type::accept, 0, 0 }
    };

    /* *( unreserved / pct-encoded / sub-delims ) */
    static transition_type const reg_name_entry_tr[] = {
          {-1, 1, fsm_type::tr(unreserved_tr) , fsm_type::accept, 0, 0 }
        , {-1, 2, fsm_type::tr(pct_encoded_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(sub_delims_tr) , fsm_type::accept, 0, 0 }
    };
    static transition_type const reg_name_tr[] = {
          {-1,-1, fsm_type::rpt_tr(reg_name_entry_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /*  host = IP-literal / IPv4address / reg-name */
    static transition_type const host_tr[] = {
          {-1, 1, fsm_type::tr(ip_literal_tr) , fsm_type::accept, set_raw_host, 0}
        , {-1, 2, fsm_type::tr(ipv4address_tr), fsm_type::accept, set_raw_host, 0}

        /* OBSOLETE NOTE: this poses a parsing problem - all IPv4 addresses are valid
        reg_name as well. Fix this by doing the reg_name_tr now,
        then on match, do a function to check if its an IPv4
        address */
        , {-1,-1, fsm_type::tr(reg_name_tr), fsm_type::accept, 0, 0 }
    };

    /* *DIGIT */
    static transition_type const port_tr[] = {
          {-1,-1, fsm_type::rpt_tr(digit_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /* ":" port */
    static transition_type const authority_tr_2[] = {
          { 1,-1, fsm_type::seq(string_type(":")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(port_tr), fsm_type::accept, set_port, 0}
    };

    /* *( unreserved / pct-encoded / sub-delims / ":" ) */
    static transition_type const userinfo_tr[] = {
          { 0, 1, fsm_type::tr(unreserved_tr)    , fsm_type::accept, 0, 0 }
        , { 0, 2, fsm_type::tr(pct_encoded_tr)   , fsm_type::accept, 0, 0 }
        , { 0, 3, fsm_type::tr(sub_delims_tr)    , fsm_type::accept, 0, 0 }
        , { 0, 4, fsm_type::seq(string_type(":")), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::nothing(), fsm_type::accept, 0, 0 }
    };

    /* userinfo "@" */
    static transition_type const authority_tr_1[] = {
          { 1,-1, fsm_type::tr(userinfo_tr)      , fsm_type::normal, set_userinfo, 0 }
        , {-1,-1, fsm_type::seq(string_type("@")), fsm_type::accept, 0, 0 }
    };

    /* [ userinfo "@" ] host [ ":" port ] */
    /* [ authority_tr_1 ] host [ authority_tr_2 ] */
    static transition_type const authority_tr[] = {
          { 1, 3, fsm_type::opt_tr(authority_tr_1) , fsm_type::normal, 0, 0 }
        , { 2, 3, fsm_type::tr(host_tr)            , fsm_type::normal, set_host, 0 }
        , {-1, 3, fsm_type::opt_tr(authority_tr_2) , fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::nothing()              , fsm_type::reject, unset_userinfo, 0 }
    };

    /*  *( "/" segment ) */
    static transition_type const path_abempty_tr[] = {
          {-1,-1, fsm_type::rpt_tr(slash_segment_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /*
    relative-part = "//" authority path-abempty
                  / path-absolute
                  / path-noscheme`
                  / path-empty
    */
    static transition_type const relative_part_tr[] = {
          { 1, 3, fsm_type::seq(string_type("//")), fsm_type::normal, 0, 0 }
        , { 2, 3, fsm_type::tr(authority_tr)      , fsm_type::normal, set_path, 0 }
        , {-1, 3, fsm_type::tr(path_abempty_tr)   , fsm_type::accept, set_path, 0 }

        , {-1, 4, fsm_type::tr(path_absolute_tr), fsm_type::accept, set_path, 0 }
        , {-1, 5, fsm_type::tr(path_noscheme_tr), fsm_type::accept, set_path, 0 }
        , {-1,-1, fsm_type::tr(path_empty_tr)   , fsm_type::accept, set_path, 0 }
    };

    /* *( pchar / "/" / "?" ) */
    static transition_type const query_chars_tr[] = {
          {-1, 1, fsm_type::tr(pchar_tr)             , fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::one_of(string_type("/?")), fsm_type::accept, 0, 0 }
    };
    static transition_type const query_tr[] = {
        {-1,-1, fsm_type::rpt_tr(query_chars_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /* "?" query */
    static transition_type const relative_ref_tr_1[] = {
          { 1,-1, fsm_type::seq(string_type("?")) , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(query_tr), fsm_type::accept, set_query, 0 }
    };

    /* *( pchar / "/" / "?" ) */
    static transition_type const fragment_tr[] = {
        {-1,-1, fsm_type::rpt_tr(query_chars_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /* "#" fragment */
    static transition_type const relative_ref_tr_2[] = {
          { 1,-1, fsm_type::seq(string_type("#")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(fragment_tr), fsm_type::accept, set_fragment, 0}
    };

    /* relative-part [ "?" query ] [ "#" fragment ] */
    static transition_type const relative_ref_tr[] = {
          { 1,-1, fsm_type::tr(relative_part_tr)           , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::rpt_tr(relative_ref_tr_1, 0, 1), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_tr(relative_ref_tr_2, 0, 1), fsm_type::accept, 0, 0 }
    };

    /* ALPHA *( ALPHA / DIGIT / "+" / "-" / "." ) */
    static transition_type const scheme_chars_tr[] = {
          {-1, 1, fsm_type::one_of(alpha), fsm_type::accept, 0, 0 }
        , {-1, 2, fsm_type::one_of(digit), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::one_of(string_type("+-.")), fsm_type::accept, 0, 0 }
    };
    static transition_type const scheme_tr[] = {
          { 1,-1, fsm_type::one_of(alpha), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_tr(scheme_chars_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /* segment-nz *( "/" segment ) */
    static transition_type const path_rootless_tr[] = {
          { 1,-1, fsm_type::tr(segment_nz_tr)              , fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::rpt_tr(slash_segment_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /*
        "//" authority path-abempty
             / path-absolute
             / path-rootless
             / path-empty
    */
    static transition_type const hier_part_tr[] = {
          { 1, 3, fsm_type::seq(string_type("//")), fsm_type::normal, 0, 0 }
        , { 2, 3, fsm_type::tr(authority_tr)      , fsm_type::normal, set_path, 0 }
        , {-1, 3, fsm_type::tr(path_abempty_tr)   , fsm_type::accept, set_path, 0 }

        , {-1, 4, fsm_type::tr(path_absolute_tr)  , fsm_type::accept, set_path, 0 }
        , {-1, 5, fsm_type::tr(path_rootless_tr)  , fsm_type::accept, set_path, 0 }
        , {-1,-1, fsm_type::tr(path_empty_tr)     ,    fsm_type::accept, set_path, 0 }
    };


    /* "?" query */
    static transition_type const uri_tr_1[] = {
          { 1,-1, fsm_type::seq(string_type("?")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(query_tr), fsm_type::accept, set_query, 0 }
    };

    /* "#" fragment */
    static transition_type const uri_tr_2[] = {
          { 1,-1, fsm_type::seq(string_type("#")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(fragment_tr), fsm_type::accept, set_fragment, 0 }
    };


    /* scheme ":" hier-part [ "?" query ] [ "#" fragment ] */
    static transition_type const uri_tr[] = {
          { 1,-1, fsm_type::tr(scheme_tr)         , fsm_type::normal, set_scheme, 0 }
        , { 2,-1, fsm_type::seq(string_type(":")) , fsm_type::normal, 0, 0 }
        , { 3,-1, fsm_type::tr(hier_part_tr)      , fsm_type::normal, 0, 0 }
        , { 4,-1, fsm_type::rpt_tr(uri_tr_1, 0, 1), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_tr(uri_tr_2, 0, 1), fsm_type::accept, 0, 0 }
    };

    /* URI / relative-ref */
    static transition_type const uri_reference_tr[] = {
          {-1, 1, fsm_type::tr(uri_tr)         , fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(relative_ref_tr), fsm_type::accept, 0, 0 }
    };
    
    p_uri_tr = & uri_tr[0];
    p_uri_reference_tr = & uri_reference_tr[0];
    
#if __PFS_TEST__
    p_authority_tr = & authority_tr[0];
#endif
}

template <typename StringT>
bool uri_grammar<StringT>::set_port (const_iterator begin
		, const_iterator end
		, void * context
		, void *)
{
	if (context) {
		user_context * ctx = reinterpret_cast<user_context *>(context);
		string_type digits(begin, end);

        try {
            ctx->port = lexical_cast<string_type, uint16_t>(digits, 10);
        } catch (bad_lexical_cast<StringT> ex) {
            return false;
        }
	}
	return true;
}

template <typename StringT>
inline bool uri_grammar<StringT>::set_raw_host (const_iterator
		, const_iterator
		, void * context
		, void *)
{
	if (context) {
		user_context * ctx = reinterpret_cast<user_context *>(context);
		ctx->is_raw_host = true;
	}
	return true;
}

template <typename StringT>
inline bool uri_grammar<StringT>::unset_userinfo (const_iterator
		, const_iterator
		, void * context
		, void *)
{
	if (context) {
		user_context * ctx = reinterpret_cast<user_context *>(context);
		ctx->userinfo.clear();
	}
	return true;
}

template <typename StringT>
inline bool uri_grammar<StringT>::set_scheme (const_iterator begin
		, const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<user_context *>(context)->scheme = string_type(begin, end);
	}
	return true;
}

template <typename StringT>
inline bool uri_grammar<StringT>::set_userinfo (const_iterator begin
		, const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<user_context *>(context)->userinfo = string_type(begin, end);
	}
	return true;
}

template <typename StringT>
inline bool uri_grammar<StringT>::set_host (const_iterator begin
		, const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<user_context *>(context)->host = string_type(begin, end);
	}
	return true;
}

template <typename StringT>
inline bool uri_grammar<StringT>::set_path (const_iterator begin
		, const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<user_context *>(context)->path = string_type(begin, end);
	}
	return true;
}

template <typename StringT>
inline bool uri_grammar<StringT>::set_query (const_iterator begin
		, const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<user_context *>(context)->query = string_type(begin, end);
	}
	return true;
}

template <typename StringT>
inline bool uri_grammar<StringT>::set_fragment (const_iterator begin
		, const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<user_context *>(context)->fragment = string_type(begin, end);
	}
	return true;
}


/**
 * @brief  Parse the given string as URI.
 * @details Implementation of RFC 3986 (http://tools.ietf.org/html/rfc3986)
 *
 * @param uri string representation of URI.
 */
template <typename StringT>
bool uri<StringT>::parse (string_type const & str)
{
    typedef pfs::fsm::fsm<string_type> fsm_type;
    
    // Initialize grammar's static memebers
    static uri_grammar<string_type> grammar;

    _d.clear();
    
	fsm_type fsm(*grammar.p_uri_tr, & _d);
	typename fsm_type::result_type r = fsm.exec(0, str.begin(), str.end());

	if (r.first)
		return true;

	return false;
}

}} // pfs::net

#endif /* __PFS_URI_RFC3986_HPP__ */
