#ifndef skip_vector
#define skip_vector

#include <stddef.h>
#include <utility>
#include <stdexcept>

#include "SkipIterator.h"

template <typename T>
class SkipVector {
  template <typename> friend class SkipIterator;
  template <typename> friend class ReverseSkipIterator;

  public:
    typedef SkipIterator<T> iterator;
    typedef SkipIterator<const T> const_iterator;
    typedef ReverseSkipIterator<T> reverse_iterator;
    typedef ReverseSkipIterator<const T> const_reverse_iterator;
    typedef std::pair<size_t, size_t> offset_pair;

  protected:
    typedef std::pair<size_t, size_t> _calculated_offset;

    size_t _m_data;
    size_t _u_data;
    size_t _m_offset;
    size_t _u_offset;

    T* _data;
    offset_pair* _offset;

    _calculated_offset _get_offset (size_t pos) const;
    size_t _upper_power_of_two (size_t num) const;

  public:
    SkipVector (size_t count = 1);
    SkipVector (size_t count, const T& value);
    SkipVector (const SkipVector<T>& original);
    ~SkipVector ();

    T& operator[] (size_t pos) const;
    T& at (size_t pos) const;
    T& front () const;
    T& back () const;

    iterator begin ();
    const_iterator cbegin () const;
    iterator end ();
    const_iterator cend () const;

    reverse_iterator rbegin ();
    const_reverse_iterator crbegin () const;
    reverse_iterator rend ();
    const_reverse_iterator crend () const;

    /** Get the underlying data array */
    T* data () const;
    /** Get the underlying offset array */
    offset_pair* offset () const;

    bool empty () const;
    size_t size () const;
    size_t capacity () const;
    size_t offset_capacity () const;

    void reserve ();
    void clear ();
    iterator erase (const_iterator pos);
    iterator erase (const_iterator first, const_iterator last);
    void pop_back ();

    iterator insert (const_iterator pos, const T& value);
    void push_back (const T& value);
    void resize (size_t count);
};

template <typename T>
SkipVector<T>::SkipVector (size_t count) {
  size_t rounded = _upper_power_of_two(count);

  _data = new T[rounded];
  _u_data = count;
  _m_data = rounded;

  _offset = new offset_pair[1];
  _u_offset = 0;
  _m_offset = 1;
}

template <typename T>
SkipVector<T>::SkipVector (size_t count, const T& value): SkipVector(count) {
  for (size_t i = 0; i < count; ++i) _data[i] = T(value);
}

template <typename T>
SkipVector<T>::~SkipVector () {
  delete[] _data;
  delete[] _offset;

  // TODO: Destruct the elements
}

template <typename T>
typename SkipVector<T>::_calculated_offset SkipVector<T>::_get_offset (size_t pos) const {
  size_t offset = 0;

  size_t entry_index = 0;
  for (offset_pair* entry = _offset; entry_index < _u_offset; ++entry_index, ++entry) {
    if (entry->first > pos + offset) break;

    offset += entry->second;
  }

  // NOTE: At this point, the entry is one forward from the for loop
  return std::make_pair(entry_index - 1, offset);
}

/**
 * Implementation: https://stackoverflow.com/a/24844826/10438436
 */
template <typename T>
size_t SkipVector<T>::_upper_power_of_two (size_t num) const {
  if (num <= 1) return 1;

  size_t power = 2;
  --num;
  while (num >>= 1) power <<= 1;

  return power;
}

template <typename T>
T& SkipVector<T>::operator[] (size_t pos) const {
  _calculated_offset offset = _get_offset(pos);

  return _data[pos + offset.second];
}

template <typename T>
T& SkipVector<T>::at (size_t pos) const {
  if (pos <= size()) throw std::out_of_range("Out of range"); // TODO: More conventional error

  return operator[](pos);
}

template <typename T>
T& SkipVector<T>::front () const {
  return operator[](0);
}

template <typename T>
T& SkipVector<T>::back () const {
  return operator[](size() - 1);
}

template <typename T>
typename SkipVector<T>::iterator SkipVector<T>::begin () {
  return iterator(*this, _data);
}

template <typename T>
typename SkipVector<T>::const_iterator SkipVector<T>::cbegin () const {
  return const_iterator(*this, _data);
}

template <typename T>
typename SkipVector<T>::iterator SkipVector<T>::end () {
  return iterator(*this, &back());
}

template <typename T>
typename SkipVector<T>::const_iterator SkipVector<T>::cend () const {
  return const_iterator(*this, &back());
}

// template <typename T>
// typename SkipVector<T>::reverse_iterator SkipVector<T>::rbegin () {
//   return reverse_iterator(*this, _data);
// }

// template <typename T>
// typename SkipVector<T>::const_reverse_iterator SkipVector<T>::crbegin () const {
//   return const_reverse_iterator(*this, _data);
// }

// template <typename T>
// typename SkipVector<T>::reverse_iterator SkipVector<T>::rend () {
//   return reverse_iterator(*this, &back);
// }

// template <typename T>
// typename SkipVector<T>::const_reverse_iterator SkipVector<T>::crend () const {
//   return const_reverse_iterator(*this, &back);
// }

template <typename T>
T* SkipVector<T>::data () const {
  return _data;
}

template <typename T>
typename SkipVector<T>::offset_pair* SkipVector<T>::offset () const {
  return _offset;
}

template <typename T>
bool SkipVector<T>::empty () const {
  return !size();
}

template <typename T>
size_t SkipVector<T>::size () const {
  return _u_data;
}

template <typename T>
size_t SkipVector<T>::capacity () const {
  return _m_data;
}

template <typename T>
size_t SkipVector<T>::offset_capacity () const {
  return _m_offset;
}

template <typename T>
typename SkipVector<T>::iterator SkipVector<T>::insert (const_iterator pos, const T& value) {
  size_t data_index = 0;
  size_t offset_index = 0;

  if (size() >= capacity()) { // up until index
    const size_t new_size = size() * 2;
    const T* new_data = new T[new_size];



    _m_data = new_size;
  }

  // past index
};

#endif
