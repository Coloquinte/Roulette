
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

  Weight getWeight(std::size_t elt) const;
  void setWeight(std::size_t elt, Weight w);
  std::size_t select(Weight roll) const;

  Weight totalWeight() const;
  void resize(std::size_t sz);

  void checkConsistency() const;

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
inline Weight Roulette<Weight>::getWeight(std::size_t elt) const {
  return weights_[elt + offset_];
}

template <class Weight>
inline void Roulette<Weight>::setWeight(std::size_t elt, Weight w) {
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
    Weight leftTreeWeight = weights_[2*ind+1];
    ind = 2 * ind + 1;
    if (roll >= leftTreeWeight) {
      ++ind;
      roll -= leftTreeWeight;
    }
  }
  return ind - offset_;
}

template <class Weight>
inline Weight Roulette<Weight>::totalWeight() const {
  return weights_.front();
}

template <class Weight>
inline void Roulette<Weight>::resize(std::size_t sz) {
  std::size_t nextPowerOfTwo = 1;
  while (nextPowerOfTwo < sz) {
    nextPowerOfTwo *= 2;
  }
  std::vector<Weight> newWeights(2 * nextPowerOfTwo - 1, 0);
  std::size_t newOffset = nextPowerOfTwo - 1;

  std::size_t copiedSize = std::min(weights_.size() - offset_, sz);
  for (std::size_t i = 0; i < copiedSize; ++i) {
    newWeights[i + newOffset] = weights_[i + offset_];
  }

  for (std::size_t i = newOffset; i > 0; --i) {
    std::size_t j = i-1;
    newWeights[j] = newWeights[2*j+1] + newWeights[2*j+2];
  }

  std::swap(weights_, newWeights);
  offset_ = newOffset;
}

template <class Weight>
inline void Roulette<Weight>::checkConsistency() const {
  assert (weights_.size() == 2 * offset_ + 1);
  for (Weight w : weights_) {
    assert (w >= 0);
  }
  for (std::size_t i = 0; i < offset_; ++i) {
    assert (weights_[i] == weights_[2*i+1] + weights_[2*i+2]);
  }
}

#endif

