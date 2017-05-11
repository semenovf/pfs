/**
 * @file   fsm-char.c
 * @author wladt
 * @date   Oct 10, 2012 11:40:55 AM
 *
 * @brief
 */

#include <pfs/test/test.hpp>
#include <pfs/fsm/fsm.hpp>
#include <pfs/fsm/traits.hpp>
#include <pfs/iterator.hpp>

typedef std::string                  sequence_type;
typedef pfs::fsm::fsm<sequence_type> fsm_type;
typedef fsm_type::char_type          char_type;

/* DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */
static sequence_type const _DIGITS("0123456789");
static sequence_type const _HEXDIGITS("ABCDEFabcdef");

static fsm_type::transition_type hexdig_tr[] = {
	  {-1, 1, fsm_type::one_of(_DIGITS)    , fsm_type::accept, 0, 0 }
    , {-1,-1, fsm_type::one_of(_HEXDIGITS) , fsm_type::accept, 0, 0 }
};

static void test_alternatives_simple ()
{
    ADD_TESTS(4);
    
	sequence_type const hexdig("F");
	sequence_type const digit("9");
	sequence_type const notdigit("w");
    
	fsm_type fsm(hexdig_tr);
    fsm_type::result_type r = fsm.exec(0, hexdig.begin()
            , hexdig.begin());
	TEST_FAIL(r.first == false);

	sequence_type::const_iterator it_end;
	
	it_end = hexdig.begin();
	pfs::advance(it_end, 1);
	r = fsm.exec(0, hexdig.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(hexdig.begin(), r.second) == 1);

	it_end = digit.begin();
	pfs::advance(it_end, 1);
	r = fsm.exec(0, digit.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(digit.begin(), r.second) == 1);

	it_end = notdigit.begin();
	pfs::advance(it_end, 1);
	r = fsm.exec(0, notdigit.begin(), it_end);
	TEST_FAIL(!r.first && r.second == it_end);
}

static sequence_type const alphabet("ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz");

static fsm_type::transition_type alphabet_length_tr[] = {
	  { 1,-1, fsm_type::length(1), fsm_type::normal, 0, 0 }
	, { 2,-1, fsm_type::length(2), fsm_type::normal, 0, 0 }
	, { 3,-1, fsm_type::length(3), fsm_type::normal, 0, 0 }
	, { 4,-1, fsm_type::length(4), fsm_type::normal, 0, 0 }
	, { 5,-1, fsm_type::length(5), fsm_type::normal, 0, 0 }
	, { 6,-1, fsm_type::length(6), fsm_type::normal, 0, 0 }
	, { 7,-1, fsm_type::length(7), fsm_type::normal, 0, 0 }
	, { 8,-1, fsm_type::length(8), fsm_type::normal, 0, 0 }
	, { 9,-1, fsm_type::length(9), fsm_type::normal, 0, 0 }
	, {-1,-1, fsm_type::length(7), fsm_type::accept, 0, 0 }
};

static fsm_type::transition_type z_pos_tr[] = {
	  { 1,-1, fsm_type::length(25)                , fsm_type::normal, 0, 0 }
	, { 2,-1, fsm_type::seq(sequence_type("Z")), fsm_type::normal, 0, 0 }
	, { 3,-1, fsm_type::length(25)                , fsm_type::normal, 0, 0 }
	, {-1,-1, fsm_type::one_of(sequence_type("z")), fsm_type::accept, 0, 0 }
};

void test_length ()
{
    ADD_TESTS(2);
    
	fsm_type fsm1(alphabet_length_tr);
	fsm_type fsm2(z_pos_tr);

	TEST_OK(fsm1.exec(0, alphabet.begin(), alphabet.end())
			== fsm_type::result_type(true, alphabet.end()));

	TEST_OK(fsm2.exec(0, alphabet.begin(), alphabet.end())
			== fsm_type::result_type(true, alphabet.end()));
}

static fsm_type::transition_type subseq_tr[] = {
	  { 1,-1, fsm_type::seq(sequence_type("ABCD")), fsm_type::normal, 0, 0 }
	, { 2,-1, fsm_type::seq(sequence_type("EFGH")), fsm_type::normal, 0, 0 }
	, { 3,-1, fsm_type::seq(sequence_type("IJKL")), fsm_type::normal, 0, 0 }
	, { 4,-1, fsm_type::seq(sequence_type("MNOP")), fsm_type::normal, 0, 0 }
	, { 5,-1, fsm_type::seq(sequence_type("QRST")), fsm_type::normal, 0, 0 }
	, { 6,-1, fsm_type::seq(sequence_type("UVWX")), fsm_type::normal, 0, 0 }
	, { 7,-1, fsm_type::seq(sequence_type("YZ"))  , fsm_type::normal, 0, 0 }
    , {-1,-1, fsm_type::seq(sequence_type("abcdefghijklmnopqrstuvwxyz")), fsm_type::accept, 0, 0 }
};

void test_subseq ()
{
    ADD_TESTS(3);
    
	fsm_type fsm(subseq_tr);

	TEST_OK(fsm.exec(0, alphabet.begin(), alphabet.end())
			== fsm_type::result_type(true, alphabet.end()));

    TEST_OK(fsm.exec(0, _DIGITS.begin(), _DIGITS.begin()).first == false)
	TEST_OK(fsm.exec(0, _DIGITS.begin(), _DIGITS.end()).first == false);
}

static fsm_type::transition_type range_tr[] = {
	  { 1,-1, fsm_type::range(char_type('A'), char_type('B')), fsm_type::normal, 0, 0 }
	, {-1,-1, fsm_type::range(char_type('B'), char_type('C')), fsm_type::accept, 0, 0 }
};

void test_range ()
{
    ADD_TESTS(2);
    
    fsm_type fsm(range_tr);
    
    TEST_OK(fsm.exec(0, alphabet.begin(), alphabet.end()).first == true);
    TEST_OK(fsm.exec(0, _DIGITS.begin(), _DIGITS.end()).first == false);
}


static fsm_type::result_type is_alphabet (
                          fsm_type::const_iterator begin
                        , fsm_type::const_iterator end
                        , void * parse_context
                        , void * fn_context)
{
    PFS_UNUSED2(parse_context, fn_context);
    
    sequence_type sample(begin, end);
            
    if (sample == alphabet) {
        return fsm_type::result_type(true, begin + alphabet.size());
    }
    
    return fsm_type::result_type(false, end);
}

static fsm_type::transition_type alphabet_func_tr[] = {
	  {-1,-1, fsm_type::func(is_alphabet, 0), fsm_type::accept, 0, 0 }
};

void test_func ()
{
    ADD_TESTS(2);
    
    fsm_type fsm(alphabet_func_tr);
    
    TEST_OK(fsm.exec(0, alphabet.begin(), alphabet.end()).first == true);
    TEST_OK(fsm.exec(0, _DIGITS.begin(), _DIGITS.end()).first == false);
}

/* 0*DIGIT */
static fsm_type::transition_type decimal0more_tr[] = {
	  { 0, 1, fsm_type::one_of(_DIGITS), fsm_type::accept, 0, 0 }
	, {-1,-1, fsm_type::nothing()      , fsm_type::accept, 0, 0 }
};

static void test_repetition_0more ()
{
    ADD_TESTS(11);
    
	sequence_type const dec("1972");
	sequence_type const notdec("x1972");
    
	fsm_type fsm(decimal0more_tr);

	sequence_type::const_iterator it_end;
	fsm_type::result_type r;

	it_end = dec.begin();
	r = fsm.exec(0, dec.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.begin(), r.second) == 0);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.begin(), r.second) == 1);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.begin(), r.second) == 2);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.begin(), r.second) == 3);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.begin(), r.second) == 4);

	it_end = notdec.begin();
	r = fsm.exec(0, notdec.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(notdec.begin(), r.second) == 0);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, notdec.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(notdec.begin(), r.second) == 0);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, notdec.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(notdec.begin(), r.second) == 0);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, notdec.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(notdec.begin(), r.second) == 0);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, notdec.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(notdec.begin(), r.second) == 0);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, notdec.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(notdec.begin(), r.second) == 0);
}

/* 1*DIGIT */
static fsm_type::transition_type decimal1more_tr[] = {
	{ 0,-1, fsm_type::one_of(_DIGITS), fsm_type::accept, 0, 0 }
};

/* 2*DIGIT */
static fsm_type::transition_type decimal2more_tr[] = {
	  { 1,-1, fsm_type::one_of(_DIGITS), fsm_type::normal, 0, 0 }
	, { 1,-1, fsm_type::one_of(_DIGITS), fsm_type::accept, 0, 0 }
};

/* 1*HEXDIG_FSM */
static fsm_type::transition_type hex_tr[] = {
    { 0,-1, fsm_type::tr(hexdig_tr), fsm_type::accept, 0, 0 }
};

static void test_repetition_1or2more ()
{
    ADD_TESTS(23);
    
	sequence_type const dec("1972");
	sequence_type const notdec("x1972");
	sequence_type const hex("BEAF");
	sequence_type const nothex("BEAR");

	fsm_type fsm(decimal1more_tr, 0);

	sequence_type::const_iterator it_end;
	fsm_type::result_type r;

	it_end = dec.begin();
	r = fsm.exec(0, dec.begin(), it_end);
	TEST_FAIL(!r.first && r.second == it_end);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.begin(), r.second) == 1);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.begin(), r.second) == 2);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.begin(), r.second) == 3);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.begin(), r.second) == 4);

	it_end = notdec.begin();
	r = fsm.exec(0, notdec.begin(), it_end);
	TEST_FAIL(!r.first);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, notdec.begin(), it_end);
	TEST_FAIL(!r.first);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, notdec.begin(), it_end);
	TEST_FAIL(!r.first);

	fsm.set_transition_table(decimal2more_tr);

	it_end = dec.begin();
	r = fsm.exec(0, dec.begin(), it_end);
	TEST_FAIL(!r.first);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.begin(), it_end);
	TEST_FAIL(!r.first);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.begin(), r.second) == 2);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.begin(), r.second) == 3);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, dec.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(dec.begin(), r.second) == 4);

	fsm.set_transition_table(hex_tr);

	it_end = hex.begin();
	r = fsm.exec(0, hex.begin(), it_end);
	TEST_FAIL(!r.first);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, hex.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(hex.begin(), r.second) == 1);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, hex.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(hex.begin(), r.second) == 2);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, hex.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(hex.begin(), r.second) == 3);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, hex.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(hex.begin(), r.second) == 4);

	it_end = nothex.begin();
	r = fsm.exec(0, nothex.begin(), it_end);
	TEST_FAIL(!r.first);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, nothex.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(nothex.begin(), r.second) == 1);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, nothex.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(nothex.begin(), r.second) == 2);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, nothex.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(nothex.begin(), r.second) == 3);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, nothex.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(nothex.begin(), r.second) == 3);
}

/* NON-ZERO_DIGIT *DIGIT */
static fsm_type::transition_type non_zero_decimal_tr[] = {
	  { 1,-1, fsm_type::one_of(sequence_type("123456789")), fsm_type::accept, 0, 0 }
	, { 1,-1, fsm_type::one_of(_DIGITS)                   , fsm_type::accept, 0, 0 }
};


/* (non-zero-dec dec)  / ( "0" ("x" / "X") hex ) */
static fsm_type::transition_type number_tr[] = {
	  {-1, 1, fsm_type::tr(non_zero_decimal_tr)    , fsm_type::accept, 0, 0 }
	, { 2,-1, fsm_type::seq(sequence_type("0")) , fsm_type::normal, 0, 0 }
	, { 3,-1, fsm_type::one_of(sequence_type("xX")), fsm_type::normal, 0, 0 }
	, {-1,-1, fsm_type::tr(hex_tr)                 , fsm_type::accept, 0, 0 }
};

static void test_alternatives ()
{
    ADD_TESTS(16);

	sequence_type const hex("0xDEAD");
	sequence_type const decimal("1972");
	sequence_type const notnumber("[number]");
    
	fsm_type fsm(number_tr, 0);

	sequence_type::const_iterator it_end;
	fsm_type::result_type r;

	it_end = hex.begin();
	TEST_FAIL(fsm.exec(0, hex.begin(), it_end) 
            == fsm_type::result_type(false, it_end));

	pfs::advance(it_end, 1);
	TEST_FAIL(fsm.exec(0, hex.begin(), it_end) 
            == fsm_type::result_type(false, it_end));

	pfs::advance(it_end, 1);
	TEST_FAIL(fsm.exec(0, hex.begin(), it_end)
            == fsm_type::result_type(false, it_end));

	pfs::advance(it_end, 1);
	r = fsm.exec(0, hex.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(hex.begin(), r.second) == 3);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, hex.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(hex.begin(), r.second) == 4);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, hex.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(hex.begin(), r.second) == 5);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, hex.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(hex.begin(), r.second) == 6);

	it_end = decimal.begin();
	TEST_FAIL(fsm.exec(0, decimal.begin(), it_end) 
            == fsm_type::result_type(false, it_end));

	pfs::advance(it_end, 1);
	r = fsm.exec(0, decimal.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(decimal.begin(), r.second) == 1);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, decimal.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(decimal.begin(), r.second) == 2);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, decimal.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(decimal.begin(), r.second) == 3);

	pfs::advance(it_end, 1);
	r = fsm.exec(0, decimal.begin(), it_end);
	TEST_FAIL(r.first && pfs::distance(decimal.begin(), r.second) == 4);

	it_end = notnumber.begin();
	pfs::advance(it_end, 1);
	TEST_FAIL(fsm.exec(0, notnumber.begin(), it_end)
            == fsm_type::result_type(false, it_end));

	pfs::advance(it_end, 1);
	TEST_FAIL(fsm.exec(0, notnumber.begin(), it_end) 
            == fsm_type::result_type(false, it_end));

	pfs::advance(it_end, 1);
	TEST_FAIL(fsm.exec(0, notnumber.begin(), it_end)
            == fsm_type::result_type(false, it_end));

	pfs::advance(it_end, 5);
	TEST_FAIL(fsm.exec(0, notnumber.begin(), it_end) 
            == fsm_type::result_type(false, it_end));
}

static fsm_type::transition_type rpt_tr[] = {
	{-1,-1, fsm_type::rpt_seq(sequence_type("_ABC"), 0, 10), fsm_type::accept, 0, 0 }
};

void test_rpt ()
{
    ADD_TESTS(1);
    
    static sequence_type const rpt_chars("_ABC_ABC_ABC_ABC");
	fsm_type fsm(rpt_tr, 0);
    
	TEST_FAIL(fsm.exec(0, rpt_chars.begin(), rpt_chars.end())
			== fsm_type::result_type(true, rpt_chars.end()));
}

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	BEGIN_TESTS(0);

    test_alternatives_simple();
    test_length();
    test_subseq();
    test_range();
    test_func();
    test_repetition_0more();
    test_repetition_1or2more();
    test_alternatives();
    test_rpt();

	return END_TESTS;
}
