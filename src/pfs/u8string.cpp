#include "pfs/byte_string.hpp"
#include "pfs/string.hpp"
#include "pfs/traits/stdcxx/string.hpp"

#if HAVE_QT_CORE
#   include <QByteArray>
#   include "pfs/traits/qt/string.hpp"
#endif

namespace pfs {

typedef string<stdcxx::string>           stdcxx_string_type;
typedef string<stdcxx::string_reference> stdcxx_string_reference_type;

template <>
byte_string u8string<byte_string> (stdcxx_string_type const & s)
{
    return byte_string(s.native().begin(), s.native().end());
}

template <>
std::string u8string<std::string> (stdcxx_string_type const & s)
{
    return s.native();
}

template <>
byte_string u8string<byte_string> (stdcxx_string_reference_type const & s)
{
    return byte_string(s.native().begin(), s.native().end());
}

template <>
std::string u8string<std::string> (stdcxx_string_reference_type const & s)
{
    return s.native();
}

} // pfs

#if HAVE_QT_CORE
namespace pfs {

typedef string<qt::string>           qt_string_type;
typedef string<qt::string_reference> qt_string_reference_type;

template <>
byte_string u8string<byte_string> (qt_string_type const & s)
{
    QByteArray b = s.native().toUtf8();
    return byte_string(b.constData(), b.size());
}

template <>
std::string u8string<std::string> (qt_string_type const & s)
{
    return s.native().toStdString();
}

template <>
byte_string u8string<byte_string> (qt_string_reference_type const & s)
{
    QByteArray b = s.native().toUtf8();
    return byte_string(b.constData(), b.size());
}

template <>
std::string u8string<std::string> (qt_string_reference_type const & s)
{
    return s.native().toStdString();
}

template <>
QByteArray u8string<QByteArray> (qt_string_type const & s)
{
    QByteArray b = s.native().toUtf8();
    return QByteArray(b.constData(), b.size());
}

template <>
QByteArray u8string<QByteArray> (qt_string_reference_type const & s)
{
    QByteArray b = s.native().toUtf8();
    return QByteArray(b.constData(), b.size());
}

template <>
QByteArray u8string<QByteArray> (stdcxx_string_type const & s)
{
    return QByteArray(s.c_str(), s.size());
}

template <>
QByteArray u8string<QByteArray> (stdcxx_string_reference_type const & s)
{
    return QByteArray(s.c_str(), s.size());
}

} // pfs

#endif // HAVE_QT_CORE