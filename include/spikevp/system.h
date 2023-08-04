#ifndef SPIKE_VP_SYSTEM_H
#define SPIKE_VP_SYSTEM_H

#include "vcml.h"
#include "spikevp/cpu.h"

namespace spikevp {

class system : public vcml::system
{
public:
    vcml::property<unsigned int> nrcpu;
    vcml::property<vcml::range> mem;

    system() = delete;
    system(const sc_core::sc_module_name& name);
    virtual ~system();

    // TODO : Q: ??
    VCML_KIND(::system);

    virtual const char* version() const override;

    virtual int run() override;

    virtual void end_of_elaboration() override;

private:
    std::vector<cpu*> m_cpus;

    vcml::generic::clock m_clock;
    vcml::generic::reset m_reset;
    vcml::generic::bus m_bus;
    vcml::generic::memory m_mem;
};

} // spikevp

#endif
