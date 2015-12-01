#ifndef LIB_ZIP_H
#define LIB_ZIP_H

template <class S, class T>
struct zip_iterator
    : std::iterator<
          std::forward_iterator_tag,
          std::pair<const typename S::value_type &,
                    const typename std::iterator_traits<T>::value_type &>> {
  friend bool operator!=(const zip_iterator<S, T> &a,
                         const zip_iterator<S, T> &b) {
    return a.s_ != b.s_ || a.t_ != b.t_;
  }

  zip_iterator(S s, T t) : s_(s), t_(t) {}

  zip_iterator& operator++() {
    ++s_;
    ++t_;
    return *this;
  }

  std::pair<const typename S::value_type&, const typename std::iterator_traits<T>::value_type&>
  operator*() {
    return {*s_, *t_};
  }

 private:
  S s_;
  T t_;
};

template <class S, class T>
struct zip_impl {
  zip_impl(S &&s, T &&t) : s_(&s), t_(&t) {}

  using S_ = typename std::remove_reference<S>::type;
  using T_ = typename std::remove_reference<T>::type;

  zip_iterator<typename S_::const_iterator, typename T_::const_iterator> begin() {
    return zip_iterator<typename S_::const_iterator, typename T_::const_iterator>(s_->begin(), t_->begin());
  }

  zip_iterator<typename S_::const_iterator, typename T_::const_iterator> end() {
    return zip_iterator<typename S_::const_iterator, typename T_::const_iterator>(s_->end(), t_->end());
  }

private:
  S_ *s_;
  T_ *t_;
};

template <class S, class T>
zip_impl<S, T> zip(S &&s, T &&t) {
  return zip_impl<S, T>(std::forward<S>(s), std::forward<T>(t));
}

template <class S>
struct concat_iterator
    : std::iterator<std::forward_iterator_tag, typename S::value_type> {
  friend bool operator!=(const concat_iterator<S> &lhs,
                         const concat_iterator<S> &rhs) {
    return lhs.it_ != rhs.it_;
  }

  concat_iterator(S it, std::pair<S, S> &&flip) : it_(it), flip_(std::forward(flip)) {}

  concat_iterator& operator++() {
    ++it_;
    if (it_ == flip_.first) it_ = flip_.second;
    return *this;
  }

  const typename S::value_type&
  operator*() {
    return *it_;
  }

 private:
  S it_;
  std::pair<S, S> flip_;
};

template <class S>
struct concat_impl {
  concat_impl(S &&first, S &&second) : first_(&first), second_(&second) {}

  using S_ = typename std::remove_reference<S>::type;

  concat_iterator<typename S_::const_iterator> begin() {
    return concat_iterator<typename S_::const_iterator>(first_->begin(), {first_->end(), second_->begin()});
  }

  concat_iterator<typename S_::const_iterator> end() {
    return concat_iterator<typename S_::const_iterator>(second_->end(), {first_->end(), second_->begin()});
  }

private:
  S_ *first_;
  S_ *second_;
};

template <class S>
concat_impl<S> concat(S &&first, S &&second) {
  return concat_impl<S>(std::forward<S>(first), std::forward<S>(second));
}

#endif