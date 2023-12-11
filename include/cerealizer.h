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
void Serialize(Context &context, const uint32_t& data)
{
    SerializePrimitive(context, data);
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
bool Deserialize(Context &context, uint32_t& data)
{
    return DeserializePrimitive(context, data);
}

}

#endif
