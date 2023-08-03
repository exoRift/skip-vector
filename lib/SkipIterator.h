#ifndef skip_iterator
#define skip_iterator

#include <stddef.h>

template <typename> class SkipVector;

template <typename T>
class SkipIterator {
  template <typename> friend class SkipVector;
  template <typename> friend class ReverseSkipIterator;

  private:
    SkipVector<T>& _skip_vector;
    T* _cur_elem;
    size_t _cur_pos;

    SkipIterator (SkipVector<T>& vec, T* elem, size_t pos): _skip_vector(vec), _cur_elem(elem), _cur_pos(pos) {};

  public:
    T& operator* () {
      if (_cur_pos == _skip_vector.size()) return NULL;
    }
};

template <typename T>
class ReverseSkipIterator {
  private:
    SkipIterator<T> _base;

    // SkipIterator ();

  
};

#endif
