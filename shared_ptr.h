#ifndef SHARED_POINTER_H__
#define SHARED_POINTER_H__

#include "smart_ptr_base.h"

template<class T> class weak_ptr;

template<class T>
class shared_ptr {
 public:
  // Default constructor.
  shared_ptr() = default;

  // Delcare as explicit to prevent raw pointer p being
  // implicitly used to construct multiple smart pointers.
  template<class U>
  explicit shared_ptr(U* p) :
      pointer_(p),
      refs_(new RefImpl<U, default_deleter<U>>(p, default_deleter<U>())) {
  }

  template<class U, class Deleter>
  shared_ptr(U* p, Deleter d) :
      pointer_(p),
      refs_(new RefImpl<U, Deleter>(p, d)) {
  }

  ~shared_ptr() { clear(); }

  // copy constructor
  shared_ptr(const shared_ptr& other) :
      pointer_(other.pointer_),
      refs_(other.refs_) {
    if (refs_) {
      refs_->count++;
    }
  }

  template<class U>
  shared_ptr(const shared_ptr<U>& other) :
      pointer_(other.pointer_),
      refs_(other.refs_) {
    if (refs_) {
      refs_->count++;
    }
  }

  // move constructor
  shared_ptr(shared_ptr&& other) :
      pointer_(other.pointer_),
      refs_(other.refs_) {
    other.pointer_ = nullptr;
    other.refs_ = nullptr;
  }

  template<class U>
  shared_ptr(shared_ptr<U>&& other) :
      pointer_(other.pointer_),
      refs_(other.refs_) {
    other.pointer_ = nullptr;
    other.refs_ = nullptr;
  }

  // Copy assignment.
  shared_ptr& operator=(const shared_ptr& other) {
    if (this != &other) {
      clear();
      pointer_ = other.pointer_;
      refs_ = other.refs_;
      refs_->count++;  // atomic inc
    }
  }

  // Move assignment.
  shared_ptr& operator=(const shared_ptr&& other) {
    if (this != &other) {
      clear();

      pointer_ = other.pointer_;
      refs_ = other.refs_;

      other.pointer_ = nullptr;
      other.refs_ = nullptr;
    }
  }

  // reset shared pointer.
  template<class U>
  void reset(U* p) {
    clear();
    if (p) {
      pointer_ = p;
      refs_ = new RefImpl<T, default_deleter<U>>(p,default_deleter<U>());
    }
  }

  template<class U, class Deleter>
  void reset(U* p, Deleter d) {
    clear();
    if (p) {
      pointer_ = p;
      refs_ = new RefImpl<U, Deleter>(p, d);
    }
  }

  void reset() { reset<T>(nullptr); }  // default reset().

  T* operator->() {
    return pointer_;
  }

  const T* operator->() const {
    return pointer_;
  }

  T& operator*() {
    return *pointer_;
  }

  const T& operator*() const {
    return *pointer_;
  }

  explicit operator bool() {
    return refs_ && refs_->count > 0;
  }

  T* get() const { return pointer_; }

  // NOT standard API - only for testing.
  int ref_num() const {
    if (refs_) {
      return refs_->count;
    }
    return 0;
  }

  template<class> friend class weak_ptr;
  template<class> friend class shared_ptr;

 private:
  T* pointer_ = nullptr;
  Ref* refs_ = nullptr;

  void clear() {
    if (refs_ && (--(refs_->count) <= 0)) {
      std::cout << "deleting " << *pointer_ << std::endl;
      refs_->destroy();
      if (refs_->weak_count <= 0) {
        delete refs_;
        refs_ = nullptr;
      }
      pointer_ = nullptr;
    }
  }
};


#endif /* SHARED_POINTER_H__ */
