// Copyright [2024] <@damianWu>
#ifndef SRC_CH1_CH1_CH1_HPP_
#define SRC_CH1_CH1_CH1_HPP_

#include <fmt/core.h>  // TODO(damianWu) - to delete

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <memory>
#include <utility>

namespace ch1
{
using size_t = std::size_t;

template <typename Item>
class MultiSet
{
   // TODO(damianWu). Does access specifier have impact here? Yes it does.
   using iterator = Item*;

 public:
   MultiSet() = default;

   void add(Item item);
   [[nodiscard]] bool isEmpty() const;
   [[nodiscard]] std::size_t size() const;

   iterator begin() const;
   iterator cbegin() const;
   iterator end() const;
   iterator cend() const;
};

// Queue of type FIFO
template <typename Item>
class Queue
{
   using iterator = Item*;

 public:
   Queue() = default;

   void enqueue(Item item);
   Item dequeue();
   [[nodiscard]] bool isEmpty() const;
   [[nodiscard]] std::size_t size() const;

   iterator begin() const;
   iterator cbegin() const;
   iterator end() const;
   iterator cend() const;
};

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
      std::cout << "m_first=" << m_first << '\n';
      std::cout << "m_firstFree=" << m_firstFree << '\n';
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

   Item* m_first;
   Item* m_onePastLast;
   Item* m_firstFree;

   static constexpr int16_t m_extraAllocFactor{2};
};
}  // namespace efficient_stack

namespace linked_list_stack
{

template <typename Item>
class Stack
{
   struct Node
   {
      Item item{};
      Node* next{};
   };

   using iterator = Node*;

 public:
   Stack() = default;
   Stack(const Stack&) = delete;
   Stack(Stack&&) = delete;
   Stack& operator=(Stack&) = delete;
   Stack& operator=(Stack&&) = delete;
   ~Stack();

   [[nodiscard]] bool isEmpty() const;
   [[nodiscard]] std::size_t size() const;

   [[nodiscard]] Item pop();

   void push(Item&&);
   void push(const Item&);
   void clear();

   // TODO(damianWu) - to implement
   iterator begin() const;
   iterator end() const;

 private:
   iterator m_first{};
   std::size_t m_size{};
};
}  // namespace linked_list_stack

/* Implementation */
namespace linked_list_stack
{

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
      (void)pop();
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

   const auto* const oldFirst{m_first};
   Item deletedItem{oldFirst->item};

   m_first = oldFirst->next;
   delete oldFirst;

   return deletedItem;
}

template <typename Item>
void Stack<Item>::push(Item&& item)
{
   auto* oldFirst{m_first};
   m_first = new Node{std::move(item)};
   m_first->next = oldFirst;

   ++m_size;
}

template <typename Item>
void Stack<Item>::push(const Item& item)
{
   auto* oldFirst{m_first};
   m_first = new Node{std::move(item)};
   m_first->next = oldFirst;

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

namespace efficient_stack
{
template <typename Item>
std::allocator<Item> Stack<Item>::ms_allocator;

template <typename Item>
std::allocator_traits<decltype(Stack<Item>::ms_allocator)> Stack<Item>::ms_allocatorTraits;

template <typename Item>
Stack<Item>::Stack(size_t capacity)
    : m_first{ms_allocatorTraits.allocate(ms_allocator, capacity)},
      m_onePastLast{m_first + capacity},
      m_firstFree{m_first}
{
}

template <typename Item>
Stack<Item>::~Stack()
{
   free(m_first, m_firstFree, m_onePastLast - m_first);
}

template <typename Item>
[[nodiscard]] inline std::ptrdiff_t Stack<Item>::size() const
{
   return m_firstFree - m_first;
}

template <typename Item>
[[nodiscard]] inline std::ptrdiff_t Stack<Item>::capacity() const
{
   return m_onePastLast - m_first;
}

template <typename Item>
void Stack<Item>::push(Item item)
{
   if (m_firstFree == m_onePastLast)
   {
      reallocate();
   }
   ms_allocatorTraits.construct(ms_allocator, m_firstFree++, std::move(item));
}

template <typename Item>
void Stack<Item>::reallocate()
{
   auto* const prevFirstElement{m_first};
   auto* const prevFirstFreeElement{m_firstFree};
   auto const noOfElToDeallocate{m_onePastLast - prevFirstElement};

   allocate(prevFirstElement, prevFirstFreeElement);
   free(prevFirstElement, prevFirstFreeElement, noOfElToDeallocate);
}

template <typename Item>
void Stack<Item>::allocate(iterator first, iterator firstFree)
{
   const auto newCapacity{calculateNewCapacity()};
   m_first = ms_allocatorTraits.allocate(ms_allocator, newCapacity);
   m_onePastLast = m_first + newCapacity;
   assert(newCapacity == m_onePastLast - m_first);  // TODO(damianWu) - to delete?
   m_firstFree = std::uninitialized_move(first, firstFree, m_first);
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
   if (m_firstFree != nullptr && m_firstFree != m_first)
   {
      item = *(m_firstFree - 1);
      ms_allocatorTraits.destroy(ms_allocator, --m_firstFree);
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
   return m_first;
}

// TODO(damianWu) - how to improve this (using aliases)?
template <typename Item>
const Item* Stack<Item>::cbegin() const
{
   return m_first;
}

template <typename Item>
inline typename Stack<Item>::iterator Stack<Item>::end() const
{
   return m_firstFree;
}

// TODO(damianWu) - how to improve this?
template <typename Item>
typename Stack<Item>::iterator Stack<Item>::cend() const
{
   return const_cast<iterator>(m_firstFree);  // TODO(damianWu) - It is incorrect.
}

// Points on the first free element
template <typename Item>
std::reverse_iterator<typename Stack<Item>::iterator> Stack<Item>::rbegin()
{
   return std::reverse_iterator<Stack<Item>::iterator>(m_firstFree);
}

// Points on the first element
template <typename Item>
std::reverse_iterator<typename Stack<Item>::iterator> Stack<Item>::rend()
{
   return std::reverse_iterator<Stack<Item>::iterator>(m_first);
}
}  // namespace efficient_stack

// template <typename Item>
// void MultiSet<Item>::add(Item item)
// {
// }

// template <typename Item>
// [[nodiscard]] bool MultiSet<Item>::isEmpty() const;

// template <typename Item>
// [[nodiscard]] std::size_t MultiSet<Item>::size() const;

// template <typename Item>
// typename MultiSet<Item>::iterator MultiSet<Item>::begin() const
// {
// }

// template <typename Item>
// MultiSet::iterator MultiSet<Item>::cbegin() const;

// template <typename Item>
// MultiSet::iterator MultiSet<Item>::end() const;

// template <typename Item>
// MultiSet::iterator MultiSet<Item>::cend() const;

}  // namespace ch1

#endif  // SRC_CH1_CH1_CH1_HPP_
