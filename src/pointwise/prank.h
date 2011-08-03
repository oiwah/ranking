#ifndef RANKING_POINTWISE_PRANK_H_
#define RANKING_POINTWISE_PRANK_H_

#include <tool/datastructure.h>

namespace ranking {
class PRank {
public:
  PRank();
  ~PRank();
  void init(int max_level);
  void Train(const Queries& queries, size_t iteration = 1);
  int Test(const FeatureVector& fv);

private:
  void Train(const Document& document);
  void UpdateWeight(int scalar, const FeatureVector& fv);
  int max_level_;
  WeightVector wv_;
  std::vector<int> b_;
};
} //namespace

#endif //RANKING_POINTWISE_PRANK_H_
