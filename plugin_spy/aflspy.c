#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <qemu-plugin-spy.h>
#include "aflspy.h"

QEMU_PLUGIN_EXPORT int qemu_plugin_version = QEMU_PLUGIN_VERSION;

static const char *QEMU_MODE = NULL;
static uint32_t target_ctx = 0;
static gboolean system_started = false;


void vcpu_insn_trans(qemu_plugin_id_t id,
                            CPUState *cpu, CPUArchState *env,
                            uint32_t insn)
{
    static uint64_t count = 0;
    count++;
    if (count % 100000 == 0) {
        g_autofree gchar *log = g_strdup_printf("count: %ld, insn: %08x\n", count, insn);
        qemu_plugin_outs(log);
    }
}

void vcpu_tb_exec_spy(qemu_plugin_id_t id,
                        CPUState *cpu, CPUArchState *env,
                        void *data)
{
    TBInfo *info = (TBInfo *)data;
    gboolean log_tb_exec_spy = LOG_MASK(true);
    
    if (log_tb_exec_spy)
        LOG_STATEMENT("ctx: %08x  tb_exec pc: %08x\n"
                        , info->ctx
                        , info->pc);
}

void vcpu_syscall_spy(qemu_plugin_id_t id,
                            CPUState *cpu, CPUArchState *env,
                            void *data)
{
    SyscallInfo *info = (SyscallInfo *)data;

    gboolean log_exit = LOG_MASK(true);
    gboolean log_fork = LOG_MASK(true);
    gboolean log_read = LOG_MASK(true) || QEMU_USER_MODE;
    gboolean log_write = LOG_MASK(true) || QEMU_USER_MODE;
    gboolean log_open = LOG_MASK(true) || QEMU_USER_MODE;
    gboolean log_close = LOG_MASK(false) || QEMU_USER_MODE;
    gboolean log_execve = LOG_MASK(true);
    gboolean log_clone = LOG_MASK(true);
    gboolean log_send = LOG_MASK(true) || QEMU_USER_MODE;
    gboolean log_sendto = LOG_MASK(true) || QEMU_USER_MODE;
    gboolean log_recv = LOG_MASK(true) || QEMU_USER_MODE;
    gboolean log_recvfrom = LOG_MASK(true) || QEMU_USER_MODE;

    gboolean log_socket = LOG_MASK(true) || QEMU_USER_MODE;
    gboolean log_bind = LOG_MASK(true) || QEMU_USER_MODE;
    gboolean log_listen = LOG_MASK(true) || QEMU_USER_MODE;
    gboolean log_accept = LOG_MASK(true) || QEMU_USER_MODE;
    gboolean log_default = LOG_MASK(false);

    switch (info->num) {
        case EXIT: {
            if(log_exit) {
                g_autofree gchar *log = g_strdup_printf(
                    "ctx: %08x  exit\n"
                    , info->ctx
                );
                qemu_plugin_outs(log);
            }
        } break;
        case FORK: {
            if(log_fork) {
                g_autofree gchar *log = g_strdup_printf(
                    "ctx: %08x  fork\n"
                    , info->ctx
                );
                qemu_plugin_outs(log);
            }  
        } break;
        case READ: {
            if(log_read) {
                g_autofree gchar *log = g_strdup_printf(
                    "ctx: %08x  read  from %d %d bytes\n"
                    , info->ctx
                    , info->params.read_params->fd
                    , info->params.read_params->count
                );
                qemu_plugin_outs(log);
            }
        } break;
        case WRITE: {
            if (log_write) {
                if (info->params.write_params->fd == 5 &&
                    info->params.write_params->count == 8 ||
                    info->params.write_params->fd == 7 &&
                    info->params.write_params->count == 1)
                    break;
                g_autofree gchar *log = g_strdup_printf(
                    "ctx: %08x  write  into %d %d bytes\n"
                    "\t%s\n"
                    , info->ctx
                    , info->params.write_params->fd
                    , info->params.write_params->count
                    , info->params.write_params->buf
                );
                qemu_plugin_outs(log);
            }
        } break;
        case OPEN: {
            if(log_open) {
                g_autofree gchar *log = g_strdup_printf(
                    "ctx: %08x  open  %s\n"
                    , info->ctx
                    , info->params.open_params->filename
                );
                qemu_plugin_outs(log);
            }
        } break;
        case CLOSE: {
            if(log_close) {
                g_autofree gchar *log = g_strdup_printf(
                    "ctx: %08x  close  fd: %d\n"
                    , info->ctx
                    , info->params.close_params->fd
                );
                qemu_plugin_outs(log);
            }
        } break;
        case EXECVE: {
            if (strstr(info->params.execve_params->filename, SYSTEM_STARTED_INDICATOR_PROCESS) != NULL) {
                system_started = true;
            }
            if (log_execve) {
                g_autofree gchar *log = g_strdup_printf(
                    "ctx: %08x  execve  %s\n"
                    , info->ctx
                    , info->params.execve_params->filename
                );
                qemu_plugin_outs(log);
            }
        } break;
        case CLONE: {
            if (log_clone) {
                g_autofree gchar *log = g_strdup_printf(
                    "ctx: %08x  clone\n"
                    , info->ctx
                );
                qemu_plugin_outs(log);
            }
        } break;
        case SEND: {
            if (log_send) {
                g_autofree gchar *log = g_strdup_printf(
                    "ctx: %08x  send %d bytes to %d, flags: %d\n"
                    "\t%s\n"
                    , info->ctx
                    , info->params.send_params->len
                    , info->params.send_params->sockfd
                    , info->params.send_params->flags
                    , (char*)info->params.send_params->buf
                );
                qemu_plugin_outs(log);

                const uint8_t *buf = info->params.send_params->buf;
                for (int i = 0; i < info->params.send_params->len; i++) {
                    if ((i+1) % 11 == 0) {
                        log = g_strdup_printf("%02x\n", buf[i]);
                    } else {
                        log = g_strdup_printf("%02x ", buf[i]);
                    }
                    qemu_plugin_outs(log);
                }
                qemu_plugin_outs("\n");
            }
        } break;
        case SENDTO: {
            if (log_sendto) {
                g_autofree gchar *log = g_strdup_printf(
                    "ctx: %08x  sendto %d bytes to %d, flags: %d, dest_addr: %08x, dest_len: %d\n"
                    "\t%s\n"
                    , info->ctx
                    , info->params.sendto_params->len
                    , info->params.sendto_params->sockfd
                    , info->params.sendto_params->flags
                    , info->params.sendto_params->dest_addr
                    , info->params.sendto_params->dest_len
                    , (char*)info->params.sendto_params->buf
                );
                qemu_plugin_outs(log);

                const uint8_t *buf = info->params.sendto_params->buf;
                for (int i = 0; i < info->params.sendto_params->len; i++) {
                    if ((i+1) % 11 == 0) {
                        log = g_strdup_printf("%02x\n", buf[i]);
                    } else {
                        log = g_strdup_printf("%02x ", buf[i]);
                    }
                    qemu_plugin_outs(log);
                }
                qemu_plugin_outs("\n");
            }
        } break;
        case RECV: {
            if (log_recv) {
                g_autofree gchar *log = g_strdup_printf(
                    "ctx: %08x  recv %d bytes from %d, flags: %d\n"
                    , info->ctx
                    , info->params.recv_params->len
                    , info->params.recv_params->sockfd
                    , info->params.recv_params->flags
                );
                qemu_plugin_outs(log);
            }
        } break;
        case RECVFROM: {
            if (log_recvfrom) {
                g_autofree gchar *log = g_strdup_printf(
                    "ctx: %08x  recvfrom %d bytes from %d, flags: %d, src_addr: %08x, src_len: %d\n"
                    , info->ctx
                    , info->params.recvfrom_params->len
                    , info->params.recvfrom_params->sockfd
                    , info->params.recvfrom_params->flags
                    , info->params.recvfrom_params->src_addr
                    , info->params.recvfrom_params->src_len
                );
                qemu_plugin_outs(log);
            }
        } break;
        case SOCKET: {
            if (log_socket) {
                g_autofree gchar *log = g_strdup_printf(
                    "ctx: %08x  socket domain: %08x, type: %d, protocol: %d\n"
                    , info->ctx
                    , info->params.socket_params->domain
                    , info->params.socket_params->type
                    , info->params.socket_params->protocol
                );
                qemu_plugin_outs(log);
            }
        } break;
        case BIND: {
            if (log_bind) {
                g_autofree gchar *log = g_strdup_printf(
                    "ctx: %08x  bind sockfd: %d, sock_addr: %08x, addr_len: %d\n"
                    , info->ctx
                    , info->params.bind_params->sockfd
                    , info->params.bind_params->sock_addr
                    , info->params.bind_params->addr_len
                );
                qemu_plugin_outs(log);
            }
        } break;
        case LISTEN: {
            if (log_listen) {
                g_autofree gchar *log = g_strdup_printf(
                    "ctx: %08x  listen sockfd: %d, backlog: %d\n"
                    , info->ctx
                    , info->params.listen_params->sockfd
                    , info->params.listen_params->backlog
                );
                qemu_plugin_outs(log);
            }
        } break;
        case ACCEPT: {
            if (system_started && !target_ctx) {
                target_ctx = info->ctx;
            }
            if (log_accept) {
                g_autofree gchar *log = g_strdup_printf(
                    "ctx: %08x  accept sockfd: %d, sock_addr: %08x, addr_len: %d\n"
                    , info->ctx
                    , info->params.accept_params->sockfd
                    , info->params.accept_params->sock_addr
                    , info->params.accept_params->addr_len
                );
                qemu_plugin_outs(log);
            }
        } break;
        default: {
            if (log_default) {
                g_autofree gchar *log = g_strdup_printf(
                        "ctx: %08x  unnamed syscall %d\n"
                        , info->ctx
                        , info->num
                    );
                    qemu_plugin_outs(log);
            }
        }
    }
}

QEMU_PLUGIN_EXPORT int qemu_plugin_install(qemu_plugin_id_t id,
                                           const qemu_info_t *info, int argc,
                                           char **argv)
{
    QEMU_MODE = getenv("QEMU_MODE");
    /* Register init, translation block and exit callbacks */
    // qemu_plugin_register_vcpu_init_cb(id, vcpu_init);
    // qemu_plugin_register_vcpu_tb_trans_cb(id, vcpu_tb_trans);
    qemu_plugin_register_vcpu_insn_trans_cb(id, vcpu_insn_trans);
    qemu_plugin_register_vcpu_syscall_spy_cb(id, vcpu_syscall_spy);
    qemu_plugin_register_vcpu_tb_exec_spy_cb(id, vcpu_tb_exec_spy);
    return 0;
}
