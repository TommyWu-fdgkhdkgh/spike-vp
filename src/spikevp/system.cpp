#include "spikevp/system.h"
#include "spikevp/cpu.h"
#include "spikevp/config.h"

namespace spikevp {

system::system(const sc_core::sc_module_name& nm):
    vcml::system(nm),
    nrcpu("nrcpu", 1),
    mem("mem", vcml::range(SPIKEVP_MEM_ADDR, SPIKEVP_MEM_END)),
    m_cpus(nrcpu),
    m_clock("clock", SPIKEVP_CPU_DEFCLK),
    m_reset("reset"),
    m_bus("bus"),
    m_mem("mem", mem.get().length()) {

    /* 
     * default cfg 
     * TODO : need to be configurable in the future
     */
    cfg_arg_t<size_t> nprocs(1);
    cfg_t cfg(/*default_initrd_bounds=*/std::make_pair((reg_t)0, (reg_t)0),
              /*default_bootargs=*/nullptr,
              /*default_isa=*/"RV64IMAFDC_zicntr_zihpm",
              /*default_priv=*/"MSU",
              /*default_varch=*/"vlen:128,elen:64",
              /*default_misaligned=*/false,
              /*default_endianness*/endianness_little,
              /*default_pmpregions=*/16,
              /*default_mem_layout=*/{},
              /*default_hartids=*/std::vector<size_t>(),
              /*default_real_time_clint=*/false,
              /*default_trigger_count=*/4);

    for (unsigned int cpu_id = 0; cpu_id < nrcpu; cpu_id++) {
        std::stringstream ss;
        ss << "cpu" << cpu_id;
        m_cpus[cpu_id] = new cpu(ss.str().c_str(), cpu_id, &cfg);
    }

    // Bus mapping
    for (cpu* now_cpu : m_cpus) {
        m_bus.bind(now_cpu->insn);
        m_bus.bind(now_cpu->data);
    }
    m_bus.bind(m_mem.in, mem);

    // clock
    for (auto cpu : m_cpus) {
        m_clock.clk.bind(cpu->clk);
    }
    m_clock.clk.bind(m_bus.clk);
    m_clock.clk.bind(m_mem.clk);

    // reset
    for (auto cpu : m_cpus) {
        m_reset.rst.bind(cpu->rst);
    }
    m_reset.rst.bind(m_bus.rst);
    m_reset.rst.bind(m_mem.rst);
}

system::~system() {
}

const char* system::version() const {
    return SPIKEVP_VERSION_STRING;
}

int system::run() {
    double simstart = mwr::timestamp();
    int result = vcml::system::run();
    double realtime = mwr::timestamp() - simstart;
    double duration = sc_core::sc_time_stamp().to_seconds();

    log_info("duration           %.9fs", duration);
    log_info("runtime            %.4fs", realtime);
    log_info("real time ratio    %.2fs / 1s",
             duration == 0.0 ? 0.0 : realtime / duration);

    vcml::u64 ninsn = 0;

    log_info("sim speed          %.1f MIPS",
             realtime == 0.0 ? 0.0 : ninsn / realtime / 1e6);

    return result;
}

void system::end_of_elaboration() {
    std::stringstream ss;
    vcml::log_debug("%s", ss.str().c_str());
}

} // namespace or1kmvp
