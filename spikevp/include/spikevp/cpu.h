#ifndef SPIKEVP_CPU_H
#define SPIKEVP_CPU_H

#include <cstring>

#include "riscv/processor.h"
#include "riscv/log_file.h"
#include "riscv/isa_parser.h"
#include "riscv/cfg.h"
#include "riscv/simif.h"
#include "riscv/cachesim.h"
// fix function name conflic between spike and vcml
#undef get_field
#undef set_field
#undef likely
#undef unlikely

#include "vcml.h"

namespace spikevp {

class cpu : public vcml::processor, public simif_t
{
private:
    processor_t *spike_core;

    /* simif_t */
    isa_parser_t isa;
    const cfg_t * const cfg;
    log_file_t log_file;
    std::ostream sout_; // used for socket and terminal interface
    static const size_t INTERLEAVE = 5000;
public:
    vcml::property<uint64_t> reset_vec;
    vcml::property<bool> enable_insn_dmi;
    vcml::property<bool> enable_data_dmi;
    vcml::property<std::string> icache_config; 
    vcml::property<std::string> dcache_config;
    vcml::property<bool> enable_simple_pc_trace;
    vcml::property<uint64_t> simple_pc_mod;
    vcml::property<bool> enable_icache_log;
    vcml::property<bool> enable_dcache_log;
    vcml::property<uint64_t> cache_blocksz;
    vcml::property<uint64_t> max_cycles;

    // TODO : workaround
    // TODO : add an interface for clint like timer
    vcml::riscv::clint *m_clint;

    cpu(const sc_core::sc_module_name& nm,
        unsigned int coreid,
        const cfg_t *cfg);

    /* vcml::processor */
    ~cpu();

    virtual void reset() override;

    virtual const char* version() const override;

    virtual void stacktrace(std::vector<vcml::debugging::stackframe>&,
                            size_t limit) override;

    virtual bool disassemble(vcml::u8*, vcml::u64&, std::string&) override;

    virtual vcml::u64 program_counter() override;
    virtual vcml::u64 link_register() override;
    virtual vcml::u64 stack_pointer() override;
    virtual vcml::u64 frame_pointer() override;

    virtual vcml::u64 core_id() override;
    virtual void set_core_id(vcml::u64 id);

    vcml::u64 cycle_count() const override;

    /* simif_t */
    // should return NULL for MMIO addresses
    virtual char* addr_to_mem(reg_t paddr) override;
    //virtual bool reservable(reg_t paddr) override { return addr_to_mem(paddr); }
    // TODO : it's workaround, need to be implemented in the future.
    virtual bool reservable(reg_t paddr) override { return true; }
    // used for MMIO addresses
    virtual bool mmio_fetch(reg_t paddr, size_t len, uint8_t* bytes) override;
    virtual bool mmio_load(reg_t paddr, size_t len, uint8_t* bytes) override;
    virtual bool mmio_store(reg_t paddr, size_t len, const uint8_t* bytes) override;
    // Callback for processors to let the simulation know they were reset.
    virtual void proc_reset(unsigned id) override;

    virtual const cfg_t &get_cfg() const override;
    virtual const std::map<size_t, processor_t*>& get_harts() const override;

    virtual const char* get_symbol(uint64_t paddr) override;

    unsigned nprocs() const { return get_cfg().nprocs(); }

protected:
    /* vcml::processor */
    virtual void interrupt(size_t irq, bool set) override;
    virtual void simulate(size_t cycles) override;
    virtual void handle_clock_update(vcml::hz_t prev, vcml::hz_t cur) override;

    virtual bool read_reg_dbg(size_t idx, void* buf, size_t len) override;
    virtual bool write_reg_dbg(size_t idx, const void*, size_t l) override;

    virtual bool page_size(vcml::u64& size) override;
    virtual bool virt_to_phys(vcml::u64 va, vcml::u64& pa) override;

    virtual bool insert_breakpoint(vcml::u64 addr) override;
    virtual bool remove_breakpoint(vcml::u64 addr) override;

    virtual bool insert_watchpoint(const vcml::range& mem,
                                   vcml::vcml_access acs) override;
    virtual bool remove_watchpoint(const vcml::range& mem,
                                   vcml::vcml_access acs) override;
};

} // namespace spikevp

#endif
