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

struct Context2
{
    std::vector<uint8_t> buffer;
    size_t cursor{ 0 };
};

class Context
{
public:
    Context() = default;
    Context(std::vector<uint8_t> buffer)
        : _buffer(std::move(buffer)) {}
    
    Context(Context&) = delete;
    Context& operator=(Context&) = delete;

    Context(Context&&) = default;
    Context& operator=(Context&&) = default;
    
    void Rewind() { _cursor = 0; }

    void SetBuffer(std::vector<uint8_t> buffer) { _buffer = std::move(buffer); }
    const std::vector<uint8_t>& GetBuffer() const { return _buffer; }

    template<typename T>
    void Serialize(const T& data)
    {
        data.Serialize(*this);
    }

    template<typename T, typename... Args>
    void Serialize(const T& data, const Args&... args)
    {
        Serialize(data);
        Serialize(args...);
    }

    template<typename T>
    bool Deserialize(T& data)
    {
        return data.Deserialize(*this);
    }
    
    template<typename T, typename... Args>
    bool Deserialize(T& data, Args&... args)
    {
        if (!Deserialize(data))
        {
            return false;
        }

        return Deserialize(args...);
    }
    
    
    template<typename T, typename U>
    void Serialize(const std::pair<T, U>& data);

    template<typename T>
    void Serialize(const std::vector<T>& data);

    template<typename T, typename U>
    void Serialize(const std::map<T, U>& data);

    template<typename T, typename U>
    void Serialize(const std::unordered_map<T, U> &data);

    template<typename T, typename U>
    bool Deserialize(std::pair<T, U>& data);

    template<typename T>
    bool Deserialize(std::vector<T>& data);

    template<typename T, typename U>
    bool Deserialize(std::map<T, U>& data);

    template<typename T, typename U>
    bool Deserialize(std::unordered_map<T, U>& data);

private:
    template<typename T>
    void _SerializePrimitive(const T& data)
    {
        size_t spaceAvailable = _buffer.size() - _cursor;
        if (spaceAvailable < sizeof(T))
        {
            _buffer.resize(_cursor + sizeof(T));
        }

        *reinterpret_cast<T*>(&_buffer[_cursor]) = data;
        _cursor += sizeof(T);
    }

    template<typename T>
    void _SerializeContainer(const T& data)
    {
        assert(data.size() <= std::numeric_limits<uint32_t>::max());

        Serialize(static_cast<uint32_t>(data.size()));
        for (const auto& element : data)
        {
            Serialize(element);
        }
    }
    
    template<typename T>
    bool _DeserializePrimitive(T& data)
    {
        size_t spaceRemaining = _buffer.size() - _cursor; 
        if (spaceRemaining < sizeof(T))
        {
            return false;
        }

        data = *reinterpret_cast<T*>(&_buffer[_cursor]);
        _cursor += sizeof(T);

        return true;
    }

    template<typename T>
    bool _DeserializeContainer(T& data)
    {
        uint32_t length;
        if (!Deserialize(length))
        {
            return false;
        }

        data.resize(length);

        for (uint32_t i = 0; i < length; i++)
        {
            if (!Deserialize(data[i]))
            {
                return false;
            }
        }

        return true;
    }

    std::vector<uint8_t> _buffer;
    size_t _cursor{ 0 };
};

template<>
inline void Context::Serialize(const uint8_t& data)
{
    _SerializePrimitive(data);
}

template<>
inline void Context::Serialize(const uint16_t& data)
{
    _SerializePrimitive(data);
}

template<>
inline void Context::Serialize(const uint32_t& data)
{
    _SerializePrimitive(data);
}

template<>
inline void Context::Serialize(const uint64_t& data)
{
    _SerializePrimitive(data);
}

template<>
inline void Context::Serialize(const int8_t& data)
{
    _SerializePrimitive(data);
}

template<>
inline void Context::Serialize(const int16_t& data)
{
    _SerializePrimitive(data);
}

template<>
inline void Context::Serialize(const int32_t& data)
{
    _SerializePrimitive(data);
}

template<>
inline void Context::Serialize(const int64_t& data)
{
    _SerializePrimitive(data);
}

template<>
inline void Context::Serialize(const float& data)
{
    _SerializePrimitive(data);
}

template<>
inline void Context::Serialize(const double& data)
{
    _SerializePrimitive(data);
}

template<>
inline void Context::Serialize(const bool& data)
{
    _SerializePrimitive(static_cast<uint8_t>(data));
}

template<>
inline void Context::Serialize(const char& data)
{
    uint8_t byte = *reinterpret_cast<const uint8_t*>(&data);
    _SerializePrimitive(byte);
}

template<typename T, typename U>
inline void Context::Serialize(const std::pair<T, U>& data)
{
    Serialize(data.first);
    Serialize(data.second);
}

template<>
inline void Context::Serialize(const std::string& data)
{
    _SerializeContainer(data);
}

template<typename T>
inline void Context::Serialize(const std::vector<T>& data)
{
    _SerializeContainer(data);
}

template<typename T, typename U>
inline void Context::Serialize(const std::map<T, U>& data)
{
    _SerializeContainer(data);
}

template<typename T, typename U>
inline void Context::Serialize(const std::unordered_map<T, U>& data)
{
    _SerializeContainer(data);
}

template<>
inline bool Context::Deserialize(uint8_t& data)
{
    return _DeserializePrimitive(data);
}

template<>
inline bool Context::Deserialize(uint16_t& data)
{
    return _DeserializePrimitive(data);
}

template<>
inline bool Context::Deserialize(uint32_t& data)
{
    return _DeserializePrimitive(data);
}

template<>
inline bool Context::Deserialize(uint64_t& data)
{
    return _DeserializePrimitive(data);
}

template<>
inline bool Context::Deserialize(int8_t& data)
{
    return _DeserializePrimitive(data);
}

template<>
inline bool Context::Deserialize(int16_t& data)
{
    return _DeserializePrimitive(data);
}

template<>
inline bool Context::Deserialize(int32_t& data)
{
    return _DeserializePrimitive(data);
}

template<>
inline bool Context::Deserialize(int64_t& data)
{
    return _DeserializePrimitive(data);
}

template<>
inline bool Context::Deserialize(float& data)
{
    return _DeserializePrimitive(data);
}

template<>
inline bool Context::Deserialize(double& data)
{
    return _DeserializePrimitive(data);
}

template<>
inline bool Context::Deserialize(bool& data)
{
    uint8_t tmp;
    bool result = _DeserializePrimitive(tmp);
    data = static_cast<bool>(tmp);
    return result;
}

template<>
inline bool Context::Deserialize(char& data)
{
    uint8_t byte;
    if (!_DeserializePrimitive(byte))
    {
        return false;
    }

    *reinterpret_cast<uint8_t*>(&data) = byte;

    return true;
}

template<typename T, typename U>
inline bool Context::Deserialize(std::pair<T, U>& data)
{
    if (!Deserialize(data.first))
    {
        return false;
    }

    if (!Deserialize(data.second))
    {
        return false;
    }

    return true;
}

template<>
inline bool Context::Deserialize(std::string& data)
{
    return _DeserializeContainer(data);
}

template<typename T>
inline bool Context::Deserialize(std::vector<T>& data)
{
    return _DeserializeContainer(data);
}

template<typename T, typename U>
inline bool Context::Deserialize(std::map<T, U>& data)
{
    uint32_t length;
    if (!Deserialize(length))
    {
        return false;
    }

    for (uint32_t i = 0; i < length; i++)
    {
        std::pair<T, U> kvPair;
        if (!Deserialize(kvPair))
        {
            return false;
        }

        data.emplace(kvPair);
    }

    return true;
}

template<typename T, typename U>
inline bool Context::Deserialize(std::unordered_map<T, U>& data)
{
    uint32_t length;
    if (!Deserialize(length))
    {
        return false;
    }

    for (uint32_t i = 0; i < length; i++)
    {
        std::pair<T, U> kvPair;
        if (!Deserialize(kvPair))
        {
            return false;
        }

        data.emplace(kvPair);
    }

    return true;
}

}

#endif
