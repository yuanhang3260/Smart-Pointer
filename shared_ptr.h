#ifndef SHARED_PTR_H__
#define SHARED_PTR_H__

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
      refs_(new RefImpl<U, default_deleter<U>>(p, default_deleter<U>())),
      weak_counts_(new int(0)) {
  }

  template<class U, class Deleter>
  shared_ptr(U* p, Deleter d) :
      pointer_(p),
      refs_(new RefImpl<U, Deleter>(p, d)),
      weak_counts_(new int(0)) {
  }

  ~shared_ptr() { clear(); }

  // copy constructor
  shared_ptr(const shared_ptr& other) :
      pointer_(other.pointer_),
      refs_(other.refs_),
      weak_counts_(other.weak_counts_) {
    refs_->count++;
  }

  template<class U>
  shared_ptr(const shared_ptr<U>& other) :
      pointer_(other.pointer_),
      refs_(other.refs_),
      weak_counts_(other.weak_counts_) {
    refs_->count++;
  }

  // move constructor
  shared_ptr(shared_ptr&& other) :
      pointer_(other.pointer_),
      refs_(other.refs_),
      weak_counts_(other.weak_counts_) {
    other.pointer_ = nullptr;
    other.refs_ = nullptr;
    other.weak_counts_ = nullptr;
  }

  template<class U>
  shared_ptr(shared_ptr<U>&& other) :
      pointer_(other.pointer_),
      refs_(other.refs_),
      weak_counts_(other.weak_counts_) {
    other.pointer_ = nullptr;
    other.refs_ = nullptr;
    other.weak_counts_ = nullptr;
  }

  // Copy assignment.
  shared_ptr& operator=(const shared_ptr& other) {
    if (this != &other) {
      clear();
      pointer_ = other.pointer_;
      refs_ = other.refs_;
      weak_counts_ = other.weak_counts_;
      refs_->count++;  // atomic inc
    }
  }

  // Move assignment.
  shared_ptr& operator=(const shared_ptr&& other) {
    if (this != &other) {
      clear();

      pointer_ = other.pointer_;
      refs_ = other.refs_;
      weak_counts_ = other.weak_counts_;

      other.pointer_ = nullptr;
      other.refs_ = nullptr;
      other.weak_counts_ = nullptr;
    }
  }

  // reset shared pointer.
  template<class U>
  void reset(U* p) {
    clear();
    if (p) {
      pointer_ = p;
      refs_ = new RefImpl<T, default_deleter<U>>(p,default_deleter<U>()),
      weak_counts_ = new int(0);
    }
  }

  template<class U, class Deleter>
  void reset(U* p, Deleter d) {
    clear();
    if (p) {
      pointer_ = p;
      refs_ = new RefImpl<U, Deleter>(p, d);
      weak_counts_ = new int(0);
    }
  }

  void reset() { reset<T>(nullptr); }  // default reset().

  T* operator->() {
    return pointer_;
  }

  const T* operator->() const {
    return *pointer_;
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
  int* weak_counts_ = nullptr;

  void clear() {
    if (refs_ && (--(refs_->count) == 0)) {
      refs_->destroy();
      // if (weak_counts_ && *weak_counts_ == 0) {
      //   delete refs_;
      //   delete weak_counts_;
      // }
      pointer_ = nullptr;
      refs_ = nullptr;
      weak_counts_ = nullptr;
    }
  }
};


#endif /* SHARED_PTR_H__ */
