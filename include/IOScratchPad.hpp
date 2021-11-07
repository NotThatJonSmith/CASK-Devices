#pragma once

#include <IOTarget.hpp>
#include <queue>

namespace CASK {

/*
 * IOScratchPad is an IOTarget that maintains a pointer to another IOTarget and
 * a queue of transactions to perform against it when Commit() is called. It is
 * used, for example, when doing large transactions against virtual memory. This
 * lets us translate addresses and call IO functions against the scratchpad, but
 * only commit the transaction at the end, when we're sure all the translations
 * succeed.
 */
template <typename T, AccessType accessType>
class IOScratchPad final : public IOTarget {

private:

    IOTarget *target;

    struct TransactionCall {
        T startAddress;
        T size;
        char* buf;
    };

    std::queue<TransactionCall> transactionQueue;

    template <typename callerT, AccessType callerAccessType>
    constexpr T scratchPadCall(T startAddress, T size, char* buf) {
        if constexpr(!std::is_same<T, callerT>() || accessType != callerAccessType) {
            return 0;
        } else {
            transactionQueue.push({startAddress, size, buf});
            return size;
        }
    }
    
public:

    IOScratchPad(IOTarget* ioTarget) :
        target(ioTarget) {
    };

    T Commit() {
        T sizeSum = 0;
        while (!transactionQueue.empty()) {
            TransactionCall transaction = transactionQueue.front();
            transactionQueue.pop();
            T successSize = target->Transact<T, accessType>(transaction.startAddress, transaction.size, transaction.buf);
            sizeSum += successSize;
            if (successSize != transaction.size) {
                break;
            }
        }
        return sizeSum;
    }

    virtual __uint32_t Read32(__uint32_t startAddress, __uint32_t size, char* dst) override {
        return scratchPadCall<__uint32_t, AccessType::R>(startAddress, size, dst);
    }

    virtual __uint64_t Read64(__uint64_t startAddress, __uint64_t size, char* dst) override {
        return scratchPadCall<__uint64_t, AccessType::R>(startAddress, size, dst);
    }

    virtual __uint128_t Read128(__uint128_t startAddress, __uint128_t size, char* dst) override {
        return scratchPadCall<__uint128_t, AccessType::R>(startAddress, size, dst);
    }

    virtual __uint32_t Write32(__uint32_t startAddress, __uint32_t size, char* src) override {
        return scratchPadCall<__uint32_t, AccessType::W>(startAddress, size, src);
    }

    virtual __uint64_t Write64(__uint64_t startAddress, __uint64_t size, char* src) override {
        return scratchPadCall<__uint64_t, AccessType::W>(startAddress, size, src);
    }

    virtual __uint128_t Write128(__uint128_t startAddress, __uint128_t size, char* src) override {
        return scratchPadCall<__uint128_t, AccessType::W>(startAddress, size, src);
    }

    virtual __uint32_t Fetch32(__uint32_t startAddress, __uint32_t size, char* dst) override {
        return scratchPadCall<__uint32_t, AccessType::X>(startAddress, size, dst);
    }

    virtual __uint64_t Fetch64(__uint64_t startAddress, __uint64_t size, char* dst) override {
        return scratchPadCall<__uint64_t, AccessType::X>(startAddress, size, dst);
    }

    virtual __uint128_t Fetch128(__uint128_t startAddress, __uint128_t size, char* dst) override {
        return scratchPadCall<__uint128_t, AccessType::X>(startAddress, size, dst);
    }

};

} // namespace CASK
