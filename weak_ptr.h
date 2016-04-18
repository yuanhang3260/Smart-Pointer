#ifndef WEAK_POINTER_H__
#define WEAK_POINTER_H__

#include "shared_ptr.h"

template<class T>
class weak_ptr {
 public:
  weak_ptr() = default;
  ~weak_ptr() { clear(); }

  // copy constructor
  weak_ptr(const weak_ptr& other) :
      pointer_(other.pointer_),
      refs_(other.refs_) {
    if (refs_) {
      refs_->weak_count++;
    }
  }

  // move constructor
  weak_ptr(const weak_ptr&& other) :
      pointer_(other.pointer_),
      refs_(other.refs_) {
    refs_ = nullptr;
    other.refs_ = nullptr;
  }

  // construct from shared_ptr
  weak_ptr(const shared_ptr<T>& sp) :
      pointer_(sp.pointer_),
      refs_(sp.refs_) {
    if (sp.refs_) {
      sp.refs_->weak_count++;
    }
  }

  weak_ptr& operator=(const weak_ptr& other) {
    if (this != &other) {
      clear();
      pointer_ = other.pointer_;
      refs_ = other.refs_;
      if (refs_) {
        other.refs_->weak_count++;
      }
    }
  }

  weak_ptr& operator=(const shared_ptr<T>& sp) {
    clear();
    pointer_ = sp.pointer_;
    refs_ = sp.refs_;
    if (refs_) {
      sp.refs_->weak_count++;
    }
  }

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

  bool expired() const { return refs_->count <= 0; }

  shared_ptr<T> lock() const {
    shared_ptr<T> sp;
    if (!expired()) {
      sp.reset(pointer_);
      sp.refs_ = refs_;
      refs_->count++;
    }
    return sp; 
  }

  void reset() {
    clear();
  }

  template<class> friend class weak_ptr;

 private:
  T* pointer_ = nullptr;
  Ref* refs_ = nullptr;

  void clear() {
    if (refs_ && (--(refs_->weak_count) == 0) &&
        refs_->count == 0) {
      std::cout << "weak clear " << std::endl;
      delete refs_;
      pointer_ = nullptr;
    }
  }
};


#endif /* WEAK_POINTER_H__ */
