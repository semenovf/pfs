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

#include <pfs/fsm/fsm.hpp>

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
    
    static bool set_host_is_row (const_iterator begin
            , const_iterator end
            , void * context
            , void * action_args);

    
    static string_type const alpha;
    static string_type const digit;
    static string_type const hexdigit; // DIGIT / "A" / "B" / "C" / "D" / "E" / "F"
    
    static transition_type digit_tr[];
    static transition_type const hexdig_tr[];
    static transition_type const unreserved_tr[];
    static transition_type const pct_encoded_tr[];
    static transition_type const sub_delims_tr[];
    static transition_type const pchar_tr[];
    static transition_type const segment_nz_tr[];
    static transition_type const segment_tr[];
    static transition_type const slash_segment_tr[];
    static transition_type const segment_nz_slash_tr[];
    static transition_type const path_absolute_tr[];
    static transition_type const segment_nc_tr[];
    static transition_type const segment_nz_nc_tr[];
    static transition_type const path_noscheme_tr[];
    static transition_type const path_empty_tr[];
    static transition_type const h16_tr[];
    static transition_type const dec_octet_tr_4[];
    static transition_type const dec_octet_tr_3[];
    static transition_type const dec_octet_tr_2[];
    static transition_type const dec_octet_tr_1[];
    static transition_type const dec_octet_tr[];
    static transition_type const ipv4address_tr[];
    static transition_type const ls32_tr[];
    static transition_type const h16_semi_tr[];
    static transition_type const ipv6address_tr_4_1[];
    static transition_type const ipv6address_tr_5_1[];
    static transition_type const ipv6address_tr_6_1[];
    static transition_type const ipv6address_tr_7_1[];
    static transition_type const ipv6address_tr_8_1[];
    static transition_type const ipv6address_tr_9_1[];
    static transition_type const ipv6address_tr_1[];
    static transition_type const ipv6address_tr_2[];
    static transition_type const ipv6address_tr_3[];
    static transition_type const ipv6address_tr_4[];
    static transition_type const ipv6address_tr_5[];
    static transition_type const ipv6address_tr_6[];
    static transition_type const ipv6address_tr_7[];
    static transition_type const ipv6address_tr_8[];
    static transition_type const ipv6address_tr_9[];
    static transition_type const ipv6address_tr[];
    static transition_type const ipvfuture_tail_tr[];
    static transition_type const ipvfuture_tr[];
    static transition_type const ip_literal_entry_tr[];
    static transition_type const ip_literal_tr[];
    static transition_type const reg_name_entry_tr[];
    static transition_type const reg_name_tr[];
    static transition_type const host_tr[];
    static transition_type const port_tr[];
    static transition_type const authority_tr_2[];
    static transition_type const userinfo_tr[];
    static transition_type const authority_tr_1[];
    static transition_type const authority_tr[];
    static transition_type const path_abempty_tr[];
    static transition_type const relative_part_tr[];
    static transition_type const query_chars_tr[];
    static transition_type const query_tr[];
    static transition_type const relative_ref_tr_1[];
    static transition_type const fragment_tr[];
    static transition_type const relative_ref_tr_2[];
    static transition_type const relative_ref_tr[];
    static transition_type const scheme_chars_tr[];
    static transition_type const scheme_tr[];
    static transition_type const path_rootless_tr[];
    static transition_type const hier_part_tr[];
    static transition_type const uri_tr_1[];
    static transition_type const uri_tr_2[];
    static transition_type const uri_tr[];
    static transition_type const uri_reference_tr[];
};

#define PFS_URI_GRAMMAR     uri_grammar<UriT>
#define PFS_URI_FSM         PFS_URI_GRAMMAR::fsm_type
#define PFS_URI_TRANSITION_DEF(x)  template <typename UriT> \
                            typename uri_grammar<UriT>::transition_type const x[]
#define PFS_URI_FSM_TR(x)   PFS_URI_FSM::tr(PFS_URI_GRAMMAR::x)

template <typename UriT>
typename uri_grammar<UriT>::string_type const
uri_grammar<UriT>::alpha("ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz");

template <typename UriT>
typename uri_grammar<UriT>::string_type const
uri_grammar<UriT>::digit("0123456789");

template <typename UriT>
typename uri_grammar<UriT>::string_type const
uri_grammar<UriT>::hexdigit("0123456789ABCDEFabcdef");

template <typename UriT>
typename uri_grammar<UriT>::transition_type digit_tr[] = {};

//PFS_URI_TRANSITION_DEF(digit_tr) = {
//    {-1,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::digit), PFS_URI_FSM::accept, 0, 0 }
//};

PFS_URI_TRANSITION_DEF(hexdig_tr) = {
    {-1,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::hexdigit), PFS_URI_FSM::accept, 0, 0 }
};

/* ALPHA / DIGIT / "-" / "." / "_" / "~" */
PFS_URI_TRANSITION_DEF(unreserved_tr) = {
      {-1, 1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::alpha), PFS_URI_FSM::accept, 0, 0 }
    , {-1, 2, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::digit), PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::string_type("-._~")), PFS_URI_FSM::accept, 0, 0 }
};

/* "%" HEXDIG HEXDIG */
PFS_URI_TRANSITION_DEF(pct_encoded_tr) = {
      { 1,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("%")) , PFS_URI_FSM::normal, 0, 0 }
    , { 2,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::hexdigit), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::hexdigit), PFS_URI_FSM::accept, 0, 0 }
};

/* "!" / "$" / "&" / "'" / "(" / ")"
       / "*" / "+" / "," / ";" / "="
*/
PFS_URI_TRANSITION_DEF(sub_delims_tr) = {
    {-1,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::string_type("!$&'()*+,;=")), PFS_URI_FSM::accept, 0, 0 }
};

/* unreserved / pct-encoded / sub-delims / ":" / "@" */
PFS_URI_TRANSITION_DEF(pchar_tr) = {
      {-1, 1, PFS_URI_FSM_TR(unreserved_tr) , PFS_URI_FSM::accept, 0, 0 }
    , {-1, 2, PFS_URI_FSM_TR(pct_encoded_tr), PFS_URI_FSM::accept, 0, 0 }
    , {-1, 3, PFS_URI_FSM_TR(sub_delims_tr) , PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::string_type(":@")), PFS_URI_FSM::accept, 0, 0 }
};


/* 1*pchar */
PFS_URI_TRANSITION_DEF(segment_nz_tr) = {
      { 1,-1, PFS_URI_FSM_TR(pchar_tr),PFS_URI_FSM::accept, 0, 0 }
    , { 1,-1, PFS_URI_FSM_TR(pchar_tr),PFS_URI_FSM::accept, 0, 0 }
};

/* *pchar */
PFS_URI_TRANSITION_DEF(segment_tr) = {
      { 0, 1, PFS_URI_FSM_TR(pchar_tr), PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM::nothing()   , PFS_URI_FSM::accept, 0, 0 }
};

/* "/" segment */
PFS_URI_TRANSITION_DEF(slash_segment_tr) = {
      { 1,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("/")), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(segment_tr)          , PFS_URI_FSM::accept, 0, 0 }
};

/* segment-nz *slash_segment */
PFS_URI_TRANSITION_DEF(segment_nz_slash_tr) = {
      { 1,-1, PFS_URI_FSM_TR(segment_nz_tr)              , PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM::rpt_tr(slash_segment_tr, 0, -1), PFS_URI_FSM::accept, 0, 0 }
};

/* "/" [ segment-nz *slash_segment ] */
PFS_URI_TRANSITION_DEF(path_absolute_tr) = {
      { 1,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("/"))    , PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM::opt_tr(segment_nz_slash_tr) , PFS_URI_FSM::accept, 0, 0 }
};

/* 1*( unreserved / pct-encoded / sub-delims / "@" )
        ; non-zero-length segment without any colon ":"
*/
PFS_URI_TRANSITION_DEF(segment_nc_tr) = {
      {-1, 1, PFS_URI_FSM_TR(unreserved_tr)       , PFS_URI_FSM::accept, 0, 0 }
    , {-1, 2, PFS_URI_FSM_TR(pct_encoded_tr)      , PFS_URI_FSM::accept, 0, 0 }
    , {-1, 3, PFS_URI_FSM_TR(sub_delims_tr)       , PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::string_type("@")), PFS_URI_FSM::accept, 0, 0 }
};

PFS_URI_TRANSITION_DEF(segment_nz_nc_tr) = {
    {-1,-1, PFS_URI_FSM::rpt_tr(segment_nc_tr, 1, -1), PFS_URI_FSM::accept, 0, 0 }
};

/* segment-nz-nc *slash-segment */
PFS_URI_TRANSITION_DEF(path_noscheme_tr) = {
      { 1,-1, PFS_URI_FSM_TR(segment_nz_nc_tr), PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM::rpt_tr(slash_segment_tr, 0, -1), PFS_URI_FSM::accept, 0, 0 }
};

/* 0<pchar> */
PFS_URI_TRANSITION_DEF(path_empty_tr) = {
    {-1,-1, PFS_URI_GRAMMAR::nothing(), PFS_URI_FSM::accept, 0, 0 }
};

/* 1*4HEXDIG */
PFS_URI_TRANSITION_DEF(h16_tr) = {
      { 1,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::hexdigit), PFS_URI_FSM::accept, 0, 0 }
    , { 2,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::hexdigit), PFS_URI_FSM::accept, 0, 0 }
    , { 3,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::hexdigit), PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::hexdigit), PFS_URI_FSM::accept, 0, 0 }
};

/* "25" %x30-35        ; 250-255 */
PFS_URI_TRANSITION_DEF(dec_octet_tr_4) = {
      { 1,-1, PFS_URI_FSM::seq(string_type("25"))    , PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::string_type("012345")), PFS_URI_FSM::accept, 0, 0 }
};

/* "2" %x30-34 DIGIT   ; 200-249 */
PFS_URI_TRANSITION_DEF(dec_octet_tr_3) = {
      { 1,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("2"))         , PFS_URI_FSM::normal, 0, 0 }
    , { 2,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::string_type("01234"))  , PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::digit), PFS_URI_FSM::accept, 0, 0 }
};

/* "1" 2DIGIT  ; 100-199 */
PFS_URI_TRANSITION_DEF(dec_octet_tr_2) = {
      { 1,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("1")), PFS_URI_FSM::normal, 0, 0 }
    , { 2,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::digit)      , PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::digit)      , PFS_URI_FSM::accept, 0, 0 }
};

/* %x31-39 DIGIT       ; 10-99*/
PFS_URI_TRANSITION_DEF(dec_octet_tr_1) = {
      { 1,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::string_type("123456789")), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::digit), PFS_URI_FSM::accept, 0, 0 }
};

/*
  DIGIT                 ; 0-9
  / %x31-39 DIGIT       ; 10-99
  / "1" 2DIGIT          ; 100-199
  / "2" %x30-34 DIGIT   ; 200-249
  / "25" %x30-35        ; 250-255
*/
PFS_URI_TRANSITION_DEF(dec_octet_tr) = {
      {-1, 1, PFS_URI_FSM_TR(dec_octet_tr_4), PFS_URI_FSM::accept, 0, 0 } /* 250 - 255 */
    , {-1, 2, PFS_URI_FSM_TR(dec_octet_tr_3), PFS_URI_FSM::accept, 0, 0 } /* 200 - 249 */
    , {-1, 3, PFS_URI_FSM_TR(dec_octet_tr_2), PFS_URI_FSM::accept, 0, 0 } /* 100 - 199 */
    , {-1, 4, PFS_URI_FSM_TR(dec_octet_tr_1), PFS_URI_FSM::accept, 0, 0 } /* 10 - 99 */
    , {-1,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::digit), PFS_URI_FSM::accept, 0, 0 } /* 0 - 9 */
};

/* dec-octet "." dec-octet "." dec-octet "." dec-octet */
PFS_URI_TRANSITION_DEF(ipv4address_tr) = {
      { 1,-1, PFS_URI_FSM_TR(dec_octet_tr)     , PFS_URI_FSM::normal, 0, 0 }
    , { 2,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type(".")), PFS_URI_FSM::normal, 0, 0 }
    , { 3,-1, PFS_URI_FSM_TR(dec_octet_tr)     , PFS_URI_FSM::normal, 0, 0 }
    , { 4,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type(".")), PFS_URI_FSM::normal, 0, 0 }
    , { 5,-1, PFS_URI_FSM_TR(dec_octet_tr)     , PFS_URI_FSM::normal, 0, 0 }
    , { 6,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type(".")), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(dec_octet_tr)     , PFS_URI_FSM::accept, 0, 0 }
};

/* ( h16 ":" h16 ) / IPv4address */
PFS_URI_TRANSITION_DEF(ls32_tr) = {
      {-1, 1, PFS_URI_FSM_TR(ipv4address_tr), PFS_URI_FSM::accept, 0, 0 }
    , { 2,-1, PFS_URI_FSM_TR(h16_tr)        , PFS_URI_FSM::normal, 0, 0 }
    , { 3,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type(":")), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(h16_tr)        , PFS_URI_FSM::accept, 0, 0 }
};

/* h16 ":" */
PFS_URI_TRANSITION_DEF(h16_semi_tr) = {
      { 1,-1, PFS_URI_FSM_TR(h16_tr), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type(":")), PFS_URI_FSM::accept, 0, 0 }
};

/* *1( h16 ":" ) h16 */
PFS_URI_TRANSITION_DEF(ipv6address_tr_4_1) = {
      { 1,-1, PFS_URI_FSM::rpt_tr(h16_semi_tr, 0, 1), PFS_URI_FSM::normal, 0, 0 }
    , {-1, 2, PFS_URI_FSM_TR(h16_tr), PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(h16_tr), PFS_URI_FSM::accept, 0, 0 }
};

/* *2( h16 ":" ) h16 */
PFS_URI_TRANSITION_DEF(ipv6address_tr_5_1) = {
      { 1, 2, PFS_URI_FSM::rpt_tr(h16_semi_tr, 0, 2), PFS_URI_FSM::normal, 0, 0 }
    , {-1, 2, PFS_URI_FSM_TR(h16_tr), PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(h16_tr), PFS_URI_FSM::accept, 0, 0 }
};

/* *3( h16 ":" ) h16 */
PFS_URI_TRANSITION_DEF(ipv6address_tr_6_1) = {
      { 1, 2, PFS_URI_FSM::rpt_tr(h16_semi_tr, 0, 3), PFS_URI_FSM::normal, 0, 0 }
    , {-1, 2, PFS_URI_FSM_TR(h16_tr), PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(h16_tr), PFS_URI_FSM::accept, 0, 0 }
};

/* *4( h16 ":" ) h16 */
PFS_URI_TRANSITION_DEF(ipv6address_tr_7_1) = {
      { 1, 2, PFS_URI_FSM::rpt_tr(h16_semi_tr, 0, 4), PFS_URI_FSM::normal, 0, 0 }
    , {-1, 2, PFS_URI_FSM_TR(h16_tr), PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(h16_tr), PFS_URI_FSM::accept, 0, 0 }
};

/* *5( h16 ":" ) h16 */
PFS_URI_TRANSITION_DEF(ipv6address_tr_8_1) = {
      { 1, 2, PFS_URI_FSM::rpt_tr(h16_semi_tr, 0, 5), PFS_URI_FSM::normal, 0, 0 }
    , {-1, 2, PFS_URI_FSM_TR(h16_tr), PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(h16_tr), PFS_URI_FSM::accept, 0, 0 }
};

/* *6( h16 ":" ) h16 */
PFS_URI_TRANSITION_DEF(ipv6address_tr_9_1) = {
      { 1, 2, PFS_URI_FSM::rpt_tr(h16_semi_tr, 0, 6), PFS_URI_FSM::normal, 0, 0 }
    , {-1, 2, PFS_URI_FSM_TR(h16_tr), PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(h16_tr), PFS_URI_FSM::accept, 0, 0 }
};

/* 6( h16 ":" ) ls32 */
PFS_URI_TRANSITION_DEF(ipv6address_tr_1) = {
      { 1,-1, PFS_URI_FSM::rpt_tr(h16_semi_tr, 6, 6), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(ls32_tr), PFS_URI_FSM::accept, 0, 0 }
};

/* "::" 5( h16 ":" ) ls32 */
PFS_URI_TRANSITION_DEF(ipv6address_tr_2) = {
      { 1,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("::")), PFS_URI_FSM::normal, 0, 0 }
    , { 2,-1, PFS_URI_FSM::rpt_tr(h16_semi_tr, 5, 5), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(ls32_tr)              , PFS_URI_FSM::accept, 0, 0 }
};

/* [ h16 ] "::" 4( h16 ":" ) ls32 */
PFS_URI_TRANSITION_DEF(ipv6address_tr_3) = {
      { 1, 1, PFS_URI_FSM_TR(h16_tr), PFS_URI_FSM::normal, 0, 0 }
    , { 2,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("::")), PFS_URI_FSM::normal, 0, 0 }
    , { 3,-1, PFS_URI_FSM::rpt_tr(h16_semi_tr, 4, 4), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(ls32_tr)              , PFS_URI_FSM::accept, 0, 0 }
};

/* [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32 */
PFS_URI_TRANSITION_DEF(ipv6address_tr_4) = {
      { 1, 1, PFS_URI_FSM_TR(ipv6address_tr_4_1), PFS_URI_FSM::normal, 0, 0 }

    , { 2,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("::")), PFS_URI_FSM::normal, 0, 0 }
    , { 3,-1, PFS_URI_FSM_TR(h16_semi_tr), PFS_URI_FSM::normal, 0, 0 }
    , { 4,-1, PFS_URI_FSM_TR(h16_semi_tr), PFS_URI_FSM::normal, 0, 0 }
    , { 5,-1, PFS_URI_FSM_TR(h16_semi_tr), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(ls32_tr)    , PFS_URI_FSM::accept, 0, 0 }
};

/* [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32 */
PFS_URI_TRANSITION_DEF(ipv6address_tr_5) = {
      { 1, 2, PFS_URI_FSM_TR(ipv6address_tr_5_1), PFS_URI_FSM::normal, 0, 0 }
    , { 2, 2, PFS_URI_FSM_TR(ipv6address_tr_5_1), PFS_URI_FSM::normal, 0, 0 }

    , { 3,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("::")), PFS_URI_FSM::normal, 0, 0 }
    , { 4,-1, PFS_URI_FSM_TR(h16_semi_tr), PFS_URI_FSM::normal, 0, 0 }
    , { 5,-1, PFS_URI_FSM_TR(h16_semi_tr), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(ls32_tr)    , PFS_URI_FSM::accept, 0, 0 }
};

/* [ *3( h16 ":" ) h16 ] "::" h16 ":"   ls32 */
PFS_URI_TRANSITION_DEF(ipv6address_tr_6) = {
      { 1, 3, PFS_URI_FSM_TR(ipv6address_tr_6_1), PFS_URI_FSM::normal, 0, 0 }
    , { 2, 3, PFS_URI_FSM_TR(ipv6address_tr_6_1), PFS_URI_FSM::normal, 0, 0 }
    , { 3, 3, PFS_URI_FSM_TR(ipv6address_tr_6_1), PFS_URI_FSM::normal, 0, 0 }

    , { 4,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("::")), PFS_URI_FSM::normal, 0, 0 }
    , { 5,-1, PFS_URI_FSM_TR(h16_semi_tr), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(ls32_tr)    , PFS_URI_FSM::accept, 0, 0 }
};


/* [ *4( h16 ":" ) h16 ] "::" ls32 */
PFS_URI_TRANSITION_DEF(ipv6address_tr_7) = {
      { 1, 4, PFS_URI_FSM_TR(ipv6address_tr_7_1), PFS_URI_FSM::normal, 0, 0 }
    , { 2, 4, PFS_URI_FSM_TR(ipv6address_tr_7_1), PFS_URI_FSM::normal, 0, 0 }
    , { 3, 4, PFS_URI_FSM_TR(ipv6address_tr_7_1), PFS_URI_FSM::normal, 0, 0 }
    , { 4, 4, PFS_URI_FSM_TR(ipv6address_tr_7_1), PFS_URI_FSM::normal, 0, 0 }

    , { 5,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("::")), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(ls32_tr) , PFS_URI_FSM::accept, 0, 0 }
};

/* [ *5( h16 ":" ) h16 ] "::"              h16 */
PFS_URI_TRANSITION_DEF(ipv6address_tr_8) = {
      { 1, 5, PFS_URI_FSM_TR(ipv6address_tr_8_1), PFS_URI_FSM::normal, 0, 0 }
    , { 2, 5, PFS_URI_FSM_TR(ipv6address_tr_8_1), PFS_URI_FSM::normal, 0, 0 }
    , { 3, 5, PFS_URI_FSM_TR(ipv6address_tr_8_1), PFS_URI_FSM::normal, 0, 0 }
    , { 4, 5, PFS_URI_FSM_TR(ipv6address_tr_8_1), PFS_URI_FSM::normal, 0, 0 }
    , { 5, 5, PFS_URI_FSM_TR(ipv6address_tr_8_1), PFS_URI_FSM::normal, 0, 0 }

    , { 6,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("::")) , PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(h16_tr)   , PFS_URI_FSM::accept, 0, 0 }
};


/* [ *6( h16 ":" ) h16 ] "::" */
PFS_URI_TRANSITION_DEF(ipv6address_tr_9) = {
      { 1, 6, PFS_URI_FSM_TR(ipv6address_tr_9_1), PFS_URI_FSM::normal, 0, 0 }
    , { 2, 6, PFS_URI_FSM_TR(ipv6address_tr_9_1), PFS_URI_FSM::normal, 0, 0 }
    , { 3, 6, PFS_URI_FSM_TR(ipv6address_tr_9_1), PFS_URI_FSM::normal, 0, 0 }
    , { 4, 6, PFS_URI_FSM_TR(ipv6address_tr_9_1), PFS_URI_FSM::normal, 0, 0 }
    , { 5, 6, PFS_URI_FSM_TR(ipv6address_tr_9_1), PFS_URI_FSM::normal, 0, 0 }
    , { 6, 6, PFS_URI_FSM_TR(ipv6address_tr_9_1), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("::"))          , PFS_URI_FSM::accept, 0, 0 }
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
PFS_URI_TRANSITION_DEF(ipv6address_tr) = {
      {-1, 1, PFS_URI_FSM_TR(ipv6address_tr_1), PFS_URI_FSM::accept, 0, 0 }
    , {-1, 2, PFS_URI_FSM_TR(ipv6address_tr_2), PFS_URI_FSM::accept, 0, 0 }
    , {-1, 3, PFS_URI_FSM_TR(ipv6address_tr_3), PFS_URI_FSM::accept, 0, 0 }
    , {-1, 4, PFS_URI_FSM_TR(ipv6address_tr_4), PFS_URI_FSM::accept, 0, 0 }
    , {-1, 5, PFS_URI_FSM_TR(ipv6address_tr_5), PFS_URI_FSM::accept, 0, 0 }
    , {-1, 6, PFS_URI_FSM_TR(ipv6address_tr_6), PFS_URI_FSM::accept, 0, 0 }
    , {-1, 7, PFS_URI_FSM_TR(ipv6address_tr_7), PFS_URI_FSM::accept, 0, 0 }
    , {-1, 8, PFS_URI_FSM_TR(ipv6address_tr_8), PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(ipv6address_tr_9), PFS_URI_FSM::accept, 0, 0 }
};

/* "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" ) */
PFS_URI_TRANSITION_DEF(ipvfuture_tail_tr) = {
      {-1, 1, PFS_URI_FSM_TR(unreserved_tr)    , PFS_URI_FSM::accept, 0, 0 }
    , {-1, 2, PFS_URI_FSM_TR(sub_delims_tr)    , PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type(":")), PFS_URI_FSM::accept, 0, 0 }
};

PFS_URI_TRANSITION_DEF(ipvfuture_tr) = {
      { 1,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("v"))           , PFS_URI_FSM::normal, 0, 0 }
    , { 2,-1, PFS_URI_FSM::rpt_tr(hexdig_tr, 1, -1)        , PFS_URI_FSM::normal, 0, 0 }
    , { 3,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("."))           , PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM::rpt_tr(ipvfuture_tail_tr, 1, -1), PFS_URI_FSM::accept, 0, 0 }
};

/* "[" ( IPv6address / IPvFuture  ) "]" */
PFS_URI_TRANSITION_DEF(ip_literal_entry_tr) = {
      {-1, 1, PFS_URI_FSM_TR(ipv6address_tr), PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(ipvfuture_tr)  , PFS_URI_FSM::accept, 0, 0 }
};
PFS_URI_TRANSITION_DEF(ip_literal_tr) = {
      { 1,-1, PFS_URI_FSM::seq(string_type("["))  , PFS_URI_FSM::normal, 0, 0 }
    , { 2,-1, PFS_URI_FSM_TR(ip_literal_entry_tr), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("]"))  , PFS_URI_FSM::accept, 0, 0 }
};

/* *( unreserved / pct-encoded / sub-delims ) */
PFS_URI_TRANSITION_DEF(reg_name_entry_tr) = {
      {-1, 1, PFS_URI_FSM_TR(unreserved_tr) , PFS_URI_FSM::accept, 0, 0 }
    , {-1, 2, PFS_URI_FSM_TR(pct_encoded_tr), PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(sub_delims_tr) , PFS_URI_FSM::accept, 0, 0 }
};
PFS_URI_TRANSITION_DEF(reg_name_tr) = {
      {-1,-1, PFS_URI_FSM::rpt_tr(reg_name_entry_tr, 0, -1), PFS_URI_FSM::accept, 0, 0 }
};

/*  host = IP-literal / IPv4address / reg-name */
PFS_URI_TRANSITION_DEF(host_tr) = {
      {-1, 1, PFS_URI_FSM_TR(ip_literal_tr) , PFS_URI_FSM::accept, set_host_is_ip, 0}
    , {-1, 2, PFS_URI_FSM_TR(ipv4address_tr), PFS_URI_FSM::accept, set_host_is_ip, 0}

    /* OBSOLETE NOTE: this poses a parsing problem - all IPv4 addresses are valid
    reg_name as well. Fix this by doing the reg_name_tr now,
    then on match, do a function to check if its an IPv4
    address */
    , {-1,-1, PFS_URI_FSM_TR(reg_name_tr), PFS_URI_FSM::accept, 0, 0 }
};

/* *DIGIT */
PFS_URI_TRANSITION_DEF(port_tr) = {
      {-1,-1, PFS_URI_FSM::rpt_tr(digit_tr, 0, -1), PFS_URI_FSM::accept, 0, 0 }
};

/* ":" port */
PFS_URI_TRANSITION_DEF(authority_tr_2) = {
      { 1,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type(":")), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(port_tr), PFS_URI_FSM::accept, set_port, 0}
};

/* *( unreserved / pct-encoded / sub-delims / ":" ) */
PFS_URI_TRANSITION_DEF(userinfo_tr) = {
      { 0, 1, PFS_URI_FSM_TR(unreserved_tr)    , PFS_URI_FSM::accept, 0, 0 }
    , { 0, 2, PFS_URI_FSM_TR(pct_encoded_tr)   , PFS_URI_FSM::accept, 0, 0 }
    , { 0, 3, PFS_URI_FSM_TR(sub_delims_tr)    , PFS_URI_FSM::accept, 0, 0 }
    , { 0, 4, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type(":")), PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM::nothing(), PFS_URI_FSM::accept, 0, 0 }
};

/* userinfo "@" */
PFS_URI_TRANSITION_DEF(authority_tr_1) = {
      { 1,-1, PFS_URI_FSM_TR(userinfo_tr)      , PFS_URI_FSM::normal, set_userinfo, 0 }
    , {-1,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("@")), PFS_URI_FSM::accept, 0, 0 }
};

/* [ userinfo "@" ] host [ ":" port ] */
/* [ authority_tr_1 ] host [ authority_tr_2 ] */
PFS_URI_TRANSITION_DEF(authority_tr) = {
      { 1, 3, PFS_URI_FSM::opt_tr(authority_tr_1) , PFS_URI_FSM::normal, 0, 0 }
    , { 2, 3, PFS_URI_FSM_TR(host_tr)            , PFS_URI_FSM::normal, set_host, 0 }
    , {-1, 3, PFS_URI_FSM::opt_tr(authority_tr_2) , PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM::nothing()              , PFS_URI_FSM::reject, unset_userinfo, 0 }
};

/*  *( "/" segment ) */
PFS_URI_TRANSITION_DEF(path_abempty_tr) = {
      {-1,-1, PFS_URI_FSM::rpt_tr(slash_segment_tr, 0, -1), PFS_URI_FSM::accept, 0, 0 }
};

/*
relative-part = "//" authority path-abempty
              / path-absolute
              / path-noscheme`
              / path-empty
*/
PFS_URI_TRANSITION_DEF(relative_part_tr) = {
      { 1, 3, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("//")), PFS_URI_FSM::normal, 0, 0 }
    , { 2, 3, PFS_URI_FSM_TR(authority_tr)      , PFS_URI_FSM::normal, set_path, 0 }
    , {-1, 3, PFS_URI_FSM_TR(path_abempty_tr)   , PFS_URI_FSM::accept, set_path, 0 }

    , {-1, 4, PFS_URI_FSM_TR(path_absolute_tr), PFS_URI_FSM::accept, set_path, 0 }
    , {-1, 5, PFS_URI_FSM_TR(path_noscheme_tr), PFS_URI_FSM::accept, set_path, 0 }
    , {-1,-1, PFS_URI_FSM_TR(path_empty_tr)   , PFS_URI_FSM::accept, set_path, 0 }
};

/* *( pchar / "/" / "?" ) */
PFS_URI_TRANSITION_DEF(query_chars_tr) = {
      {-1, 1, PFS_URI_FSM_TR(pchar_tr)             , PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::string_type("/?")), PFS_URI_FSM::accept, 0, 0 }
};
PFS_URI_TRANSITION_DEF(query_tr) = {
    {-1,-1, PFS_URI_FSM::rpt_tr(query_chars_tr, 0, -1), PFS_URI_FSM::accept, 0, 0 }
};

/* "?" query */
PFS_URI_TRANSITION_DEF(relative_ref_tr_1) = {
      { 1,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("?")) , PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(query_tr), PFS_URI_FSM::accept, set_query, 0 }
};

/* *( pchar / "/" / "?" ) */
PFS_URI_TRANSITION_DEF(fragment_tr) = {
    {-1,-1, PFS_URI_FSM::rpt_tr(query_chars_tr, 0, -1), PFS_URI_FSM::accept, 0, 0 }
};

/* "#" fragment */
PFS_URI_TRANSITION_DEF(relative_ref_tr_2) = {
      { 1,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("#")), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(fragment_tr), PFS_URI_FSM::accept, set_fragment, 0}
};

/* relative-part [ "?" query ] [ "#" fragment ] */
PFS_URI_TRANSITION_DEF(relative_ref_tr) = {
      { 1,-1, PFS_URI_FSM_TR(relative_part_tr)           , PFS_URI_FSM::normal, 0, 0 }
    , { 2,-1, PFS_URI_FSM::rpt_tr(relative_ref_tr_1, 0, 1), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM::rpt_tr(relative_ref_tr_2, 0, 1), PFS_URI_FSM::accept, 0, 0 }
};

/* ALPHA *( ALPHA / DIGIT / "+" / "-" / "." ) */
PFS_URI_TRANSITION_DEF(scheme_chars_tr) = {
      {-1, 1, PFS_URI_FSM::one_of(__alpha), PFS_URI_FSM::accept, 0, 0 }
    , {-1, 2, PFS_URI_FSM::one_of(__digit), PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM::one_of(PFS_URI_GRAMMAR::string_type("+-.")), PFS_URI_FSM::accept, 0, 0 }
};
PFS_URI_TRANSITION_DEF(scheme_tr) = {
      { 1,-1, PFS_URI_FSM::one_of(__alpha), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM::rpt_tr(scheme_chars_tr, 0, -1), PFS_URI_FSM::accept, 0, 0 }
};

/* segment-nz *( "/" segment ) */
PFS_URI_TRANSITION_DEF(path_rootless_tr) = {
      { 1,-1, PFS_URI_FSM_TR(segment_nz_tr)              , PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM::rpt_tr(slash_segment_tr, 0, -1), PFS_URI_FSM::accept, 0, 0 }
};

/*
    "//" authority path-abempty
         / path-absolute
         / path-rootless
         / path-empty
*/
PFS_URI_TRANSITION_DEF(hier_part_tr) = {
      { 1, 3, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("//")), PFS_URI_FSM::normal, 0, 0 }
    , { 2, 3, PFS_URI_FSM_TR(authority_tr)      , PFS_URI_FSM::normal, set_path, 0 }
    , {-1, 3, PFS_URI_FSM_TR(path_abempty_tr)   , PFS_URI_FSM::accept, set_path, 0 }

    , {-1, 4, PFS_URI_FSM_TR(path_absolute_tr)  , PFS_URI_FSM::accept, set_path, 0 }
    , {-1, 5, PFS_URI_FSM_TR(path_rootless_tr)  , PFS_URI_FSM::accept, set_path, 0 }
    , {-1,-1, PFS_URI_FSM_TR(path_empty_tr)     ,    PFS_URI_FSM::accept, set_path, 0 }
};


/* "?" query */
PFS_URI_TRANSITION_DEF(uri_tr_1) = {
      { 1,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("?")), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(query_tr), PFS_URI_FSM::accept, set_query, 0 }
};

/* "#" fragment */
PFS_URI_TRANSITION_DEF(uri_tr_2) = {
      { 1,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type("#")), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(fragment_tr), PFS_URI_FSM::accept, set_fragment, 0 }
};


/* scheme ":" hier-part [ "?" query ] [ "#" fragment ] */
PFS_URI_TRANSITION_DEF(uri_tr) = {
      { 1,-1, PFS_URI_FSM_TR(scheme_tr)         , PFS_URI_FSM::normal, set_scheme, 0 }
    , { 2,-1, PFS_URI_FSM::seq(PFS_URI_GRAMMAR::string_type(":")) , PFS_URI_FSM::normal, 0, 0 }
    , { 3,-1, PFS_URI_FSM_TR(hier_part_tr)      , PFS_URI_FSM::normal, 0, 0 }
    , { 4,-1, PFS_URI_FSM::rpt_tr(uri_tr_1, 0, 1), PFS_URI_FSM::normal, 0, 0 }
    , {-1,-1, PFS_URI_FSM::rpt_tr(uri_tr_2, 0, 1), PFS_URI_FSM::accept, 0, 0 }
};


/* URI / relative-ref */
PFS_URI_TRANSITION_DEF(uri_reference_tr) = {
      {-1, 1, PFS_URI_FSM_TR(uri_tr)         , PFS_URI_FSM::accept, 0, 0 }
    , {-1,-1, PFS_URI_FSM_TR(relative_ref_tr), PFS_URI_FSM::accept, 0, 0 }
};


template <typename StringT>
bool uri_grammar<StringT>::set_port (const_iterator begin
		, const_iterator end
		, void * context
		, void *)
{
	if (context) {
		data_rep * ctx = reinterpret_cast<data_rep *>(context);
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
inline bool uri_grammar<StringT>::set_host_is_row (const_iterator
		, const_iterator
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
inline bool uri_grammar<StringT>::unset_userinfo (const_iterator
		, const_iterator
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
inline bool uri_grammar<StringT>::set_scheme (const_iterator begin
		, const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<data_rep *>(context)->scheme = string_type(begin, end);
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
		reinterpret_cast<data_rep *>(context)->userinfo = string_type(begin, end);
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
		reinterpret_cast<data_rep *>(context)->host = string_type(begin, end);
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
		reinterpret_cast<data_rep *>(context)->path = string_type(begin, end);
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
		reinterpret_cast<data_rep *>(context)->query = string_type(begin, end);
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
		reinterpret_cast<data_rep *>(context)->fragment = string_type(begin, end);
	}
	return true;
}


/**
 * @brief  Parse the given string as URI.
 * @details Implementation of RFC 3986 (http://tools.ietf.org/html/rfc3986)
 *
 * @param uri string representation of URI.
 */
template <typename Foundation, typename CharT>
bool uri<Foundation, CharT>::parse (string_type const & str)
{
    typedef pfs::fsm::fsm<string_type> fsm_type;
    typedef uri_grammar<string_type>   uri_grammar_type;

    _d.clear();
    
	fsm_type fsm(uri_grammar_type::uri_tr, & _d);
	fsm_type::result_type r = fsm.exec(0, str.begin(), str.end());

	if (r.first)
		return true;

	return false;
}

}} // pfs::net

#endif /* __PFS_URI_RFC3986_HPP__ */
