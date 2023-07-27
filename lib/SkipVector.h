#include <vector>
#include <utility>

template <typename T>
class SkipIterator {

};

template <typename T>
class SkipVector {
  protected:
    std::vector<T> _data;
    std::vector<std::pair<size_t, size_t>> _offset;

  public:
    typedef SkipIterator<T> iterator;
    typedef SkipIterator<const T> const_iterator;

    T& at (size_t pos) const;
    T& operator[] (size_t pos) const;
    T& front () const;
    T& back () const;

    /** Get the underlying data vector */
    std::vector<T>& data () const;
    /** Get the underlying offset vector */
    std::vector<std::pair<size_t, size_t>>& offset () const;

    bool empty () const;
    size_t size () const;
    void reserve ();
    size_t capacity ();

    void clear ();
    iterator insert (const_iterator pos, const T& value);
    iterator erase (const_iterator pos);
    iterator erase (const_iterator first, const_iterator last);
    void push_back (const T& value);
    void pop_back ();
    void resize (size_t count);
};
