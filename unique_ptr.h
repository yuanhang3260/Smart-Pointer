#ifndef __UNIQUE_PTR__
#define __UNIQUE_PTR__

template<class T>
class unique_ptr {
 public:
  unique_ptr() = default;
  unique_ptr(T* p) : pointer_(p) {}
  // disallow assignment and copy construct
  unique_ptr(const unique_ptr<T>& other) = delete;
  unique_ptr<T>& operator=(const unique_ptr<T>& other) = delete;
  // move constructor
  unique_ptr(unique_ptr<T>&& other) : pointer_(other.pointer_) {
    other.pointer_ = NULL;
  }
  unique_ptr<T>& operator=(unique_ptr<T>&& other) {
    release();
    pointer_ = other.pointer_;
    other.pointer_ = NULL;
  }

 	~unique_ptr() {
 		release();
 	}

 	T* get() const { return pointer_; }

 	void release() {
 		if (pointer_) {
 			std::cout << "releasing " << *pointer_ << std::endl;
 			delete pointer_;
 			pointer_ = NULL;
 		}
 	}

 	void reset(T* p) {
 		release();
 		pointer_ = p;
 	}

 	explicit operator bool() {
    return pointer_ != NULL;
  }

 private:
 	T* pointer_ = NULL;
};

#endif /* __UNIQUE_PTR__ */
