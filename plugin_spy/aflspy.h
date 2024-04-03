#define EXIT    1
#define FORK    2
#define READ    3
#define WRITE   4
#define OPEN    5
#define CLOSE  6
#define EXECVE  11
#define CLONE   120


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


typedef struct {
    uint32_t num;
    uint32_t ctx;
    union {
        ReadParams      *read_params;
        WriteParams     *write_params;
        OpenParams      *open_params;
        CloseParams     *close_params;
        ExecveParams    *execve_params;
    } params;
} SyscallInfo;



