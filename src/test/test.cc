#include <pointwise/prank.h>

#include <test/test.h>

namespace {
} //namespace

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << " [training file] [test file]" << std::endl;
    return -1;
  }

  ranking::feature2id f2i;
  ranking::Queries train;
  ranking::Query query;
  if (!ParseFile(argv[1], &query, &f2i, true))
    return -1;
  train.push_back(query);

  ranking::Query test;
  if (!ParseFile(argv[2], &test, &f2i, true))
    return -1;

  ranking::PRank prank;
  prank.init(5);
  if (ranking::Run(prank, "PRank", train, test) == -1) {
    std::cerr << "error occurring!" << std::endl;
  }

  return 0;
}
