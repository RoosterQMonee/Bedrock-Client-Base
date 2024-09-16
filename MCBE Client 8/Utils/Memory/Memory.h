#pragma once

#include <Utils/Memory/Macros.h>

#include <optional>
#include <vector>
#include <string>


class Memory {
public:
    template <typename R, typename... Args>
    static inline R CallFunc(void* func, Args... args) {
        return ((R(*)(Args...))func)(args...);
    }

    template <unsigned int IIdx, typename TRet, typename... TArgs>
    static inline auto CallVFunc(void* thisptr, TArgs... argList) -> TRet {
        using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
        return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
    }

    template <typename TRet, typename... TArgs>
    static inline auto* GetFastcall(void* Original) {
        using Fn = TRet(__fastcall*)(TArgs...);
        return reinterpret_cast<Fn>(Original);
    }

    template <typename Ret, typename Type>
	static Ret& DirectAccess(Type* type, size_t offset);

    static void PatchBytes(void* dst, void* src, unsigned int size);
    static inline uintptr_t SignatureOffset(uintptr_t sig, int offset);

	static std::vector<std::optional<uint8_t>> PatternToBytes(std::string pattern);
	static std::optional<std::uintptr_t> ScanSignature(std::string signature);
};



// i don't really have a better way to add this  (:wholenotherlevel:)
#define BUILD_ACCESS(ptr, type, name, offset)                                                                        \
AS_FIELD(type, name, get##name);                                                                                     \
type get##name() const { return Memory::DirectAccess<type>(ptr, offset); }												     \
void set##name(type v) const { Memory::DirectAccess<type>(ptr, offset) = v; }
