#include <pointwise/mcrank.h>

#include <cfloat>
#include <cmath>
#include <algorithm>
#include <iostream>

namespace ranking {
McRank::McRank() : max_level_(0) {
  WeightMatrix().swap(wx_);
}

McRank::~McRank() {}

void McRank::init(int max_level) {
  if (max_level < 1) return;
  max_level_ = max_level;

  WeightMatrix(max_level_).swap(wx_);
}

void McRank::Train(const Queries& queries, size_t iteration) {
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

void McRank::Train(const Document& document) {
  std::vector<float> scores;
  for (int rank = 0; rank < max_level_; ++rank) {
    WeightVector& wv = wx_[rank];
    float score = InnerProduct(document.fv, wv);
    scores.push_back(score);
  }

  std::vector<float> probabilities;
  CalcProbabilities(scores, probabilities);

  for (int rank = 0; rank < max_level_; ++rank) {
    float probability = probabilities[rank];
    if (rank == document.level) {
      UpdateWeight(rank, 1.f - probability, document.fv);
    } else {
      UpdateWeight(rank, -probability, document.fv);
    }
  }
}

void McRank::UpdateWeight(int rank, float update_width, const FeatureVector& fv) {
  WeightVector& wv = wx_[rank];
  for (size_t i = 0; i < fv.size(); ++i) {
    if (fv[i].first >= (int)wv.size()) wv.resize(fv[i].first + 1);
    wv[fv[i].first] += update_width * fv[i].second;
  }
}

int McRank::Test(const FeatureVector& fv) {
  std::vector<float> scores;
  for (int rank = 0; rank < max_level_; ++rank) {
    WeightVector& wv = wx_[rank];
    float score = InnerProduct(fv, wv);
    scores.push_back(score);
  }

  std::vector<float> probabilities;
  CalcProbabilities(scores, probabilities);

  float predict_probability = 0.5f;
  for (int rank = 1; rank < max_level_; ++rank) {
    predict_probability += rank * probabilities[rank];
  }

  return (int)predict_probability;
}

void McRank::CalcProbabilities(const std::vector<float> scores,
                               std::vector<float>& probabilities) {
  std::vector<float>(max_level_).swap(probabilities);
  float max_score = *max_element(scores.begin(), scores.end());

  float score_sum = 0.f;
  for (int rank = 0; rank < max_level_; ++rank) {
    float exp_score = std::exp(scores[rank] - max_score);
    probabilities[rank] = exp_score;
    score_sum += exp_score;
  }

  for (int rank = 0; rank < max_level_; ++rank) {
    probabilities[rank] = probabilities[rank] / score_sum;
  }
}

} //namespace
