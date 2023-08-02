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
    T* _cur_pos;

    SkipIterator (SkipVector<T>& vec, T* pos): _cur_pos(pos), _skip_vector(vec) {};
};

template <typename T>
class ReverseSkipIterator {
  private:
    SkipIterator<T> _base;

    // SkipIterator ();

  
};

#endif
