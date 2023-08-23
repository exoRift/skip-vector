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
    size_t _cur_pos; // actual pos
    size_t _cur_p_pos; // pseudopos
    typename SkipVector<T>::offset_pair* _cur_offset; // ptr

    SkipIterator (SkipVector<T>& vec, size_t pos, size_t p_pos): _skip_vector(vec), _cur_pos(pos), _cur_p_pos(p_pos) {};

  public:
    SkipIterator (const SkipIterator<T>& original);
    SkipIterator<T>& operator= (const SkipIterator<T>& original);

    T& operator* ();

    SkipIterator<T> operator+ ();
    SkipIterator<T> operator- ();
    SkipIterator<T> operator++ (int);
    SkipIterator<T>& operator++ ();
    SkipIterator<T> operator-- (int);
    SkipIterator<T>& operator-- ();
};

template <typename T>
T& SkipIterator<T>::operator* () {
  if (_cur_pos == _skip_vector.size()) return NULL;

  return _skip_vector._data[_cur_pos];
}

template <typename T>
SkipIterator<T>& SkipIterator<T>::operator++ () {
  ++_cur_p_pos;

  if (
    _cur_offset < _skip_vector.offset + _skip_vector._u_offset && // offset within bounds
    _cur_pos == _cur_offset->first // we've reached offset point
  ) {
    _cur_pos += _cur_offset->second;

    ++_cur_offset;
  }

  return *this;
}

template <typename T>
SkipIterator<T>& SkipIterator<T>::operator-- () {
  --_cur_p_pos;

  if (
    _cur_offset >= _skip_vector.offset && // offset within bounds
    _cur_pos < _cur_offset->first + _cur_offset->second // we're within the offset point boundaries
  ) {
    _cur_pos -= _cur_offset->second;

    --_cur_offset;
  }

  return *this;
}

template <typename T>
class ReverseSkipIterator {
  private:
    SkipIterator<T> _base;

    // SkipIterator ();

  
};

#endif
