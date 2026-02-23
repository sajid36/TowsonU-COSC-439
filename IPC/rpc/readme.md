RPC Socket Example

Build Instructions

Compile Server

g++ -std=c++17 -Wall -Wextra -g rpcServer.cpp -o rpcServer

Compile Client

g++ -std=c++17 -Wall -Wextra -g client.cpp rpc_client.cpp -o client

Run Instructions

Step 1: Start the Server

Open Terminal 1 and run:

./rpcServer

You should see:

RPC server on port 8080

Step 2: Run the Client

Open Terminal 2 and run:

./client

If running on a different machine, specify the server IP:

./client <server_ip>

Example:

./client 192.168.1.10

Expected Output

Client output:

add = 12
sub = 16
mul = 54
div = 4

Server output:

client connected
server done

Notes

Make sure the following files are in the same directory:

rpcServer.cpp

client.cpp

rpc_client.cpp

rpc_client.hpp

Always compile using g++ (not gcc).

