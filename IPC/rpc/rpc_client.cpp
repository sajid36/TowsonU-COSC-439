// File name: rpc_client.cpp

#include "rpc_client.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <string>

static bool call_rpc(int fd, const char* name, long long a, long long b, long long& out) {
    std::string req = std::string(name) + " " + std::to_string(a) + " " + std::to_string(b) + "\n";
    if (write(fd, req.c_str(), req.size()) < 0) return false;

    char buf[1024];
    ssize_t r = read(fd, buf, sizeof(buf) - 1);
    if (r <= 0) return false;
    buf[r] = '\0';

    if (sscanf(buf, "OK %lld", &out) == 1) return true;
    return false; // ERR ... cases
}

bool rpc_connect(RpcClient& c, const char* ip, uint16_t port) {
    c.fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (c.fd < 0) return false;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) return false;
    if (::connect(c.fd, (sockaddr*)&addr, sizeof(addr)) < 0) return false;

    return true;
}

void rpc_close(RpcClient& c) {
    if (c.fd >= 0) {
        const char* quit = "quit\n";
        write(c.fd, quit, strlen(quit));
        close(c.fd);
        c.fd = -1;
    }
}

bool rpc_add(RpcClient& c, long long a, long long b, long long& out) { return call_rpc(c.fd, "add", a, b, out); }
bool rpc_sub(RpcClient& c, long long a, long long b, long long& out) { return call_rpc(c.fd, "sub", a, b, out); }
bool rpc_mul(RpcClient& c, long long a, long long b, long long& out) { return call_rpc(c.fd, "mul", a, b, out); }
bool rpc_div(RpcClient& c, long long a, long long b, long long& out) { return call_rpc(c.fd, "div", a, b, out); }