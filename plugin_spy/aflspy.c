#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <qemu-plugin-spy.h>
#include "aflspy.h"
QEMU_PLUGIN_EXPORT int qemu_plugin_version = QEMU_PLUGIN_VERSION;


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


void vcpu_syscall_spy(qemu_plugin_id_t id,
                            CPUState *cpu, CPUArchState *env,
                            void *data)
{

    SyscallInfo *info = (SyscallInfo *)data;
    switch (info->num) {
        case EXIT: {
            g_autofree gchar *log = g_strdup_printf(
                "ctx: %08x  exit\n"
                , info->ctx
            );
            qemu_plugin_outs(log);
        } break;
        case FORK: {
            g_autofree gchar *log = g_strdup_printf(
                "ctx: %08x  fork\n"
                , info->ctx
            );
            qemu_plugin_outs(log);
        } break;
        case READ: {
            // g_autofree gchar *log = g_strdup_printf(
            //     "ctx: %08x  read  from %d %d bytes\n"
            //     , info->ctx
            //     , info->params.read_params->fd
            //     , info->params.read_params->count
            // );
            // qemu_plugin_outs(log);
        } break;
        case WRITE: {
            if (info->params.write_params->fd == 5 &&
                    info->params.write_params->count == 8)
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
        } break;
        case EXECVE: {
            g_autofree gchar *log = g_strdup_printf(
                "ctx: %08x  execve  %s\n"
                , info->ctx
                , info->params.execve_params->filename
            );
            qemu_plugin_outs(log);
        } break;
        case CLONE: {
            g_autofree gchar *log = g_strdup_printf(
                "ctx: %08x  clone\n"
                , info->ctx
            );
            qemu_plugin_outs(log);
        } break;
    }
}

QEMU_PLUGIN_EXPORT int qemu_plugin_install(qemu_plugin_id_t id,
                                           const qemu_info_t *info, int argc,
                                           char **argv)
{

    /* Register init, translation block and exit callbacks */
    // qemu_plugin_register_vcpu_init_cb(id, vcpu_init);
    // qemu_plugin_register_vcpu_tb_trans_cb(id, vcpu_tb_trans);
    qemu_plugin_register_vcpu_insn_trans_cb(id, vcpu_insn_trans);
    qemu_plugin_register_vcpu_syscall_spy_cb(id, vcpu_syscall_spy);

    return 0;
}
