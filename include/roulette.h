
#ifndef ROULETTE_H
#define ROULETTE_H

/*
 * Dynamic weighted random selection
 *
 * Given an array of weights, select() picks an index at random with probability proportional to its weight, in O(log n) time
 * Modifying a weight is O(log n) time
 * Resizing the array is O(1) amortized time
 */

#include <random>
#include <cassert>

template <class Weight>
class Roulette {
 public:
  Roulette(std::size_t sz=0);

  Weight get_weight(std::size_t elt) const;
  void set_weight(std::size_t elt, Weight w);
  std::size_t select(Weight roll) const;

  template <typename RNG>
  std::size_t select(RNG &rng) const;

  Weight total_weight() const;
  void resize(std::size_t sz);

  void check_consistency() const;

 private:
  std::vector<Weight> weights_;
  std::size_t offset_;
};

template <class Weight>
inline Roulette<Weight>::Roulette(std::size_t sz)
: offset_(0) {
  resize(sz);
}

template <class Weight>
inline Weight Roulette<Weight>::get_weight(std::size_t elt) const {
  return weights_[elt + offset_];
}

template <class Weight>
inline void Roulette<Weight>::set_weight(std::size_t elt, Weight w) {
  std::size_t ind = offset_ + elt;
  weights_[ind] = w;
  while (ind > 0) {
    ind = (ind-1) / 2;
    weights_[ind] = weights_[2*ind+1] + weights_[2*ind+2]; 
  }
}

template <class Weight>
inline std::size_t Roulette<Weight>::select(Weight roll) const {
  std::size_t ind = 0;
  while (ind < offset_) {
    Weight left_tree_weight = weights_[2*ind+1];
    ind = 2 * ind + 1;
    if (roll >= left_tree_weight) {
      ++ind;
      roll -= left_tree_weight;
    }
  }
  return ind - offset_;
}

template <class Weight>
template <typename RNG>
inline std::size_t Roulette<Weight>::select(RNG &rng) const {
  std::uniform_int_distribution<Weight> dist(0, total_weight() - 1);
  Weight roll = dist(rng);
  return select(roll);
}

template <class Weight>
inline Weight Roulette<Weight>::total_weight() const {
  return weights_.front();
}

template <class Weight>
inline void Roulette<Weight>::resize(std::size_t sz) {
  std::size_t next_power_of_two = 1;
  while (next_power_of_two < sz) {
    next_power_of_two *= 2;
  }
  std::vector<Weight> new_weights(2 * next_power_of_two - 1, 0);
  std::size_t new_offset = next_power_of_two - 1;

  std::size_t copiedSize = std::min(weights_.size() - offset_, sz);
  for (std::size_t i = 0; i < copiedSize; ++i) {
    new_weights[i + new_offset] = weights_[i + offset_];
  }

  for (std::size_t i = new_offset; i > 0; --i) {
    std::size_t j = i-1;
    new_weights[j] = new_weights[2*j+1] + new_weights[2*j+2];
  }

  std::swap(weights_, new_weights);
  offset_ = new_offset;
}

template <class Weight>
inline void Roulette<Weight>::check_consistency() const {
  assert (weights_.size() == 2 * offset_ + 1);
  for (Weight w : weights_) {
    assert (w >= 0);
  }
  for (std::size_t i = 0; i < offset_; ++i) {
    assert (weights_[i] == weights_[2*i+1] + weights_[2*i+2]);
  }
}

#endif

