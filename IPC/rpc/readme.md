# RPC Socket Example

## Build Instructions

### Compile Server

```bash
g++ -std=c++17 -Wall -Wextra -g rpcServer.cpp -o rpcServer
```

### Compile Client

```bash
g++ -std=c++17 -Wall -Wextra -g client.cpp rpc_client.cpp -o client
```

## Run Instructions

### Step 1: Start the Server

Open Terminal 1 and run:

```bash
./rpcServer
```

You should see:

```text
RPC server on port 8080
```

### Step 2: Run the Client

Open Terminal 2 and run:

```bash
./client
```

If running on a different machine, specify the server IP:

```bash
./client <server_ip>
```

Example:

```bash
./client 192.168.1.10
```

## Expected Output

### Client Output

```text
add = 12
sub = 16
mul = 54
div = 4
```

### Server Output

```text
client connected
server done
```

## Notes

1. Make sure these files are in the same directory:

```text
rpcServer.cpp
client.cpp
rpc_client.cpp
rpc_client.hpp
```

2. Always compile using g++ not gcc.
