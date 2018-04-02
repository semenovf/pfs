#include <cstring>
#include <pfs/test/test.hpp>
#include <pfs/string.hpp>
#include <pfs/traits/stdcxx/string.hpp>
#include <pfs/v2/app/modulus.hpp>

typedef pfs::string<pfs::stdcxx::string> string_t;
typedef pfs::app::modulus<string_t>      modulus_t;
typedef modulus_t::module                module_t;
typedef modulus_t::dispatcher            dispatcher_t;

class module : public module_t
{
public:
	module (dispatcher_t * pdisp) : module_t(pdisp)
	{}

	~module ()
	{}

	virtual bool on_start ()
	{
	    emitZeroArg   ();
	    emitOneArg    (true);
	    emitTwoArgs   (true, 'c');
	    emitThreeArgs (true, 'c', pfs::numeric_limits<short>::max());
	    emitFourArgs  (true, 'c', pfs::numeric_limits<short>::max(), pfs::numeric_limits<int>::max());
	    emitFiveArgs  (true, 'c', pfs::numeric_limits<short>::max(), pfs::numeric_limits<int>::max(), pfs::numeric_limits<int>::max());
	    emitSixArgs   (true, 'c', pfs::numeric_limits<short>::max(), pfs::numeric_limits<int>::max(), pfs::numeric_limits<int>::max(), "Hello, World!");
//	    emitSevenArgs (true, 'c', pfs::max_value<short>(), pfs::max_value<int>(), pfs::max_value<int>(), "Hello, World!", _u8("Hello, Unicode World!"));
//	    emitEightArgs (true, 'c', pfs::max_value<short>(), pfs::max_value<int>(), pfs::max_value<int>(), pfs::max_value<long>(), "Hello, World!", _u8("Hello, Unicode World!"));

        emit_quit();
		return true;
	}

	virtual bool on_finish ()
	{
		return true;
	}

	PFS_V2_MODULE_EMITTERS_INLINE_BEGIN
		  PFS_V2_MODULE_EMITTER(0, emitZeroArg)
		, PFS_V2_MODULE_EMITTER(1, emitOneArg)
		, PFS_V2_MODULE_EMITTER(2, emitTwoArgs)
		, PFS_V2_MODULE_EMITTER(3, emitThreeArgs)
		, PFS_V2_MODULE_EMITTER(4, emitFourArgs)
		, PFS_V2_MODULE_EMITTER(5, emitFiveArgs)
		, PFS_V2_MODULE_EMITTER(6, emitSixArgs)
	PFS_V2_MODULE_EMITTERS_END

	PFS_V2_MODULE_DETECTORS_INLINE_BEGIN
		  PFS_V2_MODULE_DETECTOR(0, module::onZeroArg)
		, PFS_V2_MODULE_DETECTOR(1, module::onOneArg)
		, PFS_V2_MODULE_DETECTOR(2, module::onTwoArgs)
		, PFS_V2_MODULE_DETECTOR(3, module::onThreeArgs)
		, PFS_V2_MODULE_DETECTOR(4, module::onFourArgs)
		, PFS_V2_MODULE_DETECTOR(5, module::onFiveArgs)
		, PFS_V2_MODULE_DETECTOR(6, module::onSixArgs)
	PFS_V2_MODULE_DETECTORS_END

public: /*signal*/
	pfs::signal0<> emitZeroArg;
	pfs::signal1<bool> emitOneArg;
	pfs::signal2<bool, char> emitTwoArgs;
	pfs::signal3<bool, char, short> emitThreeArgs;
	pfs::signal4<bool, char, short, int> emitFourArgs;
	pfs::signal5<bool, char, short, int, long> emitFiveArgs;
	pfs::signal6<bool, char, short, int, long, const char*> emitSixArgs;

public: /*slots*/
	void onZeroArg   ();
	void onOneArg    (bool ok);
	void onTwoArgs   (bool ok, char ch);
	void onThreeArgs (bool ok, char, short i);
	void onFourArgs  (bool ok, char, short, int i);
	void onFiveArgs  (bool ok, char, short, int, long i);
	void onSixArgs   (bool ok, char, short, int, long, const char*);
};

inline void module::onZeroArg ()
{
	TEST_OK2(true == true, "onZeroArg()");
}

inline void module::onOneArg  (bool ok)
{
	TEST_OK2(ok == true, "onOneArg(bool)");
}

inline void module::onTwoArgs (bool ok, char ch)
{
	TEST_OK2(ok == true && ch == 'c', "onTwoArgs(true, 'c')");
}

inline void module::onThreeArgs (bool ok, char, short i)
{
	TEST_OK2(ok == true && i == pfs::numeric_limits<short>::max(), "onThreeArgs(bool,..., PFS_SHORT_MAX)");
}

inline void module::onFourArgs (bool ok, char, short, int i)
{
	TEST_OK2(ok == true && i == pfs::numeric_limits<int>::max(), "onFourArgs(bool,..., PFS_INT_MAX)");
}

inline void module::onFiveArgs (bool ok, char, short, int, long i)
{
	TEST_OK2(ok == true && i == pfs::numeric_limits<int>::max(), "onFiveArgs(bool,... PFS_INT_MAX)");
}

inline void module::onSixArgs (bool ok, char, short, int, long, const char *hello)
{
	TEST_OK2(ok == true && strcmp("Hello, World!", hello) == 0, "onSixArgs(bool,...\"Hello, World!\")");
}

//inline void module::onSevenArgs (bool ok, char, short, int, long, const char*, const pfs::string & hello)
//{
//	TEST_OK2(ok == true && hello == pfs::string("Hello, Unicode World!"), "onSevenArgs(bool,..., pfs::string(\"Hello, Unicode World!\")");
//}
//
//inline void module::onEightArgs (bool ok, char, short, int, long, long long i, const char*, const pfs::string &)
//{
//	TEST_OK2(ok == true && i == pfs::max_value<long>(), "onEightArgs(bool,..., PFS_LONG_MAX,...)");
//}


static modulus_t::api_item_type API[] = {
	  { 0 , new modulus_t::sigslot_mapping0, "ZeroArg()" }
	, { 1 , new modulus_t::sigslot_mapping1<bool>, "OneArg(bool b)\n\t boolean value" }
	, { 2 , new modulus_t::sigslot_mapping2<bool, char>, "TwoArgs(bool b, char ch)" }
	, { 3 , new modulus_t::sigslot_mapping3<bool, char, short>, "ThreeArgs(bool b, char ch, short n)" }
	, { 4 , new modulus_t::sigslot_mapping4<bool, char, short, int>, "FourArgs description" }
	, { 5 , new modulus_t::sigslot_mapping5<bool, char, short, int, long>, "FiveArgs description" }
	, { 6 , new modulus_t::sigslot_mapping6<bool, char, short, int, long, const char*>, "SixArgs description" }
//	, { 7 , new pfs::sigslot_mapping7_t<bool, char, short, int, long, const char*, const pfs::string &>, _u8("SevenArgs description") }
//	, { 8 , new pfs::sigslot_mapping8_t<bool, char, short, int, long, long long, const char*, const pfs::string &>, _u8("EightArgs description") }
};

int main (int argc, char *argv[])
{
    PFS_UNUSED(argc);
    BEGIN_TESTS(11);

    dispatcher_t dispatcher(API, sizeof(API)/sizeof(API[0]));
    dispatcher.add_search_path(pfs::filesystem::path("."));

    //TEST_OK(dispatcher.register_module_for_name("module-for-test-app"));
    TEST_OK(dispatcher.register_local_module(new module(& dispatcher), "mod-local-for-test-app"));
    TEST_OK(!dispatcher.register_module_for_name("module-for-test-app-nonexistence"));

    //TEST_OK(dispatcher.count() == 2);
    TEST_OK(dispatcher.count() == 1);

    TEST_OK(dispatcher.exec() == 0);

    return END_TESTS;
}
