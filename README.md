# spike-vp

A proof of concept virtual platform that use Spike as the ISS, and use VCML as the SystemC library.

# install dependencies

## SystemC

```
git clone git@github.com:TommyWu-fdgkhdkgh/systemc.git

cd systemc

git checkout 2.3.3

export SYSTEMC_HOME=xxx/spike-vp/install-systemc-2.3.3
mkdir objdir
cd objdir

../configure --prefix=xxx/spike-vp/install-systemc-2.3.3 CXXFLAGS="-g -std=c++17" LDFLAGS="-std=c++17"

make
make install
```

## VCML

```
git clone git@github.com:TommyWu-fdgkhdkgh/vcml.git

cd vcml

git submodule init
git submodule update

export SYSTEMC_HOME=xxx/spike-vp/install-systemc-2.3.3
export TARGET_ARCH=linux64

mkdir -p BUILD/DEBUG/BUILD
cd BUILD/DEBUG/BUILD
cmake -DCMAKE_INSTALL_PREFIX=xxx/spike-vp/install-vcml -DCMAKE_BUILD_TYPE=DEBUG ../../../
make -j 4
make install
make -C mwr install
```

## spike


```
git clone git@github.com:TommyWu-fdgkhdkgh/riscv-isa-sim.git

git checkout spike-for-systemc

mkdir build
cd build

../configure  --prefix=xxx/spike-vp/install-riscv-isa-sim CXXFLAGS="-g -std=c++17" LDFLAGS="-g -std=c++17"
make -j6
make install
```

# compile spike-vp

```
make clean 
make
```

# run spike-vp

```
./spike-vp -f ./config/up-fw-jump.cfg
```

# Updates

2024/2/4

Optimize the memory access strategy of Spike for non-dmi access.
With this optimization, it takes 160 seconds to boot the linux kernel.
It's nine times faster than the original memory access strategy.

---

2024/2/3

Add Spike L1DCache and L1ICache simulation.
Add a config for running the dhrystone, the dhrystone binary is from the chipyard project.

---

2023/8/14

If the memory simulation support DMI, it takes 1 minute to boot the linux kernel.

---

2023/8/9

If we don't use dmi,
it takes 25 minutes to boot the linux kernel.
