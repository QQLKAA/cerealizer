#ifndef CEREALIZER_H
#define CEREALIZER_H

#include <cassert>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>

#ifndef CEREALIZER_LITTLE_ENDIAN
#error "Cerealizer only supports little endian"
#endif

namespace Cerealizer
{

struct Context
{
    std::vector<uint8_t> buffer;
    size_t cursor{ 0 };
};

template<typename T>
inline void Serialize(Context& context, const T& data)
{
    data.Serialize(context);
}

template<typename T>
inline bool Deserialize(Context &context,  T& data)
{
    return data.Deserialize(context);
}

template<typename T>
inline void SerializePrimitive(Context &context, const T& data)
{
    size_t spaceAvailable = context.buffer.size() - context.cursor;
    if (spaceAvailable < sizeof(T))
    {
        context.buffer.resize(context.cursor + sizeof(T));
    }

    *reinterpret_cast<T*>(&context.buffer[context.cursor]) = data;
    context.cursor += sizeof(T);
}

template<>
inline void Serialize(Context &context, const uint8_t& data)
{
    SerializePrimitive(context, data);
}

template<>
inline void Serialize(Context &context, const uint16_t& data)
{
    SerializePrimitive(context, data);
}

template<>
inline void Serialize(Context &context, const uint32_t& data)
{
    SerializePrimitive(context, data);
}

template<>
inline void Serialize(Context &context, const uint64_t& data)
{
    SerializePrimitive(context, data);
}

template<>
inline void Serialize(Context &context, const int8_t& data)
{
    SerializePrimitive(context, data);
}

template<>
inline void Serialize(Context &context, const int16_t& data)
{
    SerializePrimitive(context, data);
}

template<>
inline void Serialize(Context &context, const int32_t& data)
{
    SerializePrimitive(context, data);
}

template<>
inline void Serialize(Context &context, const int64_t& data)
{
    SerializePrimitive(context, data);
}

template<>
inline void Serialize(Context &context, const bool& data)
{
    SerializePrimitive(context, static_cast<uint8_t>(data));
}

template<>
inline void Serialize(Context &context, const char& data)
{
    uint8_t byte = *reinterpret_cast<const uint8_t*>(&data);
    SerializePrimitive(context, byte);
}

template<>
inline void Serialize(Context &context, const std::string& data)
{
    assert(data.size() <= std::numeric_limits<uint32_t>::max());

    Serialize(context, static_cast<uint32_t>(data.size()));
    for (char ch : data)
    {
        Serialize(context, ch);
    }
}

template<typename T>
inline bool DeserializePrimitive(Context &context, T& data)
{
    size_t spaceRemaining = context.buffer.size() - context.cursor; 
    if (spaceRemaining < sizeof(T))
    {
        return false;
    }

    data = *reinterpret_cast<T*>(&context.buffer[context.cursor]);
    context.cursor += sizeof(T);

    return true;
}

template<>
inline bool Deserialize(Context &context, uint8_t& data)
{
    return DeserializePrimitive(context, data);
}

template<>
inline bool Deserialize(Context &context, uint16_t& data)
{
    return DeserializePrimitive(context, data);
}

template<>
inline bool Deserialize(Context &context, uint32_t& data)
{
    return DeserializePrimitive(context, data);
}

template<>
inline bool Deserialize(Context &context, uint64_t& data)
{
    return DeserializePrimitive(context, data);
}

template<>
inline bool Deserialize(Context &context, int8_t& data)
{
    return DeserializePrimitive(context, data);
}

template<>
inline bool Deserialize(Context &context, int16_t& data)
{
    return DeserializePrimitive(context, data);
}

template<>
inline bool Deserialize(Context &context, int32_t& data)
{
    return DeserializePrimitive(context, data);
}

template<>
inline bool Deserialize(Context &context, int64_t& data)
{
    return DeserializePrimitive(context, data);
}

template<>
inline bool Deserialize(Context &context, bool& data)
{
    uint8_t tmp;
    bool result = DeserializePrimitive(context, tmp);
    data = static_cast<bool>(tmp);
    return result;
}

template<>
inline bool Deserialize(Context &context, char& data)
{
    uint8_t byte;
    if (!DeserializePrimitive(context, byte))
    {
        return false;
    }

    *reinterpret_cast<uint8_t*>(&data) = byte;

    return true;
}

template<>
inline bool Deserialize(Context &context, std::string& data)
{
    uint32_t length;
    if (!Deserialize(context, length))
    {
        return false;
    }

    data.resize(length);

    for (uint32_t i = 0; i < length; i++)
    {
        if (!Deserialize(context, data[i]))
        {
            return false;
        }
    }

    return true;
}

}

#endif
