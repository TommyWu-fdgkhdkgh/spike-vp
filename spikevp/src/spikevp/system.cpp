#include "spikevp/system.h"
#include "spikevp/cpu.h"
#include "spikevp/config.h"

namespace spikevp {

system::system(const sc_core::sc_module_name& nm):
    vcml::system(nm),
    nrcpu("nrcpu", 1),
    mem("mem", vcml::range(SPIKEVP_MEM_ADDR, SPIKEVP_MEM_END)),
    mrom("mrom", vcml::range(SPIKEVP_MROM_ADDR, SPIKEVP_MROM_END)),
    plic("plic", vcml::range(SPIKEVP_PLIC_ADDR, SPIKEVP_PLIC_END)),
    clint("clint", vcml::range(SPIKEVP_CLINT_ADDR, SPIKEVP_CLINT_END)),
    uart("uart", vcml::range(SPIKEVP_UART_ADDR, SPIKEVP_UART_END)),
    m_cpus(nrcpu),
    m_clock("clock", SPIKEVP_CPU_DEFCLK),
    m_reset("reset"),
    m_bus("bus"),
    m_mem("mem", mem.get().length()),
    m_mrom("mrom", mrom.get().length()),
    m_plic("plic"),
    m_clint("clint"),
    m_uart("uart"),
    m_term("term") {

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

    m_uart.set_big_endian();

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
    m_bus.bind(m_mrom.in, mrom);
    m_bus.bind(m_plic.in, plic);
    m_bus.bind(m_clint.in, clint);
    m_bus.bind(m_uart.in, uart);

    // IRQ mapping
    unsigned int hartid_base = 0;
    for (unsigned int cpu_id = 0; cpu_id < nrcpu; cpu_id++) {
        m_cpus[cpu_id]->irq[SPIKEVP_IRQ_M_EXT].bind(m_plic.irqt[cpu_id - hartid_base]);
        m_cpus[cpu_id]->irq[SPIKEVP_IRQ_S_EXT].bind(m_plic.irqt[cpu_id - hartid_base + nrcpu]);
        m_cpus[cpu_id]->irq[SPIKEVP_IRQ_M_SOFT].bind(m_clint.irq_sw[cpu_id]);
        m_cpus[cpu_id]->irq[SPIKEVP_IRQ_M_TIMER].bind(m_clint.irq_timer[cpu_id]);
    }
    m_plic.irqs[SPIKEVP_UART_IRQ].bind(m_uart.irq);

    // clock
    for (auto cpu : m_cpus) {
        m_clock.clk.bind(cpu->clk);
    }
    m_clock.clk.bind(m_bus.clk);
    m_clock.clk.bind(m_mem.clk);
    m_clock.clk.bind(m_mrom.clk);
    m_clock.clk.bind(m_plic.clk);
    m_clock.clk.bind(m_clint.clk);
    m_clock.clk.bind(m_uart.clk);

    // reset
    for (auto cpu : m_cpus) {
        m_reset.rst.bind(cpu->rst);
    }
    m_reset.rst.bind(m_bus.rst);
    m_reset.rst.bind(m_mem.rst);
    m_reset.rst.bind(m_mrom.rst);
    m_reset.rst.bind(m_plic.rst);
    m_reset.rst.bind(m_clint.rst);
    m_reset.rst.bind(m_uart.rst);

    // Serial connections
    m_uart.serial_tx.bind(m_term.serial_rx);
    m_term.serial_tx.bind(m_uart.serial_rx);

    // workaround pointers
    // spike need this ;(
    for (cpu* now_cpu : m_cpus) {
        now_cpu->m_clint = &m_clint;
    }
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
    for (auto cpu : m_cpus) {
        ninsn += cpu->insn_count();
    }

    log_info("sim speed          %.1f MIPS",
             realtime == 0.0 ? 0.0 : ninsn / realtime / 1e6);

    // get per cpu informations
    /*
    for (auto cpu : mcpus) {
        cpu->get_info();
    }
     */

    return result;
}

void system::end_of_elaboration() {
    std::stringstream ss;
    vcml::log_debug("%s", ss.str().c_str());
}

} // namespace spikevp
