#ifndef CEREALIZER_H
#define CEREALIZER_H

#include <cstdint>
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
void Serialize(Context& context, const T& data)
{
    data.Serialize(context);
}

template<typename T>
bool Deserialize(Context &context,  T& data)
{
    return data.Deserialize(context);
}

template<typename T>
void SerializePrimitive(Context &context, const T& data)
{
    size_t spaceAvailable = context.buffer.size() - context.cursor;
    if (spaceAvailable < sizeof(T))
    {
        context.buffer.resize(context.cursor + sizeof(T));
    }

    *reinterpret_cast<T*>(&context.buffer[context.cursor]) = data;
}

template<>
void Serialize(Context &context, const uint8_t& data)
{
    SerializePrimitive(context, data);
}

template<>
void Serialize(Context &context, const uint16_t& data)
{
    SerializePrimitive(context, data);
}

template<>
void Serialize(Context &context, const uint32_t& data)
{
    SerializePrimitive(context, data);
}

template<>
void Serialize(Context &context, const uint64_t& data)
{
    SerializePrimitive(context, data);
}

template<>
void Serialize(Context &context, const int8_t& data)
{
    SerializePrimitive(context, data);
}

template<>
void Serialize(Context &context, const int16_t& data)
{
    SerializePrimitive(context, data);
}

template<>
void Serialize(Context &context, const int32_t& data)
{
    SerializePrimitive(context, data);
}

template<>
void Serialize(Context &context, const int64_t& data)
{
    SerializePrimitive(context, data);
}

template<>
void Serialize(Context &context, const bool& data)
{
    SerializePrimitive(context, static_cast<uint8_t>(data));
}

template<typename T>
bool DeserializePrimitive(Context &context, T& data)
{
    size_t spaceRemaining = context.buffer.size() - context.cursor; 
    if (spaceRemaining < sizeof(T))
    {
        return false;
    }

    data = *reinterpret_cast<T*>(&context.buffer[context.cursor]);
    return true;
}

template<>
bool Deserialize(Context &context, uint8_t& data)
{
    return DeserializePrimitive(context, data);
}

template<>
bool Deserialize(Context &context, uint16_t& data)
{
    return DeserializePrimitive(context, data);
}

template<>
bool Deserialize(Context &context, uint32_t& data)
{
    return DeserializePrimitive(context, data);
}

template<>
bool Deserialize(Context &context, uint64_t& data)
{
    return DeserializePrimitive(context, data);
}

template<>
bool Deserialize(Context &context, int8_t& data)
{
    return DeserializePrimitive(context, data);
}

template<>
bool Deserialize(Context &context, int16_t& data)
{
    return DeserializePrimitive(context, data);
}

template<>
bool Deserialize(Context &context, int32_t& data)
{
    return DeserializePrimitive(context, data);
}

template<>
bool Deserialize(Context &context, int64_t& data)
{
    return DeserializePrimitive(context, data);
}

template<>
bool Deserialize(Context &context, bool& data)
{
    uint8_t tmp;
    bool result = DeserializePrimitive(context, tmp);
    data = static_cast<bool>(tmp);
    return result;
}

}

#endif
