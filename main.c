/* https://www.codequoi.com/en/sockets-and-network-programming-in-c */
// https://mislove.org/teaching/cs4700/spring11/handouts/project1-primer.pdf

// Network Bytes order is always Big Endian
// But Host Order is free game
#include <arpa/inet.h> // For H and N conversion

// Get DNS server address
// #include <resolv.h>

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

struct Packet
{
    struct Header H;
    struct Body B;
};

char *encodeURL(char *url);

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("No URL Supplied\n");
        return -1;
    }

    if (argc < 3)
    {
        printf("No DNS Server supploed\n");
        return -1;
    }
    printf("DNS Resolver Tool:\n");
    printf("DNS Server: %s\n", argv[2]);

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
    body.qname = encodeURL(argv[1]);
    body.qclass = 1;
    body.qtype = 1;

    // Concatanating everything together
    struct Packet packet;
    packet.H = header;
    packet.B = body;

    // Before sending data to the socket we have to serialize the data
    // Because we have strings (pointers)

    return 0;
}

char *encodeURL(char *url)
{

    // https://www.educative.io/answers/splitting-a-string-using-strtok-in-c

    // Check for HTTP or HTTPS existence
    // Trim http:// or https:// if exists
    // https://stackoverflow.com/questions/12784766/check-substring-exists-in-a-string-in-c
    // https is superset of http anyway
    if (strstr(url, "http"))
    {
        char *token = strtok(url, "//");
        token = strtok(NULL, "/");
        url = token;
    }
    // Removing any subdomain
    char *subdomain = strtok(url, "/");
    url = subdomain;
    // printf("%s\n\n", url);

    // Now split at '.'
    /// www.google.com
    // 3www6google3com0
    // Original String Size + 2 (Front and back 0)
    // char encoded_url[strlen(url) + 3]; // For the \0 (strlen doesnt count \0)
    // // First element = strlen(first group)

    // char *delim = strtok(url, ".");
    // while (delim != NULL)
    // {
    //     strcat(encoded_url, strlen(delim));
    //     strcat(encoded_url, delim);
    //     printf("%s\n", encoded_url);
    //     delim = strtok(NULL, ".");
    // }
    // encoded_url[strlen(encoded_url) - 1] = '0';
    char encoded_url[256] = ""; // Ensure this is large enough

    // Tokenize the string by '.'
    char *token = strtok(url, ".");
    while (token != NULL)
    {
        // Convert the length of the token to a string
        char lenStr[10];
        sprintf(lenStr, "%d", (int)strlen(token));

        // Append the length and then the token to the encoded URL
        strcat(encoded_url, lenStr);
        strcat(encoded_url, token);

        // Get next token
        token = strtok(NULL, ".");
    }
    // Append the final '0'
    strcat(encoded_url, "0");

    printf("Encoded URL: %s\n", encoded_url);
    return encoded_url;
}
