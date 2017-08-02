/**
 * @file fsm_common.hpp
 * @author wladt
 * @date Feb 24, 2014
 */

/*
number:
    sign? integer-constant
    | sign? floating-point-constant

comma-wsp:
    (wsp+ comma? wsp*) | (comma wsp*)

comma:
    ","

integer-constant:
    digit-sequence

floating-point-constant:
    fractional-constant exponent?
    | digit-sequence exponent

fractional-constant:
    digit-sequence? "." digit-sequence
    | digit-sequence "."

exponent:
    ( "e" | "E" ) sign? digit-sequence

sign:
    "+" | "-"

digit-sequence:
    digit
    | digit digit-sequence

digit:
    "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"

wsp:
    (#x20 | #x9 | #xD | #xA)
 */

#include <pfs/fsm.hpp>

namespace cwt { namespace svg {

static pfs::string _SVG_WSP("\x20\x09\x0D\x0A");
static pfs::string _SVG_DIGIT("0123456789");
static pfs::string _SVG_SIGN("+-");
static pfs::string _SVG_POINT(1, '.');
static pfs::string _SVG_COMMA(1, ',');

// digit digit-sequence = digit / digit digit-sequence
// 		is equivalent to
// digit-sequence = digit *digit
static cwt::fsm::transition<pfs::string> digit_sequence[] = {
	  { 1,-1, FSM_MATCH_CHAR(_SVG_DIGIT)           , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_CHAR(_SVG_DIGIT, 0, -1), FSM_ACCEPT, nullptr, nullptr }
};

// exponent = ( "e" / "E" ) [ sign ] digit-sequence
static cwt::fsm::transition<pfs::string> exponent[] = {
	  { 1,-1, FSM_MATCH_CHAR(_SVG_DIGIT)    , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_OPT_CHAR(_SVG_SIGN) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(digit_sequence) , FSM_ACCEPT, nullptr, nullptr }
};


// fractional-constant = [ digit-sequence ] "." digit-sequence
//		/ digit-sequence "."
static cwt::fsm::transition<pfs::string> fractional_constant_0[] = {
	  { 1,-1, FSM_MATCH_OPT_FSM(digit_sequence) , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_OPT_CHAR(_SVG_POINT)    , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(digit_sequence)     , FSM_ACCEPT, nullptr, nullptr }
};

static cwt::fsm::transition<pfs::string> fractional_constant_1[] = {
	  { 1,-1, FSM_MATCH_FSM(digit_sequence) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_OPT_CHAR(_SVG_POINT), FSM_ACCEPT, nullptr, nullptr }
};

static cwt::fsm::transition<pfs::string> fractional_constant[] = {
	  {-1, 1, FSM_MATCH_FSM(fractional_constant_0) , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(fractional_constant_1) , FSM_ACCEPT, nullptr, nullptr }
};

// floating-point-constant = fractional-constant [ exponent ]
//    / digit-sequence exponent
static cwt::fsm::transition<pfs::string> floating_point_constant_0[] = {
	  { 1,-1, FSM_MATCH_FSM(fractional_constant) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_OPT_FSM(exponent)        , FSM_ACCEPT, nullptr, nullptr }
};

static cwt::fsm::transition<pfs::string> floating_point_constant_1[] = {
	  { 1,-1, FSM_MATCH_FSM(digit_sequence) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(exponent)       , FSM_ACCEPT, nullptr, nullptr }
};

static cwt::fsm::transition<pfs::string> floating_point_constant[] = {
	  {-1, 1, FSM_MATCH_FSM(floating_point_constant_0) , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(floating_point_constant_1) , FSM_ACCEPT, nullptr, nullptr }
};

// integer-constant = digit-sequence
#define integer_constant digit_sequence

// comma-wsp = (1*wsp [ comma ] *wsp)
//		/ (comma *wsp)
static cwt::fsm::transition<pfs::string> comma_wsp_0[] = {
	  { 1,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP, 1, -1) , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_OPT_CHAR(_SVG_COMMA)      , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1, -1) , FSM_ACCEPT, nullptr, nullptr }
};

static cwt::fsm::transition<pfs::string> comma_wsp_1[] = {
	  { 1,-1, FSM_MATCH_OPT_CHAR(_SVG_COMMA)      , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1, -1) , FSM_ACCEPT, nullptr, nullptr }
};

static cwt::fsm::transition<pfs::string> comma_wsp[] = {
	  {-1, 1, FSM_MATCH_FSM(comma_wsp_0) , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(comma_wsp_1) , FSM_ACCEPT, nullptr, nullptr }
};


// number = [ sign ] integer-constant
//    / [ sign ] floating-point-constant
static cwt::fsm::transition<pfs::string> number_0[] = {
	  { 1,-1, FSM_MATCH_OPT_CHAR(_SVG_SIGN)          , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(floating_point_constant) , FSM_ACCEPT, nullptr, nullptr }
};

static cwt::fsm::transition<pfs::string> number_1[] = {
	  { 1,-1, FSM_MATCH_OPT_CHAR(_SVG_SIGN)   , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(integer_constant) , FSM_ACCEPT, nullptr, nullptr }
};

static cwt::fsm::transition<pfs::string> number[] = {
	  {-1, 1, FSM_MATCH_FSM(number_0) , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(number_1) , FSM_ACCEPT, nullptr, nullptr }
};

}} // cwt::svg
