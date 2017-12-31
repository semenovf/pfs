#ifndef __PFS_BITS_COMPILER_H__
#define __PFS_BITS_COMPILER_H__

/*
   The compiler, must be one of: (CWT_CC_x)
     SYM      - Digital Mars C/C++ (used to be Symantec C++)
     MWERKS   - Metrowerks CodeWarrior
     MSVC     - Microsoft Visual C/C++, Intel C++ for Windows
     BOR      - Borland/Turbo C++
     WAT      - Watcom C++
     GNU      - GNU C++
     COMEAU   - Comeau C++
     EDG      - Edison Design Group C++
     OC       - CenterLine C++
     SUN      - Forte Developer, or Sun Studio C++
     MIPS     - MIPSpro C++
     DEC      - DEC C++
     HPACC    - HP aC++
     USLC     - SCO OUDK and UDK
     CDS      - Reliant C++
     KAI      - KAI C++
     INTEL    - Intel C++ for Linux, Intel C++ for Windows
     HIGHC    - MetaWare High C/C++
     PGI      - Portland Group C++
     GHS      - Green Hills Optimizing C++ Compilers
     GCCE     - GCCE (Symbian GCCE builds)
     RVCT     - ARM Realview Compiler Suite
     NOKIAX86 - Nokia x86 (Symbian WINSCW builds)
     CLANG    - C++ front-end for the LLVM compiler

   Should be sorted most to least authoritative.
*/

/* Symantec C++ is now Digital Mars */
#if defined(_MSC_VER)
#	define PFS_CC_MSVC 1
#	define PFS_CC_MSC 1
// see http://stackoverflow.com/questions/70013/how-to-detect-if-im-compiling-code-with-visual-studio-2008
//MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
//MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
//MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
//MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
//MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
//MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio 2003)
//MSVC++ 7.0  _MSC_VER == 1300
//MSVC++ 6.0  _MSC_VER == 1200
//MSVC++ 5.0  _MSC_VER == 1100
#	define PFS_CC_MSVC_VERSION _MSC_VER /* In Visual Studio 2010, _MSC_VER is defined as 1600 */
#	define PFS_CC_MSC_VERSION  _MSC_VER
/* Intel C++ disguising as Visual C++: the `using' keyword avoids warnings */
#	if defined(__INTEL_COMPILER)
#		define PFS_CC_INTEL 1
#		define PFS_CC_INTEL_VERSION _MSC_VER
#		define PFS_CC_INTEL_VERSION _MSC_VER
#       define __PFS_CC_HPREFIX__ intel
#   else
#       define __PFS_CC_HPREFIX__ msc
#	endif
#elif defined(__GNUC__)
#  	define PFS_CC_GNUC 1
#	define PFS_CC_GCC 1

// [Common Predefined Macros](https://gcc.gnu.org/onlinedocs/gcc-4.0.1/cpp/Common-Predefined-Macros.html)
#   define PFS_CC_GCC_VERSION \
        (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#   define PFS_CC_GNUC_VERSION PFS_CC_GCC_VERSION

#	if defined(__INTEL_COMPILER)
    	/* Intel C++ also masquerades as GCC 3.2.0 */
#		define PFS_CC_INTEL 1
#       define __PFS_CC_HPREFIX__ intel
#	elif defined(__clang__)
/* Clang also masquerades as GCC 4.2.1 */
#		define PFS_CC_CLANG 1
#       define __PFS_CC_HPREFIX__ clang
#   else
#       define __PFS_CC_HPREFIX__ gnuc
#	endif
#else
#   error "Unsupported C/C++ compiler"
#endif

#ifdef EXTERN_C_BEGIN
#   undef EXTERN_C_BEGIN
#endif

#ifdef __cplusplus
#   ifndef  EXTERN_C_BEGIN
#       define EXTERN_C_BEGIN extern "C" {
#   endif
#   define EXTERN_C_END }
#else
#   ifndef  EXTERN_C_BEGIN
#       define EXTERN_C_BEGIN
#   endif
#   define EXTERN_C_END
#endif

/* http://support.microsoft.com/default.aspx?scid=KB;EN-US;168958 */
#ifdef PFS_DLL_API
#   undef PFS_DLL_API
#endif

#if defined(_MSC_VER)
#   define PFS_DLL_API_EXPORT __declspec(dllexport)
#   define PFS_DLL_API_IMPORT __declspec(dllimport)
#   ifdef PFS_DLL_EXPORTS
#       define PFS_DLL_API PFS_DLL_API_EXPORT
#       define PFS_DLL_TEMPLATE
#   else
#       define PFS_DLL_API PFS_DLL_API_IMPORT
#       define PFS_DLL_TEMPLATE extern
#   endif
#else
#   define PFS_DLL_API_EXPORT
#   define PFS_DLL_API_IMPORT
#   define PFS_DLL_API
#   define PFS_DLL_TEMPLATE
#endif

#endif /* __PFS_BITS_COMPILER_H__ */

