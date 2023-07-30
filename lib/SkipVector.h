#include <vector>
#include <utility>
#include <stdexcept>

template <typename T>
class SkipIterator {

};

template <typename T>
class ReverseSkipIterator {

};

template <typename T>
class SkipVector {
  public:
    typedef SkipIterator<T> iterator;
    typedef SkipIterator<const T> const_iterator;
    typedef ReverseSkipIterator<T> reverse_iterator;
    typedef ReverseSkipIterator<const T> const_reverse_iterator;
    typedef std::pair<size_t, size_t> offset_pair;

  protected:
    typedef std::pair<offset_pair*, size_t> _calculated_offset;

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

    T& at (size_t pos) const;
    T& operator[] (size_t pos) const;
    T& front () const;
    T& back () const;

    iterator begin ();
    const_iterator cbegin () const;

    iterator end ();
    const_iterator cend () const;

    /** Get the underlying data array */
    T* data () const;
    /** Get the underlying offset array */
    offset_pair* offset () const;

    bool empty () const;
    size_t size () const;
    void reserve ();
    size_t capacity () const;
    size_t offset_capacity () const;

    void clear ();
    iterator erase (const_iterator pos);
    iterator erase (const_iterator first, const_iterator last);
    void pop_back ();

    iterator insert (const_iterator pos, const T& value);
    void push_back (const T& value);
    void resize (size_t count);
};

/**
 * Implementation: https://stackoverflow.com/a/466278/10438436
 */
template <typename T>
size_t SkipVector<T>::_upper_power_of_two (size_t num) const {
  --num;
  num |= num >> 1;
  num |= num >> 2;
  num |= num >> 4;
  num |= num >> 8;
  num |= num >> 16;
  ++num;

  return num;
}

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

  offset_pair* entry = _offset;
  for (size_t index = 0; index < _u_offset; ++index, ++entry) {
    if (entry->first > pos + offset) break;

    offset += entry->second;
  }

  // NOTE: At this point, the entry is one forward from the for loop
  if (entry == _offset) entry = NULL;
  else --entry;

  return std::make_pair(entry, offset);
}

template <typename T>
T& SkipVector<T>::operator[] (size_t pos) const {
  _calculated_offset offset = _get_offset(pos);

  return _data[pos + offset.second];
}

template <typename T>
T& SkipVector<T>::at (size_t pos) const {
  if (pos <= _u_data) throw std::out_of_range("Out of range"); // TODO: More conventional error

  return operator[](pos);
}

template <typename T>
T* SkipVector<T>::data () const {
  return _data;
}

template <typename T>
typename SkipVector<T>::offset_pair* SkipVector<T>::offset () const {
  return _offset;
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
