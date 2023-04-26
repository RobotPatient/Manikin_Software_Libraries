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

#ifndef RINGBUFFER_HPP
#define RINGBUFFER_HPP
#include <stdio.h>

/**
 * @brief A template class for a ring buffer with a fixed capacity.
 * @tparam T The type of items stored in the buffer.
 * @tparam Capacity The maximum capacity of the buffer.
 */
template <typename T, size_t Capacity>
class RingBufferT {
 public:
  /**
   * @brief Default constructor.
   */
  RingBufferT() : head_(0), tail_(0), size_(0), read_(0) {}

  /**
   * @brief Add an item to the buffer.
   * @param item The item to be added to the buffer.
   */
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

  /**
   * @brief Retrieve a reference to the oldest item in the buffer.
   * @return A reference to the oldest item in the buffer.
   */
  T& front() {
    if (size_ == 0) {
      return buffer_[0];
    }
    return buffer_[head_];
  }

  /**
   * @brief Retrieve a reference to the newest item in the buffer.
   * @return A reference to the newest item in the buffer.
   */
  T& back() {
    if (size_ == 0) {
      return buffer_[0];
    }
    size_t index = (tail_ + Capacity - 1) % Capacity;
    return buffer_[index];
  }

  /**
   * @brief Retrieve a reference to the next item in the buffer. This moves the
   * read_ pointer in the buffer one item forwards
   * @return A reference to the item in the buffer at the read_ pointer;
   */
  T& read() {
    if (size_ == 0) {
      return buffer_[0];
    }

    T& temp = buffer_[read_];
    if (read_ == (tail_ + Capacity - 1) % Capacity) {
      // read_ has reached the newest item of the buffer. Skipping over items
      // that don't exist.
      resetRead();
    } else {
      read_ = (read_ + 1) % Capacity;
    }
    return temp;
  }

  /**
   * @brief Remove the oldest item from the buffer.
   */
  void pop() {
    if (size_ == 0) {
      // Buffer is empty, nothing to remove
      return;
    }
    head_ = (head_ + 1) % Capacity;
    size_--;
  }

  /**
   * @brief Retrieve an item from the buffer by index.
   * @param index The index of the item to be retrieved.
   * @return The item at the specified index.
   */
  T operator[](const unsigned int index) const { return buffer_[index]; }

  /**
   * @brief Resets the read_ pointer to the oldest item.
   */
  void resetRead() { read_ = head_; }

  /**
   * @brief Fill the buffer with a given value. This does NOT change size of the
   * buffer. Only use if a default value is needed
   * @param value The value to fill the buffer with.
   */
  void fill(const T& value) {
    for (size_t i = 0; i < Capacity; i++) {
      buffer_[i] = value;
    }
  }

  /**
   * @brief Get the number of items currently in the buffer.
   * @return The number of items in the buffer.
   */
  size_t size() const { return size_; }

  /**
   * @brief Check if the buffer is empty.
   * @return true if the buffer is empty, false otherwise.
   */
  bool empty() const { return size_ == 0; }

 private:
  T buffer_[Capacity];
  size_t head_;
  size_t tail_;
  size_t size_;
  size_t read_;
};

#endif
