#ifndef __SMART_PTR_H__
#define __SMART_PTR_H__

template<class T>
class smart_ptr {
 public:
  // Default constructor.
  smart_ptr() = default;
  // Delcare as explicit to prevent raw pointer p being
  // implicitly used to construct multiple smart pointers.
  explicit smart_ptr(T* p = NULL)
     : pointer_(p), counts_(new int(1)), weak_counts_(new int(0)) {}
  // copy constructor
  smart_ptr(const smart_ptr<T>& other):
       pointer_(other.pointer_),
       weak_counts_(other.weak_counts_) {
    ++*other.counts_; // atomic_increment ?
    counts_ = other.counts_;
  }
  // move constructor
  smart_ptr(smart_ptr<T>&& other) :
       pointer_(other.pointer_),
       counts_(other.counts_),
       weak_counts_(other.weak_counts_) {
    other.pointer_ = NULL;
    other.counts_ = NULL;
    other.weak_counts_ = NULL;
  }

  ~smart_ptr() { clear(); }

  void reset(T* p) {
    clear();
    if (p != NULL) {
      pointer_ = p;
      counts_ = new int(1);
      weak_counts_ = new int(0);
    }
  }

  smart_ptr<T>& operator=(const smart_ptr<T>& other) {
    if (this != &other) {
      clear();
      ++*other.counts_; // atomic_increment ?
      counts_ = other.counts_;
      weak_counts_ = other.weak_counts_;
      pointer_ = other.pointer_;
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

  explicit operator bool() {
    return counts_ && *counts_ > 0;
  }

  T* get() const { return pointer_; }
  int* counts() const { return counts_; }
  int ref_num() const {
    if (counts_) {
      return *counts_;
    }
    return 0;
  }

 private:
  T* pointer_ = NULL;
  int* counts_ = NULL;
  int* weak_counts_ = NULL;

  void clear() {
    if (counts_ && (--*counts_ == 0)) {
      std::cout << "deleting " << *pointer_ << std::endl;
      delete pointer_;
      if (weak_counts_ && *weak_counts_ == 0) {
        delete counts_;
        delete weak_counts_;
      }
      pointer_ = NULL;
      counts_ = NULL;
      weak_counts_ = NULL;
    }
  }
};



#endif /* __SMART_PTR_H__ */