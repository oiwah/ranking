#include <pointwise/prank.h>

#include <climits>
#include <algorithm>

namespace ranking {
PRank::PRank() : max_level_(0) {
  WeightVector().swap(wv_);
  std::vector<int>().swap(b_);
}

PRank::~PRank() {}

void PRank::init(int max_level) {
  if (max_level < 1) return;
  max_level_ = max_level;

  std::vector<int>(max_level_).swap(b_);
  b_[max_level_ - 1] = INT_MAX;
}

void PRank::Train(const Queries& queries, size_t iteration) {
  if (max_level_ < 1) return;

  for (size_t iter = 0; iter < iteration; ++iter) {
    for (size_t i = 0; i < queries.size(); ++i) {
      const Query &q = queries[i];
      for (size_t j = 0; j < q.size(); ++j) {
        const Document& d = q[j];
        Train(d);
      }
    }
  }
}

void PRank::Train(const Document& document) {
  float score = InnerProduct(document.fv, wv_);
  int predict = std::lower_bound(b_.begin(), b_.end(), score) - b_.begin();
  if (predict == document.level) return;

  int eta_sum = 0;
  for (int level = 0; level < max_level_ - 1; ++level) {
    if (level < document.level) {
      if (score - b_[level] <= 0.0) {
        b_[level] -= 1;
        eta_sum += 1;
      }
    } else if (level >= document.level) {
      if (score - b_[level] >= 0.0) {
        b_[level] += 1;
        eta_sum -= 1;
      }
    }
  }
  UpdateWeight(eta_sum, document.fv);
}

void PRank::UpdateWeight(int scalar, const FeatureVector& fv) {
  for (size_t i = 0; i < fv.size(); ++i) {
    if (fv[i].first >= (int)wv_.size()) wv_.resize(fv[i].first + 1);
    wv_[fv[i].first] += scalar * fv[i].second;
  }
}

int PRank::Test(const FeatureVector& fv) {
  float score = InnerProduct(fv, wv_);
  int predict = std::lower_bound(b_.begin(), b_.end(), score) - b_.begin();
  return predict;
}

} //namespace
