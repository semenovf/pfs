#include <cstdio>
#include <cstdlib>
#include <string>
#include "pfs/test/test.hpp"
#include "pfs/system_string.hpp"
#include "pfs/net/inet4_addr.hpp"
#include "inet_interface.hpp"

#include <iostream>

using pfs::net::inet4_addr;
using std::cout;
using std::cerr;
using std::endl;

#define UINT24_MAX 0x00FFFFFF

//static const char * prefix (uint32_t x, int base)
//{
//    return x == 0 ? ""
//           : base == 16
//                  ? "0x" : base == 8 ? "0" : "";
//}

static pfs::system_string build_inet_addr_str (int addrClass
        , uint32_t a
        , uint32_t b
        , uint32_t c
        , uint32_t d
        , int base)
{
	pfs::system_char_t buffer[64];

    if (addrClass == 1) {
    	switch (base) {
    	case 8:
    		PFS_SPRINTF(buffer, PFS_LITERAL("0%o"), a);
    		break;
    	case 10:
    		PFS_SPRINTF(buffer, PFS_LITERAL("%u"), a);
    		break;
    	case 16:
    		PFS_SPRINTF(buffer, PFS_LITERAL("0x%X"), a);
    		break;
    	}
    } else if (addrClass == 2) {
    	switch (base) {
    	case 8:
    		PFS_SPRINTF(buffer, PFS_LITERAL("0%03o.0%o"), a, b);
    		break;
    	case 10:
    		PFS_SPRINTF(buffer, PFS_LITERAL("%u.%u"), a, b);
    		break;
    	case 16:
    		PFS_SPRINTF(buffer, PFS_LITERAL("0x%02X.0x%X"), a, b);
    		break;
    	}
    } else if (addrClass == 3) {
    	switch (base) {
    	case 8:
    		PFS_SPRINTF(buffer, PFS_LITERAL("0%03o.0%03o.0%o"), a, b, c);
    		break;
    	case 10:
    		PFS_SPRINTF(buffer, PFS_LITERAL("%u.%u.%u"), a, b, c);
    		break;
    	case 16:
    		PFS_SPRINTF(buffer, PFS_LITERAL("0x%02X.0x%02X.0x%X"), a, b, c);
    		break;
    	}
    } else if (addrClass == 4) {
    	switch (base) {
    	case 8:
    		PFS_SPRINTF(buffer, PFS_LITERAL("0%03o.0%03o.0%03o.0%03o"), a, b, c, d);
    		break;
    	case 10:
    		PFS_SPRINTF(buffer, PFS_LITERAL("%u.%u.%u.%u"), a, b, c, d);
    		break;
    	case 16:
    		PFS_SPRINTF(buffer, PFS_LITERAL("0x%02X.0x%02X.0x%02X.0x%02X"), a, b, c, d);
    		break;
    	}
    }

    return pfs::system_string(buffer);
}


bool test_check_valid (int addrClass, int ntests)
{
    bool ok = true;

    for (int i = 0; ok && i < ntests; ++i) {
        uint32_t  a = 0, b = 0, c = 0, d = 0;

        if (addrClass == 4) {
            a = random() % pfs::numeric_limits<uint8_t>::max();
            b = random() % pfs::numeric_limits<uint8_t>::max();
            c = random() % pfs::numeric_limits<uint8_t>::max();
            d = random() % pfs::numeric_limits<uint8_t>::max();
        } else if (addrClass == 3) {
            a = random() % pfs::numeric_limits<uint8_t>::max();
            b = random() % pfs::numeric_limits<uint8_t>::max();
            c = random() % pfs::numeric_limits<uint16_t>::max();
        } else if (addrClass == 2) {
            a = random() % pfs::numeric_limits<uint8_t>::max();
            b = random() % UINT24_MAX;
        } else if (addrClass == 1) {
            a = random();
        } else {
            return false;
        }

        pfs::system_string addrDecStr = build_inet_addr_str(addrClass, a, b, c, d, 10);
        pfs::system_string addrOctStr = build_inet_addr_str(addrClass, a, b, c, d, 8);
        pfs::system_string addrHexStr = build_inet_addr_str(addrClass, a, b, c, d, 16);

        inet4_addr addrDec(addrDecStr);
        inet4_addr addrOct(addrOctStr);
        inet4_addr addrHex(addrHexStr);

//        cout << addrDecStr << ' ' << addrOctStr << ' ' << addrHexStr << endl;

        if (!addrDec) {
            cerr << "ERROR: unexpected invalid address in decimal format: "
                    << addrDecStr << " at " << i << " iteration" << endl;
            ok = false;
        }

        if (!addrOct) {
            cerr << "ERROR: unexpected invalid address in octal format: "
                    << addrOctStr << " at " << i << " iteration" << endl;
            ok = false;
        }

        if (!addrHex) {
            cerr << "ERROR: unexpected invalid address in hexadecimal format: "
                    << addrHexStr << " at " << i << " iteration" << endl;
            ok = false;
        }
    }
    return ok;
}

bool test_check_to_string (pfs::system_string const & format, int ntests)
{
    bool ok = true;

    int addrClass = format.starts_with(PFS_LITERAL("%a.%b.%c.%d"))
            ? 4
            : format.starts_with(PFS_LITERAL("%a.%b.%C"))
                  ? 3
                  : format.starts_with(PFS_LITERAL("%a.%B"))
                        ? 2
                        : format.starts_with(PFS_LITERAL("%A"))
                              ? 1 : 0;


    for (int i = 0; ok && i < ntests; ++i) {
        uint32_t  a, b, c, d;

        if (addrClass == 4) {
            a = random() % pfs::numeric_limits<uint8_t>::max();
            b = random() % pfs::numeric_limits<uint8_t>::max();
            c = random() % pfs::numeric_limits<uint8_t>::max();
            d = random() % pfs::numeric_limits<uint8_t>::max();
        } else if (addrClass == 3) {
            a = random() % pfs::numeric_limits<uint8_t>::max();
            b = random() % pfs::numeric_limits<uint8_t>::max();
            c = random() % pfs::numeric_limits<uint16_t>::max();
        } else if (addrClass == 2) {
            a = random() % pfs::numeric_limits<uint8_t>::max();
            b = random() % UINT24_MAX;
        } else if (addrClass == 1) {
            a = random();
        } else {
            return false;
        }

        pfs::system_string addrDecStr = build_inet_addr_str(addrClass, a, b, c, d, 10);
        pfs::system_string addrOctStr = build_inet_addr_str(addrClass, a, b, c, d, 8);
        pfs::system_string addrHexStr = build_inet_addr_str(addrClass, a, b, c, d, 16);

        inet4_addr addrDec(addrDecStr);
        inet4_addr addrOct(addrOctStr);
        inet4_addr addrHex(addrHexStr);

        pfs::system_string addrDecStr1 = pfs::to_string(addrDec, format, 10);
        pfs::system_string addrOctStr1 = pfs::to_string(addrOct, format, 8);
        pfs::system_string addrHexStr1 = pfs::to_string(addrHex, format, 16);

//        cout << addrDecStr << ' ' << addrOctStr << ' ' << addrHexStr << endl;
//        cout << addrDec.addrData() << endl;
//        cout << addrOct.addrData() << endl;
//        cout << addrHex.addrData() << endl;

        if (addrDecStr != addrDecStr1) {
            cerr << "ERROR: failed comparison in decimal format: " << addrDecStr << " != " << addrDecStr1 << endl;
            return false;
        }

        if (addrOctStr != addrOctStr1) {
            cerr << "ERROR: failed comparison in octal format: " << addrOctStr << " != " << addrOctStr1 << endl;
            return false;
        }

        if (addrHexStr != addrHexStr1) {
            cerr << "ERROR: failed comparison in hexadecimal format: " << addrHexStr << " != " << addrHexStr1 << endl;
            return false;
        }
    }

    return ok;
}

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(8);

    static const int CHECK_VALID_NTESTS  = 10000;
    static const int CHECK_STRING_NTESTS = 10000;
    static const pfs::system_char_t * addrClassesStr[] = {
          PFS_LITERAL("")
        , PFS_LITERAL("%A")
        , PFS_LITERAL("%a.%B")
        , PFS_LITERAL("%a.%b.%C")
        , PFS_LITERAL("%a.%b.%c.%d")
    };

    pfs::system_string msg;

    for (int i = 1; i < 5; ++i) {
        msg.clear();

        msg.append(pfs::to_string<pfs::system_string>(CHECK_VALID_NTESTS));
        msg.append(PFS_LITERAL(" random IP addresses ("));
        msg.append(addrClassesStr[i]);
        msg.append(PFS_LITERAL(" format)"));

        TEST_OK2(test_check_valid(i, CHECK_VALID_NTESTS), msg.c_str());
    }

    TEST_OK(test_check_to_string(pfs::system_string(PFS_LITERAL("%a.%b.%c.%d")), CHECK_STRING_NTESTS));
    TEST_OK(test_check_to_string(pfs::system_string(PFS_LITERAL("%a.%b.%C"))   , CHECK_STRING_NTESTS));
    TEST_OK(test_check_to_string(pfs::system_string(PFS_LITERAL("%a.%B"))      , CHECK_STRING_NTESTS));
    TEST_OK(test_check_to_string(pfs::system_string(PFS_LITERAL("%A"))         , CHECK_STRING_NTESTS));

    test_inet_interface();

    return END_TESTS;
}
