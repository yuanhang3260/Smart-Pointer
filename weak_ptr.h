#ifndef __WEAK_PTR_H__
#define __WEAK_PTR_H__

template<class T>
class weak_ptr {
 public:
  weak_ptr() = default;
  ~weak_ptr() { clear(); }
  // copy constructor
  weak_ptr(const weak_ptr<T>& other) :
    ref_pointer_(other.ref_pointer_),
    counts_(other.counts_)
  {
    ++*other.weak_counts_;
    weak_counts(other.weak_counts_);
  }
  // move constructor
  weak_ptr(const weak_ptr<T>&& other) :
    ref_pointer_(other.ref_pointer_),
    counts_(other.counts_),
    weak_counts_(other.weak_counts)
  {
    other.ref_pointer_ = NULL;
    other.counts_ = NULL;
    other.weak_counts = NULL;
  }

  weak_ptr<T>& operator=(const weak_ptr<T>& other) {
    if (this != &other) {
      clear();
      ref_pointer_ = other.ref_pointer_;
      counts_ = other.counts_;
      ++*other.weak_counts_;
      weak_counts_ = other.weak_counts_;
    }
  }

  weak_ptr<T>& operator=(const shared_ptr<T>& other) {
    clear();
    if (other.counts_ && *other.counts_ > 0) {
      ref_pointer_ = other.get();
      counts_ = other.counts_;
      ++(*other.weak_counts_);
      weak_counts_ = other.weak_counts_;
    }
  }

  int void_count() const { return *counts_; }

  bool expired() const { return *counts_ == 0; }

  shared_ptr<T> lock() const {
    shared_ptr<T> sp;
    if (!expired()) {
      sp.reset(ref_pointer_);
      ++*counts_;
      sp.counts_ = counts_;
    }
    return sp; 
  }

  void reset() {
    clear();
  }

 private:
  T* ref_pointer_ = NULL;
  int* counts_ = NULL; // strong reference count
  int* weak_counts_ = NULL; // weak reference count

  void clear() {
    if (weak_counts_ && (--*weak_counts_ == 0) &&
        counts_ && *counts_ == 0) {
      std::cout << "weak clear " << std::endl;
      delete weak_counts_;
      delete counts_;
      ref_pointer_ = NULL;
      counts_ = NULL;
      weak_counts_ = NULL;
    }
  }
};


#endif /* __WEAK_PTR_H__ */