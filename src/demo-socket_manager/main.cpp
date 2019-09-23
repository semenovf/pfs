#include <string>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

static int const BUFSIZE = 256;
static int const SERVER_PORT = 10000;

void process_server (int port);
void process_client (std::string const & serveraddr, int port);

/*
 * error - wrapper for perror
 */
inline void error (char const * msg)
{
    perror(msg);
    exit(1);
}

int main (int argc, char * argv[])
{
    bool is_server = true;

    if (argc > 1)
        is_server = false;

    if (is_server) {
        process_server(SERVER_PORT);
    } else {
        std::string server_addr(argv[1]);
        process_client(server_addr, SERVER_PORT);
    }

    return EXIT_SUCCESS;
}

/**
 * @param portno port to listen on
 */
void process_server (int portno)
{
    char buf[BUFSIZE];

    /*
     * socket: create the parent socket
     */
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
        error("ERROR opening socket");

    /*
     * setsockopt: Handy debugging trick that lets
     * us rerun the server immediately after we kill it;
     * otherwise we have to wait about 20 secs.
     * Eliminates "ERROR on binding: Address already in use" error.
     */
    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, & optval, sizeof(int));

    /*
     * build the server's Internet address
     */
    sockaddr_in serveraddr;  // server's addr
    std::memset((char *) & serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short) portno);

    /*
     * bind: associate the parent socket with a port
     */
    if (bind(sockfd, (struct sockaddr *)& serveraddr, sizeof (serveraddr)) < 0)
        error("ERROR on binding");

    /*
     * main loop: wait for a datagram, then echo it
     */
    sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr); // byte size of client's address

    int timeout_msecs = 500;
    pollfd fds[1];
    fds[0].fd     = sockfd;
    fds[0].events = (short int)0xFFFF;

    while (true) {
        int ret = poll(fds, 1, timeout_msecs);

        if (ret == 0)
            continue;

        if (ret < 0) {
            error("ERROR in poll()");
            break;
        }

        if (fds[0].revents & POLLHUP) {
            error("A hangup has occurred on device");
            break;
        }

        if (fds[0].revents & (POLLOUT | POLLWRNORM | POLLWRBAND)) {
            /*puts("Data may be written on device");
            continue; */
        }

        if (fds[0].revents & (POLLIN | POLLRDNORM | POLLRDBAND)) {
            puts("Data may be read on device");

            /*
             * recvfrom: receive a UDP datagram from a client
             */
            bzero(buf, BUFSIZE);

            int n = recvfrom(sockfd
                    , buf
                    , BUFSIZE
                    , 0
                    , reinterpret_cast<sockaddr *>(& clientaddr)
                    , & clientlen);

            if (n < 0)
                error("ERROR in recvfrom");

            /*
             * gethostbyaddr: determine who sent the datagram
             */
            // Client host info
            hostent * hostp = gethostbyaddr(
                      reinterpret_cast<char const *>(& clientaddr.sin_addr.s_addr)
                    , sizeof(clientaddr.sin_addr.s_addr)
                    , AF_INET);

            if (hostp == nullptr)
                error("ERROR on gethostbyaddr");

            // dotted decimal host addr string
            char * hostaddrp = inet_ntoa(clientaddr.sin_addr);

            if (hostaddrp == nullptr)
                error("ERROR on inet_ntoa\n");

            printf("server received datagram from %s (%s)\n"
                    , hostp->h_name
                    , hostaddrp);

            printf("server received %u/%d bytes: %s\n"
                    , static_cast<unsigned int>(strlen(buf))
                    , static_cast<int>(n)
                    , buf);

            /*
             * sendto: echo the input back to the client
             */
            n = sendto(sockfd
                    , buf
                    , strlen(buf)
                    , 0
                    , reinterpret_cast<sockaddr *>(& clientaddr)
                    , clientlen);

            if (n < 0)
                error("ERROR in sendto");
        }
    }
}

void process_client (std::string const & serveraddr, int port)
{
    in_addr addr;
    int ret = inet_aton(serveraddr.c_str(), & addr);

    if (ret == 0)
        error("bad server address representation");
}
