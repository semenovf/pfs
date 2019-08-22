#include "pfs/net/rtsp.hpp"
#include <iostream>
#include <map>
#include <string>
#include "../catch.hpp"

using namespace pfs::net;

TEST_CASE("RTSP / parse transport") {
    typedef rtsp::transport_entry<std::string::iterator> transport_entry;

    struct {
        std::string s;
        bool good;
    } samples[] = {
          { "RTP", false }
        , { "RTP/AVP", true }
        , { "rtp/avp", true }
        , { "RTP/AVP/UDP", true }
        , { "RTP/AVP/TCP", true }
        , { "RTP/AVP;unicast", true }
        , { "RTP/AVP;multicast", true }
        , { "RTP/AVP;destination", true }
        , { "RTP/AVP;destination=", false }
        , { "RTP/AVP;destination=;", false }
        , { "RTP/AVP;destination=239.0.0.1", true }
        , { "RTP/AVP;append", true }
        , { "RTP/AVP;interleaved", false }
        , { "RTP/AVP;interleaved=1", true }
        , { "RTP/AVP;interleaved=123-456", true }
        , { "RTP/AVP;ttl=127", true }
        , { "RTP/AVP;layers", false }
        , { "RTP/AVP;layers=", false }
        , { "RTP/AVP;layers=123", true }
        , { "RTP/AVP;port", false }
        , { "RTP/AVP;port;", false }
        , { "RTP/AVP;port=", false }
        , { "RTP/AVP;port=3456-", false }
        , { "RTP/AVP;port=3456", true }
        , { "RTP/AVP;port=3456-3457", true }
        , { "RTP/AVP;client_port", false }
        , { "RTP/AVP;client_port;", false }
        , { "RTP/AVP;client_port=", false }
        , { "RTP/AVP;client_port=3456-", false }
        , { "RTP/AVP;client_port=3456", true }
        , { "RTP/AVP;client_port=3456-3457", true }
        , { "RTP/AVP;server_port", false }
        , { "RTP/AVP;server_port;", false }
        , { "RTP/AVP;server_port=", false }
        , { "RTP/AVP;server_port=3456-", false }
        , { "RTP/AVP;server_port=3456", true }
        , { "RTP/AVP;server_port=3456-3457", true }
        , { "RTP/AVP;ssrc", false }
        , { "RTP/AVP;ssrc;", false }
        , { "RTP/AVP;ssrc=", false }
        , { "RTP/AVP;ssrc=127", true }
        , { "RTP/AVP;mode", false }
        , { "RTP/AVP;mode=", false }
        , { "RTP/AVP;mode=\"", false }
        , { "RTP/AVP;mode=\"\"", true }
        , { "RTP/AVP;mode=unknown", false }
        , { "RTP/AVP;mode=describe", true }
        , { "RTP/AVP;mode=DESCRIBE", true }
        , { "RTP/AVP;mode=\"describe\"", true }
        , { "RTP/AVP;mode=\"DESCRIBE\"", true }
        , { "RTP/AVP;mode=\"DESCRIBE,announce\"", true }
        , { "RTP/AVP;mode=\"DESCRIBE,announce,unknown\"", false }
        , { "RTP/AVP;mode=\"DESCRIBE,announce,announce,announce,announce,announce,announce,announce,announce,announce,announce,announce,announce\"", true }
        , { "RTP/AVP;multicast;ttl=127;mode=\"PLAY\","
            "RTP/AVP;unicast;client_port=3456-3457;mode=\"PLAY\"", true }
    };

    int count = sizeof(samples) / sizeof(samples[0]);

    for (int i = 0; i < count; i++) {
        std::vector<transport_entry> transports;
        std::string::iterator pos = std::begin(samples[i].s);
        std::string::iterator last = std::end(samples[i].s);

        bool ok = rtsp::advance_transport(pos, last, std::back_inserter(transports));
        CHECK(ok == samples[i].good);

        if (ok != samples[i].good) {
            std::cout << "Suspicious transport: \"" << samples[i].s << "\"\n\n";
        }
    }

    {
        std::string s = "RTP/AVP;multicast;ttl=127;mode=\"PLAY\""
                ",RTP/AVP/TCP;unicast;client_port=3456-3457;mode=\"PLAY,DESCRIBE\"";

        std::vector<transport_entry> transports;
        std::string::iterator pos = std::begin(s);
        std::string::iterator last = std::end(s);

        REQUIRE(rtsp::advance_transport(pos, last, std::back_inserter(transports)));
        CHECK(transports.size() == 2);

        transport_entry * t = & transports[0];

        CHECK(proto::compare(t->protocol.first, t->protocol.second, "RTP") == 0);
        CHECK(proto::compare(t->profile.first, t->profile.second, "AVP") == 0);
        CHECK(t->lower_transport == rtsp::RTSP_LOWER_TRANSPORT_UDP);
        CHECK(proto::compare(t->destination.first, t->destination.second, "") == 0);
        CHECK(t->multicast);
        CHECK((t->interleaved.first == 0 && t->interleaved.second == 0));
        CHECK(!t->append);
        CHECK(t->ttl == 127);
        CHECK(t->layers == 0);
        CHECK((t->port.first == 0 && t->port.second == 0));
        CHECK((t->client_port.first == 0 && t->client_port.second == 0));
        CHECK((t->server_port.first == 0 && t->server_port.second == 0));
        CHECK(t->ssrc == 0);
        REQUIRE(t->methods.size() == 1);
        CHECK(t->methods[0] == rtsp::RTSP_METHOD_PLAY);

        t = & transports[1];
        CHECK(proto::compare(t->protocol.first, t->protocol.second, "RTP") == 0);
        CHECK(proto::compare(t->profile.first, t->profile.second, "AVP") == 0);
        CHECK(t->lower_transport == rtsp::RTSP_LOWER_TRANSPORT_TCP);
        CHECK(proto::compare(t->destination.first, t->destination.second, "") == 0);
        CHECK(!t->multicast);
        CHECK((t->interleaved.first == 0 && t->interleaved.second == 0));
        CHECK(!t->append);
        CHECK(t->ttl == 0);
        CHECK(t->layers == 0);
        CHECK((t->port.first == 0 && t->port.second == 0));
        CHECK((t->client_port.first == 3456 && t->client_port.second == 3457));
        CHECK((t->server_port.first == 0 && t->server_port.second == 0));
        CHECK(t->ssrc == 0);
        REQUIRE(t->methods.size() == 2);
        CHECK(t->methods[0] == rtsp::RTSP_METHOD_PLAY);
        CHECK(t->methods[1] == rtsp::RTSP_METHOD_DESCRIBE);
    }
}

TEST_CASE("RTSP / parse headers") {
    typedef std::map<std::string, std::string> property_tree;
    property_tree headers;

    std::string request1 = "OPTIONS rtsp://127.0.0.1:1234/live.h264 RTSP/1.0\n"
            "CSeq: 123\n"
            "User-Agent: Lavf/57.83.100\n"
            "Session: FEEDFEEDBEAFBEAF\n\n";
    std::string::iterator pos = request1.begin();
    REQUIRE(rtsp::advance_headers(pos, request1.end(), & headers));
    CHECK(headers["#method"] == "OPTIONS");
    CHECK(headers["#uri"] == "rtsp://127.0.0.1:1234/live.h264");
    CHECK(headers["#proto"] == "RTSP");
    CHECK(headers["#version"] == "1.0");
    CHECK(headers["cseq"] == "123");
    CHECK(headers["user-agent"] == "Lavf/57.83.100");
    CHECK(headers["session"] == "FEEDFEEDBEAFBEAF");
}

TEST_CASE("RTSP / headers") {
    rtsp::headers<std::string> headers;
    rtsp::headers<std::string>::transport * tran = 0;

    std::string request = "OPTIONS rtsp://127.0.0.1:1234/live.h264 RTSP/1.0\n"
            "CSeq: 123\n"
            "User-Agent: Lavf/57.83.100\n"
            "Session: FEEDFEEDBEAFBEAF\n"
            "Transport: RTP/AVP;multicast;ttl=127;mode=\"PLAY\","
                    "RTP/AVP/TCP;client_port=3456-3457;mode=\"PLAY,OPTIONS\"\n"
            "\n\n";
    REQUIRE(headers.parse(request.begin(), request.end()));

    CHECK(headers.method.value() == "OPTIONS");
    CHECK(headers.uri.value() == "rtsp://127.0.0.1:1234/live.h264");
    CHECK(headers.protocol.value() == "RTSP");
    CHECK(headers.version.value() == "1.0");
    CHECK(headers.sequence_number.value() == 123);
    CHECK(headers["user-agent"] == "Lavf/57.83.100");
    CHECK(headers.session_id.value() == "FEEDFEEDBEAFBEAF");

    CHECK(headers.transport_count() == 2);

    tran = headers.transport_at(0);
    CHECK(tran->protocol == "RTP");
    CHECK(tran->profile == "AVP");
    CHECK(tran->lower_transport == rtsp::RTSP_LOWER_TRANSPORT_UDP);
    CHECK(tran->multicast);
    CHECK(tran->ttl == 127);
    CHECK(tran->methods.size() == 1);
    CHECK(tran->methods[0] == rtsp::RTSP_METHOD_PLAY);

    tran = headers.transport_at(1);
    CHECK(tran->protocol == "RTP");
    CHECK(tran->profile == "AVP");
    CHECK(tran->lower_transport == rtsp::RTSP_LOWER_TRANSPORT_TCP);
    CHECK(!tran->multicast);
    CHECK(tran->client_port.first == 3456);
    CHECK(tran->client_port.second == 3457);
    CHECK(tran->methods.size() == 2);
    CHECK(tran->methods[0] == rtsp::RTSP_METHOD_PLAY);
    CHECK(tran->methods[1] == rtsp::RTSP_METHOD_OPTIONS);
}
