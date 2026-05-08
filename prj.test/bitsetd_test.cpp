#include  <bitsetd/bitsetd.cpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("[bitsetd] - constructors") {
  BitsetD b1;
  CHECK(0 == b1.size());
  BitsetD b2(10, true);
  CHECK(10 == b2.size());
  for (int i = 0; i < 10; ++i) CHECK(b2.get(i));
  
  BitsetD b3(0xFFFFFFFFFFFFFFFFULL, 64);
  CHECK(64 == b3.size());
  for (int i = 0; i < 64; ++i) CHECK(b3.get(i));
  
  BitsetD b4(b2);
  CHECK(b2 == b4);
  
  BitsetD b5(std::move(b2));
  CHECK(0 == b2.size());
  CHECK(b4 == b5);
}

TEST_CASE("[bitsetd] - set/get and bounds") {
  BitsetD b(10, false);
  
  CHECK_THROWS(b.get(-1));
  CHECK_THROWS(b.get(10));
  CHECK_THROWS(b.set(-1, true));
  CHECK_THROWS(b.set(10, true));
  
  b.set(5, true);
  CHECK(b.get(5));
  CHECK(!b.get(3));
  
  b.set(5, false);
  CHECK(!b.get(5));
}

TEST_CASE("[bitsetd] - operator[]") {
  BitsetD b(10, false);
  b[3] = true;
  CHECK(b[3]);
  CHECK(!b[2]);
  
  const BitsetD c(0xAAAAULL, 16);
  CHECK(c[0] == false);
  CHECK(c[1] == true);
}

TEST_CASE("[bitsetd] - resize") {
  BitsetD b(5, true);
  b.resize(10, false);
  CHECK(10 == b.size());
  for (int i = 0; i < 5; ++i) CHECK(b.get(i));
  for (int i = 5; i < 10; ++i) CHECK(!b.get(i));
  
  b.resize(3);
  CHECK(3 == b.size());
  
  CHECK_THROWS(b.resize(-1));
}

TEST_CASE("[bitsetd] - operations") {
  BitsetD b1(8, false);
  BitsetD b2(8, false);
  b1.set(0, true);
  b1.set(2, true);
  b2.set(0, true);
  b2.set(1, true);
  
  BitsetD anded = b1 & b2;
  CHECK(anded.get(0));
  CHECK(!anded.get(1));
  CHECK(!anded.get(2));
  
  BitsetD ored = b1 | b2;
  CHECK(ored.get(0) && ored.get(1) && ored.get(2));
  
  BitsetD xored = b1 ^ b2;
  CHECK(!xored.get(0));
  CHECK(xored.get(1));
  CHECK(xored.get(2));
  
  BitsetD inv = ~b1;
  CHECK(!inv.get(0));
  CHECK(inv.get(1));
  CHECK(!inv.get(2));
}

TEST_CASE("[bitsetd] - shifts") {
  BitsetD b(10, false);
  b.set(0, true);
  b <<= 3;
  CHECK(b.get(3));
  
  b >>= 2;
  CHECK(b.get(1));
  
  BitsetD b2 = b << 1;
  CHECK(b2.get(2));
  
  BitsetD b3(5, true);
  b3 <<= 10;
  for (int i = 0; i < 5; ++i) CHECK(!b3.get(i));
}

TEST_CASE("[bitsetd] - fill and invert") {
  BitsetD b(10, false);
  b.fill(true);
  for (int i = 0; i < 10; ++i) CHECK(b.get(i));
  
  b.invert();
  for (int i = 0; i < 10; ++i) CHECK(!b.get(i));
}
