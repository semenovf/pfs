/**
 * @file path.cpp
 * @author wladt
 * @date Feb 20, 2014
 */

// see [8.3.9 The grammar for path data](http://www.w3.org/TR/SVG11/paths.html#PathDataBNF)

/*
svg-path:
    wsp* moveto-drawto-command-groups? wsp*

moveto-drawto-command-groups:
    moveto-drawto-command-group
    | moveto-drawto-command-group wsp* moveto-drawto-command-groups

moveto-drawto-command-group:
    moveto wsp* drawto-commands?

drawto-commands:
    drawto-command
    | drawto-command wsp* drawto-commands

drawto-command:
    closepath
    | lineto
    | horizontal-lineto
    | vertical-lineto
    | curveto
    | smooth-curveto
    | quadratic-bezier-curveto
    | smooth-quadratic-bezier-curveto
    | elliptical-arc

moveto:
    ( "M" | "m" ) wsp* moveto-argument-sequence

moveto-argument-sequence:
    coordinate-pair
    | coordinate-pair comma-wsp? lineto-argument-sequence

closepath:
    ("Z" | "z")

lineto:
    ( "L" | "l" ) wsp* lineto-argument-sequence

lineto-argument-sequence:
    coordinate-pair
    | coordinate-pair comma-wsp? lineto-argument-sequence

horizontal-lineto:
    ( "H" | "h" ) wsp* horizontal-lineto-argument-sequence

horizontal-lineto-argument-sequence:
    coordinate
    | coordinate comma-wsp? horizontal-lineto-argument-sequence

vertical-lineto:
    ( "V" | "v" ) wsp* vertical-lineto-argument-sequence

vertical-lineto-argument-sequence:
    coordinate
    | coordinate comma-wsp? vertical-lineto-argument-sequence

curveto:
    ( "C" | "c" ) wsp* curveto-argument-sequence

curveto-argument-sequence:
    curveto-argument
    | curveto-argument comma-wsp? curveto-argument-sequence

curveto-argument:
    coordinate-pair comma-wsp? coordinate-pair comma-wsp? coordinate-pair

smooth-curveto:
    ( "S" | "s" ) wsp* smooth-curveto-argument-sequence

smooth-curveto-argument-sequence:
    smooth-curveto-argument
    | smooth-curveto-argument comma-wsp? smooth-curveto-argument-sequence

smooth-curveto-argument:
    coordinate-pair comma-wsp? coordinate-pair

quadratic-bezier-curveto:
    ( "Q" | "q" ) wsp* quadratic-bezier-curveto-argument-sequence

quadratic-bezier-curveto-argument-sequence:
    quadratic-bezier-curveto-argument
    | quadratic-bezier-curveto-argument comma-wsp?
        quadratic-bezier-curveto-argument-sequence

quadratic-bezier-curveto-argument:
    coordinate-pair comma-wsp? coordinate-pair

smooth-quadratic-bezier-curveto:
    ( "T" | "t" ) wsp* smooth-quadratic-bezier-curveto-argument-sequence

smooth-quadratic-bezier-curveto-argument-sequence:
    coordinate-pair
    | coordinate-pair comma-wsp? smooth-quadratic-bezier-curveto-argument-sequence

elliptical-arc:
    ( "A" | "a" ) wsp* elliptical-arc-argument-sequence

elliptical-arc-argument-sequence:
    elliptical-arc-argument
    | elliptical-arc-argument comma-wsp? elliptical-arc-argument-sequence

elliptical-arc-argument:
    nonnegative-number comma-wsp? nonnegative-number comma-wsp?
        number comma-wsp flag comma-wsp? flag comma-wsp? coordinate-pair

coordinate-pair:
    coordinate comma-wsp? coordinate

coordinate:
    number

nonnegative-number:
    integer-constant
    | floating-point-constant

number:
    sign? integer-constant
    | sign? floating-point-constant

flag:
    "0" | "1"

comma-wsp:
    (wsp+ comma? wsp*) | (comma wsp*)
*/

#include "../include/cwt/svg/path.hpp"
#include "fsm_common.hpp"

namespace cwt { namespace svg {

path_parse_handlers path_seg::NullPathParseHandlers;

struct path_parse_context
{
	path_parse_handlers * h;
	ushort_t type;
	pfs::vector<double> coordinates;
	bool largeArcFlag;
	bool sweepFlag;
};

static bool __set_type         (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void *, void *);
static bool __set_largeArcFlag (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void *, void *);
static bool __set_sweepFlag    (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void *, void *);
static bool __add_coordinate   (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void *, void *);

static pfs::string _SVG_FLAG("01");

// nonnegative-number = integer-constant / floating-point-constant
static cwt::fsm::transition<pfs::string> nonnegative_number[] = {
	  {-1, 1, FSM_MATCH_FSM(integer_constant)        , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(floating_point_constant) , FSM_ACCEPT, nullptr, nullptr }
};

// coordinate = number
#define coordinate number

// coordinate-pair = coordinate [ comma-wsp ] coordinate
static cwt::fsm::transition<pfs::string> coordinate_pair[] = {
	  { 1,-1, FSM_MATCH_FSM(coordinate)    , FSM_NORMAL, __add_coordinate, nullptr }
	, { 2,-1, FSM_MATCH_OPT_FSM(comma_wsp) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(coordinate)    , FSM_ACCEPT, __add_coordinate, nullptr }
};

//elliptical-arc-argument = nonnegative-number [ comma-wsp ] nonnegative-number [ comma-wsp ]
//        number comma-wsp flag [ comma-wsp ] flag [ comma-wsp ] coordinate-pair
static cwt::fsm::transition<pfs::string> elliptical_arc_argument[] = {
	  { 1,-1, FSM_MATCH_FSM(nonnegative_number) , FSM_NORMAL, __add_coordinate, nullptr }
	, { 2,-1, FSM_MATCH_OPT_FSM(comma_wsp)      , FSM_NORMAL, nullptr, nullptr }
	, { 3,-1, FSM_MATCH_FSM(nonnegative_number) , FSM_NORMAL, __add_coordinate, nullptr }
	, { 4,-1, FSM_MATCH_OPT_FSM(comma_wsp)      , FSM_NORMAL, nullptr, nullptr }
	, { 5,-1, FSM_MATCH_FSM(number)             , FSM_NORMAL, __add_coordinate, nullptr }
	, { 6,-1, FSM_MATCH_FSM(comma_wsp)          , FSM_NORMAL, nullptr, nullptr }
	, { 7,-1, FSM_MATCH_CHAR(_SVG_FLAG)         , FSM_NORMAL, __set_largeArcFlag, nullptr }
	, { 8,-1, FSM_MATCH_OPT_FSM(comma_wsp)      , FSM_NORMAL, nullptr, nullptr }
	, { 9,-1, FSM_MATCH_CHAR(_SVG_FLAG)         , FSM_NORMAL, __set_sweepFlag, nullptr }
	, {10,-1, FSM_MATCH_OPT_FSM(comma_wsp)      , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(coordinate_pair)    , FSM_ACCEPT, nullptr, nullptr }
};

// elliptical-arc-argument-sequence = elliptical-arc-argument
//    / elliptical-arc-argument [ comma-wsp ] elliptical-arc-argument-sequence
//
//		is equivalent to
//
// elliptical-arc-argument-sequence =  elliptical-arc-argument *( [ comma-wsp ] elliptical-arc-argument)
static cwt::fsm::transition<pfs::string> elliptical_arc_argument_sequence_0[] = {
	  { 1,-1, FSM_MATCH_OPT_FSM(comma_wsp)           , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(elliptical_arc_argument) , FSM_ACCEPT, nullptr, nullptr }
};

static cwt::fsm::transition<pfs::string> elliptical_arc_argument_sequence[] = {
	  { 1,-1, FSM_MATCH_FSM(elliptical_arc_argument)                      , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(elliptical_arc_argument_sequence_0,-1,-1) , FSM_ACCEPT, nullptr, nullptr }
};

static ushort_t __arc_abs[] = {path_seg::PATHSEG_ARC_ABS};
static ushort_t __arc_rel[] = {path_seg::PATHSEG_ARC_REL};

// elliptical-arc = ( "A" | "a" ) *wsp elliptical-arc-argument-sequence
static cwt::fsm::transition<pfs::string> elliptical_arc[] = {
	  { 2, 1, FSM_MATCH_CHAR(_l1("A"))                        , FSM_NORMAL, __set_type, __arc_abs }
	, { 2,-1, FSM_MATCH_CHAR(_l1("a"))                        , FSM_NORMAL, __set_type, __arc_rel }
	, { 3,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1)              , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(elliptical_arc_argument_sequence) , FSM_ACCEPT, nullptr, nullptr }
};

// smooth-quadratic-bezier-curveto-argument-sequence =  coordinate-pair
//	    / coordinate-pair [ comma-wsp ] smooth-quadratic-bezier-curveto-argument-sequence
// =>
// smooth-quadratic-bezier-curveto-argument-sequence = coordinate-pair *( [ comma-wsp ] coordinate-pair)
static cwt::fsm::transition<pfs::string> smooth_quadratic_bezier_curveto_argument_sequence_0[] = {
	  { 1,-1, FSM_MATCH_OPT_FSM(comma_wsp)   , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(coordinate_pair) , FSM_ACCEPT, nullptr, nullptr }
};

static cwt::fsm::transition<pfs::string> smooth_quadratic_bezier_curveto_argument_sequence[] = {
	  { 1,-1, FSM_MATCH_FSM(coordinate_pair)                                               , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(smooth_quadratic_bezier_curveto_argument_sequence_0,-1,-1) , FSM_ACCEPT, nullptr, nullptr }
};

static ushort_t __curveto_quadratic_smooth_abs[] = {path_seg::PATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS};
static ushort_t __curveto_quadratic_smooth_rel[] = {path_seg::PATHSEG_CURVETO_QUADRATIC_SMOOTH_REL};

// smooth-quadratic-bezier-curveto = ( "T" | "t" ) *wsp smooth-quadratic-bezier-curveto-argument-sequence
static cwt::fsm::transition<pfs::string> smooth_quadratic_bezier_curveto[] = {
	  { 2, 1, FSM_MATCH_CHAR(_l1("T"))                                         , FSM_NORMAL, __set_type, __curveto_quadratic_smooth_abs }
	, { 2,-1, FSM_MATCH_CHAR(_l1("t"))                                         , FSM_NORMAL, __set_type, __curveto_quadratic_smooth_rel }
	, { 3,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1)                               , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(smooth_quadratic_bezier_curveto_argument_sequence) , FSM_ACCEPT, nullptr, nullptr }
};

// quadratic-bezier-curveto-argument =  coordinate-pair [ comma-wsp ] coordinate-pair
static cwt::fsm::transition<pfs::string> quadratic_bezier_curveto_argument[] = {
	  { 1,-1, FSM_MATCH_FSM(coordinate_pair) , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_OPT_FSM(comma_wsp)   , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(coordinate_pair) , FSM_ACCEPT, nullptr, nullptr }
};

// quadratic-bezier-curveto-argument-sequence = quadratic-bezier-curveto-argument
//     	| quadratic-bezier-curveto-argument [ comma-wsp ] quadratic-bezier-curveto-argument-sequence
// =>
// quadratic-bezier-curveto-argument-sequence =
//     	quadratic-bezier-curveto-argument *( [ comma-wsp ] quadratic-bezier-curveto-argument )
static cwt::fsm::transition<pfs::string> quadratic_bezier_curveto_argument_sequence_0[] = {
	  { 1,-1, FSM_MATCH_OPT_FSM(comma_wsp)                     , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(quadratic_bezier_curveto_argument) , FSM_ACCEPT, nullptr, nullptr }
};

static cwt::fsm::transition<pfs::string> quadratic_bezier_curveto_argument_sequence[] = {
	  { 1,-1, FSM_MATCH_FSM(quadratic_bezier_curveto_argument)                      , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(quadratic_bezier_curveto_argument_sequence_0,-1,-1) , FSM_ACCEPT, nullptr, nullptr }
};

static ushort_t __curveto_quadratic_abs[] = {path_seg::PATHSEG_CURVETO_QUADRATIC_ABS};
static ushort_t __curveto_quadratic_rel[] = {path_seg::PATHSEG_CURVETO_QUADRATIC_REL};

// quadratic-bezier-curveto = ( "Q" | "q" ) *wsp quadratic-bezier-curveto-argument-sequence
static cwt::fsm::transition<pfs::string> quadratic_bezier_curveto[] = {
	  { 2, 1, FSM_MATCH_CHAR(_l1("Q"))                                  , FSM_NORMAL, __set_type, __curveto_quadratic_abs }
	, { 2,-1, FSM_MATCH_CHAR(_l1("q"))                                  , FSM_NORMAL, __set_type, __curveto_quadratic_rel }
	, { 3,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1)                        , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(quadratic_bezier_curveto_argument_sequence) , FSM_ACCEPT, nullptr, nullptr }
};

// smooth-curveto-argument = coordinate-pair [ comma-wsp ] coordinate-pair
static cwt::fsm::transition<pfs::string> smooth_curveto_argument[] = {
	  { 1,-1, FSM_MATCH_FSM(coordinate_pair) , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_OPT_FSM(comma_wsp)   , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(coordinate_pair) , FSM_ACCEPT, nullptr, nullptr }
};

// smooth-curveto-argument-sequence = smooth-curveto-argument
// 		/ smooth-curveto-argument [ comma-wsp ] smooth-curveto-argument-sequence
//	=>
// smooth-curveto-argument-sequence = smooth-curveto-argument *( [ comma-wsp ] smooth-curveto-argument)
static cwt::fsm::transition<pfs::string> smooth_curveto_argument_sequence_0[] = {
	  { 1,-1, FSM_MATCH_OPT_FSM(comma_wsp)           , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(smooth_curveto_argument) , FSM_ACCEPT, nullptr, nullptr }
};

static cwt::fsm::transition<pfs::string> smooth_curveto_argument_sequence[] = {
	  { 1,-1, FSM_MATCH_FSM(smooth_curveto_argument)                      , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(smooth_curveto_argument_sequence_0,-1,-1) , FSM_ACCEPT, nullptr, nullptr }
};

static ushort_t __curveto_cubic_smooth_abs[] = {path_seg::PATHSEG_CURVETO_CUBIC_SMOOTH_ABS};
static ushort_t __curveto_cubic_smooth_rel[] = {path_seg::PATHSEG_CURVETO_CUBIC_SMOOTH_REL};

// smooth-curveto = ( "S" | "s" ) *wsp smooth-curveto-argument-sequence
static cwt::fsm::transition<pfs::string> smooth_curveto[] = {
	  { 2, 1, FSM_MATCH_CHAR(_l1("S"))                        , FSM_NORMAL, __set_type, __curveto_cubic_smooth_abs }
	, { 2,-1, FSM_MATCH_CHAR(_l1("s"))                        , FSM_NORMAL, __set_type, __curveto_cubic_smooth_rel }
	, { 3,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1)              , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(smooth_curveto_argument_sequence) , FSM_ACCEPT, nullptr, nullptr }
};

// curveto-argument = coordinate-pair [ comma-wsp ] coordinate-pair [ comma-wsp ] coordinate-pair
static cwt::fsm::transition<pfs::string> curveto_argument[] = {
	  { 1,-1, FSM_MATCH_FSM(coordinate_pair) , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_OPT_FSM(comma_wsp)   , FSM_NORMAL, nullptr, nullptr }
	, { 3,-1, FSM_MATCH_FSM(coordinate_pair) , FSM_NORMAL, nullptr, nullptr }
	, { 4,-1, FSM_MATCH_OPT_FSM(comma_wsp)   , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(coordinate_pair) , FSM_ACCEPT, nullptr, nullptr }
};

// curveto-argument-sequence = curveto-argument
//		/ curveto-argument [ comma-wsp ] curveto-argument-sequence
//	=>
// curveto-argument-sequence = curveto-argument *([ comma-wsp ] curveto-argument)
static cwt::fsm::transition<pfs::string> curveto_argument_sequence_0[] = {
	  { 1,-1, FSM_MATCH_OPT_FSM(comma_wsp)    , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(curveto_argument) , FSM_ACCEPT, nullptr, nullptr }
};

static cwt::fsm::transition<pfs::string> curveto_argument_sequence[] = {
	  { 1,-1, FSM_MATCH_FSM(curveto_argument)                      , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(curveto_argument_sequence_0,-1,-1) , FSM_ACCEPT, nullptr, nullptr }
};

static ushort_t __curveto_cubic_abs[] = {path_seg::PATHSEG_CURVETO_CUBIC_ABS};
static ushort_t __curveto_cubic_rel[] = {path_seg::PATHSEG_CURVETO_CUBIC_REL};

// curveto = ( "C" | "c" ) *wsp curveto-argument-sequence
static cwt::fsm::transition<pfs::string> curveto[] = {
	  { 2, 1, FSM_MATCH_CHAR(_l1("C"))                 , FSM_NORMAL, __set_type, __curveto_cubic_abs }
	, { 2,-1, FSM_MATCH_CHAR(_l1("c"))                 , FSM_NORMAL, __set_type, __curveto_cubic_rel }
	, { 3,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1)       , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(curveto_argument_sequence) , FSM_ACCEPT, nullptr, nullptr }
};

// vertical-lineto-argument-sequence = coordinate
//		/ coordinate [ comma-wsp ] vertical-lineto-argument-sequence
//	=>
// vertical-lineto-argument-sequence = coordinate *( [ comma-wsp ] coordinate )
static cwt::fsm::transition<pfs::string> vertical_lineto_argument_sequence_0[] = {
	  { 1,-1, FSM_MATCH_OPT_FSM(comma_wsp) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(coordinate)    , FSM_ACCEPT, __add_coordinate, nullptr }
};

static cwt::fsm::transition<pfs::string> vertical_lineto_argument_sequence[] = {
	  { 1,-1, FSM_MATCH_FSM(coordinate)                                    , FSM_NORMAL, __add_coordinate, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(vertical_lineto_argument_sequence_0,-1,-1) , FSM_ACCEPT, nullptr, nullptr }
};

static ushort_t __lineto_vertical_abs[] = {path_seg::PATHSEG_LINETO_VERTICAL_ABS};
static ushort_t __lineto_vertical_rel[] = {path_seg::PATHSEG_LINETO_VERTICAL_REL};

// vertical-lineto = ( "V" | "v" ) *wsp vertical-lineto-argument-sequence
static cwt::fsm::transition<pfs::string> vertical_lineto[] = {
	  { 2, 1, FSM_MATCH_CHAR(_l1("V"))                         , FSM_NORMAL, __set_type, __lineto_vertical_abs }
	, { 2,-1, FSM_MATCH_CHAR(_l1("v"))                         , FSM_NORMAL, __set_type, __lineto_vertical_rel }
	, { 3,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1)               , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(vertical_lineto_argument_sequence) , FSM_ACCEPT, nullptr, nullptr }
};

// horizontal-lineto-argument-sequence = coordinate
//		/ coordinate [ comma-wsp ] horizontal-lineto-argument-sequence
//	=>
// horizontal-lineto-argument-sequence = coordinate *( [ comma-wsp ] coordinate )
static cwt::fsm::transition<pfs::string> horizontal_lineto_argument_sequence_0[] = {
	  { 1,-1, FSM_MATCH_OPT_FSM(comma_wsp) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(coordinate)    , FSM_ACCEPT, __add_coordinate, nullptr }
};

static cwt::fsm::transition<pfs::string> horizontal_lineto_argument_sequence[] = {
	  { 1,-1, FSM_MATCH_FSM(coordinate)                                      , FSM_NORMAL, __add_coordinate, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(horizontal_lineto_argument_sequence_0,-1,-1) , FSM_ACCEPT, nullptr, nullptr }
};

static ushort_t __lineto_horizontal_abs[] = {path_seg::PATHSEG_LINETO_HORIZONTAL_ABS};
static ushort_t __lineto_horizontal_rel[] = {path_seg::PATHSEG_LINETO_HORIZONTAL_REL};

// horizontal-lineto = ( "H" | "h" ) *wsp horizontal-lineto-argument-sequence
static cwt::fsm::transition<pfs::string> horizontal_lineto[] = {
	  { 2, 1, FSM_MATCH_CHAR(_l1("H"))                           , FSM_NORMAL, __set_type, __lineto_horizontal_abs }
	, { 2,-1, FSM_MATCH_CHAR(_l1("h"))                           , FSM_NORMAL, __set_type, __lineto_horizontal_rel }
	, { 3,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1)                 , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(horizontal_lineto_argument_sequence) , FSM_ACCEPT, nullptr, nullptr }
};

// lineto-argument-sequence = coordinate-pair
//		/ coordinate-pair [ comma-wsp ] lineto-argument-sequence
//	=>
// lineto-argument-sequence = coordinate-pair *([ comma-wsp ] coordinate-pair)
static cwt::fsm::transition<pfs::string> lineto_argument_sequence_0[] = {
	  { 1,-1, FSM_MATCH_OPT_FSM(comma_wsp)   , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(coordinate_pair) , FSM_ACCEPT, nullptr, nullptr }
};

static cwt::fsm::transition<pfs::string> lineto_argument_sequence[] = {
	  { 1,-1, FSM_MATCH_FSM(coordinate_pair)                      , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(lineto_argument_sequence_0,-1,-1) , FSM_ACCEPT, nullptr, nullptr }
};

static ushort_t __lineto_abs[] = {path_seg::PATHSEG_LINETO_ABS};
static ushort_t __lineto_rel[] = {path_seg::PATHSEG_LINETO_REL};

// lineto = ( "L" | "l" ) *wsp lineto-argument-sequence
static cwt::fsm::transition<pfs::string> lineto[] = {
	  { 2, 1, FSM_MATCH_CHAR(_l1("L"))                , FSM_NORMAL, __set_type, __lineto_abs }
	, { 2,-1, FSM_MATCH_CHAR(_l1("l"))                , FSM_NORMAL, __set_type, __lineto_rel }
	, { 3,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1)      , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(lineto_argument_sequence) , FSM_ACCEPT, nullptr, nullptr }
};

static ushort_t __closepath[] = {path_seg::PATHSEG_CLOSEPATH};
// closepath = ("Z" | "z")
static cwt::fsm::transition<pfs::string> closepath[] = {
	  {-1,-1, FSM_MATCH_CHAR(_l1("Zz")) , FSM_ACCEPT, __set_type, __closepath }
};

// moveto-argument-sequence =  coordinate-pair
//		/ coordinate-pair [ comma-wsp ] lineto-argument-sequence
// =>
// moveto-argument-sequence =  coordinate-pair [ comma-wsp ] *( lineto-argument-sequence )
static cwt::fsm::transition<pfs::string> moveto_argument_sequence[] = {
	  { 1,-1, FSM_MATCH_FSM(coordinate_pair)                    , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_OPT_FSM(comma_wsp)                      , FSM_NORMAL, __set_type, __lineto_abs }
	, {-1,-1, FSM_MATCH_RPT_FSM(lineto_argument_sequence,-1,-1) , FSM_ACCEPT, nullptr, nullptr }
};

static ushort_t __moveto_abs[] = {path_seg::PATHSEG_MOVETO_ABS};
static ushort_t __moveto_rel[] = {path_seg::PATHSEG_MOVETO_REL};

// moveto = ( "M" | "m" ) *wsp moveto-argument-sequence
static cwt::fsm::transition<pfs::string> moveto[] = {
	  { 2, 1, FSM_MATCH_CHAR(_l1("M"))                , FSM_NORMAL, __set_type, __moveto_abs }
	, { 2,-1, FSM_MATCH_CHAR(_l1("m"))                , FSM_NORMAL, __set_type, __moveto_rel }
	, { 3,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1)      , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(moveto_argument_sequence) , FSM_ACCEPT, nullptr, nullptr }
};

// drawto-command = closepath
//    / lineto
//    / horizontal-lineto
//    / vertical-lineto
//    / curveto
//    / smooth-curveto
//    / quadratic-bezier-curveto
//    / smooth-quadratic-bezier-curveto
//    / elliptical-arc
static cwt::fsm::transition<pfs::string> drawto_command[] = {
	  {-1, 1, FSM_MATCH_FSM(closepath)                       , FSM_ACCEPT, nullptr, nullptr }
	, {-1, 2, FSM_MATCH_FSM(lineto)                          , FSM_ACCEPT, nullptr, nullptr }
	, {-1, 3, FSM_MATCH_FSM(horizontal_lineto)               , FSM_ACCEPT, nullptr, nullptr }
	, {-1, 4, FSM_MATCH_FSM(vertical_lineto)                 , FSM_ACCEPT, nullptr, nullptr }
	, {-1, 5, FSM_MATCH_FSM(curveto)                         , FSM_ACCEPT, nullptr, nullptr }
	, {-1, 6, FSM_MATCH_FSM(smooth_curveto)                  , FSM_ACCEPT, nullptr, nullptr }
	, {-1, 7, FSM_MATCH_FSM(quadratic_bezier_curveto)        , FSM_ACCEPT, nullptr, nullptr }
	, {-1, 8, FSM_MATCH_FSM(smooth_quadratic_bezier_curveto) , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(elliptical_arc)                  , FSM_ACCEPT, nullptr, nullptr }
};

// drawto-commands = drawto-command
//		/ drawto-command *wsp drawto-commands
// =>
// drawto-commands = drawto-command *( *wsp drawto-command )
static cwt::fsm::transition<pfs::string> drawto_commands_0[] = {
	  { 1,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(drawto_command)      , FSM_ACCEPT, nullptr, nullptr }
};

static cwt::fsm::transition<pfs::string> drawto_commands[] = {
	  { 1,-1, FSM_MATCH_FSM(drawto_command)              , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(drawto_commands_0,-1,-1) , FSM_ACCEPT, nullptr, nullptr }
};

static ushort_t __unknown[] = {path_seg::PATHSEG_UNKNOWN};

// moveto-drawto-command-group = moveto *wsp [ drawto-commands ]
static cwt::fsm::transition<pfs::string> moveto_drawto_command_group[] = {
	  { 1,-1, FSM_MATCH_FSM(moveto)              , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_OPT_FSM(drawto_commands) , FSM_ACCEPT, __set_type, __unknown } // finish
};


// moveto-drawto-command-groups = moveto-drawto-command-group
//		/ moveto-drawto-command-group wsp* moveto-drawto-command-groups
// =>
// moveto-drawto-command-groups = moveto-drawto-command-group *( *wsp moveto-drawto-command-group)
static cwt::fsm::transition<pfs::string> moveto_drawto_command_groups_0[] = {
	  { 1,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1)         , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(moveto_drawto_command_group) , FSM_ACCEPT, nullptr, nullptr }
};

static cwt::fsm::transition<pfs::string> moveto_drawto_command_groups[] = {
	  { 1,-1, FSM_MATCH_FSM(moveto_drawto_command_group)              , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(moveto_drawto_command_groups_0,-1,-1) , FSM_ACCEPT, nullptr, nullptr }
};

// svg-path = *wsp [ moveto-drawto-command-groups ] *wsp
static cwt::fsm::transition<pfs::string> svg_path[] = {
	  { 1,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1)              , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_OPT_FSM(moveto_drawto_command_groups) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1)              , FSM_ACCEPT, nullptr, nullptr }
};

bool __set_type (const pfs::string::const_iterator & /*begin*/, const pfs::string::const_iterator & /*end*/, void * context, void * action_args)
{
	if (context) {
		PFS_ASSERT(action_args);
		path_parse_context * ctx = static_cast<path_parse_context *>(context);

		switch (ctx->type) {
		case path_seg::PATHSEG_CLOSEPATH: {
			path_seg_closepath p;
			ctx->h->onClosePath(p);
		}
			break;

		case path_seg::PATHSEG_MOVETO_ABS:
		case path_seg::PATHSEG_MOVETO_REL:
		case path_seg::PATHSEG_LINETO_ABS:
		case path_seg::PATHSEG_LINETO_REL: {
			path_seg_lineto_base p;
			size_t n = ctx->coordinates.size();

			if (!n)
				break;
			PFS_ASSERT(n % 2 == 0); // must be an even elements (coordinate pairs)

			for (size_t i = 0; i < n; i += 2) {
				p.setX(ctx->coordinates[i]);
				p.setY(ctx->coordinates[i+1]);

				switch (ctx->type) {
					case path_seg::PATHSEG_MOVETO_ABS: ctx->h->onMoveToAbs(p); break;
					case path_seg::PATHSEG_MOVETO_REL: ctx->h->onMoveToRel(p); break;
					case path_seg::PATHSEG_LINETO_ABS: ctx->h->onLineToAbs(p); break;
					case path_seg::PATHSEG_LINETO_REL: ctx->h->onLineToRel(p); break;
				}
			}
		}
			break;

		case path_seg::PATHSEG_CURVETO_CUBIC_ABS:
		case path_seg::PATHSEG_CURVETO_CUBIC_REL: {
			path_seg_curveto_cubic_base p;
			size_t n = ctx->coordinates.size();

			if (!n)
				break;

			PFS_ASSERT(n % 6 == 0); // list of three coordinate paires

			for (size_t i = 0; i < n; i += 6) {
				p.setX1(ctx->coordinates[i]);
				p.setY1(ctx->coordinates[i+1]);
				p.setX2(ctx->coordinates[i+2]);
				p.setY2(ctx->coordinates[i+3]);
				p.setX(ctx->coordinates[i+4]);
				p.setY(ctx->coordinates[i+5]);

				switch (ctx->type) {
				case path_seg::PATHSEG_CURVETO_CUBIC_ABS: ctx->h->onCurveToCubicAbs(p); break;
				case path_seg::PATHSEG_CURVETO_CUBIC_REL: ctx->h->onCurveToCubicRel(p); break;
				}
			}
		}
			break;

		case path_seg::PATHSEG_CURVETO_QUADRATIC_ABS:
		case path_seg::PATHSEG_CURVETO_QUADRATIC_REL: {
			path_seg_curveto_quadratic_base p;
			size_t n = ctx->coordinates.size();

			if (!n)
				break;

			PFS_ASSERT(n % 4 == 0);

			for (size_t i = 0; i < n; i += 4) {
				p.setX1(ctx->coordinates[i]);
				p.setY1(ctx->coordinates[i+1]);
				p.setX(ctx->coordinates[i+2]);
				p.setY(ctx->coordinates[i+3]);

				switch (ctx->type) {
				case path_seg::PATHSEG_CURVETO_QUADRATIC_ABS: ctx->h->onCurveToQuadraticAbs(p); break;
				case path_seg::PATHSEG_CURVETO_QUADRATIC_REL: ctx->h->onCurveToQuadraticRel(p); break;
				}
			}
		}
		break;

		case path_seg::PATHSEG_ARC_ABS:
		case path_seg::PATHSEG_ARC_REL: {
			path_seg_arc_base p;
			size_t n = ctx->coordinates.size();

			if (!n)
				break;

			PFS_ASSERT(n % 5 == 0);

			for (size_t i = 0; i < n; i += 5) {
				p.setRx(ctx->coordinates[i]);
				p.setRy(ctx->coordinates[i+1]);
				p.setAngle(ctx->coordinates[i+2]);
				p.setX(ctx->coordinates[i+3]);
				p.setY(ctx->coordinates[i+4]);
				p.setFlags(ctx->largeArcFlag, ctx->sweepFlag);

				switch (ctx->type) {
					case path_seg::PATHSEG_ARC_ABS: ctx->h->onArcAbs(p); break;
					case path_seg::PATHSEG_ARC_REL: ctx->h->onArcRel(p); break;
				}
			}
		}
			break;

		case path_seg::PATHSEG_LINETO_HORIZONTAL_ABS:
		case path_seg::PATHSEG_LINETO_HORIZONTAL_REL:
		case path_seg::PATHSEG_LINETO_VERTICAL_ABS:
		case path_seg::PATHSEG_LINETO_VERTICAL_REL: {
			path_seg_lineto_dir_base p;
			size_t n = ctx->coordinates.size();

			if (!n)
				break;

			for (size_t i = 0; i < n; ++i) {
				p.setX(ctx->coordinates[i]);
				switch (ctx->type) {
				case path_seg::PATHSEG_LINETO_HORIZONTAL_ABS: ctx->h->onLineToHorizontalAbs(p); break;
				case path_seg::PATHSEG_LINETO_HORIZONTAL_REL: ctx->h->onLineToHorizontalRel(p); break;
				case path_seg::PATHSEG_LINETO_VERTICAL_ABS:   ctx->h->onLineToVerticalAbs(p); break;
				case path_seg::PATHSEG_LINETO_VERTICAL_REL:   ctx->h->onLineToVerticalRel(p); break;
				}
			}
		}
			break;

		case path_seg::PATHSEG_CURVETO_CUBIC_SMOOTH_ABS:
		case path_seg::PATHSEG_CURVETO_CUBIC_SMOOTH_REL: {
			path_seg_curveto_cubic_smooth_base p;
			size_t n = ctx->coordinates.size();

			if (!n)
				break;

			PFS_ASSERT(n % 4 == 0);

			for (size_t i = 0; i < n; i += 4) {
				p.setX2(ctx->coordinates[i]);
				p.setY2(ctx->coordinates[i+1]);
				p.setX(ctx->coordinates[i+2]);
				p.setY(ctx->coordinates[i+3]);

				switch (ctx->type) {
				case path_seg::PATHSEG_CURVETO_CUBIC_SMOOTH_ABS: ctx->h->onCurveToCubicSmoothAbs(p); break;
				case path_seg::PATHSEG_CURVETO_CUBIC_SMOOTH_REL: ctx->h->onCurveToCubicSmoothRel(p); break;
				}
			}
		}
			break;

		case path_seg::PATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS:
		case path_seg::PATHSEG_CURVETO_QUADRATIC_SMOOTH_REL: {
			path_seg_curveto_quadratic_smooth_base p;
			size_t n = ctx->coordinates.size();

			if (!n)
				break;

			PFS_ASSERT(n % 2 == 0);

			for (size_t i = 0; i < n; i += 2) {
				p.setX(ctx->coordinates[i]);
				p.setY(ctx->coordinates[i+1]);

				switch (ctx->type) {
				case path_seg::PATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS: ctx->h->onCurveToQuadraticSmoothAbs(p); break;
				case path_seg::PATHSEG_CURVETO_QUADRATIC_SMOOTH_REL: ctx->h->onCurveToQuadraticSmoothRel(p); break;
				}
			}
		}
			break;

		case path_seg::PATHSEG_UNKNOWN:
			break;
		}

		ctx->type = *static_cast<ushort_t *>(action_args);
		ctx->coordinates.clear();
	}
	return true;
}


static bool __set_largeArcFlag (const pfs::string::const_iterator & begin, const pfs::string::const_iterator & end, void * context, void * /*action_args*/)
{
	if (context) {
		path_parse_context * ctx = static_cast<path_parse_context *>(context);
		pfs::string(begin, end).toDouble();
		ctx->largeArcFlag = pfs::string(begin, end).toInt() == 0 ? false : true;
	}
	return true;
}

static bool __set_sweepFlag (const pfs::string::const_iterator & begin, const pfs::string::const_iterator & end, void * context, void * /*action_args*/)
{
	if (context) {
		path_parse_context * ctx = static_cast<path_parse_context *>(context);
		pfs::string(begin, end).toDouble();
		ctx->sweepFlag = pfs::string(begin, end).toInt() == 0 ? false : true;
	}
	return true;
}


static bool __add_coordinate (const pfs::string::const_iterator & begin, const pfs::string::const_iterator & end, void * context, void * /*action_args*/)
{
	if (context) {
		path_parse_context * ctx = static_cast<path_parse_context *>(context);
		double coord = pfs::string(begin, end).toDouble();
		ctx->coordinates.append(coord);
	}
	return true;
}

bool path_seg::parse (const pfs::string & path, path_parse_handlers & h)
{
	path_parse_context ctx;
	ctx.h = & h;
	ctx.type = PATHSEG_UNKNOWN;

	fsm::fsm<pfs::string> fsm(svg_path, & ctx);
	h.begin();
	ssize_t r = fsm.exec(0, path.cbegin(), path.cend());

	if (r >= 0 && size_t(r) == path.length()) {
		h.end(true);
		return true;
	}

	h.end(false);
	return false;
}

}} // cwt::svg
