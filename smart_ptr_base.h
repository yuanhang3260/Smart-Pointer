#ifndef SMART_POINTER_BASE__
#define SMART_POINTER_BASE__

#include <atomic>

struct Ref {
  std::atomic<unsigned int> count;
  std::atomic<unsigned int> weak_count;

  Ref() : count(1), weak_count(0) {}
  virtual void destroy() = 0;
  virtual ~Ref() {}
};

template<class T, class Deleter>
struct RefImpl : public Ref {
  T* p;
  Deleter d;

  RefImpl(T* _p, Deleter _d) : p(_p), d(_d) {}
  virtual void destroy() {
    d(p);
  }
};

template<class T, class Deleter>
struct UniqueRefImpl : public Ref {
  T* p;
  Deleter d;

  UniqueRefImpl(T* _p, Deleter _d) : p(_p), d(_d) {}
  virtual void destroy() {
    d(p);
  }
};

template<class T>
struct default_deleter {
  void operator()(T* p) {
    if (p) {
      delete p;
    }
  }
};

#endif  /* SMART_POINTER_BASE__ */
