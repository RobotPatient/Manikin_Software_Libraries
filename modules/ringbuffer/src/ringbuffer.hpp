/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Thomas van den Oever
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

#include <Arduino.h>

template <typename T, size_t Capacity>
class RingBuffer {
 public:
  RingBuffer() : head_(0), tail_(0), size_(0) {}

  void push(const T& item) {
    if (size_ == Capacity) {
      // Buffer is full, overwrite oldest item
      buffer_[head_] = item;
      head_ = (head_ + 1) % Capacity;
    } else {
      // Buffer has space, add item to tail
      buffer_[tail_] = item;
      tail_ = (tail_ + 1) % Capacity;
      size_++;
    }
  }

  T& front() {
    if (size_ == 0) {
      return buffer_[0];
    }
    return buffer_[head_];
  }

  void pop() {
    if (size_ == 0) {
      // Buffer is empty, nothing to remove
      return;
    }
    head_ = (head_ + 1) % Capacity;
    size_--;
  }

  void fill(const T& value) { std::fill(buffer_, buffer_ + N, value); }

  size_t size() const { return size_; }

  bool empty() const { return size_ == 0; }

 private:
  T buffer_[Capacity];
  size_t head_;
  size_t tail_;
  size_t size_;
};