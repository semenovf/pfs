/* 
 * File:   i18n.hpp
 * Author: wladt
 *
 * Created on April 15, 2017, 4:35 PM
 */

#ifndef __PFS_I18N_HPP__
#define __PFS_I18N_HPP__

#ifdef _
#   error "Underscore macro already defined"
#endif

#if PFS_HAVE_GETTEXT

#   include <libintl.h>

// xgettext -C -c -k_ *.cpp

#   define _(x) gettext(x)

#else // ! PFS_HAVE_GETTEXT

#   define _(x) x

#endif // PFS_HAVE_GETTEXT

#endif /* __PFS_I18N_HPP__ */
