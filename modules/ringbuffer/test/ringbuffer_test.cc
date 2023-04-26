/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Victor Hogeweij
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 *
 * including without limitation the rights to use, copy, modify, merge, publish,
 *distribute, sublicense, and/or sell copies of the Software, and to permit
 *persons to whom the Software is furnished to do so,
 *
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 *OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
 *OR OTHER DEALINGS IN THE SOFTWARE.
 ***********************************************************************************************/
#include "ringbuffer.hpp"

#include <gmock/gmock.h>

#define SIZE 5

TEST(RINGBUFFER, INIT) {
  RingBufferT<int, SIZE> buff;
  EXPECT_EQ(buff.empty(), true);
  buff.push(1);
  EXPECT_EQ(buff.front(), 1);
}

TEST(RINGBUFFER, PUSH) {
  RingBufferT<int, SIZE> buff;
  buff.fill(0);
  buff.push(1);
  buff.push(2);
  EXPECT_EQ(buff[1], 2);
}

TEST(RINGBUFFER, WRAP_AROUND) {
  RingBufferT<int, SIZE> buff;
  buff.push(1);
  buff.push(2);
  buff.push(3);
  buff.push(4);
  buff.push(5);
  buff.push(6);
  EXPECT_EQ(buff[0], 6);
  EXPECT_EQ(buff[1], 2);
  EXPECT_EQ(buff[2], 3);
  EXPECT_EQ(buff[3], 4);
  EXPECT_EQ(buff[4], 5);
}

TEST(RINGBUFFER, FRONT) {
  RingBufferT<int, SIZE> buff;
  buff.fill(0);
  buff.push(1);
  buff.push(2);
  buff.push(3);
  EXPECT_EQ(buff.front(), 1);
  buff.pop();
  EXPECT_EQ(buff.front(), 2);

}

TEST(RINGBUFFER, BACK) {
  RingBufferT<int, SIZE> buff;
  buff.push(1);
  buff.push(2);
  buff.push(3);
  buff.push(4);
  EXPECT_EQ(buff.back(), 4);
}

TEST(RINGBUFFER, FLOAT) {
  RingBufferT<float, SIZE> buff;
  buff.push(1.5f);
  buff.push(4.235f);
  buff.push(5.3f);
  buff.pop();
  EXPECT_EQ(buff.front(), 4.235f);
  EXPECT_EQ(buff.back(), 5.3f);
}

int main(int argc, char** argv) {
  // ::testing::InitGoogleTest(&argc, argv);
  // if you plan to use GMock, replace the line above with
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS()) {
  }

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}