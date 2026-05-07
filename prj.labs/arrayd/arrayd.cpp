#include <arrayd/arrayd.hpp>

#include <cstring>
#include <stdexcept>
#include <utility>

ArrayD::ArrayD(const ArrayD& src)
  : capacity_(src.size_)
  , size_(src.size_)
  , data_(src.size_ > 0 ? new float[src.size_] : nullptr) {
  if (src.size_ > 0) {
    std::memcpy(data_, src.data_, size_ * sizeof(*data_));
  }
}

ArrayD::ArrayD(const std::ptrdiff_t size)
  : capacity_(size)
  , size_(size) {
  if (size < 0) {
    throw std::invalid_argument("ArrayD::ArrayD - negative size");
  }
  if (size > 0) {
    data_ = new float[capacity_]{0.0f};
  }
}

ArrayD::~ArrayD() {
  delete[] data_;
}

ArrayD& ArrayD::operator=(const ArrayD& rhs) {
  if (this != &rhs) {
    resize(rhs.size_);
    std::memcpy(data_, rhs.data_, size_ * sizeof(*data_));
  }
  return *this;
}

void ArrayD::resize(const std::ptrdiff_t size) {
  if (size < 0) {
    throw std::invalid_argument("ArrayD::resize - negative size");
  }
  if (capacity_ < size) {
    auto data = new float[size]{0.0f};
    if (size_ > 0) {
      std::memcpy(data, data_, size_ * sizeof(*data_));
    }
    std::swap(data_, data);
    delete[] data;
    capacity_ = size;
  } else if (size_ < size) {
    std::memset(data_ + size_, 0, (size - size_) * sizeof(*data_));
  }
  size_ = size;
}

float& ArrayD::operator[](const std::ptrdiff_t idx) {
  if (idx < 0 || size_ <= idx) {
    throw std::invalid_argument("ArrayD::operator[] - invalid index");
  }
  return *(data_ + idx);
}

float ArrayD::operator[](const std::ptrdiff_t idx) const {
  if (idx < 0 || size_ <= idx) {
    throw std::invalid_argument("ArrayD::operator[] - invalid index");
  }
  return data_[idx];
}

void ArrayD::insert(const std::ptrdiff_t idx, const float val) {
  if (idx < 0 || size_ < idx) {
    throw std::invalid_argument("ArrayD::insert - invalid index");
  }
  resize(size_ + 1);
  if (idx != size_ - 1) {
    std::memmove(data_ + idx + 1, data_ + idx, (size_ - idx - 1) * sizeof(float));
  }
  data_[idx] = val;
}

void ArrayD::remove(const std::ptrdiff_t idx) {
  if (idx < 0 || size_ <= idx) {
    throw std::invalid_argument("ArrayD::remove - invalid index");
  }
  if (idx != size_ - 1) {
    std::memmove(data_ + idx, data_ + idx + 1, (size_ - idx - 1) * sizeof(float));
  }
  resize(size_ - 1);
}
