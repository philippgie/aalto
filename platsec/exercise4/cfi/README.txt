# Pre-conditions

# Clang installed with support for aarch64, initially the ubuntu one should work.
# Download and installed the sysroot for aarch64 from linaro, e.g: https://releases.linaro.org/components/toolchain/binaries/latest-7/aarch64-linux-gnu/ (from 2019) You can get something newer if you want.
# Change the sysroot path in the Makefile to point to your sysroot.

# Build instruction (note: debug is enabled by default in the makefile)
$ make cfi

# Dump assembly (note there is already an dump of the assembly in this tarball)
$ make dumpcfi
