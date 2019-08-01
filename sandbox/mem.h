#ifndef MEM_INCLUDED_H
#define MEM_INCLUDED_H

#include <cstddef>
#include <cstdint>
#include <sys/mman.h>

namespace mem {

static constexpr size_t alignment = 8;

struct header {
  bool mark;
  bool free;
}; // Assume sizeof(header) < alignment

// Force alignment + ensure that W >= sizeof(void*)
// (need that space for free list's next pointers)
constexpr size_t chunk_size_of(size_t w) {
  auto aligned = w + (alignment - w%alignment);
  return sizeof(void*) > aligned ? sizeof(void*) : aligned;
}

template<typename T>
constexpr size_t size_class_of = chunk_size_of(sizeof(T));

struct free_list { free_list* next; };

// Arena for items of size w where W_ = chunk_size_of(w)
template<size_t W_>
class size_class {
public:
  // max is the capacity in bytes
  static size_class<W_>* init(size_t max);

  static void* alloc(size_class<W_>*);
  static void free(size_class<W_>*, void* p);

public:

  // Reserve space for header
  static constexpr size_t W = W_ + alignment;

  using block = uint8_t[W];

  // block <-> heap pointer

  static void* of_block(block* p)
  { return reinterpret_cast<void*>(reinterpret_cast<char*>(p) + alignment); }

  static block* to_block(void* p)
  { return reinterpret_cast<block*>(reinterpret_cast<char*>(p) - alignment); }

  // Updating block flags

  static block* set_mark(block* p, bool b)
  { reinterpret_cast<header*>(p)->mark = b; return p; }

  static block* set_free(block* p, bool b)
  { reinterpret_cast<header*>(p)->free = b; return p; }
 
public:
  free_list free_;
  block* end_;
  block* cap_;
  block data_[0];
  // x in free <=> reinterpret_cast<header*>(to_block(x))->free
  // end in [&data .. cap)
  // x in free ==> x in [&data .. cap) /\ x < end
};

// Hacky constructor.
template<size_t W_>
size_class<W_>* size_class<W_>::init(size_t max) {
  using C = size_class<W_>;
  auto m = reinterpret_cast<C*>(mmap(
    nullptr, sizeof(C) + max,
    PROT_READ | PROT_WRITE,
    MAP_PRIVATE | MAP_ANONYMOUS,
    -1, 0));
  m->free_.next = nullptr;
  m->end_ = reinterpret_cast<typename C::block*>(&m->data_);
  m->cap_ = reinterpret_cast<typename C::block*>(
    max + reinterpret_cast<char*>(&m->data_));
  return m;
}

template<size_t W_>
void* size_class<W_>::alloc(size_class<W_>* m) {
  if (m->free_.next == nullptr)
    return m->end_ + sizeof(block) > m->cap_
      ? nullptr
      : of_block(set_free(m->end_++, false));
  auto res = of_block(set_free(to_block(m->free_.next), false));
  m->free_ = *(m->free_.next);
  return res;
}

template<size_t W_>
void size_class<W_>::free(size_class<W_>* m, void* p) {
  (void) set_free(to_block(p), true);
  auto q = reinterpret_cast<free_list*>(p);
  *q = m->free_;
  m->free_.next = q;
}

// ----------------------------------------

// Specialization will generate a global arena for every size class
template<size_t W>
auto arena = size_class<W>::init(1 << 30);

// Allocate in the proper size class
template<typename T>
T* alloc() {
  constexpr size_t w = size_class_of<T>;
  return reinterpret_cast<T*>(size_class<w>::alloc(arena<w>));
}

// Free in the proper size class
template<typename T>
void free(T* p) {
  constexpr size_t w = size_class_of<T>;
  size_class<w>::free(arena<w>, p);
}

// Read flags

template<typename T>
bool get_free(T* p) {
  constexpr size_t w = size_class_of<T>;
  return reinterpret_cast<header*>(size_class<w>::to_block(p))->free;
}

template<typename T>
bool get_mark(T* p) {
  constexpr size_t w = size_class_of<T>;
  return reinterpret_cast<header*>(size_class<w>::to_block(p))->mark;
}

// Write mark flag

template<typename T>
void set_mark(T* p, bool b) {
  constexpr size_t w = size_class_of<T>;
  reinterpret_cast<header*>(size_class<w>::to_block(p))->mark = b;
}

} // mem

#endif // MEM_INCLUDED_H