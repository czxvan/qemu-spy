#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <qemu-plugin-spy.h>

QEMU_PLUGIN_EXPORT int qemu_plugin_version = QEMU_PLUGIN_VERSION;

static void vcpu_insn_trans(qemu_plugin_id_t id,
                            CPUState *cpu, CPUArchState *env,
                            uint32_t insn)
{
    g_autofree gchar *log = g_strdup_printf("insn: %08x\n", insn);
    qemu_plugin_outs(log);
}


QEMU_PLUGIN_EXPORT int qemu_plugin_install(qemu_plugin_id_t id,
                                           const qemu_info_t *info, int argc,
                                           char **argv)
{

    /* Register init, translation block and exit callbacks */
    // qemu_plugin_register_vcpu_init_cb(id, vcpu_init);
    // qemu_plugin_register_vcpu_tb_trans_cb(id, vcpu_tb_trans);
    qemu_plugin_register_vcpu_insn_trans_cb(id, vcpu_insn_trans);

    return 0;
}
