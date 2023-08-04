#include "spikevp/cpu.h"
#include "spikevp/config.h"

namespace spikevp {

cpu::cpu(const sc_core::sc_module_name& nm,
         unsigned int id,
         const cfg_t *cfg):
    vcml::processor(nm, "riscv"),
    spike_core(NULL),
    isa(cfg->isa(), cfg->priv()),
    cfg(cfg),
    log_file(nullptr),
    sout_(nullptr) {

    // TODO
    // log_file_t log_file(nullptr); 
    // std::ostream sout_(nullptr);
    sout_.rdbuf(std::cerr.rdbuf()); // debug output goes to stderr by default

    spike_core = new processor_t(&isa, cfg, this, id, false,
                                 log_file.get(), sout_);
    spike_core->set_state_pc(SPIKEVP_RESETVEC);
}

cpu::~cpu() {
}

/* vcml::processor public */
void cpu::reset() {
    // assert(0);
}

const char *cpu::version() const {
    assert(0);
    return "";
}

void cpu::stacktrace(std::vector<vcml::debugging::stackframe>&,
                size_t limit) {
    assert(0);
}

bool cpu::disassemble(vcml::u8*, vcml::u64&, std::string&) {
    assert(0);
    return false;
}

vcml::u64 cpu::program_counter() {
    vcml::u64 ret = 0;
    assert(0);
    return ret;
}

vcml::u64 cpu::link_register() {
    vcml::u64 ret = 0;
    assert(0);
    return ret;
}

vcml::u64 cpu::stack_pointer() {
    vcml::u64 ret = 0;
    assert(0);
    return ret;
}

vcml::u64 cpu::frame_pointer() {
    vcml::u64 ret = 0;
    assert(0);
    return ret;
}

vcml::u64 cpu::core_id() {
    vcml::u64 ret = 0;
    assert(0);
    return ret;
}

void cpu::set_core_id(vcml::u64 id) {
    assert(0);
}

vcml::u64 cpu::cycle_count() const {
    vcml::u64 ret = spike_core->get_num_cycles();
    return ret;
}

/* vcml::processor protect */
// TODO
void cpu::interrupt(size_t irq, bool set) {
    assert(0);
}

void cpu::simulate(size_t cycles) {
    spike_core->step(cycles);
}

void cpu::handle_clock_update(vcml::hz_t prev, vcml::hz_t cur) {
    assert(0);
}
bool cpu::read_reg_dbg(size_t idx, void* buf, size_t len) {
    assert(0);
    return false;
}

bool cpu::write_reg_dbg(size_t idx, const void*, size_t l) {
    assert(0);
    return false;
}

bool cpu::page_size(vcml::u64& size) {
    assert(0);
    return false;
}

bool cpu::virt_to_phys(vcml::u64 va, vcml::u64& pa) {
    assert(0);
    return false;
}

bool cpu::insert_breakpoint(vcml::u64 addr) {
    assert(0);
    return false;
}

bool cpu::remove_breakpoint(vcml::u64 addr) {
    assert(0);
    return false;
}

bool cpu::insert_watchpoint(const vcml::range& mem,
                            vcml::vcml_access acs) {
    assert(0);
    return false;
}

bool cpu::remove_watchpoint(const vcml::range& mem,
                       vcml::vcml_access acs) {
    assert(0);
    return false;
}

/* spike::simif_t */
// should return NULL for MMIO addresses
char* cpu::addr_to_mem(reg_t paddr) {
    // Now we don't support the DMI,
    // so we always return NULL.
    return NULL;
}


bool cpu::mmio_fetch(reg_t paddr, size_t len, uint8_t* bytes) {
    vcml::tlm_sbi info = vcml::SBI_NONE;
    tlm::tlm_response_status rs;
    vcml::tlm_initiator_socket& port = insn;

    sc_core::sc_time now = local_time_stamp();

    unsigned int nbytes = 0;
    rs = port.read(paddr, bytes, len, info, &nbytes);

    // Time-keeping
    // TODO

    // Check bus error 
    if (rs != tlm::TLM_OK_RESPONSE || nbytes != len) {
        return false;
    }
    return true;

    // dmi handling
    // TODO
}

bool cpu::mmio_load(reg_t paddr, size_t len, uint8_t* bytes) {
    vcml::tlm_sbi info = vcml::SBI_NONE;
    tlm::tlm_response_status rs;
    vcml::tlm_initiator_socket& port = data;

    sc_core::sc_time now = local_time_stamp();

    unsigned int nbytes = 0;
    rs = port.read(paddr, bytes, len, info, &nbytes);

    // Time-keeping
    // TODO

    // Check bus error 
    if (rs != tlm::TLM_OK_RESPONSE || nbytes != len) {
        return false;
    }
    return true;

    // dmi handling
    // TODO
}

bool cpu::mmio_store(reg_t paddr, size_t len, const uint8_t* bytes) {
    vcml::tlm_sbi info = vcml::SBI_NONE;
    tlm::tlm_response_status rs;
    vcml::tlm_initiator_socket& port = data;

    sc_core::sc_time now = local_time_stamp();

    unsigned int nbytes = 0;
    rs = port.write(paddr, bytes, len, info, &nbytes);

    // Time-keeping
    // TODO

    // Check bus error 
    if (rs != tlm::TLM_OK_RESPONSE || nbytes != len) {
        return false;
    }
    return true;
}

// Callback for processors to let the simulation know they were reset.
void cpu::proc_reset(unsigned id) {
    /*
     * when we new the processor,
     * we don't have the pointer.
     */
    if (spike_core) {
        spike_core->set_state_pc(SPIKEVP_RESETVEC);
    }
}

const cfg_t &cpu::get_cfg() const {
    return *cfg;
}

const std::map<size_t, processor_t*>& cpu::get_harts() const {
    // hope that we don't use this method.
    assert(0);
    return {};
}

const char* cpu::get_symbol(uint64_t paddr) {
    assert(0);
    return NULL;
}

} // namespace spikevp
