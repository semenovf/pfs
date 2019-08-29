#include "pfs/io/utils.hpp"
#include <string>
#include <cstring>
#include "../catch.hpp"

// char loremipsum[] = "1.L\n2";
// const char * loremipsum_lines[] = { "1.L\n", "2" };

char loremipsum[] =
"1.Lorem ipsum dolor sit amet, consectetuer adipiscing elit,    \n\
2.sed diam nonummy nibh euismod tincidunt ut laoreet dolore     \n\
3.magna aliquam erat volutpat. Ut wisi enim ad minim veniam,    \n\
4.quis nostrud exerci tation ullamcorper suscipit lobortis      \n\
5.nisl ut aliquip ex ea commodo consequat. Duis autem vel eum   \n\
6.iriure dolor in hendrerit in vulputate velit esse molestie    \n\
7.consequat, vel illum dolore eu feugiat nulla facilisis at     \n\
8.vero eros et accumsan et iusto odio dignissim qui blandit     \n\
9.praesent luptatum zzril delenit augue duis dolore te feugait  \n\
10.nulla facilisi. Nam liber tempor cum soluta nobis eleifend    \n\
11.option congue nihil imperdiet doming id quod mazim placerat   \n\
12.facer possim assum. Typi non habent claritatem insitam; est   \n\
13.usus legentis in iis qui facit eorum claritatem.              \n\
14.Investigationes demonstraverunt lectores legere me lius quod  \n\
15.ii legunt saepius. Claritas est etiam processus dynamicus,    \n\
16.qui sequitur mutationem consuetudium lectorum. Mirum est      \n\
17.notare quam littera gothica, quam nunc putamus parum claram,  \n\
18.anteposuerit litterarum formas humanitatis per seacula quarta \n\
19.decima et quinta decima. Eodem modo typi, qui nunc nobis      \n\
20.videntur parum clari, fiant sollemnes in futurum.             \n\
21.Lorem ipsum dolor sit amet, consectetuer adipiscing elit,     \n\
22.sed diam nonummy nibh euismod tincidunt ut laoreet dolore     \n\
23.magna aliquam erat volutpat. \"Ut wisi enim ad minim veniam,  \n\
24.quis nostrud exerci tation ullamcorper suscipit lobortis      \n\
25.nisl ut aliquip ex ea commodo consequat. Duis autem vel eum   \n\
26.iriure dolor in hendrerit in vulputate velit esse molestie    \n\
27.consequat, vel illum dolore eu feugiat nulla facilisis at     \n\
28.vero eros et accumsan et iusto odio dignissim qui blandit     \n\
29.praesent luptatum zzril delenit augue duis dolore te feugait  \n\
30.nulla facilisi. Nam liber tempor cum soluta nobis eleifend    \n\
31.option congue nihil imperdiet doming id quod mazim placerat   \n\
32.facer possim assum. Typi non habent claritatem insitam; est   \n\
33.usus legentis in iis qui facit eorum claritatem.              \n\
34.Investigationes demonstraverunt lectores legere me lius quod  \n\
35.ii legunt saepius. Claritas est etiam processus dynamicus,    \n\
36.qui sequitur mutationem consuetudium lectorum. Mirum est      \n\
37.notare quam littera gothica, quam nunc putamus parum claram,  \n\
38.anteposuerit litterarum formas humanitatis per seacula quarta \n\
39.decima et quinta decima.\" Eodem modo typi, qui nunc nobis    \n\
40.videntur parum clari, fiant sollemnes in futurum.";

const char * loremipsum_lines[] = {
   "1.Lorem ipsum dolor sit amet, consectetuer adipiscing elit,    \n"
,  "2.sed diam nonummy nibh euismod tincidunt ut laoreet dolore     \n"
,  "3.magna aliquam erat volutpat. Ut wisi enim ad minim veniam,    \n"
,  "4.quis nostrud exerci tation ullamcorper suscipit lobortis      \n"
,  "5.nisl ut aliquip ex ea commodo consequat. Duis autem vel eum   \n"
,  "6.iriure dolor in hendrerit in vulputate velit esse molestie    \n"
,  "7.consequat, vel illum dolore eu feugiat nulla facilisis at     \n"
,  "8.vero eros et accumsan et iusto odio dignissim qui blandit     \n"
,  "9.praesent luptatum zzril delenit augue duis dolore te feugait  \n"
,  "10.nulla facilisi. Nam liber tempor cum soluta nobis eleifend    \n"
,  "11.option congue nihil imperdiet doming id quod mazim placerat   \n"
,  "12.facer possim assum. Typi non habent claritatem insitam; est   \n"
,  "13.usus legentis in iis qui facit eorum claritatem.              \n"
,  "14.Investigationes demonstraverunt lectores legere me lius quod  \n"
,  "15.ii legunt saepius. Claritas est etiam processus dynamicus,    \n"
,  "16.qui sequitur mutationem consuetudium lectorum. Mirum est      \n"
,  "17.notare quam littera gothica, quam nunc putamus parum claram,  \n"
,  "18.anteposuerit litterarum formas humanitatis per seacula quarta \n"
,  "19.decima et quinta decima. Eodem modo typi, qui nunc nobis      \n"
,  "20.videntur parum clari, fiant sollemnes in futurum.             \n"
,  "21.Lorem ipsum dolor sit amet, consectetuer adipiscing elit,     \n"
,  "22.sed diam nonummy nibh euismod tincidunt ut laoreet dolore     \n"
,  "23.magna aliquam erat volutpat. \"Ut wisi enim ad minim veniam,  \n"
,  "24.quis nostrud exerci tation ullamcorper suscipit lobortis      \n"
,  "25.nisl ut aliquip ex ea commodo consequat. Duis autem vel eum   \n"
,  "26.iriure dolor in hendrerit in vulputate velit esse molestie    \n"
,  "27.consequat, vel illum dolore eu feugiat nulla facilisis at     \n"
,  "28.vero eros et accumsan et iusto odio dignissim qui blandit     \n"
,  "29.praesent luptatum zzril delenit augue duis dolore te feugait  \n"
,  "30.nulla facilisi. Nam liber tempor cum soluta nobis eleifend    \n"
,  "31.option congue nihil imperdiet doming id quod mazim placerat   \n"
,  "32.facer possim assum. Typi non habent claritatem insitam; est   \n"
,  "33.usus legentis in iis qui facit eorum claritatem.              \n"
,  "34.Investigationes demonstraverunt lectores legere me lius quod  \n"
,  "35.ii legunt saepius. Claritas est etiam processus dynamicus,    \n"
,  "36.qui sequitur mutationem consuetudium lectorum. Mirum est      \n"
,  "37.notare quam littera gothica, quam nunc putamus parum claram,  \n"
,  "38.anteposuerit litterarum formas humanitatis per seacula quarta \n"
,  "39.decima et quinta decima.\" Eodem modo typi, qui nunc nobis    \n"
,  "40.videntur parum clari, fiant sollemnes in futurum." };

struct loremipsum_reader
{
    char const * pos = loremipsum;
    char const * last = loremipsum + std::strlen(loremipsum);

    //ssize_t recv (int, char * buf, size_t len, int flags)
    ssize_t operator () (int, char * buf, size_t len, int)
    {
        ssize_t n = last - pos > len ? len : last - pos;

        if (n) {
            std::memcpy(buf, pos, n);
            pos += n;
        }

        return n;
    }
};

TEST_CASE("IO / buffered_input") {
    char buf[32];
    std::string s;
    pfs::io::buffered_input<loremipsum_reader, char> input;
    ssize_t n = 0;

    CHECK(input.size() == 0);

    do {
        n = input.read(buf, sizeof(buf));

        if (n > 0) {
            s.append(buf, n);
        }

    } while (n > 0);

    CHECK(input.size() == 0);
    CHECK(s == loremipsum);
}

TEST_CASE("IO / buffered_input read line") {
    std::string s;
    pfs::io::buffered_input<loremipsum_reader, char> input;
    ssize_t n = 0;
    size_t nlines = sizeof(loremipsum_lines) / sizeof(loremipsum_lines[0]);
    size_t line_counter = 0;

    do {
        std::string line;
        n = input.read_line(std::back_inserter(line));

        if (n > 0) {
            s.append(line);
            CHECK(line == loremipsum_lines[line_counter++]);
        }
    } while (n > 0 && line_counter < nlines);

    CHECK(line_counter == nlines);
    CHECK(s == loremipsum);
}
