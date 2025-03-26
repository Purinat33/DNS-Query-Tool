/* https://www.codequoi.com/en/sockets-and-network-programming-in-c */
// https://mislove.org/teaching/cs4700/spring11/handouts/project1-primer.pdf

// Network Bytes order is always Big Endian
// But Host Order is free game
#include <arpa/inet.h> // For H and N conversion

// For socket
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>

// https://datatracker.ietf.org/doc/html/rfc1035#section-4.1.1

// http://www.tcpipguide.com/free/t_DNSMessageHeaderandQuestionSectionFormat.htm
struct Header
{
    uint16_t id;   // 16 Bits / 2 Bytes
    uint16_t flag; // Includes QR, OpCode, AA, TC, RD, RA, Z, RCODE
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
};

// http://www.tcpipguide.com/free/t_DNSMessageHeaderandQuestionSectionFormat-2.htm
struct Body
{
    char *qname;
    uint16_t qtype;
    uint16_t qclass;
};

void encodeURL(char *url);

int main(int argc, char const *argv[])
{
    // http://www.tcpipguide.com/free/t_DNSMessageHeaderandQuestionSectionFormat.htm
    struct Header header;
    header.id = 30; // ID can be anything
    // 0th = 0 if query else 1
    // 1-4th = 0 if query
    // 5th = 0
    // 6th = 0
    // 7th = 1 -> Recursion Desired: When set in a query, requests that the server receiving the query attempt to answer the query recursively, if the server supports recursive resolution. The value of this bit is not changed in the response.
    // the rest is 0 for querying
    header.flag = 0b0000000100000000; // 256
    header.qdcount = 1;
    header.ancount = 0;
    header.nscount = 0;
    header.arcount = 0;

    struct Body body;
    body.qclass = 1;
    body.qtype = 1;

    return 0;
}

void encodeURL(char *url)
{

    /// www.google.com
    // 3www6google3com
    // Recursion?
    // https://www.educative.io/answers/splitting-a-string-using-strtok-in-c
    // Trim http:// or https:// if exists
    char *token = strtok(url, "//");
    token = strtok(NULL, "/");
    printf("\n%s\n", token);

    url = token;
    printf("%s\n\n", url);
}
