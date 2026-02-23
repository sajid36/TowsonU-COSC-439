#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define BUFSZ 1024

typedef int (*proc2_fn)(long long a, long long b, long long *out);

static int add_impl(long long a, long long b, long long *out) { *out = a + b; return 0; }
static int sub_impl(long long a, long long b, long long *out) { *out = a - b; return 0; }
static int mul_impl(long long a, long long b, long long *out) { *out = a * b; return 0; }
static int div_impl(long long a, long long b, long long *out) {
    if (b == 0) return -1;
    *out = a / b;
    return 0;
}

typedef struct {
    const char *name;
    int arity;
    proc2_fn fn;   // valid when arity is 2
} Proc;

static Proc procs[] = {
    { "add", 2, add_impl },
    { "sub", 2, sub_impl },
    { "mul", 2, mul_impl },
    { "div", 2, div_impl },
};

static Proc* find_proc(const char *name) {
    size_t n = sizeof(procs) / sizeof(procs[0]);
    for (size_t i = 0; i < n; ++i)
        if (strcmp(procs[i].name, name) == 0) return &procs[i];
    return NULL;
}

static int send_line(int fd, const char *s) {
    size_t n = strlen(s);
    return (int)write(fd, s, n) == (int)n;
}

int main(void) {
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) { perror("socket"); return 1; }

    int opt = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(sfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) { perror("bind"); return 1; }
    if (listen(sfd, 1) < 0) { perror("listen"); return 1; }
    printf("RPC server on port %d\n", PORT);

    struct sockaddr_in caddr;
    socklen_t clen = sizeof(caddr);
    int cfd = accept(sfd, (struct sockaddr*)&caddr, &clen);
    if (cfd < 0) { perror("accept"); return 1; }
    printf("client connected\n");

    char buf[BUFSZ];
    while (1) {
        ssize_t r = read(cfd, buf, sizeof(buf) - 1);
        if (r <= 0) break;
        buf[r] = '\0';

        // parse one line
        char name[32];
        long long a, b;
        if (sscanf(buf, "%31s %lld %lld", name, &a, &b) < 1) {
            send_line(cfd, "ERR bad_request\n");
            continue;
        }

        if (strcmp(name, "quit") == 0) { send_line(cfd, "OK bye\n"); break; }

        Proc *p = find_proc(name);
        if (!p) { send_line(cfd, "ERR no_such_proc\n"); continue; }
        if (p->arity != 2) { send_line(cfd, "ERR wrong_arity\n"); continue; }

        long long out = 0;
        int rc = p->fn(a, b, &out);
        if (rc == 0) {
            char line[64];
            snprintf(line, sizeof(line), "OK %lld\n", out);
            send_line(cfd, line);
        } else if (p == find_proc("div")) {
            send_line(cfd, "ERR divide_by_zero\n");
        } else {
            send_line(cfd, "ERR exec_failed\n");
        }
    }

    close(cfd);
    close(sfd);
    printf("server done\n");
    return 0;
}