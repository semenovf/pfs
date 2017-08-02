#include <cwt/test.hpp>
#include <cwt/svg.hpp>

void test_path_parse ()
{
	pfs::string p0;
	pfs::string p1("M 100 100 L 300 100 L 200 300 z");
	pfs::string p2("M100,200 C100,100 250,100 250,200 S400,300 400,200");
	pfs::string p3("M200,300 Q400,50 600,300 T1000,300");
	pfs::string p4("M200,300 L400,50 L600,300 \nL800,550 L1000,300"); // with EOL
	pfs::string p5("M300,200 h-150 a150,150 0 1,0 150,-150 z");
	pfs::string p6("M275,175 v-150 a150,150 0 0,0 -150,150 z");
	pfs::string p7("M600,350 l 50,-25\n"          // multi-line
           "a25,25 -30 0,1 50,-25 l 50,-25\n  "
           "a25,50 -30 0,1 50,-25 l 50,-25 \n"
           "a25,75 -30 0,1 50,-25 l 50,-25  \n  "
           "a25,100 -30 0,1 50,-25 l 50,-25");
	pfs::string p8(" \n  ");
	pfs::string p9("M 432.85714,309.50504 274.28571,536.6479");

	TEST_OK(cwt::svg::path_seg::parse(p9));

	TEST_OK(cwt::svg::path_seg::parse(p0));
	TEST_OK(cwt::svg::path_seg::parse(p1));
	TEST_OK(cwt::svg::path_seg::parse(p2));
	TEST_OK(cwt::svg::path_seg::parse(p3));
	TEST_OK(cwt::svg::path_seg::parse(p4));
	TEST_OK(cwt::svg::path_seg::parse(p5));
	TEST_OK(cwt::svg::path_seg::parse(p6));
	TEST_OK(cwt::svg::path_seg::parse(p7));
	TEST_OK(cwt::svg::path_seg::parse(p8))


	pfs::string p10("invalid path");
	pfs::string p11("M275,175 v-150 a150,150 0 0,0 -150,150 W"); // 'W' is invalid token
	TEST_OK(!cwt::svg::path_seg::parse(p10));
	TEST_OK(!cwt::svg::path_seg::parse(p11));
}

void test_path_transform ()
{
	pfs::string t0;
	pfs::string t1("translate(50,50)");
	pfs::string t2("rotate(30)");
	pfs::string t3("scale(1.5)");
	pfs::string t4("skewX(30)");
	pfs::string t5("skewY(30)");
	pfs::string t6("rotate(-45)");
	pfs::string t7("scale(0.1 0.2)");
	pfs::string t8("rotate(-90) scale(100, 100)");
	pfs::string t9("rotate(15, 40, 40)");
	pfs::string t10("matrix(1,0,0,1,100,20)");

	TEST_OK(cwt::svg::transform::parse(t0));
	TEST_OK(cwt::svg::transform::parse(t1));
	TEST_OK(cwt::svg::transform::parse(t2));
	TEST_OK(cwt::svg::transform::parse(t3));
	TEST_OK(cwt::svg::transform::parse(t4));
	TEST_OK(cwt::svg::transform::parse(t5));
	TEST_OK(cwt::svg::transform::parse(t6));
	TEST_OK(cwt::svg::transform::parse(t7));
	TEST_OK(cwt::svg::transform::parse(t8));
	TEST_OK(cwt::svg::transform::parse(t9));
	TEST_OK(cwt::svg::transform::parse(t10));
}

int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(12);

    test_path_parse();
    test_path_transform();

    END_TESTS;
}
