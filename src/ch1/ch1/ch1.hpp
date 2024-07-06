// Copyright [2024] <@damianWu>
#ifndef SRC_CH1_CH1_CH1_HPP_
#define SRC_CH1_CH1_CH1_HPP_

#include <bits/chrono.h>
#include <fmt/core.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <utility>

namespace ch1
{
using size_t = std::size_t;

namespace it
{
template <typename Item>
struct SingleNode
{
  Item item{};
  SingleNode* next{};

  bool operator==(const SingleNode& node);
  bool operator!=(const SingleNode& node);
};

template <typename Item>
bool SingleNode<Item>::operator==(const SingleNode<Item>& node)
{
  return item == node.item && next == node.next;
}
template <typename Item>
bool SingleNode<Item>::operator!=(const SingleNode<Item>& node)
{
  return !(*this == node);
}

template <typename Item>
struct DoubleNode
{
  Item item{};
  DoubleNode<Item>* next{};
  DoubleNode<Item>* prev{};

  bool operator==(const DoubleNode& second)
  {
    return item == second.item && next == second.next && prev == second.prev;
  }

  bool operator!=(const DoubleNode& second) { return !(*this == second); }
};

template <typename NodeType>
struct Iterator
{
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = NodeType;
  using pointer = NodeType*;
  using reference = NodeType&;

  explicit Iterator(NodeType* ptr) : m_ptr(ptr) {}

  reference operator*() const { return *m_ptr; }
  pointer operator->() { return m_ptr; }

  // Prefix increment
  Iterator& operator++()
  {
    m_ptr = m_ptr->next;
    return *this;
  }

  // Postfix increment
  Iterator operator++(int)
  {
    Iterator tmp{m_ptr};
    ++(*this);
    return tmp;
  }

  Iterator& operator+(size_t noOfSteps)
  {
    for (uint16_t i{}; i < noOfSteps; ++i)
    {
      ++*this;
    }
    return *this;
  }

  friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; }
  friend bool operator!=(const Iterator& a, const Iterator& b) { return !(a.m_ptr == b.m_ptr); }

private:
  pointer m_ptr;
};

}  // namespace it

namespace cyclic_buffer
{
using it::Iterator;
using size_t = std::size_t;

// Cyclic queue
template <typename T>
class RingBuffer
{
public:
  explicit RingBuffer(std::size_t capacity) : m_capacity(capacity) {}
  ~RingBuffer();

  [[nodiscard]] size_t size() const;
  [[nodiscard]] bool isFull() const;
  [[nodiscard]] bool isEmpty() const;

  [[nodiscard]] std::optional<T> dequeue();
  bool enqueue(T item);

  T* begin();
  T* end();

private:
  const std::size_t m_capacity{};
  T* m_data = new T[m_capacity];

  bool m_isFull{false};  // TODO(damianWu) to delete?
  bool m_isEmpty{true};

  int64_t m_enqueueIndex{};
  int64_t m_dequeueIndex{};
};

template <typename T>
T* RingBuffer<T>::begin()
{
  return m_data;
}

template <typename T>
T* RingBuffer<T>::end()
{
  return m_data + m_capacity;
}

template <typename T>
RingBuffer<T>::~RingBuffer()
{
  delete[] m_data;
}

template <typename T>
inline size_t RingBuffer<T>::size() const
{
  const int64_t distance{m_enqueueIndex - m_dequeueIndex};
  fmt::print("m_enqueueIndex={}, m_dequeueIndex={}, distance={}\n", m_enqueueIndex, m_dequeueIndex,
             distance);
  if (distance < 0)
  {
    return m_capacity + distance;
  }

  if (m_isFull)
  {
    return m_capacity;
  }

  return distance;
}

template <typename T>
bool RingBuffer<T>::enqueue(T item)
{
  fmt::print("Enqueue called\n");

  if (m_isFull)
  {
    fmt::print("Queue is full\n");
    return false;
  }
  m_isEmpty = false;

  m_data[m_enqueueIndex] = std::move(item);
  m_enqueueIndex = (m_enqueueIndex + 1) % m_capacity;
  if (m_enqueueIndex == m_dequeueIndex)
  {
    m_isFull = true;
  }
  return true;
}

template <typename T>
inline bool RingBuffer<T>::isFull() const
{
  return m_isFull;
}

template <typename T>
inline bool RingBuffer<T>::isEmpty() const
{
  return m_isEmpty;
}

template <typename T>
std::optional<T> RingBuffer<T>::dequeue()
{
  fmt::print("Dequeue called\n");
  m_isFull = false;
  if (!m_isEmpty)
  {
    const auto nextIndex{(m_dequeueIndex + 1) % static_cast<int64_t>(m_capacity)};
    if (nextIndex == m_enqueueIndex)
    {
      m_isEmpty = true;
    }
    const auto returnIndex{m_dequeueIndex};
    m_dequeueIndex = nextIndex;
    return {m_data[returnIndex]};
  }
  return std::nullopt;
}
}  // namespace cyclic_buffer

namespace double_linked_list
{
using it::DoubleNode;
using it::Iterator;

// Ex 1.3.31
template <typename T>
class DoubleLinkedList
{
public:
  DoubleLinkedList() = default;
  DoubleLinkedList(const DoubleLinkedList&) = delete;
  DoubleLinkedList(DoubleLinkedList&&) = delete;
  DoubleLinkedList& operator=(DoubleLinkedList&&) = delete;
  DoubleLinkedList& operator=(const DoubleLinkedList&) = delete;
  ~DoubleLinkedList();

  [[nodiscard]] constexpr size_t size() const;
  [[nodiscard]] constexpr bool isEmpty() const;
  [[nodiscard]] std::optional<T> front() const;
  [[nodiscard]] std::optional<T> back() const;
  [[nodiscard]] std::optional<DoubleNode<T>*> find(const T& item);

  Iterator<DoubleNode<T>> begin() { return Iterator<DoubleNode<T>>{m_left}; }
  Iterator<DoubleNode<T>> end() { return Iterator<DoubleNode<T>>{nullptr}; }

  void clear();
  void pushLeft(const T& item);
  void pushRight(const T& item);
  bool putBefore(T item, T newItem);
  bool putAfter(T item, T newItem);
  bool remove(const T& item);

  void deleteFront();
  void deleteBack();

private:
  [[nodiscard]] bool putFirst(const T& item);

  DoubleNode<T>* m_left{};
  DoubleNode<T>* m_right{};

  size_t m_size{};
};

template <typename T>
bool DoubleLinkedList<T>::remove(const T& item)
{
  auto nodeOpt{find(item)};
  if (!nodeOpt.has_value())
  {
    return false;
  }

  auto* node{nodeOpt.value()};
  // Get neighbors
  auto prev{node->prev};
  auto next{node->next};

  // Handle left
  if (prev != nullptr)
  {
    prev->next = next;
  }
  else
  {
    m_left = next;
  }

  // Handle right
  if (next != nullptr)
  {
    next->prev = prev;
  }
  else
  {
    m_right = prev;
  }

  delete node;

  --m_size;
  return true;
}

template <typename T>
std::optional<DoubleNode<T>*> DoubleLinkedList<T>::find(const T& item)
{
  const auto it{std::find_if(begin(), end(), [&item](auto node) { return node.item == item; })};
  return it == end() ? std::nullopt : std::make_optional(&*it);
}

template <typename T>
bool DoubleLinkedList<T>::putAfter(T item, T newItem)
{
  const auto nodeOpt{find(item)};
  if (!nodeOpt.has_value())
  {
    return false;
  }

  auto* node{*nodeOpt};
  auto* next{node->next};
  auto* newNode{new DoubleNode<T>{std::move(newItem), next, node}};

  node->next = newNode;
  if (next != nullptr)
  {
    next->prev = newNode;
  }
  else
  {
    m_right = newNode;
  }
  ++m_size;
  return true;
}

template <typename T>
bool DoubleLinkedList<T>::putBefore(T item, T newItem)
{
  auto nodeOpt{find(item)};
  if (!nodeOpt.has_value())
  {
    return false;
  }

  auto* const node{nodeOpt.value()};
  auto* const prev{node->prev};
  auto* const newNode{new DoubleNode<T>{std::move(newItem), node, prev}};

  node->prev = newNode;

  if (prev != nullptr)
  {
    prev->next = newNode;
  }
  else  // Put before leftmost (new leftmost)
  {
    m_left = newNode;
  }

  ++m_size;
  return true;
}

template <typename T>
void DoubleLinkedList<T>::deleteBack()
{
  if (isEmpty())
  {
    return;
  }

  if (m_left == m_right)
  {
    m_left = nullptr;
  }

  auto* prev{m_right->prev};
  delete m_right;

  m_right = prev;
  if (m_right != nullptr)
  {
    m_right->next = nullptr;
  }
  --m_size;
}

template <typename T>
void DoubleLinkedList<T>::deleteFront()
{
  if (isEmpty())
  {
    return;
  }

  if (m_left == m_right)
  {
    m_right = nullptr;
  }

  auto* next{m_left->next};
  delete m_left;

  m_left = next;
  if (m_left != nullptr)
  {
    m_left->prev = nullptr;
  }
  --m_size;
}

template <typename T>
std::optional<T> DoubleLinkedList<T>::front() const
{
  if (m_left == nullptr)
  {
    return {};
  }
  return {m_left->item};
}

template <typename T>
std::optional<T> DoubleLinkedList<T>::back() const
{
  if (m_right == nullptr)
  {
    return {};
  }
  return {m_right->item};
}

template <typename T>
void DoubleLinkedList<T>::clear()
{
  if (isEmpty())
  {
    return;
  }

  for (DoubleNode<T>* tempNext{}; m_left->next != nullptr;)
  {
    tempNext = m_left->next;
    delete m_left;
    m_left = tempNext;
  }
  delete m_left;
  m_left = nullptr;
  m_right = nullptr;
  m_size = 0;
}

template <typename T>
DoubleLinkedList<T>::~DoubleLinkedList()
{
  clear();
}

template <typename T>
inline bool DoubleLinkedList<T>::putFirst(const T& item)
{
  if (m_size - 1 == 0)
  {
    m_left = new DoubleNode<T>{item};
    m_right = m_left;
    return true;
  }
  return false;
}

template <typename T>
void DoubleLinkedList<T>::pushLeft(const T& item)
{
  ++m_size;

  if (putFirst(item))
  {
    return;
  }

  m_left->prev = new DoubleNode<T>{item};
  m_left->prev->next = m_left;
  m_left = m_left->prev;

  if (m_right == m_left)
  {
    m_right = m_left->next;
  }
}

template <typename T>
void DoubleLinkedList<T>::pushRight(const T& item)
{
  ++m_size;

  if (putFirst(item))
  {
    return;
  }

  m_right->next = new DoubleNode<T>{item};
  m_right->next->prev = m_right;
  m_right = m_right->next;

  if (m_left == m_right)
  {
    m_left = m_right->prev;
  }
}

template <typename T>
[[nodiscard]] constexpr inline size_t DoubleLinkedList<T>::size() const
{
  return m_size;
}

template <typename T>
[[nodiscard]] constexpr inline bool DoubleLinkedList<T>::isEmpty() const
{
  return m_size == 0;
}

}  // namespace double_linked_list

namespace queue
{
using it::Iterator;
using it::SingleNode;

template <typename Item>
struct Queue
{
  Queue() = default;
  virtual ~Queue() = default;
  Queue(const Queue&) = delete;
  Queue(Queue&&) = delete;
  Queue& operator=(Queue&&) = delete;
  Queue& operator=(const Queue&) = delete;

  virtual Item dequeue() = 0;
  virtual void enqueue(Item item) = 0;
  virtual std::optional<Item> remove(size_t k) = 0;

  [[nodiscard]] virtual bool isEmpty() const = 0;
  [[nodiscard]] virtual std::size_t size() const = 0;

  virtual Iterator<SingleNode<Item>> begin() = 0;
  virtual Iterator<SingleNode<Item>> end() = 0;

protected:
  std::size_t m_size{};
};

// Queue of type FIFO
template <typename Item>
class QueueImpl : public Queue<Item>
{
  using Queue<Item>::m_size;

public:
  QueueImpl() = default;
  ~QueueImpl() override;

  void enqueue(Item item) override;
  Item dequeue() override;
  std::optional<Item> remove(size_t k) override;

  [[nodiscard]] bool isEmpty() const override;
  [[nodiscard]] std::size_t size() const override;
  Iterator<SingleNode<Item>> begin() override;
  Iterator<SingleNode<Item>> end() override;

  void clear();

private:
  SingleNode<Item>* m_left{};
  SingleNode<Item>* m_right{};
};

template <typename Item>
Iterator<SingleNode<Item>> QueueImpl<Item>::begin()
{
  return Iterator<SingleNode<Item>>(m_left);
}

template <typename Item>
Iterator<SingleNode<Item>> QueueImpl<Item>::end()
{
  return Iterator<SingleNode<Item>>(nullptr);
}

template <typename Item>
std::optional<Item> QueueImpl<Item>::remove(size_t k)
{
  // If out of range
  if (k >= size())
  {
    return std::nullopt;
  }

  if (size() == 1 || k == 0)
  {
    return {dequeue()};
  }

  Iterator<SingleNode<Item>> currentIt{begin()};
  Iterator<SingleNode<Item>> prevIt{currentIt};
  for (size_t i{}; i < k; ++i)
  {
    prevIt = currentIt++;
  }

  SingleNode<Item>& prevNode{*prevIt};
  const SingleNode<Item>& currentNode{*currentIt};
  Item currentNodeItem{currentNode.item};

  prevNode.next = currentNode.next;
  delete &currentNode;

  if (size() - 1 == k)
  {
    m_right = &prevNode;
  }
  --m_size;

  return {currentNodeItem};
}

template <typename Item>
void QueueImpl<Item>::clear()
{
  if (isEmpty())
  {
    return;
  }

  for (SingleNode<Item>* next{}; m_left->next != nullptr;)
  {
    next = m_left->next;
    delete m_left;
    m_left = next;
  }

  delete m_left;
  m_left = nullptr;
  m_size = 0;
}

template <typename Item>
QueueImpl<Item>::~QueueImpl()
{
  clear();
}

template <typename Item>
Item QueueImpl<Item>::dequeue()
{
  if (isEmpty())
  {
    return Item{};
  }

  --m_size;

  auto* const oldFirst{m_left};
  m_left = m_left->next;

  const Item oldItem{std::move(oldFirst->item)};
  delete oldFirst;

  return oldItem;
}

template <typename Item>
void QueueImpl<Item>::enqueue(Item item)
{
  auto oldLast{m_right};
  m_right = new SingleNode<Item>{std::move(item)};

  ++m_size;

  if (m_left == nullptr)
  {
    m_left = m_right;
    return;
  }
  oldLast->next = m_right;
}

template <typename Item>
[[nodiscard]] inline bool QueueImpl<Item>::isEmpty() const
{
  return m_size == 0;
}

template <typename Item>
[[nodiscard]] inline std::size_t QueueImpl<Item>::size() const
{
  return m_size;
}

// Ex 1.3.35
template <typename T>
struct RandomQueue : public QueueImpl<T>
{
  using QueueImpl<T>::size;
  using QueueImpl<T>::begin;

  T sample();
};

template <typename T>
T RandomQueue<T>::sample()
{
  std::random_device random_device;
  std::mt19937 random_engine(random_device());
  std::uniform_int_distribution<size_t> distribution(0, size() - 1);

  const size_t randomNumber = distribution(random_engine);

  return (begin() + randomNumber)->item;
}

}  // namespace queue

namespace efficient_stack
{
// Queue of type LIFO
template <typename Item>
class Stack
{
  using iterator = Item*;

public:
  explicit Stack(size_t capacity = 0);
  Stack(Stack&&) = delete;
  Stack(const Stack&) = delete;
  Stack& operator=(const Stack&) = delete;
  Stack& operator=(Stack&&) = delete;
  ~Stack();

  void push(Item item);
  Item pop();

  [[nodiscard]] Item peek() const;
  [[nodiscard]] bool isEmpty() const;
  [[nodiscard]] std::ptrdiff_t size() const;

  [[nodiscard]] iterator begin() const;
  [[nodiscard]] iterator end() const;
  [[nodiscard]] std::reverse_iterator<iterator> rbegin();
  [[nodiscard]] std::reverse_iterator<iterator> rend();
  // [[nodiscard]] const Item* cbegin() const;  // TODO(damianWu) to implement
  // [[nodiscard]] const Item* cend() const;    // TODO(damianWu) to implement

  void dump(std::ostream& ostream = std::cout) const;

private:
  [[nodiscard]] std::ptrdiff_t capacity() const;
  [[nodiscard]] size_t calculateNewCapacity();
  void allocate(iterator first, iterator firstFree);
  void free(iterator first, iterator firstFree, std::ptrdiff_t noOfElToDeallocate);
  void reallocate();

  static std::allocator<Item> ms_allocator;
  static std::allocator_traits<decltype(ms_allocator)> ms_allocatorTraits;

  Item* m_left;
  Item* m_onePastLast;
  Item* m_leftFree;

  static constexpr int16_t m_extraAllocFactor{2};
};

template <typename Item>
std::allocator<Item> Stack<Item>::ms_allocator;

template <typename Item>
std::allocator_traits<decltype(Stack<Item>::ms_allocator)> Stack<Item>::ms_allocatorTraits;

template <typename Item>
Stack<Item>::Stack(size_t capacity)
    : m_left{ms_allocatorTraits.allocate(ms_allocator, capacity)},
      m_onePastLast{m_left + capacity},
      m_leftFree{m_left}
{
}

template <typename Item>
Stack<Item>::~Stack()
{
  free(m_left, m_leftFree, m_onePastLast - m_left);
}

template <typename Item>
[[nodiscard]] inline std::ptrdiff_t Stack<Item>::size() const
{
  return m_leftFree - m_left;
}

template <typename Item>
[[nodiscard]] inline std::ptrdiff_t Stack<Item>::capacity() const
{
  return m_onePastLast - m_left;
}
template <typename Item>
Item Stack<Item>::peek() const
{
  if (isEmpty())
  {
    return {};
  }
  return *(m_leftFree - 1);
}

template <typename Item>
void Stack<Item>::push(Item item)
{
  if (m_leftFree == m_onePastLast)
  {
    reallocate();
  }
  ms_allocatorTraits.construct(ms_allocator, m_leftFree++, std::move(item));
}

template <typename Item>
void Stack<Item>::reallocate()
{
  auto* const prevFirstElement{m_left};
  auto* const prevFirstFreeElement{m_leftFree};
  auto const noOfElToDeallocate{m_onePastLast - prevFirstElement};

  allocate(prevFirstElement, prevFirstFreeElement);
  free(prevFirstElement, prevFirstFreeElement, noOfElToDeallocate);
}

template <typename Item>
void Stack<Item>::allocate(iterator first, iterator firstFree)
{
  const size_t newCapacity{calculateNewCapacity()};
  m_left = ms_allocatorTraits.allocate(ms_allocator, newCapacity);
  m_onePastLast = m_left + newCapacity;
  m_leftFree = std::uninitialized_move(first, firstFree, m_left);
}

template <typename Item>
void Stack<Item>::free(iterator first, iterator firstFree, std::ptrdiff_t noOfElToDeallocate)
{
  if (firstFree != nullptr)
  {
    for (auto objectToDestroy{firstFree}; objectToDestroy != first;)
    {
      ms_allocatorTraits.destroy(ms_allocator, --objectToDestroy);
    }
    ms_allocatorTraits.deallocate(ms_allocator, first, static_cast<size_t>(noOfElToDeallocate));
  }
}

template <typename Item>
inline size_t Stack<Item>::calculateNewCapacity()
{
  return (capacity() + 1) * m_extraAllocFactor;
}

template <typename Item>
Item Stack<Item>::pop()
{
  Item item{};
  if (m_leftFree != nullptr && m_leftFree != m_left)
  {
    item = *(m_leftFree - 1);
    ms_allocatorTraits.destroy(ms_allocator, --m_leftFree);
    // TODO(damianWu) - reduce capacity
    /*  if (capacity() >= 4 * size())
    {
       reallocate()
    } */
  }
  return item;
}

template <typename Item>
inline bool Stack<Item>::isEmpty() const
{
  return size() == 0;
}

template <typename Item>
inline typename Stack<Item>::iterator Stack<Item>::begin() const
{
  return m_left;
}

template <typename Item>
inline typename Stack<Item>::iterator Stack<Item>::end() const
{
  return m_leftFree;
}

// Points on the first free element
template <typename Item>
std::reverse_iterator<typename Stack<Item>::iterator> Stack<Item>::rbegin()
{
  return std::reverse_iterator<Stack<Item>::iterator>(m_leftFree);
}

// Points on the first element
template <typename Item>
std::reverse_iterator<typename Stack<Item>::iterator> Stack<Item>::rend()
{
  return std::reverse_iterator<Stack<Item>::iterator>(m_left);
}

template <typename Item>
void Stack<Item>::dump(std::ostream& ostream) const
{
  ostream << "m_left=" << m_left << '\n';
  ostream << "m_leftFree=" << m_leftFree << '\n';
  ostream << "m_onePastLast=" << m_onePastLast << '\n';
}
}  // namespace efficient_stack

namespace linked_list_stack
{
using it::Iterator;
using it::SingleNode;

// Queue of type LIFO
template <typename Item>
class Stack
{
public:
  Stack() = default;
  Stack(const Stack&) = delete;
  Stack(Stack&&) = delete;
  Stack& operator=(Stack&) = delete;
  Stack& operator=(Stack&&) = delete;
  ~Stack();

  [[nodiscard]] bool isEmpty() const;
  [[nodiscard]] std::size_t size() const;

  Item pop();

  void push(Item);
  void clear();

  Iterator<SingleNode<Item>> begin() const { return Iterator<SingleNode<Item>>{m_left}; }
  Iterator<SingleNode<Item>> end() const { return Iterator<SingleNode<Item>>{nullptr}; }

private:
  SingleNode<Item>* m_left{};
  std::size_t m_size{};
};

template <typename Item>
Stack<Item>::~Stack()
{
  clear();
}

template <typename Item>
void Stack<Item>::clear()
{
  while (!isEmpty())
  {
    pop();
  }
}

template <typename Item>
Item Stack<Item>::pop()
{
  if (isEmpty())
  {
    return Item();
  }
  --m_size;

  const auto* const oldFirst{m_left};
  Item oldItem{oldFirst->item};

  m_left = oldFirst->next;
  delete oldFirst;

  return oldItem;
}

template <typename Item>
void Stack<Item>::push(Item item)
{
  auto* oldFirst{m_left};
  m_left = new SingleNode<Item>{std::move(item), oldFirst};

  ++m_size;
}

template <typename Item>
[[nodiscard]] inline bool Stack<Item>::isEmpty() const
{
  return m_size == 0;
}
template <typename Item>
[[nodiscard]] inline std::size_t Stack<Item>::size() const
{
  return m_size;
}
}  // namespace linked_list_stack

namespace homework
{
bool ex1_3_5(std::string_view input);
void ex1_3_37(int32_t n, int32_t m);
std::string ex1_3_40(const std::string& input);
}  // namespace homework
}  // namespace ch1

#endif  // SRC_CH1_CH1_CH1_HPP_
