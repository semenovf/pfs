/**
 * @file transform.cpp
 * @author wladt
 * @date Feb 21, 2014
 */

#include "transform_p.hpp"
#include "fsm_common.hpp"

namespace cwt { namespace svg {

transform_handlers transform::NullTransformHandlers;

PFS_PIMPL_DEF(transform, transform::impl);

// ******** transform ***********
transform::transform (transform::impl * pimpl) : _pimpl(pimpl) {}
transform::transform () : _pimpl(new transform::impl) {}
transform::transform (double a, double b, double c, double d, double e, double f, double angle)
	: _pimpl(new transform::impl(a, b, c, d, e, f, angle)) {}
transform::transform (double m[6], double angle)
	: _pimpl(new transform::impl(m, angle)) {}


// ******** translate ***********
translate::translate () : transform() { _pimpl->_a = 1; _pimpl->_d = 1; }
translate::translate (double tx, double ty) : transform()
{
	_pimpl->_a = 1;
	_pimpl->_d = 1;
	set(tx, ty);
}
double translate::tx () const { return _pimpl->_e; }
double translate::ty () const { return _pimpl->_f; }
void translate::set (double tx, double ty) { _pimpl->_e = tx; _pimpl->_f = ty; }

// ******** scale ***********
scale::scale () : transform() { }
scale::scale (double sx, double sy) : transform() { set(sx, sy); }
double scale::sx () const { return _pimpl->_a; }
double scale::sy () const { return _pimpl->_d; }
void scale::set (double sx, double sy) { _pimpl->_a = sx; _pimpl->_d = sy; }


// ******** rotate ***********
rotate::rotate () : transform(new rotate_impl), _cx(0.0f), _cy(0.0f) { setAngle(0); }
rotate::rotate (double a, double cx, double cy)
	: transform(new rotate_impl), _cx(cx), _cy(cy) { setAngle(a); }

double rotate::angle () const { return _pimpl->_angle; }
void   rotate::setAngle (double angle) { static_cast<rotate_impl *>(_pimpl.get())->setAngle(angle); }
double rotate::cx () const { return static_cast<rotate_impl *>(_pimpl.get())->_cx; }
double rotate::cy () const { return static_cast<rotate_impl *>(_pimpl.get())->_cy; }
void   rotate::setCenter(double cx, double cy)
{
	static_cast<rotate_impl *>(_pimpl.get())->_cx = cx;
	static_cast<rotate_impl *>(_pimpl.get())->_cy = cy;
}

// ******** skewX ***********
skewX::skewX () : transform() { _pimpl->_a = 1; _pimpl->_d = 1; }
skewX::skewX (double angle) : transform() { _pimpl->_a = 1; _pimpl->_d = 1; setAngle(angle); }
double skewX::angle () const { return _pimpl->_angle; }
void skewX::setAngle (double angle) { _pimpl->_angle = angle; _pimpl->_c = tan(angle); }

// ******** skewY ***********
skewY::skewY () : transform() { _pimpl->_a = 1; _pimpl->_d = 1; }
skewY::skewY (double angle) : transform() { _pimpl->_a = 1; _pimpl->_d = 1; setAngle(angle); }
double skewY::angle () const { return _pimpl->_angle; }
void skewY::setAngle (double angle) { _pimpl->_angle = angle; _pimpl->_b = tan(angle); }

}} // cwt::svg
