// File name: client.cpp

#include "rpc_client.hpp"
#include <iostream>

int main(int argc, char** argv) {
    const char* ip = (argc > 1) ? argv[1] : "127.0.0.1";
    RpcClient c;
    if (!rpc_connect(c, ip, 8080)) { std::cerr << "connect failed\n"; return 1; }

    long long out;
    if (rpc_add(c, 5, 7, out))  std::cout << "add = " << out << "\n";
    if (rpc_sub(c, 20, 4, out)) std::cout << "sub = " << out << "\n";
    if (rpc_mul(c, 6, 9, out))  std::cout << "mul = " << out << "\n";
    if (rpc_div(c, 20, 5, out)) std::cout << "div = " << out << "\n";

    rpc_close(c);
    return 0;
}