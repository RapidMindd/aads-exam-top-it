#ifndef TARASENKO_YAROSLAV_COMMON_DYNAMIC_ARRAY_HPP
#define TARASENKO_YAROSLAV_COMMON_DYNAMIC_ARRAY_HPP

#include <cstddef>
#include <limits>
#include <stdexcept>

namespace tarasenko
{
  template< typename T >
  struct DynamicArray
  {
    T* data;
    std::size_t size;
    std::size_t capacity;
  };

  template< typename T >
  DynamicArray< T > makeDynamicArray()
  {
    DynamicArray< T > array = { nullptr, 0, 0 };
    return array;
  }

  template< typename T >
  void destroyDynamicArray(DynamicArray< T >& array)
  {
    delete[] array.data;
    array.data = nullptr;
    array.size = 0;
    array.capacity = 0;
  }

  template< typename T >
  void reserveDynamicArray(DynamicArray< T >& array, std::size_t newCapacity)
  {
    if (newCapacity <= array.capacity) {
      return;
    }

    T* newData = new T[newCapacity];
    try {
      for (std::size_t index = 0; index < array.size; ++index) {
        newData[index] = array.data[index];
      }
    } catch (...) {
      delete[] newData;
      throw;
    }

    delete[] array.data;
    array.data = newData;
    array.capacity = newCapacity;
  }

  template< typename T >
  void appendDynamicArray(DynamicArray< T >& array, const T& value)
  {
    const std::size_t initialCapacity = 8;
    const std::size_t growthFactor = 2;

    if (array.size == array.capacity) {
      std::size_t newCapacity = initialCapacity;
      if (array.capacity != 0) {
        if (array.capacity > (std::numeric_limits< std::size_t >::max() / growthFactor)) {
          throw std::length_error("dynamic array capacity overflow");
        }
        newCapacity = array.capacity * growthFactor;
      }
      reserveDynamicArray(array, newCapacity);
    }

    array.data[array.size] = value;
    ++array.size;
  }
}

#endif
