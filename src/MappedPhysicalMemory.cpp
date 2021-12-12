#include <MappedPhysicalMemory.hpp>
#include <sys/mman.h>
#include <cassert>

namespace CASK {

MappedPhysicalMemory::MappedPhysicalMemory() {
    memStartAddress = (char*)mmap(
        NULL,
        0x100000000,
        PROT_READ | PROT_WRITE,
        MAP_ANONYMOUS | MAP_PRIVATE,
        -1,
        0);
    assert(memStartAddress != MAP_FAILED);
}

__uint32_t MappedPhysicalMemory::Read32(__uint32_t startAddress, __uint32_t size, char* buf) {
    return TransactInternal<__uint32_t, AccessType::R>(startAddress, size, buf);
}

__uint64_t MappedPhysicalMemory::Read64(__uint64_t startAddress, __uint64_t size, char* buf) {
    return TransactInternal<__uint64_t, AccessType::R>(startAddress, size, buf);
}

__uint128_t MappedPhysicalMemory::Read128(__uint128_t startAddress, __uint128_t size, char* buf) {
    return TransactInternal<__uint128_t, AccessType::R>(startAddress, size, buf);
}

__uint32_t MappedPhysicalMemory::Write32(__uint32_t startAddress, __uint32_t size, char* buf) {
    return TransactInternal<__uint32_t, AccessType::W>(startAddress, size, buf);
}

__uint64_t MappedPhysicalMemory::Write64(__uint64_t startAddress, __uint64_t size, char* buf) {
    return TransactInternal<__uint64_t, AccessType::W>(startAddress, size, buf);
}

__uint128_t MappedPhysicalMemory::Write128(__uint128_t startAddress, __uint128_t size, char* buf) {
    return TransactInternal<__uint128_t, AccessType::R>(startAddress, size, buf);
}

__uint32_t MappedPhysicalMemory::Fetch32(__uint32_t startAddress, __uint32_t size, char* buf) {
    return TransactInternal<__uint32_t, AccessType::X>(startAddress, size, buf);
}

__uint64_t MappedPhysicalMemory::Fetch64(__uint64_t startAddress, __uint64_t size, char* buf) {
    return TransactInternal<__uint64_t, AccessType::X>(startAddress, size, buf);
}

__uint128_t MappedPhysicalMemory::Fetch128(__uint128_t startAddress, __uint128_t size, char* buf) {
    return TransactInternal<__uint128_t, AccessType::X>(startAddress, size, buf);
}

} // namespace CASK
