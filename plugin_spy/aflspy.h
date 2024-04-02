#define EXIT    1
#define FORK    2
#define EXECVE  11
#define CLONE   120



typedef struct {
    const char *filename;
} ExecveParams;


typedef struct {
    uint32_t num;
    uint32_t ctx;
    union {
        ExecveParams *execve_params;
    } params;
    
} SyscallInfo;



