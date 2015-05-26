#ifndef __SMART_PTR_H
#define __SMART_PTR_H

template<typename T>
class smart_ptr {
 public:
  // delcare as explicit to prevent raw pointer p being
  // implicitly used to construct multiple smart pointers.
  smart_ptr() = default;
  explicit smart_ptr(T* p = NULL)
     : pointer_(p), counts_(new int(1)) {}
  smart_ptr(const smart_ptr<T>& other)
     : pointer_(other.get()) {
    ++*other.counts(); // atomic_increment ?
    counts_ = other.counts();
  }
  ~smart_ptr() { clear(); }

  void reset(T* p) {
    clear();
    if (p != NULL) {
      pointer_ = p;
      counts_ = new int(1);
    }
  }

  smart_ptr<T>& operator=(const smart_ptr<T>& other) {
    if (this != &other) {
      clear();
      ++*other.counts(); // atomic_increment ?
      counts_ = other.counts();
      pointer_ = other.get();
    }
  }

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

  T* get() const { return pointer_; }
  int* counts() const { return counts_; }
  int ref_num() const { return *counts_; }

 private:
  T* pointer_ = NULL;
  int* counts_ = NULL;

  void clear() {
    if (counts_ && (--*counts_ == 0)) {
      std::cout << "deleting " << *pointer_ << std::endl;
      delete pointer_;
      delete counts_;
      pointer_ = NULL;
      counts_ = NULL;
    }
  }
};



#endif /* __SMART_PTR_H */