#pragma once
#ifndef ARRAYD_ARRAYD_HPP_20251120
#define ARRAYD_ARRAYD_HPP_20251120
#include <cstddef>
#include <initializer_list> 

class ArrayD{
public:
    ArrayD() = default;
    ArrayD(const ArrayD&);
    explicit ArrayD(std::ptrdiff_t size);
  ArrayD(std::ptrdiff_t size, float value);
  ArrayD(std::initializer_list<float> init);
  ~ArrayD();
  ArrayD& operator=(const ArrayD&);
  ArrayD& operator=(std::initializer_list<float> init);
  [[nodiscard]] float& operator[](std::ptrdiff_t idx);
  [[nodiscard]] float operator[](std::ptrdiff_t idx) const;
  [[nodiscard]] float& at(std::ptrdiff_t idx);
  [[nodiscard]] float at(std::ptrdiff_t idx) const;
  [[nodiscard]] float* begin() noexcept {return data_;}
  [[nodiscard]] const float* begin() const noexcept { return data_; }
  [[nodiscard]] float* end() noexcept { return data_ + size_; }
  [[nodiscard]] const float* end() const noexcept { return data_ + size_; }

  [[nodiscard]] std::ptrdiff_t size() const noexcept { return size_; }
  [[nodiscard]] std::ptrdiff_t capacity() const noexcept { return capacity_; }
  [[nodiscard]] bool empty() const noexcept { return size_ == 0; }
  [[nodiscard]] float* data() noexcept { return data_; }
  [[nodiscard]] const float* data() const noexcept { return data_; }

  void resize(std::ptrdiff_t size);
  void resize(std::ptrdiff_t size, float value);
  void reserve(std::ptrdiff_t new_capacity);
  void shrink_to_fit();
  void insert(std::ptrdiff_t idx, float val);
  void push_back(float val);
  void pop_back();
  void remove(std::ptrdiff_t idx);
  void clear() noexcept;
  void swap(ArrayD& other) noexcept;
  [[nodiscard]] bool operator==(const ArrayD& other) const;
  [[nodiscard]] bool operator!=(const ArrayD& other) const;

  private:
  std::ptrdiff_t capacity_ = 0;
  std::ptrdiff_t size_ = 0;
  float* data_ = nullptr;
};
void swap(ArrayD& lhs, ArrayD& rhs) noexcept;
#endif 
