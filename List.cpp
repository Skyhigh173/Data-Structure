/*
* DataStructure - List
* class name: List
*
* properties:
* - length
* - firstItem
* - lastItem
*
* methods:
* - iterator
* - push (left, right)
* - pop  (left, right)
* - insert
* - remove
* - reverse
* - merge
* - slice
* - clear
* - size
*
* common methods:
* - set
* - get
*/

#include <iostream>

template <typename T>
struct ListItem {
  T value;
  ListItem<T>* pevItem = nullptr;
  ListItem<T>* nextItem = nullptr;

  void link(ListItem<T>* PevItem, ListItem<T>* NextItem) {
    pevItem = PevItem;
    nextItem = NextItem;
  }

  ListItem<T>* reverseLink() {
    ListItem<T>* tmp = nextItem;
    nextItem = pevItem;
    pevItem = tmp;
    return tmp;
  }
};

template <typename T>
class List {
  public:
    ListItem<T>* firstItem = nullptr;
    ListItem<T>* lastItem = nullptr;

    const int& length = _length;

    void clear() {
      if (_length == 0) return;
      ListItem<T>* item = firstItem;
      ListItem<T>* item2 = firstItem;
      for (int i = 0; i < _length; i++) {
        item2 = item;
        item = item->nextItem;
        delete item2;
      }
      firstItem = lastItem = nullptr;
      _length = 0;
    }

    List() {
      firstItem = lastItem = nullptr;
      _length = 0;
    }

    ~List() {
      clear();
    }

    List(std::initializer_list<T> l) {
      for (auto it = l.begin(); it != l.end(); ++it) {
        push(*it);
      }
    }

    List(const List<T>& l) {
      ListItem<T>* item = l.firstItem;
      for (int i = 0; i < l.length; i++) {
        push(item->value);
        item = item->nextItem;
      }
    }

    ListItem<T>& iterator(int index) {
      setlen(index);
      ListItem<T>* item;

      if (_length - index > index) {
        item = firstItem;
        for (int i = 0; i < index; i++) item = item->nextItem;
      } else {
        item = lastItem;
        for (int i = 0; i < _length - index - 1; i++) item = item->pevItem;
      }

      return *item;
    }

    void pushRight(T value) {
      ListItem<T>* item = newItem(value);
      ++_length;
      if (_length == 1) {
        firstItem = lastItem = item;
      } else {
        item->pevItem = lastItem;
        lastItem->nextItem = item;
        lastItem = item;
      }
    }

    void pushLeft(T value) {
      ListItem<T>* item = newItem(value);
      ++_length;
      if (_length == 1) {
        firstItem = lastItem = item;
      } else {
        item->nextItem = firstItem;
        firstItem->pevItem = item;
        firstItem = item;
      }
    }

    T popRight() {
      --_length;
      if (length == 0) {
        T value = lastItem->value;
        clear();
        return value;
      }
      ListItem<T>* item = lastItem->pevItem;
      T value = lastItem->value;
      delete lastItem;
      item->nextItem = nullptr;
      lastItem = item;
      return value;
    }

    T popLeft() {
      --_length;
      if (length == 0) {
        T value = firstItem->value;
        clear();
        return value;
      }
      ListItem<T>* item = firstItem->nextItem;
      T value = firstItem->value;
      delete firstItem;
      item->pevItem = nullptr;
      firstItem = item;
      return value;
    }

    void insert(int index, T value) {
      setlen(index);
      if (index == 0)
        pushLeft(value);
      else if (index == _length - 1)
        pushRight(value);
      else {
        ListItem<T>* thisItem = newItem(value);
        ListItem<T>& item1 = iterator(index-1);
        ListItem<T>& item2 = iterator(index);
        thisItem->link(&item1, &item2);
        item1.nextItem = thisItem;
        item2.pevItem = thisItem;
      }
    }

    T remove(int index) {
      setlen(index);
      if (index == 0)
        return popLeft();
      else if (index == _length - 1)
        return popRight();
      else {
        ListItem<T>* item = &iterator(index);
        item->pevItem->nextItem = item->nextItem;
        item->nextItem->pevItem = item->pevItem;
        T value = item->value;
        delete item;
        --_length;
        return value;
      }
    }

    void reverse() {
      ListItem<T>* thisItem = firstItem;
      for (int i = 0; i < _length; i++) {
        thisItem = thisItem->reverseLink();
      }
      ListItem<T>* tmp = firstItem;
      firstItem = lastItem;
      lastItem = tmp;
    }

    List<T> merge(List<T> x) {
      List<T> l = *this;
      ListItem<T>* item = x.firstItem;
      for (int i = 0; i < x.length; i++) {
        l.push(item->value);
        item = item->nextItem;
      }
      return l;
    }

    List<T> slice(int startIndex, int endIndex = -1) {
      setlen(startIndex); setlen(endIndex);
      ListItem<T>* item = &iterator(startIndex);
      List<T> l;
      for (int i = startIndex; i <= endIndex; i++) {
        l.push(item->value);
        item = item->nextItem;
      }
      return l;
    }

    void set(int index, T value) {
      iterator(index).value = value;
    }

    T& get(int index) {
      return iterator(index).value;
    }

    // sizeof full object
    unsigned int size() {
      return sizeof(*this) + _length * sizeof(*firstItem);
    }

    // alias
    inline void push(T value) { pushRight(value); }
    inline T pop() { return popRight(); }

    inline T& operator[] (int index) {
      return get(index);
    }

    inline List<T> operator+ (List<T> x) {
      return merge(x);
    }

    List<T>& operator= (std::initializer_list<T> l) {
      for (auto it = l.begin(); it != l.end(); ++it) {
        push(*it);
      }
    }

    friend std::ostream& operator<<(std::ostream& out, List<T> const& list) {
      out << '[';
      ListItem<T>* item = list.firstItem;

      for (int i = 0; i < list._length; i++) {
        out << item->value;
        if (i + 1 != list._length) out << ", ";
        item = item->nextItem;
      }
      out << ']';
      return out;
    }
  
  private:
    int _length = 0;

    inline void setlen(int& x) {
      if (x < 0) x += _length;
    }

    ListItem<T>* newItem(T value) {
      ListItem<T>* item = new ListItem<T>;
      item->value = value;
      return item;
    }
};
