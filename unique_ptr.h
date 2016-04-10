#ifndef UNIQUE_PTR__
#define UNIQUE_PTR__

template<class T>
class unique_ptr {
 public:
  unique_ptr() = default;

  // Constructors
  template<class U>
  explicit unique_ptr(U* p) :
      pointer_(p),
      refs_(new UniqueRefImpl<U, default_deleter<U>>(p, default_deleter<U>())) {
  }

  template<class U, class Deleter>
  unique_ptr(U* p, Deleter d) :
      pointer_(p),
      refs_(new UniqueRefImpl<U, Deleter>(p, d)) {
  }

  // disallow assignment and copy constructor.
  unique_ptr(const unique_ptr& other) = delete;
  template<class U>
  unique_ptr(const unique_ptr<U>& other) = delete;
  unique_ptr& operator=(const unique_ptr& other) = delete;
  template<class U>
  unique_ptr<U>& operator=(const unique_ptr<U>& other) = delete;

  // move constructor
  unique_ptr(unique_ptr&& other) :
      pointer_(other.pointer_),
      refs_(other.refs_) {
    other.pointer_ = nullptr;
    other.refs_ = nullptr;
  }

  template <class U>
  unique_ptr(unique_ptr<U>&& other) :
      pointer_(other.pointer_),
      refs_(other.refs_) {
    other.pointer_ = nullptr;
    other.refs_ = nullptr;
  }

  unique_ptr<T>& operator=(unique_ptr<T>&& other) {
    clear();
    pointer_ = other.pointer_;
    refs_ = other.refs_;
    other.pointer_ = nullptr;
    other.refs_ = nullptr;
  }

  ~unique_ptr() {
    clear();
  }

  T* get() const { return pointer_; }

  T* release() {
    T* re = pointer_;
    pointer_ = nullptr;
    if (refs_) {
      delete refs_;
      refs_ = nullptr;
    }
    return re;
  }

  void clear() {
    if (refs_) {
      std::cout << "deleting " << *pointer_ << std::endl;
      refs_->destroy();
      delete refs_;
      refs_ = nullptr;
      pointer_ = nullptr;
    }
  }

  template<class U>
  void reset(U* p) {
    clear();
    refs_ = new UniqueRefImpl<U, default_deleter<U>>(p, default_deleter<U>());
    pointer_ = p;
  }

  template<class U, class Deleter>
  void reset(U* p, Deleter d) {
    clear();
    refs_ = new UniqueRefImpl<U, Deleter>(p, d);
    pointer_ = p;
  }

  void reset() {
    reset<T>(nullptr);
  }

  explicit operator bool() {
    return pointer_ != nullptr;
  }

 private:
  T* pointer_ = nullptr;
  Ref* refs_ = nullptr;
};

#endif /* UNIQUE_PTR__ */
