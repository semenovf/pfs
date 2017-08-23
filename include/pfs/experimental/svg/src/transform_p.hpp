/**
 * @file transform_p.hpp
 * @author wladt
 * @date Feb 24, 2014
 */

#include "../include/cwt/svg/transform.hpp"
#include <pfs/math.hpp>

namespace cwt { namespace svg {

//
// a c e
// b d f
// 0 0 1
//
class transform::impl
{
public:
	double _a;
	double _b;
	double _c;
	double _d;
	double _e;
	double _f;
	double _angle;

public:
	impl (): _a(0.0f), _b(0.0f), _c(0.0f), _d(0.0f), _e(0.0f), _f(0.0f), _angle(0.0) {}
	impl (double a, double b, double c, double d, double e, double f, double angle = 0.0f)
		: _a(a), _b(b), _c(c), _d(d), _e(e), _f(f), _angle(angle) {}
	impl (double m[6], double angle = 0.0f)
		: _a(m[0]), _b(m[1]), _c(m[2]), _d(m[3]), _e(m[4]), _f(m[5]), _angle(angle) {}

/*
	transform_impl multiply (const impl & other);
	transform_impl inverse ();
	transform_impl translate (double x, double y);
	transform_impl scale (double scaleFactor);
	transform_impl scaleNonUniform (double scaleFactorX, double scaleFactorY);
	transform_impl rotate (double angle);
	transform_impl rotateFromVector (double x, double y);
	transform_impl flipX ();
	transform_impl flipY ();
	transform_impl skewX (double angle);
	transform_impl skewY (double angle);
*/
};

class rotate_impl : public transform::impl
{
public:
	double _cx, _cy;

public:
	rotate_impl () : transform::impl(), _cx(0.0f), _cy(0.0f) {}
	rotate_impl (double angle, double cx = 0.0f, double cy = 0.0f)
		: transform::impl(), _cx(cx), _cy(cy) { setAngle(angle); }

	void setAngle (double angle)
	{
		_angle = angle;
		_a = _d = cos(angle);
		_b = sin(angle);
		_c = -1 * sin(angle);
	}


};

}} // cwt::svg


