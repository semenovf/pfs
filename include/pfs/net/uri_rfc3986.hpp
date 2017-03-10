/*
 * Copyright (c) 2012, 2013, 2017 Fedor Semenov
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear
 * in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * uri-rfc3986.hpp
 *
 *  Created on: 12.10.2012
 *  Modified on: 10.03.2017
 *      Author: Fedor Semenov
 */


#ifndef __PFS_NET_URI_RFC3986_HPP__
#define __PFS_NET_URI_RFC3986_HPP__

#include <pfs/fsm.hpp>

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

/**
 * @brief  Parse the given string as URI.
 * @details Implementation of RFC 3986 (http://tools.ietf.org/html/rfc3986)
 *
 * @param uri string representation of URI.
 */
template <typename StringT>
bool uri::parse (string_type const & uri)
{
    typedef pfs::fsm::fsm<string_type> fsm_type;
    
    static string_type __alpha("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    static string_type __digit("0123456789");
    static string_type __hexdigit("0123456789ABCDEFabcdef"); /* DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */

    static fsm_type::transition_type digit_tr[] = {
        {-1,-1, fsm_type::one_of(__digit), fsm_type::accept, 0, 0 }
    };

    static fsm_type::transition_type hexdig_tr[] = {
        {-1,-1, fsm_type::one_of(__hexdigit), fsm_type::accept, 0, 0 }
    };

    /* ALPHA / DIGIT / "-" / "." / "_" / "~" */
    static fsm_type::transition_type unreserved_tr[] = {
          {-1, 1, fsm_type::one_of(__alpha), fsm_type::accept, 0, 0 }
        , {-1, 2, fsm_type::one_of(__digit), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::one_of(string_type("-._~")), fsm_type::accept, 0, 0 }
    };

    /* "%" HEXDIG HEXDIG */
    static fsm_type::transition_type pct_encoded_tr[] = {
          { 1,-1, fsm_type::seq(string_type("%")) , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::one_of(__hexdigit), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::one_of(__hexdigit), fsm_type::accept, 0, 0 }
    };

    /* "!" / "$" / "&" / "'" / "(" / ")"
           / "*" / "+" / "," / ";" / "="
    */
    static fsm_type::transition_type sub_delims_tr[] = {
        {-1,-1, fsm_type::one_of(string_type("!$&'()*+,;=")), fsm_type::accept, 0, 0 }
    };

    /* unreserved / pct-encoded / sub-delims / ":" / "@" */
    static fsm_type::transition_type pchar_tr[] = {
          {-1, 1, fsm_type::tr(unreserved_tr) , fsm_type::accept, 0, 0 }
        , {-1, 2, fsm_type::tr(pct_encoded_tr), fsm_type::accept, 0, 0 }
        , {-1, 3, fsm_type::tr(sub_delims_tr) , fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::one_of(string_type(":@")), fsm_type::accept, 0, 0 }
    };


    /* 1*pchar */
    static fsm_type::transition_type segment_nz_tr[] = {
          { 1,-1, fsm_type::tr(pchar_tr),fsm_type::accept, 0, 0 }
        , { 1,-1, fsm_type::tr(pchar_tr),fsm_type::accept, 0, 0 }
    };

    /* *pchar */
    static fsm_type::transition_type segment_tr[] = {
          { 0, 1, fsm_type::tr(pchar_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::nothing()   , fsm_type::accept, 0, 0 }
    };

    /* "/" segment */
    static fsm_type::transition_type slash_segment_tr[] = {
          { 1,-1, fsm_type::seq(string_type("/")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(segment_tr)          , fsm_type::accept, 0, 0 }
    };

    /* segment-nz *slash_segment */
    static fsm_type::transition_type segment_nz_slash_tr[] = {
          { 1,-1, fsm_type::tr(segment_nz_tr)              , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_tr(slash_segment_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /* "/" [ segment-nz *slash_segment ] */
    static fsm_type::transition_type path_absolute_tr[] = {
          { 1,-1, fsm_type::seq(string_type("/"))    , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::opt_tr(segment_nz_slash_tr) , fsm_type::accept, 0, 0 }
    };

    /* 1*( unreserved / pct-encoded / sub-delims / "@" )
            ; non-zero-length segment without any colon ":"
    */
    static fsm_type::transition_type segment_nc_tr[] = {
          {-1, 1, fsm_type::tr(unreserved_tr)       , fsm_type::accept, 0, 0 }
        , {-1, 2, fsm_type::tr(pct_encoded_tr)      , fsm_type::accept, 0, 0 }
        , {-1, 3, fsm_type::tr(sub_delims_tr)       , fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::one_of(string_type("@")), fsm_type::accept, 0, 0 }
    };

    static fsm_type::transition_type segment_nz_nc_tr[] = {
        {-1,-1, fsm_type::rpt_tr(segment_nc_tr, 1, -1), fsm_type::accept, 0, 0 }
    };

    /* segment-nz-nc *slash-segment */
    static fsm_type::transition_type path_noscheme_tr[] = {
          { 1,-1, fsm_type::tr(segment_nz_nc_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::rpt_tr(slash_segment_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /* 0<pchar> */
    static fsm_type::transition_type path_empty_tr[] = {
        {-1,-1, M::nothing(), fsm_type::accept, 0, 0 }
    };

    /* 1*4HEXDIG */
    static fsm_type::transition_type h16_tr[] = {
          { 1,-1, fsm_type::one_of(__hexdigit), fsm_type::accept, 0, 0 }
        , { 2,-1, fsm_type::one_of(__hexdigit), fsm_type::accept, 0, 0 }
        , { 3,-1, fsm_type::one_of(__hexdigit), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::one_of(__hexdigit), fsm_type::accept, 0, 0 }
    };

    /* "25" %x30-35        ; 250-255 */
    static fsm_type::transition_type dec_octet_tr_4[] = {
          { 1,-1, fsm_type::seq(string_type("25"))    , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::one_of(string_type("012345")), fsm_type::accept, 0, 0 }
    };

    /* "2" %x30-34 DIGIT   ; 200-249 */
    static fsm_type::transition_type dec_octet_tr_3[] = {
          { 1,-1, fsm_type::seq(string_type("2"))    , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::one_of(string_type("01234")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::one_of(__digit)             , fsm_type::accept, 0, 0 }
    };

    /* "1" 2DIGIT  ; 100-199 */
    static fsm_type::transition_type dec_octet_tr_2[] = {
          { 1,-1, fsm_type::seq(string_type("1")), fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::one_of(__digit)      , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::one_of(__digit)      , fsm_type::accept, 0, 0 }
    };

    /* %x31-39 DIGIT       ; 10-99*/
    static fsm_type::transition_type dec_octet_tr_1[] = {
          { 1,-1, fsm_type::one_of(string_type("123456789")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::one_of(__digit), fsm_type::accept, 0, 0 }
    };

    /*
      DIGIT                 ; 0-9
      / %x31-39 DIGIT       ; 10-99
      / "1" 2DIGIT          ; 100-199
      / "2" %x30-34 DIGIT   ; 200-249
      / "25" %x30-35        ; 250-255
    */
    static fsm_type::transition_type dec_octet_tr[] = {
          {-1, 1, fsm_type::tr(dec_octet_tr_4), fsm_type::accept, 0, 0 } /* 250 - 255 */
        , {-1, 2, fsm_type::tr(dec_octet_tr_3), fsm_type::accept, 0, 0 } /* 200 - 249 */
        , {-1, 3, fsm_type::tr(dec_octet_tr_2), fsm_type::accept, 0, 0 } /* 100 - 199 */
        , {-1, 4, fsm_type::tr(dec_octet_tr_1), fsm_type::accept, 0, 0 } /* 10 - 99 */
        , {-1,-1, fsm_type::one_of(__digit)   , fsm_type::accept, 0, 0 } /* 0 - 9 */
    };

    /* dec-octet "." dec-octet "." dec-octet "." dec-octet */
    static fsm_type::transition_type ipv4address_tr[] = {
          { 1,-1, fsm_type::tr(dec_octet_tr)     , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::seq(string_type(".")), fsm_type::normal, 0, 0 }
        , { 3,-1, fsm_type::tr(dec_octet_tr)     , fsm_type::normal, 0, 0 }
        , { 4,-1, fsm_type::seq(string_type(".")), fsm_type::normal, 0, 0 }
        , { 5,-1, fsm_type::tr(dec_octet_tr)     , fsm_type::normal, 0, 0 }
        , { 6,-1, fsm_type::seq(string_type(".")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(dec_octet_tr)     , fsm_type::accept, 0, 0 }
    };

    /* ( h16 ":" h16 ) / IPv4address */
    static fsm_type::transition_type ls32_tr[] = {
          {-1, 1, fsm_type::tr(ipv4address_tr), fsm_type::accept, 0, 0 }
        , { 2,-1, fsm_type::tr(h16_tr)        , fsm_type::normal, 0, 0 }
        , { 3,-1, fsm_type::seq(string_type(":")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(h16_tr)        , fsm_type::accept, 0, 0 }
    };

    /* h16 ":" */
    static fsm_type::transition_type h16_semi_tr[] = {
          { 1,-1, fsm_type::tr(h16_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::seq(string_type(":")), fsm_type::accept, 0, 0 }
    };

    /* *1( h16 ":" ) h16 */
    static fsm_type::transition_type ipv6address_tr_4_1[] = {
          { 1,-1, fsm_type::rpt_tr(h16_semi_tr, 0, 1), fsm_type::normal, 0, 0 }
        , {-1, 2, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
    };

    /* *2( h16 ":" ) h16 */
    static fsm_type::transition_type ipv6address_tr_5_1[] = {
          { 1, 2, fsm_type::rpt_tr(h16_semi_tr, 0, 2), fsm_type::normal, 0, 0 }
        , {-1, 2, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
    };

    /* *3( h16 ":" ) h16 */
    static fsm_type::transition_type ipv6address_tr_6_1[] = {
          { 1, 2, fsm_type::rpt_tr(h16_semi_tr, 0, 3), fsm_type::normal, 0, 0 }
        , {-1, 2, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
    };

    /* *4( h16 ":" ) h16 */
    static fsm_type::transition_type ipv6address_tr_7_1[] = {
          { 1, 2, fsm_type::rpt_tr(h16_semi_tr, 0, 4), fsm_type::normal, 0, 0 }
        , {-1, 2, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
    };

    /* *5( h16 ":" ) h16 */
    static fsm_type::transition_type ipv6address_tr_8_1[] = {
          { 1, 2, fsm_type::rpt_tr(h16_semi_tr, 0, 5), fsm_type::normal, 0, 0 }
        , {-1, 2, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
    };

    /* *6( h16 ":" ) h16 */
    static fsm_type::transition_type ipv6address_tr_9_1[] = {
          { 1, 2, fsm_type::rpt_tr(h16_semi_tr, 0, 6), fsm_type::normal, 0, 0 }
        , {-1, 2, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(h16_tr), fsm_type::accept, 0, 0 }
    };

    /* 6( h16 ":" ) ls32 */
    static fsm_type::transition_type ipv6address_tr_1[] = {
          { 1,-1, fsm_type::rpt_tr(h16_semi_tr, 6, 6), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(ls32_tr), fsm_type::accept, 0, 0 }
    };

    /* "::" 5( h16 ":" ) ls32 */
    static fsm_type::transition_type ipv6address_tr_2[] = {
          { 1,-1, fsm_type::seq(string_type("::")), fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::rpt_tr(h16_semi_tr, 5, 5), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(ls32_tr)              , fsm_type::accept, 0, 0 }
    };

    /* [ h16 ] "::" 4( h16 ":" ) ls32 */
    static fsm_type::transition_type ipv6address_tr_3[] = {
          { 1, 1, fsm_type::tr(h16_tr), fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::seq(string_type("::")), fsm_type::normal, 0, 0 }
        , { 3,-1, fsm_type::rpt_tr(h16_semi_tr, 4, 4), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(ls32_tr)              , fsm_type::accept, 0, 0 }
    };

    /* [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32 */
    static fsm_type::transition_type ipv6address_tr_4[] = {
          { 1, 1, fsm_type::tr(ipv6address_tr_4_1), fsm_type::normal, 0, 0 }

        , { 2,-1, fsm_type::seq(string_type("::")), fsm_type::normal, 0, 0 }
        , { 3,-1, fsm_type::tr(h16_semi_tr), fsm_type::normal, 0, 0 }
        , { 4,-1, fsm_type::tr(h16_semi_tr), fsm_type::normal, 0, 0 }
        , { 5,-1, fsm_type::tr(h16_semi_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(ls32_tr)    , fsm_type::accept, 0, 0 }
    };

    /* [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32 */
    static fsm_type::transition_type ipv6address_tr_5[] = {
          { 1, 2, fsm_type::tr(ipv6address_tr_5_1), fsm_type::normal, 0, 0 }
        , { 2, 2, fsm_type::tr(ipv6address_tr_5_1), fsm_type::normal, 0, 0 }

        , { 3,-1, fsm_type::seq(string_type("::")), fsm_type::normal, 0, 0 }
        , { 4,-1, fsm_type::tr(h16_semi_tr), fsm_type::normal, 0, 0 }
        , { 5,-1, fsm_type::tr(h16_semi_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(ls32_tr)    , fsm_type::accept, 0, 0 }
    };

    /* [ *3( h16 ":" ) h16 ] "::" h16 ":"   ls32 */
    static fsm_type::transition_type ipv6address_tr_6[] = {
          { 1, 3, fsm_type::tr(ipv6address_tr_6_1), fsm_type::normal, 0, 0 }
        , { 2, 3, fsm_type::tr(ipv6address_tr_6_1), fsm_type::normal, 0, 0 }
        , { 3, 3, fsm_type::tr(ipv6address_tr_6_1), fsm_type::normal, 0, 0 }

        , { 4,-1, fsm_type::seq(string_type("::")), fsm_type::normal, 0, 0 }
        , { 5,-1, fsm_type::tr(h16_semi_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(ls32_tr)    , fsm_type::accept, 0, 0 }
    };


    /* [ *4( h16 ":" ) h16 ] "::" ls32 */
    static fsm_type::transition_type ipv6address_tr_7[] = {
          { 1, 4, fsm_type::tr(ipv6address_tr_7_1), fsm_type::normal, 0, 0 }
        , { 2, 4, fsm_type::tr(ipv6address_tr_7_1), fsm_type::normal, 0, 0 }
        , { 3, 4, fsm_type::tr(ipv6address_tr_7_1), fsm_type::normal, 0, 0 }
        , { 4, 4, fsm_type::tr(ipv6address_tr_7_1), fsm_type::normal, 0, 0 }

        , { 5,-1, fsm_type::seq(string_type("::")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(ls32_tr) , fsm_type::accept, 0, 0 }
    };

    /* [ *5( h16 ":" ) h16 ] "::"              h16 */
    static fsm_type::transition_type ipv6address_tr_8[] = {
          { 1, 5, fsm_type::tr(ipv6address_tr_8_1), fsm_type::normal, 0, 0 }
        , { 2, 5, fsm_type::tr(ipv6address_tr_8_1), fsm_type::normal, 0, 0 }
        , { 3, 5, fsm_type::tr(ipv6address_tr_8_1), fsm_type::normal, 0, 0 }
        , { 4, 5, fsm_type::tr(ipv6address_tr_8_1), fsm_type::normal, 0, 0 }
        , { 5, 5, fsm_type::tr(ipv6address_tr_8_1), fsm_type::normal, 0, 0 }

        , { 6,-1, fsm_type::seq(string_type("::")) , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(h16_tr)   , fsm_type::accept, 0, 0 }
    };


    /* [ *6( h16 ":" ) h16 ] "::" */
    static fsm_type::transition_type ipv6address_tr_9[] = {
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
    static fsm_type::transition_type ipv6address_tr[] = {
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
    static fsm_type::transition_type ipvfuture_tail_tr[] = {
          {-1, 1, fsm_type::tr(unreserved_tr)    , fsm_type::accept, 0, 0 }
        , {-1, 2, fsm_type::tr(sub_delims_tr)    , fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::seq(string_type(":")), fsm_type::accept, 0, 0 }
    };

    static fsm_type::transition_type ipvfuture_tr[] = {
          { 1,-1, fsm_type::seq(string_type("v"))           , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::rpt_tr(hexdig_tr, 1, -1)        , fsm_type::normal, 0, 0 }
        , { 3,-1, fsm_type::seq(string_type("."))           , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_tr(ipvfuture_tail_tr, 1, -1), fsm_type::accept, 0, 0 }
    };

    /* "[" ( IPv6address / IPvFuture  ) "]" */
    static fsm_type::transition_type ip_literal_entry_tr[] = {
          {-1, 1, fsm_type::tr(ipv6address_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(ipvfuture_tr)  , fsm_type::accept, 0, 0 }
    };
    static fsm_type::transition_type ip_literal_tr[] = {
          { 1,-1, fsm_type::seq(string_type("["))  , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::tr(ip_literal_entry_tr), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::seq(string_type("]"))  , fsm_type::accept, 0, 0 }
    };

    /* *( unreserved / pct-encoded / sub-delims ) */
    static fsm_type::transition_type reg_name_entry_tr[] = {
          {-1, 1, fsm_type::tr(unreserved_tr) , fsm_type::accept, 0, 0 }
        , {-1, 2, fsm_type::tr(pct_encoded_tr), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(sub_delims_tr) , fsm_type::accept, 0, 0 }
    };
    static fsm_type::transition_type reg_name_tr[] = {
          {-1,-1, fsm_type::rpt_tr(reg_name_entry_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /*  host = IP-literal / IPv4address / reg-name */
    static fsm_type::transition_type host_tr[] = {
          {-1, 1, fsm_type::tr(ip_literal_tr) , fsm_type::accept, set_host_is_ip, 0}
        , {-1, 2, fsm_type::tr(ipv4address_tr), fsm_type::accept, set_host_is_ip, 0}

        /* OBSOLETE NOTE: this poses a parsing problem - all IPv4 addresses are valid
        reg_name as well. Fix this by doing the reg_name_tr now,
        then on match, do a function to check if its an IPv4
        address */
        , {-1,-1, fsm_type::tr(reg_name_tr), fsm_type::accept, 0, 0 }
    };

    /* *DIGIT */
    static fsm_type::transition_type port_tr[] = {
          {-1,-1, fsm_type::rpt_tr(digit_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /* ":" port */
    static fsm_type::transition_type authority_tr_2[] = {
          { 1,-1, fsm_type::seq(string_type(":")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(port_tr)          , fsm_type::accept, set_port, 0}
    };

    /* *( unreserved / pct-encoded / sub-delims / ":" ) */
    static fsm_type::transition_type userinfo_tr[] = {
          { 0, 1, fsm_type::tr(unreserved_tr)    , fsm_type::accept, 0, 0 }
        , { 0, 2, fsm_type::tr(pct_encoded_tr)   , fsm_type::accept, 0, 0 }
        , { 0, 3, fsm_type::tr(sub_delims_tr)    , fsm_type::accept, 0, 0 }
        , { 0, 4, fsm_type::seq(string_type(":")), fsm_type::accept, 0, 0 }
        , {-1,-1, M::nothing()                   , fsm_type::accept, 0, 0 }
    };

    /* userinfo "@" */
    static fsm_type::transition_type authority_tr_1[] = {
          { 1,-1, fsm_type::tr(userinfo_tr)      , fsm_type::normal, set_userinfo, 0 }
        , {-1,-1, fsm_type::seq(string_type("@")), fsm_type::accept, 0, 0 }
    };

    /* [ userinfo "@" ] host [ ":" port ] */
    /* [ authority_tr_1 ] host [ authority_tr_2 ] */
    static fsm_type::transition_type authority_tr[] = {
          { 1, 3, fsm_type::opt_tr(authority_tr_1) , fsm_type::normal, 0, 0 }
        , { 2, 3, fsm_type::tr(host_tr)            , fsm_type::normal, set_host, 0 }
        , {-1, 3, fsm_type::opt_tr(authority_tr_2) , fsm_type::accept, 0, 0 }
        , {-1,-1, M::nothing()                     , fsm_type::reject, unset_userinfo, 0 }
    };

    /*  *( "/" segment ) */
    static fsm_type::transition_type path_abempty_tr[] = {
          {-1,-1, fsm_type::rpt_tr(slash_segment_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /*
    relative-part = "//" authority path-abempty
                  / path-absolute
                  / path-noscheme`
                  / path-empty
    */
    static fsm_type::transition_type relative_part_tr[] = {
          { 1, 3, fsm_type::seq(string_type("//")), fsm_type::normal, 0, 0 }
        , { 2, 3, fsm_type::tr(authority_tr)      , fsm_type::normal, set_path, 0 }
        , {-1, 3, fsm_type::tr(path_abempty_tr)   , fsm_type::accept, set_path, 0 }

        , {-1, 4, fsm_type::tr(path_absolute_tr), fsm_type::accept, set_path, 0 }
        , {-1, 5, fsm_type::tr(path_noscheme_tr), fsm_type::accept, set_path, 0 }
        , {-1,-1, fsm_type::tr(path_empty_tr)   , fsm_type::accept, set_path, 0 }
    };

    /* *( pchar / "/" / "?" ) */
    static fsm_type::transition_type query_chars_tr[] = {
          {-1, 1, fsm_type::tr(pchar_tr)             , fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::one_of(string_type("/?")), fsm_type::accept, 0, 0 }
    };
    static fsm_type::transition_type query_tr[] = {
        {-1,-1, fsm_type::rpt_tr(query_chars_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /* "?" query */
    static fsm_type::transition_type relative_ref_tr_1[] = {
          { 1,-1, fsm_type::seq(string_type("?")) , fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(query_tr), fsm_type::accept, set_query, 0 }
    };

    /* *( pchar / "/" / "?" ) */
    static fsm_type::transition_type fragment_tr[] = {
        {-1,-1, fsm_type::rpt_tr(query_chars_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /* "#" fragment */
    static fsm_type::transition_type relative_ref_tr_2[] = {
          { 1,-1, fsm_type::seq(string_type("#")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(fragment_tr), fsm_type::accept, set_fragment, 0}
    };

    /* relative-part [ "?" query ] [ "#" fragment ] */
    static fsm_type::transition_type relative_ref_tr[] = {
          { 1,-1, fsm_type::tr(relative_part_tr)           , fsm_type::normal, 0, 0 }
        , { 2,-1, fsm_type::rpt_tr(relative_ref_tr_1, 0, 1), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_tr(relative_ref_tr_2, 0, 1), fsm_type::accept, 0, 0 }
    };

    /* ALPHA *( ALPHA / DIGIT / "+" / "-" / "." ) */
    static fsm_type::transition_type scheme_chars_tr[] = {
          {-1, 1, fsm_type::one_of(__alpha), fsm_type::accept, 0, 0 }
        , {-1, 2, fsm_type::one_of(__digit), fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::one_of(string_type("+-.")), fsm_type::accept, 0, 0 }
    };
    static fsm_type::transition_type scheme_tr[] = {
          { 1,-1, fsm_type::one_of(__alpha), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_tr(scheme_chars_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /* segment-nz *( "/" segment ) */
    static fsm_type::transition_type path_rootless_tr[] = {
          { 1,-1, fsm_type::tr(segment_nz_tr)              , fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::rpt_tr(slash_segment_tr, 0, -1), fsm_type::accept, 0, 0 }
    };

    /*
        "//" authority path-abempty
             / path-absolute
             / path-rootless
             / path-empty
    */
    static fsm_type::transition_type hier_part_tr[] = {
          { 1, 3, fsm_type::seq(string_type("//")), fsm_type::normal, 0, 0 }
        , { 2, 3, fsm_type::tr(authority_tr)      , fsm_type::normal, set_path, 0 }
        , {-1, 3, fsm_type::tr(path_abempty_tr)   , fsm_type::accept, set_path, 0 }

        , {-1, 4, fsm_type::tr(path_absolute_tr)  , fsm_type::accept, set_path, 0 }
        , {-1, 5, fsm_type::tr(path_rootless_tr)  , fsm_type::accept, set_path, 0 }
        , {-1,-1, fsm_type::tr(path_empty_tr)     ,    fsm_type::accept, set_path, 0 }
    };


    /* "?" query */
    static fsm_type::transition_type uri_tr_1[] = {
          { 1,-1, fsm_type::seq(string_type("?")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(query_tr), fsm_type::accept, set_query, 0 }
    };

    /* "#" fragment */
    static fsm_type::transition_type uri_tr_2[] = {
          { 1,-1, fsm_type::seq(string_type("#")), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::tr(fragment_tr), fsm_type::accept, set_fragment, 0 }
    };


    /* scheme ":" hier-part [ "?" query ] [ "#" fragment ] */
    static fsm_type::transition_type uri_tr[] = {
          { 1,-1, fsm_type::tr(scheme_tr)         , fsm_type::normal, set_scheme, 0 }
        , { 2,-1, fsm_type::seq(string_type(":")) , fsm_type::normal, 0, 0 }
        , { 3,-1, fsm_type::tr(hier_part_tr)      , fsm_type::normal, 0, 0 }
        , { 4,-1, fsm_type::rpt_tr(uri_tr_1, 0, 1), fsm_type::normal, 0, 0 }
        , {-1,-1, fsm_type::rpt_tr(uri_tr_2, 0, 1), fsm_type::accept, 0, 0 }
    };


    /* URI / relative-ref */
    static fsm_type::transition_type uri_reference_tr[] = {
          {-1, 1, fsm_type::tr(uri_tr)         , fsm_type::accept, 0, 0 }
        , {-1,-1, fsm_type::tr(relative_ref_tr), fsm_type::accept, 0, 0 }
    };

    _d.clear();
    
	fsm_type fsm(uri_tr, & _d);
	fsm_type::result_type r = fsm.exec(0, uri.begin(), uri.end());

	if (r.first)
		return true;

	return false;
}

template <typename StringT>
bool uri<StringT>::set_port (string_type::const_iterator begin
		, string_type::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		parse_context *ctx = reinterpret_cast<parse_context*>(context);
		string digits(begin, end);

		bool ok;
		ctx->uridata->port = lexical_cast<uint16_t>(digits, & ok);

		if (!ok) {
			return false;
		}
	}
	return true;
}

template <typename StringT>
inline bool uri<StringT>::set_host_is_ip (string_type::const_iterator
		, string_type::const_iterator
		, void * context
		, void *)
{
	if (context) {
		data_rep * ctx = reinterpret_cast<data_rep *>(context);
		ctx->host_is_ip = true;
	}
	return true;
}

template <typename StringT>
inline bool uri<StringT>::unset_userinfo (string_type::const_iterator
		, string_type::const_iterator
		, void * context
		, void *)
{
	if (context) {
		data_rep * ctx = reinterpret_cast<data_rep *>(context);
		ctx->userinfo.clear();
	}
	return true;
}

template <typename StringT>
inline bool uri<StringT>::set_scheme (string_type::const_iterator begin
		, string_type::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<data_rep *>(context)->scheme = string_type(begin, end);
	}
	return true;
}

template <typename StringT>
inline bool uri<StringT>::set_userinfo (string_type::const_iterator begin
		, string_type::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<data_rep *>(context)->userinfo = string_type(begin, end);
	}
	return true;
}

template <typename StringT>
inline bool uri<StringT>::set_host (string_type::const_iterator begin
		, string_type::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<data_rep *>(context)->host = string_type(begin, end);
	}
	return true;
}

template <typename StringT>
bool uri<StringT>::set_path (string_type::const_iterator begin
		, string_type::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<data_rep *>(context)->path = string_type(begin, end);
	}
	return true;
}

template <typename StringT>
inline bool uri<StringT>::set_query (string_type::const_iterator begin
		, string_type::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<data_rep *>(context)->query = string_type(begin, end);
	}
	return true;
}

template <typename StringT>
inline bool uri<StringT>::set_fragment (string_type::const_iterator begin
		, string_type::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<data_rep *>(context)->fragment = string_type(begin, end);
	}
	return true;
}

}} // pfs::net

#endif /* __PFS_URI_RFC3986_HPP__ */
