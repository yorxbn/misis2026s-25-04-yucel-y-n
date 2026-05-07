#include "bitsetd.hpp"
#include <stdexcept>
#include <algorithm>
#include <cstddef>
BitsetD::BitsetD(const int32_t size, const bool val) : size_(size), bits_((size + 31) / 32, 0) {
  if (size > 0) fill(val);
}
BitsetD::BitsetD(const std::uint64_t mask, const std::int32_t size) : size_(size), bits_((size + 31) / 32, 0) {
  if (size <= 0) return;
  bits_[0] = static_cast<uint32_t>(mask);
  if (bits_.size() > 1) bits_[1] = static_cast<uint32_t>(mask >> 32);
  const int32_t last_word_bits = size % 32;
  if (last_word_bits != 0) bits_.back() &= (UINT32_C(1) << last_word_bits) - 1;
}
bool BitsetD::get(const std::int32_t idx) const {
  if (idx < 0 || idx >= size_) throw std::out_of_range("BitsetD::get");
  return (bits_[idx / 32] >> (idx % 32)) & 1;
}
void BitsetD::set(const std::int32_t idx, const bool val) {
  if (idx < 0 || idx >= size_) throw std::out_of_range("BitsetD::set");
  const uint32_t bit_mask = UINT32_C(1) << (idx % 32);
  if (val) bits_[idx / 32] |= bit_mask;
  else bits_[idx / 32] &= ~bit_mask;
}
BitsetD& BitsetD::invert() noexcept {
  for (auto& word : bits_) word = ~word;
  const int32_t last_word_bits = size_ % 32;
  if (last_word_bits != 0 && !bits_.empty()) bits_.back() &= (UINT32_C(1) << last_word_bits) - 1;
  return *this;
}
void BitsetD::resize(const std::int32_t new_size, const bool val) {
  if (new_size < 0) throw std::invalid_argument("BitsetD::resize: new_size cannot be negative");
  if (new_size == size_) return;
  const std::size_t new_words = (new_size + 31) / 32;
  const std::size_t old_words = bits_.size();
  const int32_t old_size = size_;
  bits_.resize(new_words, 0);
  size_ = new_size;
  if (new_size > old_size) {
    const uint32_t fill_word = val ? 0xFFFFFFFFU : 0U;
    for (std::size_t i = old_words; i < new_words; ++i) bits_[i] = fill_word;
    if (old_words > 0) {
      const int32_t old_last_bits = old_size % 32;
      if (old_last_bits != 0) {
        const uint32_t old_mask = (UINT32_C(1) << old_last_bits) - 1;
        bits_[old_words - 1] = (bits_[old_words - 1] & old_mask) | (val ? ~old_mask : 0);
      }
    }
  } else if (new_size > 0) {
    const int32_t last_word_bits = new_size % 32;
    if (last_word_bits != 0) bits_[new_words - 1] &= (UINT32_C(1) << last_word_bits) - 1;
  }
}
bool BitsetD::operator==(const BitsetD& rhs) const noexcept {
  if (size_ != rhs.size_) return false;
  return bits_ == rhs.bits_;
}
void BitsetD::fill(const bool val) noexcept {
  const uint32_t fill_value = val ? 0xFFFFFFFFU : 0U;
  std::fill(bits_.begin(), bits_.end(), fill_value);
  const int32_t last_word_bits = size_ % 32;
  if (last_word_bits != 0 && !bits_.empty()) bits_.back() = val ? (UINT32_C(1) << last_word_bits) - 1 : 0;
}
BitsetD& BitsetD::shift(const std::int32_t shift) noexcept {
  if (shift == 0 || size_ == 0) return *this;
  const int32_t words = static_cast<int32_t>(bits_.size());
  if (shift > 0) {
    const int32_t word_shift = shift / 32;
    const int32_t bit_shift = shift % 32;
    if (word_shift >= words) { fill(false); return *this; }
    if (bit_shift == 0) {
      for (int32_t i = words - 1; i >= word_shift; --i) bits_[i] = bits_[i - word_shift];
      for (int32_t i = 0; i < word_shift; ++i) bits_[i] = 0;
    } else {
      for (int32_t i = words - 1; i > word_shift; --i) bits_[i] = (bits_[i - word_shift] << bit_shift) | (bits_[i - word_shift - 1] >> (32 - bit_shift));
      bits_[word_shift] = bits_[0] << bit_shift;
      for (int32_t i = 0; i < word_shift; ++i) bits_[i] = 0;
    }
  } else {
    const int32_t right_shift = -shift;
    const int32_t word_shift = right_shift / 32;
    const int32_t bit_shift = right_shift % 32;
    if (word_shift >= words) { fill(false); return *this; }
    if (bit_shift == 0) {
      for (int32_t i = 0; i < words - word_shift; ++i) bits_[i] = bits_[i + word_shift];
      for (int32_t i = words - word_shift; i < words; ++i) bits_[i] = 0;
    } else {
      for (int32_t i = 0; i < words - word_shift - 1; ++i) bits_[i] = (bits_[i + word_shift] >> bit_shift) | (bits_[i + word_shift + 1] << (32 - bit_shift));
      if (words - word_shift > 0) bits_[words - word_shift - 1] = bits_[words - 1] >> bit_shift;
      for (int32_t i = words - word_shift; i < words; ++i) bits_[i] = 0;
    }
  }
  const int32_t last_word_bits = size_ % 32;
  if (last_word_bits != 0 && !bits_.empty()) bits_.back() &= (UINT32_C(1) << last_word_bits) - 1;
  return *this;
}
BitsetD& BitsetD::operator<<=(const std::int32_t shift) {
  if (shift < 0) return operator>>=(-shift);
  return shift(shift);
}
BitsetD& BitsetD::operator>>=(const std::int32_t shift) {
  if (shift < 0) return operator<<=(-shift);
  return shift(-shift);
}
BitsetD& BitsetD::operator&=(const BitsetD& rhs) {
  if (size_ != rhs.size_) throw std::invalid_argument("BitsetD::operator&=: sizes don't match");
  for (std::size_t i = 0; i < bits_.size(); ++i) bits_[i] &= rhs.bits_[i];
  return *this;
}
BitsetD& BitsetD::operator|=(const BitsetD& rhs) {
  if (size_ != rhs.size_) throw std::invalid_argument("BitsetD::operator|=: sizes don't match");
  for (std::size_t i = 0; i < bits_.size(); ++i) bits_[i] |= rhs.bits_[i];
  return *this;
}
BitsetD& BitsetD::operator^=(const BitsetD& rhs) {
  if (size_ != rhs.size_) throw std::invalid_argument("BitsetD::operator^=: sizes don't match");
  for (std::size_t i = 0; i < bits_.size(); ++i) bits_[i] ^= rhs.bits_[i];
  return *this;
}
BitsetD operator<<(const BitsetD& lhs, const std::int32_t shift) {
  BitsetD result(lhs);
  result <<= shift;
  return result;
}
BitsetD operator>>(const BitsetD& lhs, const std::int32_t shift) {
  BitsetD result(lhs);
  result >>= shift;
  return result;
}
BitsetD operator&(const BitsetD& lhs, const BitsetD& rhs) {
  BitsetD result(lhs);
  result &= rhs;
  return result;
}
BitsetD operator|(const BitsetD& lhs, const BitsetD& rhs) {
  BitsetD result(lhs);
  result |= rhs;
  return result;
}
BitsetD operator^(const BitsetD& lhs, const BitsetD& rhs) {
  BitsetD result(lhs);
  result ^= rhs;
  return result;
}
