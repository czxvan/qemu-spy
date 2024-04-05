
../../build/qemu-arm \
    -d plugin,mmu \
    -plugin ../build/libaflspy.so \
    -D qemu_log.txt \
    ../build/hello-syscall