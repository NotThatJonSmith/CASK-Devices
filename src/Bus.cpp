#include <Bus.h>

namespace CASK {

__uint32_t Bus::Read32(__uint32_t startAddress, __uint32_t size, char* dst) {
    BusMapping map = GetMatchedMapping<__uint32_t>(startAddress, size);
    return map.target->Read32(startAddress & map.mask.w, size, dst);
}

__uint64_t Bus::Read64(__uint64_t startAddress, __uint64_t size, char* dst) {
    BusMapping map = GetMatchedMapping<__uint64_t>(startAddress, size);
    return map.target->Read64(startAddress & map.mask.dw, size, dst);
}

__uint128_t Bus::Read128(__uint128_t startAddress, __uint128_t size, char* dst) {
    BusMapping map = GetMatchedMapping<__uint128_t>(startAddress, size);
    return map.target->Read128(startAddress & map.mask.qw, size, dst);
}

__uint32_t Bus::Write32(__uint32_t startAddress, __uint32_t size, char* src) {
    BusMapping map = GetMatchedMapping<__uint32_t>(startAddress, size);
    return map.target->Write32(startAddress & map.mask.w, size, src);
}

__uint64_t Bus::Write64(__uint64_t startAddress, __uint64_t size, char* src) {
    BusMapping map = GetMatchedMapping<__uint64_t>(startAddress, size);
    return map.target->Write64(startAddress & map.mask.dw, size, src);
}

__uint128_t Bus::Write128(__uint128_t startAddress, __uint128_t size, char* src) {
    BusMapping map = GetMatchedMapping<__uint128_t>(startAddress, size);
    return map.target->Write128(startAddress & map.mask.qw, size, src);
}

__uint32_t Bus::Fetch32(__uint32_t startAddress, __uint32_t size, char* dst) {
    BusMapping map = GetMatchedMapping<__uint32_t>(startAddress, size);
    return map.target->Fetch32(startAddress & map.mask.w, size, dst);
}

__uint64_t Bus::Fetch64(__uint64_t startAddress, __uint64_t size, char* dst) {
    BusMapping map = GetMatchedMapping<__uint64_t>(startAddress, size);
    return map.target->Fetch64(startAddress & map.mask.dw, size, dst);
}

__uint128_t Bus::Fetch128(__uint128_t startAddress, __uint128_t size, char* dst) {
    BusMapping map = GetMatchedMapping<__uint128_t>(startAddress, size);
    return map.target->Fetch128(startAddress & map.mask.qw, size, dst);
}

void Bus::AddIOTarget32(IOTarget *dev, __uint32_t address, __uint32_t mask) {
    BusMapping mapping;
    mapping.addr.qw = 0;
    mapping.addr.w = address;
    mapping.mask.qw = 0;
    mapping.mask.w = mask;
    mapping.target = dev;
    mappings.push_back(mapping);
}

void Bus::AddIOTarget64(IOTarget *dev, __uint64_t address, __uint64_t mask) {
    BusMapping mapping;
    mapping.addr.qw = 0;
    mapping.addr.dw = address;
    mapping.mask.qw = 0;
    mapping.mask.dw = mask;
    mapping.target = dev;
    mappings.push_back(mapping);
}

void Bus::AddIOTarget128(IOTarget *dev, __uint128_t address, __uint128_t mask) {
    BusMapping mapping;
    mapping.addr.qw = address;
    mapping.mask.qw = mask;
    mapping.target = dev;
    mappings.push_back(mapping);
}

} // namespace CASK
