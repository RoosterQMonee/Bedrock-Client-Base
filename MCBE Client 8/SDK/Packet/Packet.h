#pragma once

#include <Utils/Memory/Memory.h>
#include <SDK/Signatures.h>
#include <SDK/Packet/PacketTypes.h>

#include <memory>
#include <cstdint>


// overmind u da goat

class Packet;
template<MinecraftPacketIds>
struct PacketTypeMap {
    using type = Packet;
};


class Packet {
public:
    uintptr_t** VTable;
private:
    char pad[0x28];
public:
    template<MinecraftPacketIds type, typename T = typename PacketTypeMap<type>::type>
    static std::shared_ptr<typename PacketTypeMap<type>::type> CreatePacket(void) {
        using CreatePacket = std::shared_ptr<T>(__fastcall*)(MinecraftPacketIds);
        static auto _CreatePacket = (CreatePacket)(Memory::ScanSignature(Signatures::SDK::Packet_CreatePacket));

        return _CreatePacket(type);
    };

    MinecraftPacketIds GetId(void) {
        return VTable ? Memory::CallVFuncI<MinecraftPacketIds>(1, this) : static_cast<MinecraftPacketIds>(0);
    };
};