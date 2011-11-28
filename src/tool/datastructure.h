#ifndef RANKING_TOOL_DATASTRUCTURE_H_
#define RANKING_TOOL_DATASTRUCTURE_H_

#include <cstring>

#include <utility>
#include <vector>

namespace ranking {
typedef std::pair<int, float> FeaturePair;
typedef std::vector<FeaturePair> FeatureVector;
typedef std::vector<float> WeightVector;
typedef std::vector<WeightVector> WeightMatrix;

struct Document {
  int level;
  FeatureVector fv;
};

typedef std::vector<Document> Query;
typedef std::vector<Query> Queries;

inline float InnerProduct(const FeatureVector& fv,
                          const WeightVector& wv) {
  float score = 0.f;
  for (size_t i = 0; i < fv.size(); ++i) {
    if (fv[i].first >= (int)wv.size()) continue;
    score += fv[i].second * wv[fv[i].first];
  }
  return score;
}
}

#endif //RANKING_TOOL_DATASTRUCTURE_H_
