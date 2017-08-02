/**
 * @file transform.cpp
 * @author wladt
 * @date Feb 21, 2014
 */

// see [7.6 The ‘transform’ attribute](http://www.w3.org/TR/SVG11/coords.html#TransformAttribute)
/*

 transform-list:
    wsp* transforms? wsp*

transforms:
    transform
    | transform comma-wsp+ transforms

transform:
    matrix
    | translate
    | scale
    | rotate
    | skewX
    | skewY

matrix:
    "matrix" wsp* "(" wsp*
       number comma-wsp
       number comma-wsp
       number comma-wsp
       number comma-wsp
       number comma-wsp
       number wsp* ")"

translate:
    "translate" wsp* "(" wsp* number ( comma-wsp number )? wsp* ")"

scale:
    "scale" wsp* "(" wsp* number ( comma-wsp number )? wsp* ")"

rotate:
    "rotate" wsp* "(" wsp* number ( comma-wsp number comma-wsp number )? wsp* ")"

skewX:
    "skewX" wsp* "(" wsp* number wsp* ")"

skewY:
    "skewY" wsp* "(" wsp* number wsp* ")"
 */

#include "../include/cwt/svg/transform.hpp"
#include "fsm_common.hpp"

namespace cwt { namespace svg {

struct transform_context
{
	transform_handlers * h;
	pfs::vector<double> coordinates;
};

//static bool __set_type         (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void *, void *);
static bool __reset          (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void *, void *);
static bool __add_coordinate (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void *, void *);
static bool __set_matrix     (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void *, void *);
static bool __set_translate  (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void * context, void *);
static bool __set_scale      (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void * context, void *);
static bool __set_rotate     (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void *, void *);
static bool __set_skewX      (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void *, void *);
static bool __set_skewY      (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void *, void *);


// skewX = "skewX" *wsp "(" *wsp number *wsp ")"
static cwt::fsm::transition<pfs::string> skewX_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_l1("skewX"))        , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, { 3,-1, FSM_MATCH_CHAR(_l1("("))           , FSM_NORMAL, nullptr, nullptr }
	, { 4,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, { 5,-1, FSM_MATCH_FSM(number)              , FSM_NORMAL, __add_coordinate, nullptr }
	, { 6,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_CHAR(_l1(")"))           , FSM_ACCEPT, __set_skewX, nullptr }
};

// skewY = "skewY" *wsp "(" *wsp number *wsp ")"
static cwt::fsm::transition<pfs::string> skewY_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_l1("skewY"))        , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, { 3,-1, FSM_MATCH_CHAR(_l1("("))           , FSM_NORMAL, nullptr, nullptr }
	, { 4,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, { 5,-1, FSM_MATCH_FSM(number)              , FSM_NORMAL, __add_coordinate, nullptr }
	, { 6,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_CHAR(_l1(")"))           , FSM_ACCEPT, __set_skewY, nullptr }
};

// rotate = "rotate" *wsp "(" *wsp number [ comma-wsp number comma-wsp number ] *wsp ")"
static cwt::fsm::transition<pfs::string> rotate_0_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(comma_wsp) , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_FSM(number)    , FSM_NORMAL, __add_coordinate, nullptr }
	, { 3,-1, FSM_MATCH_FSM(comma_wsp) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(number)    , FSM_ACCEPT, __add_coordinate, nullptr }
};

static cwt::fsm::transition<pfs::string> rotate_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_l1("rotate"))       , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, { 3,-1, FSM_MATCH_CHAR(_l1("("))           , FSM_NORMAL, nullptr, nullptr }
	, { 4,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, { 5,-1, FSM_MATCH_FSM(number)              , FSM_NORMAL, __add_coordinate, nullptr }
	, { 6,-1, FSM_MATCH_OPT_FSM(rotate_0_fsm)    , FSM_NORMAL, nullptr, nullptr }
	, { 7,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_CHAR(_l1(")"))           , FSM_ACCEPT, __set_rotate, nullptr }
};

// scale = "scale" *wsp "(" wsp* number [ comma-wsp number ] *wsp ")"
static cwt::fsm::transition<pfs::string> scale_0_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(comma_wsp) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(number)    , FSM_ACCEPT, __add_coordinate, nullptr }
};

static cwt::fsm::transition<pfs::string> scale_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_l1("scale"))        , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, { 3,-1, FSM_MATCH_CHAR(_l1("("))           , FSM_NORMAL, nullptr, nullptr }
	, { 4,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, { 5,-1, FSM_MATCH_FSM(number)              , FSM_NORMAL, __add_coordinate, nullptr }
	, { 6,-1, FSM_MATCH_OPT_FSM(scale_0_fsm)     , FSM_NORMAL, nullptr, nullptr }
	, { 7,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_CHAR(_l1(")"))           , FSM_ACCEPT, __set_scale, nullptr }
};

// translate = "translate" *wsp "(" *wsp number [ comma-wsp number ] *wsp ")"
static cwt::fsm::transition<pfs::string> translate_0_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(comma_wsp) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(number)    , FSM_ACCEPT, __add_coordinate, nullptr }
};

static cwt::fsm::transition<pfs::string> translate_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_l1("translate"))    , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, { 3,-1, FSM_MATCH_CHAR(_l1("("))           , FSM_NORMAL, nullptr, nullptr }
	, { 4,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, { 5,-1, FSM_MATCH_FSM(number)              , FSM_NORMAL, __add_coordinate, nullptr }
	, { 6,-1, FSM_MATCH_OPT_FSM(translate_0_fsm) , FSM_NORMAL, nullptr, nullptr }
	, { 7,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_CHAR(_l1(")"))           , FSM_ACCEPT, __set_translate, nullptr }
};

// matrix = "matrix" *wsp "(" *wsp
//       number comma-wsp
//       number comma-wsp
//       number comma-wsp
//       number comma-wsp
//       number comma-wsp
//       number *wsp ")"
static cwt::fsm::transition<pfs::string> matrix_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_l1("matrix"))       , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, { 3,-1, FSM_MATCH_CHAR(_l1("("))           , FSM_NORMAL, nullptr, nullptr }
	, { 4,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }

	, { 5,-1, FSM_MATCH_FSM(number)              , FSM_NORMAL, __add_coordinate, nullptr }
	, { 6,-1, FSM_MATCH_FSM(comma_wsp)           , FSM_NORMAL, nullptr, nullptr }

	, { 7,-1, FSM_MATCH_FSM(number)              , FSM_NORMAL, __add_coordinate, nullptr }
	, { 8,-1, FSM_MATCH_FSM(comma_wsp)           , FSM_NORMAL, nullptr, nullptr }

	, { 9,-1, FSM_MATCH_FSM(number)              , FSM_NORMAL, __add_coordinate, nullptr }
	, {10,-1, FSM_MATCH_FSM(comma_wsp)           , FSM_NORMAL, nullptr, nullptr }

	, {11,-1, FSM_MATCH_FSM(number)              , FSM_NORMAL, __add_coordinate, nullptr }
	, {12,-1, FSM_MATCH_FSM(comma_wsp)           , FSM_NORMAL, nullptr, nullptr }

	, {13,-1, FSM_MATCH_FSM(number)              , FSM_NORMAL, __add_coordinate, nullptr }
	, {14,-1, FSM_MATCH_FSM(comma_wsp)           , FSM_NORMAL, nullptr, nullptr }

	, {15,-1, FSM_MATCH_FSM(number)              , FSM_NORMAL, __add_coordinate, nullptr }
	, {16,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_CHAR(_l1(")"))           , FSM_ACCEPT, __set_matrix, nullptr }
};

// transform =  matrix
//    / translate
//    / scale
//    / rotate
//    / skewX
//    / skewY
static cwt::fsm::transition<pfs::string> transform_fsm[] = {
	  { 1,-1, FSM_MATCH_NOTHING            , FSM_NORMAL, __reset, nullptr }
	, {-1, 2, FSM_MATCH_FSM(matrix_fsm)    , FSM_ACCEPT, nullptr, nullptr }
	, {-1, 3, FSM_MATCH_FSM(translate_fsm) , FSM_ACCEPT, nullptr, nullptr }
	, {-1, 4, FSM_MATCH_FSM(scale_fsm)     , FSM_ACCEPT, nullptr, nullptr }
	, {-1, 5, FSM_MATCH_FSM(rotate_fsm)    , FSM_ACCEPT, nullptr, nullptr }
	, {-1, 6, FSM_MATCH_FSM(skewX_fsm)     , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(skewY_fsm)     , FSM_ACCEPT, nullptr, nullptr }
};

// transforms = transform
// 		/ transform 1*comma-wsp transforms
// =>
// transforms = transform *( 1*comma-wsp transform )
static cwt::fsm::transition<pfs::string> transforms_0_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT_FSM(comma_wsp, 1,-1), FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(transform_fsm)      , FSM_ACCEPT, nullptr, nullptr }
};

static cwt::fsm::transition<pfs::string> transforms_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(transform_fsm)              , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(transforms_0_fsm,-1,-1) , FSM_ACCEPT, nullptr, nullptr }
};

// transform-list = *wsp [ transforms ] *wsp
static cwt::fsm::transition<pfs::string> transform_list_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_OPT_FSM(transforms_fsm)  , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_CHAR(_SVG_WSP,-1,-1) , FSM_ACCEPT, nullptr, nullptr }
};

bool __reset (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void * context, void *)
{
	if (context) {
		transform_context * ctx = static_cast<transform_context *>(context);
		ctx->coordinates.clear();
	}
	return true;
}

bool __add_coordinate (const pfs::string::const_iterator & begin, const pfs::string::const_iterator & end, void * context, void * /*action_args*/)
{
	if (context) {
		transform_context * ctx = static_cast<transform_context *>(context);
		double coord = pfs::string(begin, end).toDouble();
		ctx->coordinates.append(coord);
	}
	return true;
}

// matrix(<a> <b> <c> <d> <e> <f>),
// which specifies a transformation in the form of a transformation matrix of six values.
// matrix(a,b,c,d,e,f) is equivalent to applying the transformation matrix [a b c d e f].
//
bool __set_matrix  (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void * context, void *)
{
	if (context) {
		transform_context * ctx = static_cast<transform_context *>(context);
		PFS_ASSERT(ctx->coordinates.size() == 6);
		ctx->h->onMatrix(matrix(
			  ctx->coordinates[0]
			, ctx->coordinates[1]
			, ctx->coordinates[2]
			, ctx->coordinates[3]
			, ctx->coordinates[4]
			, ctx->coordinates[5]));
	}
	return true;
}

// translate(<tx> [<ty>]), which specifies a translation by tx and ty. If <ty> is not provided, it is assumed to be zero.
//
bool __set_translate (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void * context, void *)
{
	if (context) {
		transform_context * ctx = static_cast<transform_context *>(context);
		PFS_ASSERT(ctx->coordinates.size() == 1 || ctx->coordinates.size() == 2);
		ctx->h->onTranslate(translate(
			  ctx->coordinates[0]
			, ctx->coordinates.size() == 2 ? ctx->coordinates[1] : 0));
	}
	return true;
}

// scale(<sx> [<sy>]), which specifies a scale operation by sx and sy. If <sy> is not provided, it is assumed to be equal to <sx>.
//
bool __set_scale (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void * context, void *)
{
	if (context) {
		transform_context * ctx = static_cast<transform_context *>(context);
		PFS_ASSERT(ctx->coordinates.size() == 1 || ctx->coordinates.size() == 2);
		ctx->h->onScale(scale(
			  ctx->coordinates[0]
			, ctx->coordinates.size() == 2 ? ctx->coordinates[1] : ctx->coordinates[0]));
	}
	return true;
}

// rotate(<rotate-angle> [<cx> <cy>]),
// which specifies a rotation by <rotate-angle> degrees about a given point.
// If optional parameters <cx> and <cy> are not supplied,
// the rotate is about the origin of the current user coordinate system.
// The operation corresponds to the matrix [cos(a) sin(a) -sin(a) cos(a) 0 0].
// If optional parameters <cx> and <cy> are supplied, the rotate is about the point (cx, cy).
// The operation represents the equivalent of the following specification:
// translate(<cx>, <cy>) rotate(<rotate-angle>) translate(-<cx>, -<cy>).
//
static bool __set_rotate  (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void * context, void *)
{
	if (context) {
		transform_context * ctx = static_cast<transform_context *>(context);
		PFS_ASSERT(ctx->coordinates.size() > 0 && ctx->coordinates.size() <= 3);

		ctx->h->onRotate(rotate(
			ctx->coordinates[0]
			, ctx->coordinates.size() > 1 ? ctx->coordinates[1] : 0.0f
			, ctx->coordinates.size() > 2 ? ctx->coordinates[2] : 0.0f));
	}
	return true;
}

// skewX(<skew-angle>), which specifies a skew transformation along the x-axis.
//
bool __set_skewX (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void * context, void *)
{
	if (context) {
		transform_context * ctx = static_cast<transform_context *>(context);
		PFS_ASSERT(ctx->coordinates.size() == 1);
		ctx->h->onSkewX(skewX(ctx->coordinates[0]));
	}
	return true;
}

// skewY(<skew-angle>), which specifies a skew transformation along the y-axis.
//
static bool __set_skewY (const pfs::string::const_iterator &, const pfs::string::const_iterator &, void * context, void *)
{
	if (context) {
		transform_context * ctx = static_cast<transform_context *>(context);
		PFS_ASSERT(ctx->coordinates.size() == 1);
		ctx->h->onSkewY(skewY(ctx->coordinates[0]));
	}
	return true;
}


bool transform::parse (const pfs::string & str, transform_handlers & h)
{
	transform_context ctx;
	ctx.h = & h;

	fsm::fsm<pfs::string> fsm(transform_list_fsm, & ctx);
	h.begin();
	ssize_t r = fsm.exec(0, str.cbegin(), str.cend());

	if (r >= 0 && size_t(r) == str.length()) {
		h.end(true);
		return true;
	}

	h.end(false);
	return false;
}

}} // cwt::svg
