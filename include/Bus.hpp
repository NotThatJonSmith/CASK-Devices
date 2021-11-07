#pragma once

#include <IOTarget.hpp>
#include <vector>

namespace CASK {

class Bus final : public IOTarget {

public:

    virtual __uint32_t Read32(__uint32_t startAddress, __uint32_t size, char* dst) override;
    virtual __uint64_t Read64(__uint64_t startAddress, __uint64_t size, char* dst) override;
    virtual __uint128_t Read128(__uint128_t startAddress, __uint128_t size, char* dst) override;
    virtual __uint32_t Write32(__uint32_t startAddress, __uint32_t size, char* src) override;
    virtual __uint64_t Write64(__uint64_t startAddress, __uint64_t size, char* src) override;
    virtual __uint128_t Write128(__uint128_t startAddress, __uint128_t size, char* src) override;
    virtual __uint32_t Fetch32(__uint32_t startAddress, __uint32_t size, char* src) override;
    virtual __uint64_t Fetch64(__uint64_t startAddress, __uint64_t size, char* src) override;
    virtual __uint128_t Fetch128(__uint128_t startAddress, __uint128_t size, char* src) override;

    void AddIOTarget32(IOTarget *target, __uint32_t address, __uint32_t mask);
    void AddIOTarget64(IOTarget *target, __uint64_t address, __uint64_t mask);
    void AddIOTarget128(IOTarget *target, __uint128_t address, __uint128_t mask);

private:

    union BusAddress { __uint32_t w; __uint64_t dw; __uint128_t qw; };

    struct BusMapping {
        BusAddress addr;
        BusAddress mask;
        IOTarget *target;
    };

    std::vector<BusMapping> mappings;

    template<typename T>
    inline BusMapping GetMatchedMapping(T startAddress, T size) {
        for (BusMapping &map : mappings) {
            if ((startAddress & ~map.mask.w) == map.addr.w) {
                return map;
            }
        }
        return {{0}, {0}, nullptr};
    }

};

} // namespace CASK
