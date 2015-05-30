#ifndef __SHARED_PTR_H__
#define __SHARED_PTR_H__

template<class T> class weak_ptr;

template<class T>
class shared_ptr {
 public:
  // Default constructor.
  shared_ptr() = default;
  // Delcare as explicit to prevent raw pointer p being
  // implicitly used to construct multiple smart pointers.
  explicit shared_ptr(T* p) {
    if (p != NULL) {
      pointer_ = p;
      counts_ = new int(1);
      weak_counts_ = new int(0);
    }
  }
  // copy constructor
  shared_ptr(const shared_ptr<T>& other):
       pointer_(other.pointer_),
       weak_counts_(other.weak_counts_) {
    ++*other.counts_; // atomic_increment ?
    counts_ = other.counts_;
  }
  // move constructor
  shared_ptr(shared_ptr<T>&& other) :
       pointer_(other.pointer_),
       counts_(other.counts_),
       weak_counts_(other.weak_counts_) {
    other.pointer_ = NULL;
    other.counts_ = NULL;
    other.weak_counts_ = NULL;
  }

  ~shared_ptr() { clear(); }

  void reset(T* p = NULL) {
    clear();
    if (p != NULL) {
      pointer_ = p;
      counts_ = new int(1);
      weak_counts_ = new int(0);
    }
  }

  shared_ptr<T>& operator=(const shared_ptr<T>& other) {
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
  int ref_num() const { // not standard API - only for verification
    if (counts_) {
      return *counts_;
    }
    return 0;
  }

  template<class> friend class weak_ptr;
  template<class> friend class shared_ptr;

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



#endif /* __SHARED_PTR_H__ */