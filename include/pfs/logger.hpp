/* 
 * File:   logger.hpp
 * Author: wladt
 *
 * Created on July 9, 2017, 10:26 AM
 */

#ifndef __PFS_LOGGER_HPP__
#define __PFS_LOGGER_HPP__

#include <pfs/memory.hpp>
#include <pfs/sigslot.hpp>
#include <pfs/traits/sequence_container.hpp>
//#include <pfs/3dparty/spdlog/spdlog.h>

namespace pfs {
namespace logger {

#if __cplusplus >= 201103L
enum class priority
{
#else
struct priority
{
    enum type {
#endif
		  trace
		, debug
		, info
		, warn
		, error
		, fatal
        , critical = fatal
		, no_priority
		, count
#if __cplusplus < 201103L             
    } value;
#endif    
};

template <typename Logger>
class appender;

template <typename StringImplType
    , template <typename> class SequenceContainerImplType>
class logger
{
public:
    typedef string<StringImplType>                   string_type;

private:    
    typedef appender<logger>                         appender_type;
	typedef traits::sequence_container<
              unique_ptr<appender_type>
            , SequenceContainerImplType>             appender_sequence;
	typedef signal2<priority, string_type const &>   emitter_type;

private:
	struct data_t
	{
		priority          _priority;
		appender_sequence _appenders;
		emitter_type      _emitters[static_cast<size_t>(priority::count)];
	};

	unique_ptr<data_t> _d;

public:
	logger ()
		: _d(make_unique<data_t>())
	{
		_d->_priority = priority::trace;
	}

	~logger ()
	{
		if (_d) {
			clear();
			delete _d;
		}
	}

	void clear ();

    void swap (logger & other)
    {
        _d.swap(other._d);
    }

	template <typename Appender>
	appender_type & add_appender ()
	{
		_d->_appenders.emplace_back(make_unique<Appender>());
		return *_d->_appenders.last();
	}

    // TODO Implement add_appender using C++ variadic templates 
//#if __cplusplus < 201103L    
	template <typename Appender, typename Arg1>
	appender_type & add_appender (Arg1 a1)
	{
		_d->_appenders.emplace_back(make_unique<Appender>(a1));
		return *_d->_appenders.last();
	}

	template <typename Appender, typename Arg1, typename Arg2>
	appender_type & add_appender (Arg1 a1, Arg2 a2)
	{
		_d->_appenders.emplace_back(make_unique<Appender>(a1, a2));
		return *_d->_appenders.last();
	}

	template <typename Appender, typename Arg1, typename Arg2, typename Arg3>
	appender_type & add_appender (Arg1 a1, Arg2 a2, Arg3 a3)
	{
		_d->_appenders.emplace_back(make_unique<Appender>(a1, a2, a3));
		return *_d->_appenders.last();
	}

	template <typename Appender, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	appender_type & add_appender (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
		_d->_appenders.emplace_back(make_unique<Appender>(a1, a2, a3, a4));
		return *_d->_appenders.last();
	}

	template <typename Appender, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	appender_type & add_appender (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
		_d->_appenders.emplace_back(make_unique<Appender>(a1, a2, a3, a4, a5));
		return *_d->_appenders.last();
	}

	template <typename Appender, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	appender_type & add_appender (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
	{
		_d->_appenders.emplace_back(make_unique<Appender>(a1, a2, a3, a4, a5, a6));
		return *_d->_appenders.last();
	}

	template <typename Appender, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	appender_type & add_appender (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7)
	{
		_d->_appenders.emplace_back(make_unique<Appender>(a1, a2, a3, a4, a5, a6, a7));
		return *_d->_appenders.last();
	}

	template <typename Appender, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	appender_type & add_appender (Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8)
	{
		_d->_appenders.emplace_back(make_unique<Appender>(a1, a2, a3, a4, a5, a6, a7, a8));
		return *_d->_appenders.last();
	}
//#else
//#	error "Need to implement `add_appender` using variadic templates"
//#endif

	void print (priority level, string_type const & msg)
	{
		if (level >= _d->_priority and level != priority::no_priority)
			_d->_emitters[level](level, msg);
	}

	void connect (priority level, appender_type & ap);
	void connect (appender_type & ap);

	void disconnect (int level);
	void disconnect_all ();

	void set_priority (priority pri)
	{
		_d->_priority = pri;
	}

	int get_priority ()
	{
		return _d->_priority;
	}

	void trace (string_type const & text)
	{
		print(priority::trace, text);
	}

	void debug (string_type const & text)
	{
		print(priority::debug, text);
	}

	void info  (string_type const & text)
	{
		print(priority::info, text);
	}

	void warn  (string_type const & text)
	{
		print(priority::warn, text);
	}

	void error (string_type const & text)
	{
		print(priority::error, text);
	}
    
	void critical (string_type const & text)
	{
		print(priority::critical, text);
		abort();
	}

	void fatal (string_type const & text)
	{
		critical(text);
	}

//	static logger & default_logger ();
};

template <typename Logger>
class appender : public has_slots<>
{
	template <typename String
        , template <typename> class SequenceContainer>
    friend class logger;

public:
    typedef typename Logger::string_type string_type;

protected:
	string_type _pattern;
    string_type _priority_text[static_cast<size_t>(priority::count) - 1]; // excluding no_priority

protected:
	virtual void print (string_type const & msg) = 0;

	void print_helper (priority pri, string_type const & msg)
	{
		print(_pattern.empty()
				? msg
				: patternify(this, pri, _pattern, msg));
	}
    
    void init ();

	static string_type patternify (appender * ader
        , priority pri
        , string_type const & pattern
        , string_type const & text);

public:
	appender ()
		: _pattern("%m")
	{
        init();
    }

	appender (string_type const & pattern)
		: _pattern(pattern)
	{
        init();
    }

	virtual ~appender ()
	{}

	string_type pattern () const
	{
		return _pattern;
	}

	void set_pattern (string_type const & pattern)
	{
		_pattern = pattern;
	}
    
    string_type priority_text (priority pri) const
    {
        return _priority_text[pri];
    }
    
    void set_priority_text (priority pri, string_type const & text) 
    {
        _priority_text[pri] = text;
    }
};

#if __COMMENT__

class stdout_appender : public logger_appender
{
public:
	stdout_appender ()
		: logger_appender()
	{}

protected:
	virtual void print (pfs::string const & msg) override
	{
		std::cout << msg << std::endl;
	}
};

class stderr_appender : public logger_appender
{
public:
	stderr_appender ()
		: logger_appender()
	{}

protected:
	virtual void print (string_type const & msg) override
	{
		std::cerr << msg << std::endl;
	}
};

class stringlist_appender : public logger_appender
{
	pfs::stringlist _strings;

public:
	stringlist_appender () {}
	const pfs::stringlist & data () const { return _strings; }
	pfs::stringlist & data () { return _strings; }

protected:
	virtual void print (string_type const & msg) override
	{
		_strings.push_back(msg);
	}
};

inline void logger::connect (int level, appender & ap)
{
	PFS_ASSERT(level >= 0 && level < priority_count);
	_d->_emitters[level].connect(& ap, & logger_appender::print_helper);
}

inline void logger::disconnect (int level)
{
	PFS_ASSERT(level >= 0 && level < priority_count);
	_d->_emitters[level].disconnect_all();
}

inline void logger::disconnect_all ()
{
	for (int i = 0; i < priority_count; ++i)
		_d->_emitters[i].disconnect_all();
}

#endif // __COMMENT__

template <typename StringT>
inline void trace (StringT const & text);

template <typename StringT>
inline void debug (StringT const & text);

template <typename StringT>
inline void info (StringT const & text);

template <typename StringT>
inline void warn (StringT const & text);

template <typename StringT>
inline void error (StringT const & text);

template <typename StringT>
inline void fatal (StringT const & text);

}} // pfs::logger

#endif /* __PFS_LOGGER_HPP__ */
