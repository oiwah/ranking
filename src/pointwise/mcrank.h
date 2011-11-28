#ifndef RANKING_POINTWISE_MCRANK_H_
#define RANKING_POINTWISE_MCRANK_H_

#include <tool/datastructure.h>

namespace ranking {
class McRank {
public:
  McRank();
  ~McRank();
  void init(int max_level);
  void Train(const Queries& queries, size_t iteration = 1);
  int Test(const FeatureVector& fv);

private:
  void Train(const Document& document);
  void UpdateWeight(int rank,
                    float update_width,
                    const FeatureVector& fv);
  void CalcProbabilities(const std::vector<float> scores,
                         std::vector<float>& probabilities);
  int max_level_;
  WeightMatrix wx_;
};
} //namespace

#endif //RANKING_POINTWISE_MCRANK_H_
