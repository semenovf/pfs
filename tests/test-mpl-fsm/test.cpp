/**
 * @file   fsm-char.c
 * @author wladt
 * @date   Oct 10, 2012 11:40:55 AM
 *
 * @brief
 */


#include <pfs/test/test.hpp>
#include <pfs/string.hpp>
#include <pfs/fsm/fsm.hpp>
#include <pfs/fsm/aliases.hpp>

#include <iostream>

using std::cout;
using std::endl;

typedef pfs::traits::string<char>           sequence_type;
typedef pfs::fsm::fsm<sequence_type>        fsm_type;
typedef pfs::fsm::transition<sequence_type> transition_type;
typedef fsm_type::result_type               result_type;
typedef pfs::fsm::m<sequence_type>          M;

static int const FSM_NORMAL = pfs::fsm::normal_status;
static int const FSM_ACCEPT = pfs::fsm::accept_status;
static int const FSM_REJECT = pfs::fsm::reject_status;

/* DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */
static sequence_type const _DIGITS("0123456789");
static sequence_type const _HEXDIGITS("ABCDEFabcdef");

static transition_type HEXDIG_FSM[] = {
	  {-1, 1, M::chr(_DIGITS).m    , FSM_ACCEPT, 0, 0 }
    , {-1,-1, M::chr(_HEXDIGITS).m , FSM_ACCEPT, 0, 0 }
};

static void test_alternatives_simple ()
{
    ADD_TESTS(4);
            
	sequence_type const hexdig("F");
	sequence_type const digit("9");
	sequence_type const notdigit("w");
    
	fsm_type fsm(HEXDIG_FSM, 0);

	TEST_FAIL(fsm.exec(0, hexdig.cbegin(), hexdig.cbegin()).first == false);

	sequence_type::const_iterator it_end;
	result_type r;

	it_end = hexdig.cbegin();
	pfs::advance(it_end, 1);
	r = fsm.exec(0, hexdig.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(hexdig.cbegin(), r.second) == 1);

	it_end = digit.cbegin();
	pfs::advance(it_end, 1);
	r = fsm.exec(0, digit.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(digit.cbegin(), r.second) == 1);

	it_end = notdigit.cbegin();
	pfs::advance(it_end, 1);
	r = fsm.exec(0, notdigit.cbegin(), it_end);
	TEST_FAIL(!r.first && r.second == it_end);
}

/* 0*DIGIT */
static transition_type decimal0more_fsm[] = {
	  { 0, 1, M::chr(_DIGITS).m, FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::nothing().m   , FSM_ACCEPT, 0, 0 }
};

static void test_repetition_0more ()
{
    ADD_TESTS(11);
    
	typedef pfs::fsm::fsm<pfs::string>::result_type result_type;

	sequence_type const dec("1972");
	sequence_type const notdec("x1972");
    
	fsm_type fsm(decimal0more_fsm, 0);

	sequence_type::const_iterator it_end;
	result_type r;

	it_end = dec.cbegin();
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.cbegin(), r.second) == 0);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.cbegin(), r.second) == 1);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.cbegin(), r.second) == 2);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.cbegin(), r.second) == 3);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.cbegin(), r.second) == 4);

	it_end = notdec.cbegin();
	r = fsm.exec(0, notdec.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(notdec.cbegin(), r.second) == 0);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, notdec.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(notdec.cbegin(), r.second) == 0);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, notdec.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(notdec.cbegin(), r.second) == 0);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, notdec.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(notdec.cbegin(), r.second) == 0);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, notdec.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(notdec.cbegin(), r.second) == 0);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, notdec.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(notdec.cbegin(), r.second) == 0);
}

/* 1*DIGIT */
static transition_type decimal1more_fsm[] = {
	  { 0,-1, M::chr(_DIGITS).m, FSM_ACCEPT, 0, 0 }
};

/* 2*DIGIT */
static transition_type decimal2more_fsm[] = {
	  { 1,-1, M::chr(_DIGITS).m, FSM_NORMAL, 0, 0 }
	, { 1,-1, M::chr(_DIGITS).m, FSM_ACCEPT, 0, 0 }
};


/* 1*HEXDIG_FSM */
static transition_type hex_fsm[] = {
      { 0,-1, M::tr(HEXDIG_FSM).m, FSM_ACCEPT, 0, 0 }
};

static void test_repetition_1or2more ()
{
    ADD_TESTS(23);
    
	sequence_type const dec("1972");
	sequence_type const notdec("x1972");
	sequence_type const hex("BEAF");
	sequence_type const nothex("BEAR");

	fsm_type fsm(decimal1more_fsm, 0);

	sequence_type::const_iterator it_end;
	result_type r;

	it_end = dec.cbegin();
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(!r.first && r.second == it_end);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.cbegin(), r.second) == 1);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.cbegin(), r.second) == 2);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.cbegin(), r.second) == 3);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.cbegin(), r.second) == 4);

	it_end = notdec.cbegin();
	r = fsm.exec(0, notdec.cbegin(), it_end);
	TEST_FAIL(!r.first);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, notdec.cbegin(), it_end);
	TEST_FAIL(!r.first);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, notdec.cbegin(), it_end);
	TEST_FAIL(!r.first);

	fsm.set_transition_table(decimal2more_fsm);

	it_end = dec.cbegin();
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(!r.first);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(!r.first);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.cbegin(), r.second) == 2);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.cbegin(), r.second) == 3);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.cbegin(), r.second) == 4);

	fsm.set_transition_table(hex_fsm);

	it_end = hex.cbegin();
	r = fsm.exec(0, hex.cbegin(), it_end);
	TEST_FAIL(!r.first);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, hex.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(hex.cbegin(), r.second) == 1);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, hex.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(hex.cbegin(), r.second) == 2);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, hex.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(hex.cbegin(), r.second) == 3);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, hex.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(hex.cbegin(), r.second) == 4);

	it_end = nothex.cbegin();
	r = fsm.exec(0, nothex.cbegin(), it_end);
	TEST_FAIL(!r.first);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, nothex.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(nothex.cbegin(), r.second) == 1);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, nothex.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(nothex.cbegin(), r.second) == 2);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, nothex.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(nothex.cbegin(), r.second) == 3);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, nothex.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(nothex.cbegin(), r.second) == 3);
}

/* NON-ZERO_DIGIT *DIGIT */
static transition_type non_zero_decimal_fsm[] = {
	  { 1,-1, M::chr(sequence_type("123456789")).m, FSM_ACCEPT, 0, 0 }
	, { 1,-1, M::chr(_DIGITS).m                   , FSM_ACCEPT, 0, 0 }
};


/* (non-zero-dec dec)  / ( "0" ("x" / "X") hex ) */
static transition_type number_fsm[] = {
	  {-1, 1, M::tr(non_zero_decimal_fsm).m   , FSM_ACCEPT, 0, 0 }
	, { 2,-1, M::sub(sequence_type("0")).m , FSM_NORMAL, 0, 0 }
	, { 3,-1, M::chr(sequence_type("xX")).m   , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::tr(hex_fsm).m                , FSM_ACCEPT, 0, 0 }
};

static void test_alternatives(void)
{
    ADD_TESTS(16);

	sequence_type const hex("0xDEAD");
	sequence_type const decimal("1972");
	sequence_type const notnumber("[number]");
    
	fsm_type fsm(number_fsm, 0);

	sequence_type::const_iterator it_end;
	result_type r;

	it_end = hex.cbegin();
	TEST_FAIL(fsm.exec(0, hex.cbegin(), it_end) == result_type(false, it_end));

	pfs::advance(it_end, 1);
	TEST_FAIL(fsm.exec(0, hex.cbegin(), it_end) == result_type(false, it_end));

	pfs::advance(it_end, 1);
	TEST_FAIL(fsm.exec(0, hex.cbegin(), it_end) == result_type(false, it_end));

	pfs::advance(it_end, 1);
	r = fsm.exec(0, hex.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(hex.cbegin(), r.second) == 3);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, hex.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(hex.cbegin(), r.second) == 4);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, hex.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(hex.cbegin(), r.second) == 5);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, hex.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(hex.cbegin(), r.second) == 6);

	it_end = decimal.cbegin();
	TEST_FAIL(fsm.exec(0, decimal.cbegin(), it_end) == result_type(false, it_end));

	pfs::advance(it_end, 1);
	r = fsm.exec(0, decimal.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(decimal.cbegin(), r.second) == 1);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, decimal.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(decimal.cbegin(), r.second) == 2);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, decimal.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(decimal.cbegin(), r.second) == 3);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, decimal.cbegin(), it_end);
	TEST_FAIL(r.first && pfs::distance(decimal.cbegin(), r.second) == 4);

	it_end = notnumber.cbegin();
	pfs::advance(it_end, 1);
	TEST_FAIL(fsm.exec(0, notnumber.cbegin(), it_end) == result_type(false, it_end));

	pfs::advance(it_end, 1);
	TEST_FAIL(fsm.exec(0, notnumber.cbegin(), it_end) == result_type(false, it_end));

	pfs::advance(it_end, 1);
	TEST_FAIL(fsm.exec(0, notnumber.cbegin(), it_end) == result_type(false, it_end));

	pfs::advance(it_end, 5);
	TEST_FAIL(fsm.exec(0, notnumber.cbegin(), it_end) == result_type(false, it_end));
}

static transition_type alpha_len_fsm[] = {
	  { 1,-1, M::len(1).m , FSM_NORMAL, 0, 0 }
	, { 2,-1, M::len(2).m , FSM_NORMAL, 0, 0 }
	, { 3,-1, M::len(3).m , FSM_NORMAL, 0, 0 }
	, { 4,-1, M::len(4).m , FSM_NORMAL, 0, 0 }
	, { 5,-1, M::len(5).m , FSM_NORMAL, 0, 0 }
	, { 6,-1, M::len(6).m , FSM_NORMAL, 0, 0 }
	, { 7,-1, M::len(7).m , FSM_NORMAL, 0, 0 }
	, { 8,-1, M::len(8).m , FSM_NORMAL, 0, 0 }
	, { 9,-1, M::len(9).m , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::len(7).m , FSM_ACCEPT, 0, 0 }
};

static transition_type z_pos_fsm[] = {
	  { 1,-1, M::len(25).m                 , FSM_NORMAL, 0, 0 }
	, { 2,-1, M::sub(sequence_type("Z")).m , FSM_NORMAL, 0, 0 }
	, { 3,-1, M::len(25).m                 , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::chr(sequence_type("z")).m , FSM_ACCEPT, 0, 0 }
};

void test_len ()
{
    ADD_TESTS(2);
    
    static sequence_type const alpha_chars("ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz");

	fsm_type fsm1(alpha_len_fsm, 0);
	fsm_type fsm2(z_pos_fsm, 0);

	TEST_OK(fsm1.exec(0, alpha_chars.begin(), alpha_chars.end())
			== result_type(true, alpha_chars.end()));

	TEST_OK(fsm2.exec(0, alpha_chars.begin(), alpha_chars.end())
			== result_type(true, alpha_chars.cend()));
}

static pfs::fsm::transition<pfs::string> rpt_fsm[] = {
	  {-1,-1, M::rpt_sub(sequence_type("_ABC"), 0, 10).m , FSM_ACCEPT, 0, 0 }
};

void test_rpt ()
{
    ADD_TESTS(1);
    
    static sequence_type const rpt_chars("_ABC_ABC_ABC_ABC");
	fsm_type fsm(rpt_fsm, 0);
    
	TEST_FAIL(fsm.exec(0, rpt_chars.begin(), rpt_chars.end())
			== result_type(true, rpt_chars.end()));
}

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	BEGIN_TESTS(0);

	test_alternatives_simple();
	test_repetition_0more();
	test_repetition_1or2more();
	test_alternatives();
	test_len();
	test_rpt();

	return END_TESTS;
}
