#ifndef CEREALIZER_H
#define CEREALIZER_H

#include <cassert>
#include <cstdint>
#include <limits>
#include <map>
#include <string>
#include <unordered_map>
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
inline void Serialize(Context &context, const float& data)
{
    SerializePrimitive(context, data);
}

template<>
inline void Serialize(Context &context, const double& data)
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

template<typename T, typename U>
inline void Serialize(Context &context, const std::pair<T, U>& data)
{
    Serialize(context, data.first);
    Serialize(context, data.second);
}

template<typename T>
inline void SerializeContainer(Context &context, const T& data)
{
    assert(data.size() <= std::numeric_limits<uint32_t>::max());

    Serialize(context, static_cast<uint32_t>(data.size()));
    for (const auto& element : data)
    {
        Serialize(context, element);
    }
}

template<>
inline void Serialize(Context &context, const std::string& data)
{
    SerializeContainer(context, data);
}

template<typename T>
inline void Serialize(Context &context, const std::vector<T>& data)
{
    SerializeContainer(context, data);
}

template<typename T, typename U>
inline void Serialize(Context &context, const std::map<T, U>& data)
{
    SerializeContainer(context, data);
}

template<typename T, typename U>
inline void Serialize(Context &context, const std::unordered_map<T, U>& data)
{
    SerializeContainer(context, data);
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
inline bool Deserialize(Context &context, float& data)
{
    return DeserializePrimitive(context, data);
}

template<>
inline bool Deserialize(Context &context, double& data)
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

template<typename T, typename U>
inline bool Deserialize(Context &context, std::pair<T, U>& data)
{
    if (!Deserialize(context, data.first))
    {
        return false;
    }

    if (!Deserialize(context, data.second))
    {
        return false;
    }

    return true;
}

template<typename T>
inline bool DeserializeContainer(Context &context, T& data)
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

template<>
inline bool Deserialize(Context &context, std::string& data)
{
    return DeserializeContainer(context, data);
}

template<typename T>
inline bool Deserialize(Context &context, std::vector<T>& data)
{
    return DeserializeContainer(context, data);
}

template<typename T, typename U>
inline bool Deserialize(Context &context, std::map<T, U>& data)
{
    uint32_t length;
    if (!Deserialize(context, length))
    {
        return false;
    }

    for (uint32_t i = 0; i < length; i++)
    {
        std::pair<T, U> kvPair;
        if (!Deserialize(context, kvPair))
        {
            return false;
        }

        data.emplace(kvPair);
    }

    return true;
}

template<typename T, typename U>
inline bool Deserialize(Context &context, std::unordered_map<T, U>& data)
{
    uint32_t length;
    if (!Deserialize(context, length))
    {
        return false;
    }

    for (uint32_t i = 0; i < length; i++)
    {
        std::pair<T, U> kvPair;
        if (!Deserialize(context, kvPair))
        {
            return false;
        }

        data.emplace(kvPair);
    }

    return true;
}

}

#endif
