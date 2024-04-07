#define EXIT    1
#define FORK    2
#define READ    3
#define WRITE   4
#define OPEN    5
#define CLOSE   6
#define EXECVE  11
#define CLONE   120
#define SEND    289
#define SENDTO  290
#define RECV    291
#define RECVFROM    292
#define SOCKET  281
#define BIND    282
#define LISTEN  284
#define ACCEPT  285

#define CTL_READ_FD 198
#define STATE_WRITE_FD 199

#define LOG_STATEMENT(...) \
    { \
        g_autofree gchar *log = g_strdup_printf(__VA_ARGS__); \
        qemu_plugin_outs(log); \
    }

#define LOG_STATEMENT_WITH_CTX(...) \
    g_autofree gchar *log = g_strdup_printf("ctx: %08x  " __VA_ARGS__, target_ctx); \
    qemu_plugin_outs(log);

#define LOG_MASK(cond) ((cond) && system_started && info->ctx == target_ctx)
#define QEMU_USER_MODE (g_strcmp0(QEMU_MODE, "USER") == 0)
#define QEMU_SYSTEM_MODE (g_strcmp0(QEMU_MODE, "USER") != 0)

typedef struct {
    uint32_t fd;
    const char *buf;
    uint32_t count;
} ReadParams;

typedef struct {
    uint32_t fd;
    const char *buf;
    uint32_t count;
} WriteParams;

typedef struct {
    const char *filename;
} OpenParams;

typedef struct {
    uint32_t fd;
} CloseParams;

typedef struct {
    const char *filename;
} ExecveParams;

typedef struct
{
    uint32_t    sockfd;
    const void  *buf;
    uint32_t    len;
    uint32_t    flags;
} SendParams;

typedef struct
{
    uint32_t    sockfd;
    const void  *buf;
    uint32_t    len;
    uint32_t    flags;
    uint32_t    dest_addr;
    uint32_t    dest_len;
} SendtoParams;

typedef struct
{
    uint32_t    sockfd;
    void        *buf;
    uint32_t    len;
    uint32_t    flags;
} RecvParams;

typedef struct
{
    uint32_t    sockfd;
    void        *buf;
    uint32_t    len;
    uint32_t    flags;
    uint32_t    src_addr;
    uint32_t    src_len;
} RecvfromParams;

typedef struct
{
    uint32_t    domain;
    uint32_t    type;
    uint32_t    protocol;
} SocketParams;

typedef struct
{
    uint32_t    sockfd;
    uint32_t    sock_addr;
    uint32_t    addr_len;
} BindParams;

typedef struct
{
    uint32_t    sockfd;
    uint32_t    backlog;
} ListenParams;

typedef struct
{
    uint32_t    sockfd;
    uint32_t    sock_addr;
    uint32_t    addr_len;
} AcceptParams;

typedef struct {
    uint32_t num;
    uint32_t ctx;
    union {
        ReadParams      *read_params;
        WriteParams     *write_params;
        OpenParams      *open_params;
        CloseParams     *close_params;
        ExecveParams    *execve_params;
        SendParams      *send_params;
        SendtoParams    *sendto_params;
        RecvParams      *recv_params;
        RecvfromParams  *recvfrom_params;
        SocketParams    *socket_params;
        ListenParams    *listen_params;
        BindParams      *bind_params;
        AcceptParams    *accept_params;
    } params;
} SyscallInfo;

typedef struct {
    uint32_t ctx;
    uint32_t pc;
} TBInfo;

typedef struct {
    uint32_t ctx;
    uint32_t addr;
    uint32_t paddr;
    uint32_t prot;
    uint32_t mmu_idx;
} TLBInfo;

#define SYSTEM_STARTED_INDICATOR_PROCESS "/usr/bin/phosphor-host-state-manager"

void afl_forkserver(void);

