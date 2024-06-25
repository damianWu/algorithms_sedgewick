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
#include <string>
#include <utility>

#include "ch1/ch1.hpp"

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
};

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
  Iterator operator++(int)  // NOLINT
  {
    Iterator tmp{m_ptr};
    ++(*this);
    return tmp;
  }

  Iterator& operator+(uint16_t noOfSteps)
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

namespace double_linked_list
{
using it::DoubleNode;
using it::Iterator;

template <typename T>
class LinkedList
{
public:
  LinkedList() = default;
  LinkedList(const LinkedList&) = delete;
  LinkedList(LinkedList&&) = delete;
  LinkedList& operator=(LinkedList&&) = delete;
  LinkedList& operator=(const LinkedList&) = delete;
  ~LinkedList();

  [[nodiscard]] constexpr size_t size() const;
  [[nodiscard]] constexpr bool isEmpty() const;
  [[nodiscard]] std::optional<T> front() const;
  [[nodiscard]] std::optional<T> back() const;

  Iterator<DoubleNode<T>> begin() { return Iterator<DoubleNode<T>>{m_left}; }
  Iterator<DoubleNode<T>> end() { return Iterator<DoubleNode<T>>{nullptr}; }

  void clear();
  void pushLeft(T item);
  void pushRight(T item);
  bool putBefore(T item, T newItem);
  bool putAfter(T item, T newItem);

  // [/] wstawiania za danym węzłem
  // [ ] usuwania danego węzła.
  void deleteFront();
  void deleteBack();

private:
  [[nodiscard]] bool putFirst(T&& item);
  [[nodiscard]] std::optional<DoubleNode<T>*> findNode(const T& item);

  DoubleNode<T>* m_left{};
  DoubleNode<T>* m_right{};

  size_t m_size{};
};

template <typename T>
std::optional<DoubleNode<T>*> LinkedList<T>::findNode(const T& item)
{
  auto it{std::find_if(begin(), end(), [&item](auto it) { return it.item == item; })};
  return (it == end()) ? std::nullopt : std::make_optional(&*it);
}

template <typename T>
bool LinkedList<T>::putAfter(T item, T newItem)
{
  const auto nodeOpt{findNode(item)};
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
bool LinkedList<T>::putBefore(T item, T newItem)
{
  auto nodeOpt{findNode(item)};
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
void LinkedList<T>::deleteBack()
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
void LinkedList<T>::deleteFront()
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
std::optional<T> LinkedList<T>::front() const
{
  if (m_left == nullptr)
  {
    return {};
  }
  return {m_left->item};
}

template <typename T>
std::optional<T> LinkedList<T>::back() const
{
  if (m_right == nullptr)
  {
    return {};
  }
  return {m_right->item};
}

template <typename T>
void LinkedList<T>::clear()
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
LinkedList<T>::~LinkedList()
{
  clear();
}

template <typename T>
inline bool LinkedList<T>::putFirst(T&& item)
{
  // TODO(damianWu) Refactor - too complicated/too many dependencies
  const auto isListEmpty{isEmpty()};
  ++m_size;
  if (isListEmpty)
  {
    m_left = new DoubleNode<T>{std::move(item)};
    m_right = m_left;
    return true;
  }
  return false;
}

template <typename T>
void LinkedList<T>::pushLeft(T item)
{
  if (putFirst(std::move(item)))
  {
    return;
  }

  m_left->prev = new DoubleNode<T>{std::move(item)};
  m_left->prev->next = m_left;
  m_left = m_left->prev;

  if (m_right == m_left)
  {
    m_right = m_left->next;
  }
}

template <typename T>
void LinkedList<T>::pushRight(T item)
{
  if (putFirst(std::move(item)))
  {
    return;
  }

  m_right->next = new DoubleNode<T>{std::move(item)};
  m_right->next->prev = m_right;
  m_right = m_right->next;

  if (m_left == m_right)
  {
    m_left = m_right->prev;
  }
}

template <typename T>
[[nodiscard]] constexpr inline size_t LinkedList<T>::size() const
{
  return m_size;
}

template <typename T>
[[nodiscard]] constexpr inline bool LinkedList<T>::isEmpty() const
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

  virtual void enqueue(Item item) = 0;
  virtual Item dequeue() = 0;
  virtual void clear() = 0;
  virtual bool remove(size_t k) = 0;

  [[nodiscard]] virtual bool isEmpty() const = 0;
  [[nodiscard]] virtual std::size_t size() const = 0;

  virtual Iterator<SingleNode<Item>> begin() = 0;
  virtual Iterator<SingleNode<Item>> end() = 0;
};

// Queue of type FIFO
template <typename Item>
class QueueImpl : public Queue<Item>
{
public:
  QueueImpl() = default;
  ~QueueImpl() override;

  void enqueue(Item item) override;
  Item dequeue() override;
  void clear() override;
  bool remove(size_t k) override;

  [[nodiscard]] bool isEmpty() const override;
  [[nodiscard]] std::size_t size() const override;

  Iterator<SingleNode<Item>> begin() override { return Iterator<SingleNode<Item>>(m_left); }
  Iterator<SingleNode<Item>> end() override { return Iterator<SingleNode<Item>>(nullptr); }

private:
  SingleNode<Item>* m_left{};
  SingleNode<Item>* m_right{};

  std::size_t m_size{};
};

template <typename Item>
bool QueueImpl<Item>::remove(size_t k)
{
  // If out of range
  if (k >= size())
  {
    return false;
  }

  if (size() == 1 || k == 0)
  {
    dequeue();
    return true;
  }

  Iterator<SingleNode<Item>> currentIt{begin()};
  Iterator<SingleNode<Item>> prevIt{currentIt};
  for (size_t i{}; i < k; ++i)
  {
    prevIt = currentIt++;
  }

  SingleNode<Item>& prevNode{*prevIt};
  const SingleNode<Item>& currentNode{*currentIt};

  prevNode.next = currentNode.next;
  delete &currentNode;

  if (size() - 1 == k)
  {
    m_right = &prevNode;
  }
  --m_size;

  return true;
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
  [[nodiscard]] const Item* cbegin() const;
  [[nodiscard]] iterator cend() const;

  // TODO(damianWu) - to delete
  void dump()
  {
    std::cout << "m_left=" << m_left << '\n';
    std::cout << "m_leftFree=" << m_leftFree << '\n';
    std::cout << "m_onePastLast=" << m_onePastLast << '\n';
  }

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
    ms_allocatorTraits.deallocate(ms_allocator, first, noOfElToDeallocate);
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
    // if (capacity() >= 4 * size())
    // {
    //    reallocate()
    // }
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

// TODO(damianWu) - how to improve this (using aliases)?
template <typename Item>
const Item* Stack<Item>::cbegin() const
{
  return m_left;
}

template <typename Item>
inline typename Stack<Item>::iterator Stack<Item>::end() const
{
  return m_leftFree;
}

// TODO(damianWu) - how to improve this?
template <typename Item>
typename Stack<Item>::iterator Stack<Item>::cend() const
{
  return const_cast<iterator>(m_leftFree);  // TODO(damianWu) - It is incorrect.
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

}  // namespace ch1

namespace homework
{
bool ex1_3_5(std::string_view input)
{
  ch1::efficient_stack::Stack<char> opening_parenthesis;

  const std::string open{"[({"};
  const std::string close{"])}"};

  for (const auto c : input)
  {
    if (open.contains(c))
    {
      opening_parenthesis.push(c);
    }
    else if (close.contains(c))
    {
      // c is closing bracket
      // o is opening bracket
      const auto o{opening_parenthesis.pop()};
      if (open.find(o) != close.find(c))
      {
        return false;
      }
    }
    else
    {
      return false;
    }
  }
  return opening_parenthesis.isEmpty();
}

}  // namespace homework

#endif  // SRC_CH1_CH1_CH1_HPP_
