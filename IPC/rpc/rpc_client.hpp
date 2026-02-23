#ifndef RPC_CLIENT_HPP
#define RPC_CLIENT_HPP

#include <cstdint>

struct RpcClient {
    int fd = -1;
};

bool rpc_connect(RpcClient& c, const char* ip, uint16_t port);
void rpc_close(RpcClient& c);

bool rpc_add(RpcClient& c, long long a, long long b, long long& out);
bool rpc_sub(RpcClient& c, long long a, long long b, long long& out);
bool rpc_mul(RpcClient& c, long long a, long long b, long long& out);
bool rpc_div(RpcClient& c, long long a, long long b, long long& out);

#endif