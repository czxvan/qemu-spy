/*
 * Copyright (C) 2017, Emilio G. Cota <cota@braap.org>
 *
 * License: GNU GPL, version 2 or later.
 *   See the COPYING file in the top-level directory.
 *
 * plugin-gen.h - TCG-dependent definitions for generating plugin code
 *
 * This header should be included only from plugin.c and C files that emit
 * TCG code.
 */
#ifndef QEMU_PLUGIN_GEN_H
#define QEMU_PLUGIN_GEN_H

#include "tcg/tcg.h"

struct DisasContextBase;

#ifdef CONFIG_PLUGIN

bool plugin_gen_tb_start(CPUState *cpu, const struct DisasContextBase *db,
                         bool supress);
void plugin_gen_tb_end(CPUState *cpu, size_t num_insns);
void plugin_gen_insn_start(CPUState *cpu, const struct DisasContextBase *db);
void plugin_gen_insn_end(void);

void plugin_gen_tb_trans_spy(CPUState *cpu, const struct DisasContextBase *db);
void plugin_gen_insn_trans_spy(CPUState *cpu, const struct DisasContextBase *db);
void plugin_gen_exception_spy(CPUState *cpu, uint32_t excp,
                     uint32_t syndrome, uint32_t target_el);
void plugin_gen_tlb_set_spy(CPUState* cpu, vaddr addr, hwaddr paddr, int prot, int mmu_idx);

void plugin_gen_disable_mem_helpers(void);
void plugin_gen_empty_mem_callback(TCGv_i64 addr, uint32_t info);

#else /* !CONFIG_PLUGIN */

static inline bool
plugin_gen_tb_start(CPUState *cpu, const struct DisasContextBase *db, bool sup)
{
    return false;
}

static inline
void plugin_gen_insn_start(CPUState *cpu, const struct DisasContextBase *db)
{ }

static inline void plugin_gen_insn_end(void)
{ }

static inline
void plugin_gen_tb_trans_spy(CPUState *cpu, const struct DisasContextBase *db)
{ }

static inline
void plugin_gen_insn_trans_spy(CPUState *cpu, const struct DisasContextBase *db)
{ }

static inline
void plugin_gen_exception_spy(CPUState *cpu, uint32_t excp,
                     uint32_t syndrome, uint32_t target_el)
{ }

static inline
void plugin_gen_tlb_set_spy(CPUState* cpu, vaddr addr, hwaddr paddr, int prot, int mmu_idx)
{ }

static inline void plugin_gen_tb_end(CPUState *cpu, size_t num_insns)
{ }

static inline void plugin_gen_disable_mem_helpers(void)
{ }

static inline void plugin_gen_empty_mem_callback(TCGv_i64 addr, uint32_t info)
{ }

#endif /* CONFIG_PLUGIN */

#endif /* QEMU_PLUGIN_GEN_H */

