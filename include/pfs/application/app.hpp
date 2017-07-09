/*
 * app.hpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wladt
 */

#ifndef __PFS_APPLICATION_APP_HPP__
#define __PFS_APPLICATION_APP_HPP__

#include <pfs/application/dispatcher.hpp>

namespace pfs {
namespace application {

#if __cplusplus >= 201103L
enum class exit_status
{
#else
struct exit_status
{
    enum {
#endif
          success = 0
        , failure = -1
#if __cplusplus < 201103L                  
    };
#endif    
};

struct main_proc
{
    main_proc() {}
    virtual ~main_proc() {}
    virtual int operator () () { return 0; }
};

class app
{
//	string _program;

private:
#if __cplusplus >= 201103L
    app (app const & other) = delete;
    app & operator = (app const & other) = delete;
#else    
    app (app const & other);
    app & operator = (app const & other);
#endif
    
public:
    app () {}
    
	int exec ()
	{
		return main_proc()();
	}

	int exec (main_proc & mainProc)
	{
		return mainProc();
	}

	int exec (dispatcher & d);
    
    friend app & instance ();
};

inline app & instance ()
{
    static app a;
    return a;
}

}} // pfs::application

#endif /* __PFS_APPLICATION_APP_HPP__ */
