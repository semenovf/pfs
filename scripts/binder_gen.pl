#!/usr/bin/env perl

sub binder_def
{
    defined $_[0] or die "Expected number of arguments";
    my $nargs = $_[0];
    my $isMethodBinder = (defined $_[1] ? $_[1] : 0);

    my $binderName = $isMethodBinder
        ? "binder_method${nargs}"
        : "binder_function${nargs}";

#    my $binderName = $isMethodBinder
#       ? "binder_method"
#       : "binder";

    my $r = [];

    my $s1 = $isMethodBinder 
        ? 'template <typename Class, typename ReturnType' 
        : 'template <typename ReturnType';

    for (my $i = 1; $i <= $nargs; ++$i) {
        $s1 .= ", typename Arg${i}";
    }

    $s1 .= '>';

    push @$r, $s1;

    push @$r, $isMethodBinder 
        ? "class ${binderName} : public binder_method_base<Class, ReturnType>"
        : "class ${binderName} : public binder_function_base<ReturnType>";

    push @$r, '{';

    for (my $i = 1; $i <= $nargs; ++$i) {
        push @$r, "    typename binder_type_trait<Arg${i}>::type _a${i};";
    }

    push @$r, '';
    push @$r, 'public:';
    push @$r, $isMethodBinder 
        ? '    typedef binder_method_base<Class, ReturnType> base_class;'
        : '    typedef binder_function_base<ReturnType> base_class;';
    push @$r, '    typedef ReturnType return_type;';

    my $s2 = $isMethodBinder
        ? '    typedef return_type (Class::* funcptr_type) ('
        : '    typedef return_type (* funcptr_type) (';

    if ($nargs > 0) {
        $s2 .= 'Arg1';
    }

    for (my $i = 2; $i <= $nargs; ++$i) {
        $s2 .= ", Arg${i}";
    }

    $s2 .= ');';

    push @$r, $s2;
    push @$r, '';

    my $s3 = $isMethodBinder 
        ? "    $binderName (funcptr_type f, Class * p"
        : "    $binderName (funcptr_type f";

    for (my $i = 1; $i <= $nargs; ++$i) {
        $s3 .= ", Arg${i} a${i}";
    }

    $s3 .= ')';

    push @$r, $s3;
    push @$r, "        : base_class(sizeof($binderName)";

    push @$r, $isMethodBinder 
        ? '                , reinterpret_cast<typename base_class::funcptr_type>(f), p)'
        : '                , reinterpret_cast<typename base_class::funcptr_type>(f))';

    for (my $i = 1; $i <= $nargs; ++$i) {
        push @$r, "        , _a${i}(a${i})";
    }

    push @$r, '    {}';

    push @$r, '';
    push @$r, "    virtual ~${binderName} () {}";
    push @$r, '';
    push @$r, '    virtual return_type operator () () const';
    push @$r, '    {';

    push @$r, '        funcptr_type f = reinterpret_cast<funcptr_type>(this->_f);';
    my $s4 = '';

    if ($isMethodBinder) {
        $s4 .= '        return (base_class::_p->*f)(';
    } else {
        $s4 .= '        return f(';
    }

    if ($nargs > 0) {
        $s4 .= '_a1';
    }

    for (my $i = 2; $i <= $nargs; ++$i) {
        $s4 .= ", _a${i}";
    }

    $s4 .= ');';

    push @$r, $s4;

    push @$r, '    }';

    push @$r, '';
    push @$r, '//protected:';
    push @$r, '//    virtual void * placement_copy (void * ptr) const';
    push @$r, '//    {';
    push @$r, "//        return (new (ptr) ${binderName}(*this)) + 1;";
    push @$r, '//    }';
    push @$r, '};';
    push @$r, '';

    return $r;
}

sub binder_def_signal
{
    defined $_[0] or die "Expected number of arguments";
    my $nargs = $_[0];
    my $r = [];

    if ($nargs == 0) {
        push @$r, 'class binder_signal0 : public binder_signal_base';
    } else {
        my $s = 'template <typename Arg1';

        for (my $i = 2; $i <= $nargs; ++$i) {
            $s .= ", typename Arg${i}";
        }

        $s .= '>';

        push @$r, $s;

        $s = "class binder_signal${nargs} : public binder_signal_base";
        push @$r, $s;
    }

    push @$r, '{';

    for (my $i = 1; $i <= $nargs; ++$i) {
        push @$r, "    typename binder_type_trait<Arg${i}>::type _a${i};";
    }

    push @$r, '';
    push @$r, 'public:';
    push @$r, '    typedef binder_signal_base base_class;';

    if ($nargs == 0) {
        push @$r, '    typedef signal0<> * funcptr_type;'
    } else {
        my $s = "    typedef signal${nargs}<Arg1";
        for (my $i = 2; $i <= $nargs; ++$i) {
            $s .= ", Arg${i}";
        }
        $s .= '> * funcptr_type;';
        push @$r, $s;
    }

    push @$r, '';

    my $s = "    binder_signal${nargs} (funcptr_type f";
    for (my $i = 1; $i <= $nargs; ++$i) {
        $s .= ", Arg${i} a${i}";
    }
    $s .= ')';
    push @$r, $s;

    push @$r, "        : base_class(sizeof(binder_signal${nargs}), reinterpret_cast<typename base_class::funcptr_type>(f))";

    for (my $i = 1; $i <= $nargs; ++$i) {
        push @$r, "        , _a${i}(a${i})";
    }
    push @$r, '    {}';

    push @$r, '';
    push @$r, "    virtual ~binder_signal${nargs} () {}";
    push @$r, '';
    push @$r, '    virtual void operator () () const';
    push @$r, '    {';
    push @$r, '        funcptr_type f = reinterpret_cast<funcptr_type>(this->_f);';

    $s = '        return (*f)(';
    if ($nargs > 0) {
        $s .= "_a1";
    }
    for (my $i = 2; $i <= $nargs; ++$i) {
        $s .= ", _a${i}";
    }
    $s .= ');';
    push @$r, $s;
    push @$r, '    }';

    push @$r, '};';
    push @$r, '';

    return $r;
}

my ($sec
    , $min
    , $hour
    , $mday
    , $mon
    , $year
    , $wday
    , $yday
    , $isdst) = localtime(time);
my $now = sprintf("%04d-%02d-%02d %02d:%02d:%02d"
    , $year + 1900
    , $mon + 1
    , $mday, $hour, $min, $sec);

my $thisScriptName = $0;
my $numberOfParms = 8;

if (defined $ARGV[0]) {
    $numberOfParms = 0 + $ARGV[0];
}

my $header = <<"END_OF_CODE";
/* AUTOMATICALLY GENERATED BY '${thisScriptName}' on ${now} */

#pragma once
#include <pfs/sigslot.hpp>

namespace pfs {

template <typename T>
struct binder_type_trait { typedef T type; };

template <typename T>
struct binder_type_trait<const T &> { typedef T type; };

template <typename ReturnType>
class binder_base
{
public:
    typedef ReturnType return_type;

protected:
    size_t _size;

    binder_base (size_t size)
        : _size(size)
    {}

//    virtual void * placement_copy (void * ptr) const = 0;

public:
    virtual ~binder_base () {}

    size_t size () const
    {
        return _size;
    }

    virtual return_type operator () () const = 0;

//    void * pcopy (void * ptr)
//    {
//        return placement_copy(ptr);
//    }

//    static void * move (void * dest, void * src, size_t n);
};

//template <typename ReturnType>
//void * binder_base<ReturnType>::move (void * dest, void * src, size_t n)
//{
//    void * p = dest;
//
//    while (n) {
//        binder_base * bb = static_cast<binder_base *>(src);
//
//        PFS_ASSERT(n >= bb->size());
//        n -= bb->size();
//        src = static_cast<char *>(src) + bb->size();
//
//        p = bb->pcopy(p);
//        bb->~binder_base();
//    }
//
//    return dest;
//}

template <typename ReturnType>
class binder_function_base : public binder_base<ReturnType>
{
public:
    typedef ReturnType return_type;
    typedef return_type (* funcptr_type) ();

protected:
    funcptr_type _f;

    binder_function_base (size_t size, funcptr_type f)
        : binder_base<ReturnType>(size)
        , _f(f)
    {}

public:
    virtual ~binder_function_base () {}
};

template <typename Class, typename ReturnType>
class binder_method_base : public binder_base<ReturnType>
{
public:
    typedef ReturnType return_type;
    typedef return_type (Class::* funcptr_type) ();

protected:
    Class * _p;
    funcptr_type _f;

    binder_method_base (size_t size, funcptr_type f, Class * p)
        : binder_base<ReturnType>(size)
        , _p(p)
        , _f(f)
    { }

public:
    virtual ~binder_method_base () {}
};

//class binder_signal_base : public binder_base<void>
//{
//public:
//    typedef void return_type;
//    typedef signal1<void *> * funcptr_type;
//
//protected:
//    funcptr_type _f;
//
//    binder_signal_base (size_t size, funcptr_type f)
//        : binder_base<void>(size)
//        , _f(f)
//    { }
//
//public:
//    virtual ~binder_signal_base () {}
//};

END_OF_CODE

my $footer = <<"END_OF_CODE";

} // pfs
END_OF_CODE

print $header, "\n";

for (my $i = 0; $i <= $numberOfParms; ++$i) {
    my $lines = binder_def($i, 0);
    print join "\n", @$lines;
    print "\n";
} 

# Generate binder for call class method
#
for (my $i = 0; $i <= $numberOfParms; ++$i) {
    my $lines = binder_def($i, 1);
    print join "\n", @$lines;
    print "\n";
} 

# Generate binder for signals
#
for (my $i = 0; $i <= $numberOfParms; ++$i) {
    my $lines = binder_def_signal($i);
    print join "\n", @$lines;
    print "\n";
} 

print $footer, "\n";

0;
