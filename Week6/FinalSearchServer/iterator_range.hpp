#pragma once

#include <algorithm>

template <typename It>
class IteratorRange {
public:
  IteratorRange(It first, It last) : first(first), last(last) {
  }

  It begin() const {
    return first;
  }

  It end() const {
    return last;
  }

  size_t size() const {
    return last - first;
  }

private:
  It first, last;
};

template <typename Container>
auto Head(Container& c, int top) {
    return IteratorRange(begin(c), next(begin(c), std::min<size_t>(std::max(top, 0), c.size())));
}

template <typename Container>
auto Tail(Container& c, int back) {
    //return IteratorRange(prev(end(c), std::min<size_t>(std::max(back, 0), c.size())), end(c));
    return IteratorRange(rbegin(c), next(rbegin(c), std::min<size_t>(std::max(back, 0), c.size())));
}
