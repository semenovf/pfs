#include "pfs/byte_string.hpp"
#include "pfs/string.hpp"
#include "pfs/traits/stdcxx/string.hpp"
#include "pfs/traits/qt/string.hpp"

namespace pfs {

typedef string<stdcxx::string>           stdcxx_string_type;
typedef string<stdcxx::string_reference> stdcxx_string_reference_type;

template <>
byte_string u8string<byte_string> (stdcxx_string_type const & s)
{
    return byte_string(s.native().begin(), s.native().cend());
}

template <>
byte_string u8string<byte_string> (stdcxx_string_reference_type const & s)
{
    return byte_string(s.native().begin(), s.native().cend());
}

}

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
byte_string u8string<byte_string> (qt_string_reference_type const & s)
{
    QByteArray b = s.native().toUtf8();
    return byte_string(b.constData(), b.size());
}

}