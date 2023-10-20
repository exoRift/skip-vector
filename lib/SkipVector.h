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

    size_t _m_data; // manually allocated (manual)
    size_t _u_data; // actually used (used)
    size_t _p_data; // shown to the consumer (pseudo)
    size_t _m_offset; // manually allocated (manual)
    size_t _u_offset; // actually used (used)

    T* _data;
    offset_pair* _offset;

    // _calculated_offset _get_offset (size_t pos) const;
    size_t _upper_power_of_two (size_t num) const;

  public:
    SkipVector (size_t count = 1);
    SkipVector (size_t count, const T& value);
    SkipVector (const SkipVector<T>& original); // TODO: constructor and operator=
    ~SkipVector ();

    T& operator[] (size_t pos) const;
    T& at (size_t pos) const;
    T& front () const;
    T& back () const;

    iterator begin ();
    const_iterator cbegin () const;
    iterator end ();
    const_iterator cend () const;

    reverse_iterator rbegin (); // TODO
    const_reverse_iterator crbegin () const; // TODO
    reverse_iterator rend (); // TODO
    const_reverse_iterator crend () const; // TODO

    /** Get the underlying data array */
    T* data () const;
    /** Get the underlying offset array */
    offset_pair* offset () const;

    bool empty () const;
    size_t size () const;
    size_t capacity () const;
    size_t offset_capacity () const;

    void reserve (size_t data_size, size_t offset_size = 0);
    void clear ();
    iterator erase (const_iterator pos);
    iterator erase (const_iterator first, const_iterator last); // TODO
    void pop_back (); // TODO

    iterator insert (const_iterator pos, const T& value);
    void push_back (const T& value);
    void resize (size_t count); // TODO
};

template <typename T>
SkipVector<T>::SkipVector (size_t count) {
  size_t rounded = _upper_power_of_two(count);

  _data = new T[rounded];
  _m_data = rounded;
  _u_data = count;
  _p_data = count;

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
}

// template <typename T>
// typename SkipVector<T>::_calculated_offset SkipVector<T>::_get_offset (size_t pos) const {
//   size_t offset = 0;

//   size_t entry_index = 0;
//   for (offset_pair* entry = _offset; entry_index < _u_offset; ++entry_index, ++entry) {
//     if (entry->first > pos + offset) break;

//     offset += entry->second;
//   }

//   // NOTE: At this point, the entry is one forward from the for loop
//   return std::make_pair(entry_index - 1, offset);
// }

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
  size_t offset = 0;

  for (offset_pair* entry = _offset; pos + offset < _u_offset; ++entry) {
    if (entry->first > pos + offset) break;

    offset += entry->second;
  }

  return _data[pos + offset];
}

template <typename T>
T& SkipVector<T>::at (size_t pos) const {
  if (pos >= _p_data) throw std::out_of_range("Out of range"); // TODO: More conventional error

  return operator[](pos);
}

template <typename T>
T& SkipVector<T>::front () const {
  return operator[](0);
}

template <typename T>
T& SkipVector<T>::back () const {
  return operator[](_p_data - 1);
}

template <typename T>
typename SkipVector<T>::iterator SkipVector<T>::begin () {
  return iterator(this, 0, 0, _offset);
}

template <typename T>
typename SkipVector<T>::const_iterator SkipVector<T>::cbegin () const {
  return const_iterator(this, 0, 0, _offset);
}

template <typename T>
typename SkipVector<T>::iterator SkipVector<T>::end () {
  return iterator(this, _u_data, _p_data, _offset + _m_offset);
}

template <typename T>
typename SkipVector<T>::const_iterator SkipVector<T>::cend () const {
  return const_iterator(this, _u_data, _p_data, _offset + _m_offset);
}

// TODO
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
  return !_p_data;
}

template <typename T>
size_t SkipVector<T>::size () const {
  return _p_data;
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
void SkipVector<T>::reserve (size_t data_size, size_t offset_size) {
  size_t rounded_data = _upper_power_of_two(data_size);
  size_t rounded_offset = _upper_power_of_two(offset_size);

  T* old_data = _data;
  _data = new T[rounded_data];
  for (size_t i = 0; i < _m_data; ++i) _data[i] = old_data[i];
  _m_data = rounded_data;

  offset_pair* old_offset = _offset;
  _offset = new offset_pair[rounded_offset];
  for (size_t i = 0; i < _m_offset; ++i) _offset[i] = old_offset[i];
  _m_offset = rounded_offset;
};

template <typename T>
void SkipVector<T>::clear () {
  _u_data = 0;
  _p_data = 0;
  _u_offset = 0;
};

// todo: return iterator
template <typename T>
typename SkipVector<T>::iterator SkipVector<T>::erase (const_iterator pos) {
  const size_t numeric_pos = pos._cur_pos;
  offset_pair* offset_entry = pos._cur_offset;
  offset_pair* const next_offset_entry = pos._cur_offset + 1;

  if (
    offset_entry - _offset < _u_offset && // within offset array bounds
    numeric_pos - (offset_entry->first + offset_entry->second) == 1 // current offset is 1 away
  ) {
    ++offset_entry->second; // increase width by 1
  } else if (
    next_offset_entry - _offset < _u_offset && // within offset array bounds
    next_offset_entry->first - numeric_pos == 1
  ) {
    --next_offset_entry->first; // move back 1
  } else if (_u_offset >= _m_offset) { // need to resize offset array
    _m_offset *= 2;
    offset_pair* const new_offset = new offset_pair[_m_offset];
    size_t new_u_offset = _u_offset;

    offset_pair* offset_ptr = _offset;
    offset_pair* new_offset_ptr = new_offset;
    while (offset_ptr - _offset < _u_offset) {
      if (offset_ptr == offset_entry + 1) {
        *new_offset_ptr = std::make_pair(numeric_pos, 1);

        ++new_offset_ptr;
      } else {
        if (offset_ptr->second) { // if nonzero offset, copy over and advance new offset pointer
          *new_offset_ptr = *offset_ptr;
          ++new_offset_ptr;
        } else --new_u_offset;

        ++offset_ptr;
      }
    }

    delete[] _offset;
    _offset = new_offset;
    _u_offset = new_u_offset + 1; // add one for new entry
  } else { // insert new entry into offset array
    bool overwritten = false;

    offset_pair& prior_value = *offset_entry;
    *(offset_entry++) = std::make_pair(numeric_pos, 1);
    while (offset_entry - _offset <= _u_offset) { // <= to go one above for insert
      if (offset_entry->second == 0) { // the next offset is 0 (can be replaced)
        *offset_entry = prior_value;
        overwritten = true;

        break;
      } else {
        std::swap(*offset_entry, prior_value);

        ++offset_entry;
      }
    }

    if (!overwritten) ++ _u_offset;
    --_p_data;
  }

  return iterator(this, pos._cur_pos, ) // TODO
}

// todo: return iterator
template <typename T>
typename SkipVector<T>::iterator SkipVector<T>::insert (const_iterator pos, const T& value) { // TODO: possibly doing index comparisons for no reason when we can just replace? (make sure using correct lengths as well)
  const size_t numeric_pos = pos._cur_pos;
  offset_pair* offset_entry = pos._cur_offset;

  if (
    offset_entry - _offset < _u_offset && // within offset array bounds
    numeric_pos - offset_entry->first < offset_entry->second // offset range includes pos
  ) { // we can overwrite the value
    _data[numeric_pos] = value;

    if (offset_entry->second == 1) { // single deletion
      if (offset_entry - _offset >= _u_offset) --_u_offset; // trim out
      else offset_entry->second = 0;
    } else if (numeric_pos == offset_entry->first + offset_entry->second) { // insert at tail end
      --offset_entry->second;
    } else { // splice offset
      const size_t difference = numeric_pos - offset_entry->first;
      const size_t old_offset = offset_entry->second;

      offset_entry->second = difference;

      // insert new offset
      if (_u_offset >= _m_offset) { // need to resize offset array
        _m_offset *= 2;
        offset_pair* const new_offset = new offset_pair[_m_offset];
        size_t new_u_offset = _u_offset;

        offset_pair* offset_ptr = _offset;
        offset_pair* new_offset_ptr = new_offset;
        while (offset_ptr - _offset < _u_offset) {
          if (offset_ptr == offset_entry + 1) {
            *new_offset_ptr = std::make_pair(numeric_pos + 1, old_offset - (numeric_pos - offset_entry->first));

            ++new_offset_ptr;
          } else {
            if (offset_ptr->second) { // if nonzero offset, copy over and advance new offset pointer
              *new_offset_ptr = *offset_ptr;
              ++new_offset_ptr;
            } else --new_u_offset;

            ++offset_ptr;
          }
        }

        delete[] _offset;
        _offset = new_offset;
        _u_offset = new_u_offset + 1; // add one for new entry
      } else { // default insert into offset array
        offset_pair* offset_ptr = offset_entry;
        offset_pair& prior_value = *offset_ptr;
        while (offset_ptr - _offset < _u_offset) {
          if (!offset_ptr->second) { // offset of 0 can be culled out
            ++offset_ptr;
          } else {
            std::swap(*offset_ptr, prior_value);

            ++offset_ptr;
          }
        }
      }
      
    }
  } else if (_u_data >= _m_data) { // need to resize data array
    _m_data *= 2;
    T* const new_data = new T[_m_data];

    T* new_ptr = new_data;
    T* ptr = _data;
    offset_entry = _offset;

    while (ptr - _data < _u_data) { // ptr is within used bounds
      if (ptr == _data + numeric_pos) { // upon finding the location to insert, do so
        *new_ptr = std::move(value);

        ++new_ptr;
      }

      if (offset_entry - _offset < _u_offset && ptr - _data == offset_entry->first) { // offset is within bounds and pos matches
        ptr += offset_entry->second;

        ++offset_entry;
      } else {
        *new_ptr = *ptr;

        ++ptr;
        ++new_ptr;
      }
    }

    _u_offset = 0; // purge the offset vector
    ++_p_data; // we inserted an element
    _u_data = _p_data; // used len is now the pseudolen

    delete[] _data;
    _data = new_data;
  } else { // default insert
    T* ptr = _data + pos._cur_pos;

    T& prior_value = *ptr;
    *(ptr++) = std::move(value);
    while (ptr - _data <= _u_data) { // <= to go one above for insert
      if (offset_entry - _offset < _u_offset && ptr - _data == offset_entry->first) { // offset is within bounds and pos matches
        ptr += offset_entry->second;

        ++offset_entry;
      } else {
        std::swap(*ptr, prior_value);

        ++ptr;
      }
    }

    _u_offset = pos._cur_offset - _offset; // trim down offset vector after pruning out deleted elements past pos
    ++_u_data;
    ++_p_data;
  }
};

template <typename T>
void SkipVector<T>::push_back (const T& value) {
  if (_u_data >= _m_data) { // need to resize
    _m_data *= 2;
    T* const new_data = new T[_m_data];

    T* new_ptr = new_data;
    T* ptr = _data;
    offset_pair* offset_entry = _offset;

    while (ptr - _data < _u_data) { // ptr is within used bounds
      if (offset_entry - _offset < _u_offset && ptr - _data == offset_entry->first) { // offset is within bounds and pos matches
        ptr += offset_entry->second;

        ++offset_entry;
      } else {
        *new_ptr = *ptr;

        ++ptr;
        ++new_ptr;
      }
    }

    _u_offset = 0; // purge the offset vector
    ++_p_data; // we inserted an element
    _u_data = _p_data; // used len is now the pseudolen

    delete[] _data;
    _data = new_data;
  }

  offset_pair* const last_offset_entry = _offset + _u_offset;

  if (_u_offset && last_offset_entry->first + last_offset_entry->second >= _u_data) { // there exists some offset and it reaches the end
    _data[_u_data - 1] = std::move(value);

    --last_offset_entry->second;
  } else _data[_u_data++] = std::move(value);
}

#endif
