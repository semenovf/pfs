/**
 * @file transform.hpp
 * @author wladt
 * @date Feb 21, 2014
 */

#ifndef __CWT_SVG_TRANSFORM_HPP__
#define __CWT_SVG_TRANSFORM_HPP__

#include <pfs/string.hpp>
#include <pfs/vector.hpp>
#include <pfs/pimpl.hpp>

namespace cwt { namespace svg {

class matrix;
class translate;
class scale;
class rotate;
class skewX;
class skewY;

class transform_handlers
{
public:
	transform_handlers () {}
	virtual ~transform_handlers () {}

	virtual void begin    () {}
	virtual void end      (bool /*success*/) {}
	virtual void onMatrix (const matrix &) {}
	virtual void onTranslate (const translate &) {}
	virtual void onScale  (const scale &) {}
	virtual void onRotate (const rotate &) {}
	virtual void onSkewX  (const skewX &) {}
	virtual void onSkewY  (const skewY &) {}
};

//
// a c e
// b d f
// 0 0 1
//
class matrix_base
{
	double _a;
	double _b;
	double _c;
	double _d;
	double _e;
	double _f;

public:
	matrix_base ()
		: _a(0.0f), _b(0.0f), _c(0.0f), _d(0.0f), _e(0.0f), _f(0.0f) {}

	matrix_base (double a, double b, double c, double d, double e, double f)
		: _a(a), _b(b), _c(c), _d(d), _e(e), _f(f) {}

	matrix_base (double m[6])
		: _a(m[0]), _b(m[1]), _c(m[2]), _d(m[3]), _e(m[4]), _f(m[5]) {}

/*
*/
};

// Mathematically, all transformations can be represented as 3x3 transformation matrices of the following form:
// a c e
// b d f
// 0 0 1
//
//class transform_impl;

class transform
{
//protected:
	PFS_PIMPL_DECL(transform, public, impl, protected);

public:
	static transform_handlers NullTransformHandlers;

	static const ushort_t SVG_TRANSFORM_UNKNOWN   = 0;
	static const ushort_t SVG_TRANSFORM_MATRIX    = 1;
	static const ushort_t SVG_TRANSFORM_TRANSLATE = 2;
	static const ushort_t SVG_TRANSFORM_SCALE     = 3;
	static const ushort_t SVG_TRANSFORM_ROTATE    = 4;
	static const ushort_t SVG_TRANSFORM_SKEWX     = 5;
	static const ushort_t SVG_TRANSFORM_SKEWY     = 6;

protected:
	transform (transform::impl *);

public:
	transform ();
	transform (double a, double b, double c, double d, double e, double f, double angle = 0.0f);
	transform (double m[6], double angle = 0.0f);

	virtual ushort_t type () const { return SVG_TRANSFORM_UNKNOWN; }

public:
	static bool parse (const pfs::string & str, transform_handlers & h = NullTransformHandlers);
};

class matrix : public transform
{
public:
	matrix () : transform() {}
	matrix (double a, double b, double c, double d, double e, double f)
		: transform(a, b, c, d, e, f) {}
	matrix (double m[6]) : transform(m) {}

	virtual ushort_t type () const { return SVG_TRANSFORM_MATRIX; }
};

// Translation is equivalent to the matrix
//
// 1 0 tx
// 0 1 ty
// 0 0 1
// where tx and ty are the distances to translate coordinates in X and Y, respectively.
//
class translate : public transform
{
public:
	translate ();
	translate (double tx, double ty);
	virtual ushort_t type () const { return SVG_TRANSFORM_TRANSLATE; }

	double tx () const;
	double ty () const;
	void set (double tx, double ty);
};

// Scaling is equivalent to the matrix
//
// sx 0 0
// 0 sy 0
// 0  0 1
//
class scale : public transform
{
public:
	scale ();
	scale (double sx, double sy);
	virtual ushort_t type () const { return SVG_TRANSFORM_SCALE; }
	double sx () const;
	double sy () const;
	void set (double sx, double sy);
};

// Rotation about the origin is equivalent to the matrix
//
// cos(a) -sin(a) 0
// sin(a)  cos(a) 0
//      0       0 1
//
// If optional parameters <cx> and <cy> are supplied, the rotate is about the point (cx, cy)
// The operation represents the equivalent of the following specification:
// translate(<cx>, <cy>) rotate(<rotate-angle>) translate(-<cx>, -<cy>).
//
class rotate : public transform
{
	double _cx, _cy;

public:
	rotate ();
	rotate (double a, double cx = 0.0f, double cy = 0.0f);
	virtual ushort_t type () const { return SVG_TRANSFORM_ROTATE; }
	double angle () const;
	void setAngle (double a);
	double cx () const;
	double cy () const;
	void setCenter(double cx, double cy);
};

// A skew transformation along the x-axis is equivalent to the matrix
//
// 1 tan(a) 0
// 0   1    0
// 0   0    1
//
class skewX : public transform
{
public:
	skewX ();
	skewX (double a);
	virtual ushort_t type () const { return SVG_TRANSFORM_SKEWX; }
	double angle () const;
	void setAngle (double a);
};

// A skew transformation along the y-axis is equivalent to the matrix
//
//      1  0  0
// tan(a)  1  0
//      0  0  1
//
class skewY : public transform
{
public:
	skewY ();
	skewY (double angle);
	virtual ushort_t type () const { return SVG_TRANSFORM_SKEWY; }
	double angle () const;
	void setAngle (double angle);
};


class transform_list
{
	pfs::vector<transform> _list;

public:
	transform_list () {}

/*
	void clear();
	transform initialize (const transform & newItem);
	transform getItem (size_t index);
	transform insertItemBefore (const transform & newItem, size_t index);
	transform replaceItem (const transform & newItem, size_t index);
	transform removeItem (size_t index);
	transform appendItem (const transform & newItem)
	transform createSVGTransformFromMatrix (const matrix & m);
	transform consolidate ();
*/
};

}} // cwt::svg

#endif /* __CWT_SVG_TRANSFORM_HPP__ */
