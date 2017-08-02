/**
 * @file path.hpp
 * @author wladt
 * @date Feb 20, 2014
 */

#ifndef __CWT_SVG_PATH_HPP__
#define __CWT_SVG_PATH_HPP__

#include <pfs/string.hpp>

namespace cwt { namespace svg {

class path_seg_closepath;
class path_seg_moveto_abs;
class path_seg_moveto_rel;
class path_seg_lineto_abs;
class path_seg_lineto_rel;
class path_seg_curveto_cubic_abs;
class path_seg_curveto_cubic_rel;
class path_seg_curveto_quadratic_abs;
class path_seg_curveto_quadratic_rel;
class path_seg_arc_abs;
class path_seg_arc_rel;
class path_seg_lineto_horizontal_abs;
class path_seg_lineto_horizontal_rel;
class path_seg_lineto_vertical_abs;
class path_seg_lineto_vertical_rel;
class path_seg_curveto_cubic_smooth_abs;
class path_seg_curveto_cubic_smooth_rel;
class path_seg_curveto_quadratic_smooth_abs;
class path_seg_curveto_quadratic_smooth_rel;

class path_parse_handlers
{
public:
	path_parse_handlers () {}
	virtual ~path_parse_handlers () {}

	virtual void begin () {}
	virtual void end (bool /*success*/) {}
	virtual void onClosePath (const path_seg_closepath & /*pseg*/) {}
	virtual void onMoveToAbs (const path_seg_moveto_abs & /*pseg*/) {}
	virtual void onMoveToRel (const path_seg_moveto_rel & /*pseg*/) {}
	virtual void onLineToAbs (const path_seg_lineto_abs & /*pseg*/) {}
	virtual void onLineToRel (const path_seg_lineto_rel & /*pseg*/) {}
	virtual void onCurveToCubicAbs (const path_seg_curveto_cubic_abs & /*pseg*/) {}
	virtual void onCurveToCubicRel (const path_seg_curveto_cubic_rel & /*pseg*/) {}
	virtual void onCurveToQuadraticAbs (const path_seg_curveto_quadratic_abs & /*pseg*/) {}
	virtual void onCurveToQuadraticRel (const path_seg_curveto_quadratic_rel & /*pseg*/) {}
	virtual void onArcAbs (const path_seg_arc_abs & /*pseg*/) {}
	virtual void onArcRel (const path_seg_arc_rel & /*pseg*/) {}
	virtual void onLineToHorizontalAbs (const path_seg_lineto_horizontal_abs & /*pseg*/) {}
	virtual void onLineToHorizontalRel (const path_seg_lineto_horizontal_rel & /*pseg*/) {}
	virtual void onLineToVerticalAbs (const path_seg_lineto_vertical_abs & /*pseg*/) {}
	virtual void onLineToVerticalRel (const path_seg_lineto_vertical_rel & /*pseg*/) {}
	virtual void onCurveToCubicSmoothAbs (const path_seg_curveto_cubic_smooth_abs & /*pseg*/) {}
	virtual void onCurveToCubicSmoothRel (const path_seg_curveto_cubic_smooth_rel & /*pseg*/) {}
	virtual void onCurveToQuadraticSmoothAbs (const path_seg_curveto_quadratic_smooth_abs & /*pseg*/) {}
	virtual void onCurveToQuadraticSmoothRel (const path_seg_curveto_quadratic_smooth_rel & /*pseg*/) {}
};


class path_seg
{
public:
	static path_parse_handlers NullPathParseHandlers;

	static const ushort_t PATHSEG_UNKNOWN = 0;
	static const ushort_t PATHSEG_CLOSEPATH = 1;
	static const ushort_t PATHSEG_MOVETO_ABS = 2;
	static const ushort_t PATHSEG_MOVETO_REL = 3;
	static const ushort_t PATHSEG_LINETO_ABS = 4;
	static const ushort_t PATHSEG_LINETO_REL = 5;
	static const ushort_t PATHSEG_CURVETO_CUBIC_ABS = 6;
	static const ushort_t PATHSEG_CURVETO_CUBIC_REL = 7;
	static const ushort_t PATHSEG_CURVETO_QUADRATIC_ABS = 8;
	static const ushort_t PATHSEG_CURVETO_QUADRATIC_REL = 9;
	static const ushort_t PATHSEG_ARC_ABS = 10;
	static const ushort_t PATHSEG_ARC_REL = 11;
	static const ushort_t PATHSEG_LINETO_HORIZONTAL_ABS = 12;
	static const ushort_t PATHSEG_LINETO_HORIZONTAL_REL = 13;
	static const ushort_t PATHSEG_LINETO_VERTICAL_ABS = 14;
	static const ushort_t PATHSEG_LINETO_VERTICAL_REL = 15;
	static const ushort_t PATHSEG_CURVETO_CUBIC_SMOOTH_ABS = 16;
	static const ushort_t PATHSEG_CURVETO_CUBIC_SMOOTH_REL = 17;
	static const ushort_t PATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS = 18;
	static const ushort_t PATHSEG_CURVETO_QUADRATIC_SMOOTH_REL = 19;

public:
	virtual ushort_t type () const { return PATHSEG_UNKNOWN; }
	virtual pfs::string typeLetter () const { return pfs::string(); }

public:
	static bool parse (const pfs::string & path, path_parse_handlers & h = NullPathParseHandlers);
};

class path_seg_closepath : public path_seg
{
public:
	path_seg_closepath () : path_seg () {}
	virtual ushort_t type () const { return PATHSEG_CLOSEPATH; }
	virtual pfs::string typeLetter () const { return _l1("z"); }
};

class path_seg_lineto_base : public path_seg
{
	friend class path_seg_moveto_abs;
	friend class path_seg_moveto_rel;
	friend class path_seg_lineto_abs;
	friend class path_seg_lineto_rel;

protected:
	double _x;
	double _y;

public:
	path_seg_lineto_base () : path_seg(), _x(0.0f), _y(0.0f) {}
	path_seg_lineto_base (double x, double y) : path_seg(), _x(x), _y(y) {}

	double x () const { return _x; }
	double y () const { return _y; }

	void setX (double x) { _x = x; }
	void setY (double y) { _y = y; }
};


class path_seg_moveto_abs : public path_seg_lineto_base
{
public:
	path_seg_moveto_abs () : path_seg_lineto_base() {}
	path_seg_moveto_abs (double x, double y) : path_seg_lineto_base(x, y) {}
	path_seg_moveto_abs (const path_seg_lineto_base & p) : path_seg_lineto_base(p._x, p._y) {}

	virtual ushort_t type () const { return PATHSEG_MOVETO_ABS; }
	virtual pfs::string typeLetter () const { return _l1("M"); }
};

class path_seg_moveto_rel : public path_seg_lineto_base
{
public:
	path_seg_moveto_rel () : path_seg_lineto_base() {}
	path_seg_moveto_rel (double x, double y) : path_seg_lineto_base(x, y) {}
	path_seg_moveto_rel (const path_seg_lineto_base & p) : path_seg_lineto_base(p._x, p._y) {}

	virtual ushort_t type () const { return PATHSEG_MOVETO_REL; }
	virtual pfs::string typeLetter () const { return _l1("m"); }
};

class path_seg_lineto_abs : public path_seg_lineto_base
{
public:
	path_seg_lineto_abs () : path_seg_lineto_base() {}
	path_seg_lineto_abs (double x, double y) : path_seg_lineto_base(x, y) {}
	path_seg_lineto_abs (const path_seg_lineto_base & p) : path_seg_lineto_base(p._x, p._y) {}

	virtual ushort_t type () const { return PATHSEG_LINETO_ABS; }
	virtual pfs::string typeLetter () const { return _l1("L"); }
};

class path_seg_lineto_rel : public path_seg_lineto_base
{
public:
	path_seg_lineto_rel () : path_seg_lineto_base() {}
	path_seg_lineto_rel (double x, double y) : path_seg_lineto_base(x, y) {}
	path_seg_lineto_rel (const path_seg_lineto_base & p) : path_seg_lineto_base(p._x, p._y) {}

	virtual ushort_t type () const { return PATHSEG_LINETO_REL; }
	virtual pfs::string typeLetter () const { return _l1("l"); }
};

class path_seg_curveto_cubic_base : public path_seg
{
	friend class path_seg_curveto_cubic_abs;
	friend class path_seg_curveto_cubic_rel;

protected:
	double _x, _y, _x1, _y1, _x2, _y2;

public:
	path_seg_curveto_cubic_base ()
		: path_seg()
		, _x(0.0f), _y(0.0f)
		, _x1(0.0f), _y1(0.0f)
		, _x2(0.0f), _y2(0.0f) {}
	path_seg_curveto_cubic_base (double x, double y, double x1, double y1, double x2, double y2)
		: path_seg()
		, _x(x), _y(y)
		, _x1(x1), _y1(y1)
		, _x2(x2), _y2(y2) {}

	double x () const { return _x; }
	double y () const { return _y; }
	double x1 () const { return _x1; }
	double y1 () const { return _y1; }
	double x2 () const { return _x2; }
	double y2 () const { return _y2; }

	void setX (double x) { _x = x; }
	void setY (double y) { _y = y; }
	void setX1 (double x) { _x1 = x; }
	void setY1 (double y) { _y1 = y; }
	void setX2 (double x) { _x2 = x; }
	void setY2 (double y) { _y2 = y; }
};

class path_seg_curveto_cubic_abs : public path_seg_curveto_cubic_base
{
public:
	path_seg_curveto_cubic_abs () : path_seg_curveto_cubic_base() {}
	path_seg_curveto_cubic_abs (double x, double y, double x1, double y1, double x2, double y2)
		: path_seg_curveto_cubic_base(x, y, x1, y1, x2, y2) {}
	path_seg_curveto_cubic_abs (const path_seg_curveto_cubic_base & p)
		: path_seg_curveto_cubic_base(p._x, p._y, p._x1, p._y1, p._x2, p._y2) {}

	virtual ushort_t type () const { return PATHSEG_CURVETO_CUBIC_ABS; }
	virtual pfs::string typeLetter () const { return _l1("C"); }
};

class path_seg_curveto_cubic_rel : public path_seg_curveto_cubic_base
{
public:
	path_seg_curveto_cubic_rel () : path_seg_curveto_cubic_base() {}
	path_seg_curveto_cubic_rel (double x, double y, double x1, double y1, double x2, double y2)
		: path_seg_curveto_cubic_base(x, y, x1, y1, x2, y2) {}
	path_seg_curveto_cubic_rel (const path_seg_curveto_cubic_base & p)
		: path_seg_curveto_cubic_base(p._x, p._y, p._x1, p._y1, p._x2, p._y2) {}

	virtual ushort_t type () const { return PATHSEG_CURVETO_CUBIC_REL; }
	virtual pfs::string typeLetter () const { return _l1("c"); }
};


class path_seg_curveto_quadratic_base : public path_seg
{
	friend class path_seg_curveto_quadratic_abs;
	friend class path_seg_curveto_quadratic_rel;

protected:
	double _x, _y, _x1, _y1;

public:
	path_seg_curveto_quadratic_base ()
		: path_seg()
		, _x(0.0f), _y(0.0f)
		, _x1(0.0f), _y1(0.0f) {}
	path_seg_curveto_quadratic_base (double x, double y, double x1, double y1)
		: path_seg()
		, _x(x), _y(y)
		, _x1(x1), _y1(y1) {}

	double x () const { return _x; }
	double y () const { return _y; }
	double x1 () const { return _x1; }
	double y1 () const { return _y1; }

	void setX (double x) { _x = x; }
	void setY (double y) { _y = y; }
	void setX1 (double x) { _x1 = x; }
	void setY1 (double y) { _y1 = y; }
};

class path_seg_curveto_quadratic_abs : public path_seg_curveto_quadratic_base
{
public:
	path_seg_curveto_quadratic_abs () : path_seg_curveto_quadratic_base() {}
	path_seg_curveto_quadratic_abs (double x, double y, double x1, double y1)
		: path_seg_curveto_quadratic_base(x, y, x1, y1) {}
	path_seg_curveto_quadratic_abs (const path_seg_curveto_quadratic_base & p)
		: path_seg_curveto_quadratic_base(p._x, p._y, p._x1, p._y1) {}

	virtual ushort_t type () const { return PATHSEG_CURVETO_QUADRATIC_ABS; }
	virtual pfs::string typeLetter () const { return _l1("Q"); }
};

class path_seg_curveto_quadratic_rel : public path_seg_curveto_quadratic_base
{
public:
	path_seg_curveto_quadratic_rel () : path_seg_curveto_quadratic_base() {}
	path_seg_curveto_quadratic_rel (double x, double y, double x1, double y1)
		: path_seg_curveto_quadratic_base(x, y, x1, y1) {}
	path_seg_curveto_quadratic_rel (const path_seg_curveto_quadratic_base & p)
		: path_seg_curveto_quadratic_base(p._x, p._y, p._x1, p._y1) {}

	virtual ushort_t type () const { return PATHSEG_CURVETO_QUADRATIC_REL; }
	virtual pfs::string typeLetter () const { return _l1("Q"); }
};


class path_seg_arc_base : public path_seg
{
	friend class path_seg_arc_abs;
	friend class path_seg_arc_rel;

protected:
	double _x, _y, _rx, _ry, _angle;
	bool _largeArcFlag;
	bool _sweepFlag;

public:
	path_seg_arc_base ()
		: path_seg()
		, _x(0.0f), _y(0.0f)
		, _rx(0.0f), _ry(0.0f), _angle(0.0f)
		, _largeArcFlag (false), _sweepFlag(false) {}
	path_seg_arc_base (double x, double y, double r1, double r2, double angle, bool largeArcFlag, bool sweepFlag)
		: path_seg()
	, _x(x), _y(y)
	, _rx(r1), _ry(r2), _angle(angle)
	, _largeArcFlag (largeArcFlag), _sweepFlag(sweepFlag) {}

	double x () const { return _x; }
	double y () const { return _y; }
	double rx () const { return _rx; }
	double ry () const { return _ry; }
	double angle () const { return _angle; }
	bool largeArcFlag () const { return _largeArcFlag; }
	bool sweepFlag () const { return _sweepFlag; }

	void setX (double x) { _x = x; }
	void setY (double y) { _y = y; }
	void setRx (double r) { _rx = r; }
	void setRy (double r) { _ry = r; }
	void setAngle (double a) { _angle = a; }
	void setFlags (bool largeArcFlag, bool sweepFlag)
	{
		_largeArcFlag = largeArcFlag;
		_sweepFlag = sweepFlag;
	}
};

class path_seg_arc_abs : public path_seg_arc_base
{
public:
	path_seg_arc_abs () : path_seg_arc_base() {}
	path_seg_arc_abs (double x, double y, double r1, double r2, double angle, bool largeArcFlag, bool sweepFlag)
		: path_seg_arc_base(x, y, r1, r2, angle, largeArcFlag, sweepFlag) {}
	path_seg_arc_abs (const path_seg_arc_base & p)
		: path_seg_arc_base(p._x, p._y, p._rx, p._ry, p._angle, p._largeArcFlag, p._sweepFlag) {}

	virtual ushort_t type () const { return PATHSEG_ARC_ABS; }
	virtual pfs::string typeLetter () const { return _l1("A"); }
};

class path_seg_arc_rel : public path_seg_arc_base
{
public:
	path_seg_arc_rel () : path_seg_arc_base() {}
	path_seg_arc_rel (double x, double y, double r1, double r2, double angle, bool largeArcFlag, bool sweepFlag)
		: path_seg_arc_base(x, y, r1, r2, angle, largeArcFlag, sweepFlag) {}
	path_seg_arc_rel (const path_seg_arc_base & p)
		: path_seg_arc_base(p._x, p._y, p._rx, p._ry, p._angle, p._largeArcFlag, p._sweepFlag) {}

	virtual ushort_t type () const { return PATHSEG_ARC_REL; }
	virtual pfs::string typeLetter () const { return _l1("a"); }
};


class path_seg_lineto_dir_base : public path_seg
{
	friend class path_seg_lineto_horizontal_abs;
	friend class path_seg_lineto_horizontal_rel;
	friend class path_seg_lineto_vertical_abs;
	friend class path_seg_lineto_vertical_rel;

protected:
	double _xy;

public:
	path_seg_lineto_dir_base () : path_seg(), _xy(0.0f) {}
	path_seg_lineto_dir_base (double xy) : path_seg(), _xy(xy) {}

	double x () const { return _xy; }
	double y () const { return _xy; }

	void setX (double x) { _xy = x; }
	void setY (double y) { _xy = y; }
};

class path_seg_lineto_horizontal_abs : public path_seg_lineto_dir_base
{
public:
	path_seg_lineto_horizontal_abs () : path_seg_lineto_dir_base() {}
	path_seg_lineto_horizontal_abs (double x) : path_seg_lineto_dir_base(x) {}
	path_seg_lineto_horizontal_abs (const path_seg_lineto_dir_base & p) : path_seg_lineto_dir_base(p._xy) {}

	virtual ushort_t type () const { return PATHSEG_LINETO_HORIZONTAL_ABS; }
	virtual pfs::string typeLetter () const { return _l1("H"); }
};

class path_seg_lineto_horizontal_rel : public path_seg_lineto_dir_base
{
public:
	path_seg_lineto_horizontal_rel () : path_seg_lineto_dir_base() {}
	path_seg_lineto_horizontal_rel (double x) : path_seg_lineto_dir_base(x) {}
	path_seg_lineto_horizontal_rel (const path_seg_lineto_dir_base & p) : path_seg_lineto_dir_base(p._xy) {}

	virtual ushort_t type () const { return PATHSEG_LINETO_HORIZONTAL_REL; }
	virtual pfs::string typeLetter () const { return _l1("h"); }
};

class path_seg_lineto_vertical_abs : public path_seg_lineto_dir_base
{
public:
	path_seg_lineto_vertical_abs () : path_seg_lineto_dir_base() {}
	path_seg_lineto_vertical_abs (double x) : path_seg_lineto_dir_base(x) {}
	path_seg_lineto_vertical_abs (const path_seg_lineto_dir_base & p) : path_seg_lineto_dir_base(p._xy) {}

	virtual ushort_t type () const { return PATHSEG_LINETO_VERTICAL_ABS; }
	virtual pfs::string typeLetter () const { return _l1("V"); }
};

class path_seg_lineto_vertical_rel : public path_seg_lineto_dir_base
{
public:
	path_seg_lineto_vertical_rel () : path_seg_lineto_dir_base() {}
	path_seg_lineto_vertical_rel (double x) : path_seg_lineto_dir_base(x) {}
	path_seg_lineto_vertical_rel (const path_seg_lineto_dir_base & p) : path_seg_lineto_dir_base(p._xy) {}

	virtual ushort_t type () const { return PATHSEG_LINETO_VERTICAL_REL; }
	virtual pfs::string typeLetter () const { return _l1("v"); }
};

class path_seg_curveto_cubic_smooth_base : public path_seg
{
	friend class path_seg_curveto_cubic_smooth_abs;
	friend class path_seg_curveto_cubic_smooth_rel;

protected:
	double _x, _y, _x2, _y2;

public:
	path_seg_curveto_cubic_smooth_base ()
		: path_seg()
		, _x(0.0f), _y(0.0f)
		, _x2(0.0f), _y2(0.0f) {}
	path_seg_curveto_cubic_smooth_base (double x, double y, double x2, double y2)
		: path_seg()
		, _x(x), _y(y)
		, _x2(x2), _y2(y2) {}

	double x () const { return _x; }
	double y () const { return _y; }
	double x2 () const { return _x2; }
	double y2 () const { return _y2; }

	void setX (double x) { _x = x; }
	void setY (double y) { _y = y; }
	void setX2 (double x) { _x2 = x; }
	void setY2 (double y) { _y2 = y; }
};

class path_seg_curveto_cubic_smooth_abs : public path_seg_curveto_cubic_smooth_base
{
public:
	path_seg_curveto_cubic_smooth_abs () : path_seg_curveto_cubic_smooth_base() {}
	path_seg_curveto_cubic_smooth_abs (double x, double y, double x2, double y2)
		: path_seg_curveto_cubic_smooth_base(x, y, x2, y2) {}
	path_seg_curveto_cubic_smooth_abs (const path_seg_curveto_cubic_smooth_base & p)
		: path_seg_curveto_cubic_smooth_base(p._x, p._y, p._x2, p._y2) {}

	virtual ushort_t type () const { return PATHSEG_CURVETO_CUBIC_SMOOTH_ABS; }
	virtual pfs::string typeLetter () const { return _l1("S"); }
};

class path_seg_curveto_cubic_smooth_rel : public path_seg_curveto_cubic_smooth_base
{
public:
	path_seg_curveto_cubic_smooth_rel () : path_seg_curveto_cubic_smooth_base() {}
	path_seg_curveto_cubic_smooth_rel (double x, double y, double x2, double y2)
		: path_seg_curveto_cubic_smooth_base(x, y, x2, y2) {}
	path_seg_curveto_cubic_smooth_rel (const path_seg_curveto_cubic_smooth_base & p)
		: path_seg_curveto_cubic_smooth_base(p._x, p._y, p._x2, p._y2) {}

	virtual ushort_t type () const { return PATHSEG_CURVETO_CUBIC_SMOOTH_REL; }
	virtual pfs::string typeLetter () const { return _l1("s"); }
};


class path_seg_curveto_quadratic_smooth_base : public path_seg
{
	friend class path_seg_curveto_quadratic_smooth_abs;
	friend class path_seg_curveto_quadratic_smooth_rel;

protected:
	double _x, _y;

public:
	path_seg_curveto_quadratic_smooth_base ()
		: path_seg()
		, _x(0.0f), _y(0.0f) {}
	path_seg_curveto_quadratic_smooth_base (double x, double y)
		: path_seg()
		, _x(x), _y(y) {}

	double x () const { return _x; }
	double y () const { return _y; }

	void setX (double x) { _x = x; }
	void setY (double y) { _y = y; }
};

class path_seg_curveto_quadratic_smooth_abs : public path_seg_curveto_quadratic_smooth_base
{
public:
	path_seg_curveto_quadratic_smooth_abs () : path_seg_curveto_quadratic_smooth_base() {}
	path_seg_curveto_quadratic_smooth_abs (double x, double y)
		: path_seg_curveto_quadratic_smooth_base(x, y) {}
	path_seg_curveto_quadratic_smooth_abs (const path_seg_curveto_quadratic_smooth_base & p)
		: path_seg_curveto_quadratic_smooth_base(p._x, p._y) {}

	virtual ushort_t type () const { return PATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS; }
	virtual pfs::string typeLetter () const { return _l1("T"); }
};

class path_seg_curveto_quadratic_smooth_rel : public path_seg_curveto_quadratic_smooth_base
{
public:
	path_seg_curveto_quadratic_smooth_rel () : path_seg_curveto_quadratic_smooth_base() {}
	path_seg_curveto_quadratic_smooth_rel (double x, double y)
		: path_seg_curveto_quadratic_smooth_base(x, y) {}
	path_seg_curveto_quadratic_smooth_rel (const path_seg_curveto_quadratic_smooth_base & p)
		: path_seg_curveto_quadratic_smooth_base(p._x, p._y) {}

	virtual ushort_t type () const { return PATHSEG_CURVETO_QUADRATIC_SMOOTH_REL; }
	virtual pfs::string typeLetter () const { return _l1("t"); }
};

}} // cwt::svg

#endif /* __CWT_SVG_PATH_HPP__ */
