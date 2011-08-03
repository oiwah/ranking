#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include <tool/datastructure.h>

namespace ranking {
typedef std::map<std::string, size_t> feature2id;
bool ParseFile(const char* file_path,
               Query* query,
               feature2id* f2i,
               bool libsvm=false) {
  std::vector<Document>(0).swap(*query);

  std::ifstream ifs(file_path);
  if (!ifs) {
    std::cerr << "cannot open " << file_path << std::endl;
    return false;
  }

  size_t lineN = 0;
  for (std::string line; getline(ifs, line); ++lineN) {
    Document document;
    std::istringstream iss(line);

    int level = -1;
    if (!(iss >> level)) {
      std::cerr << "parse error: you must set level in line " << lineN << std::endl;
      return false;
    }
    document.level = level;

    if (!libsvm) {
      std::string word = "";
      while (iss >> word) {
        size_t word_id = 0;
        if (f2i->find(word) == f2i->end()) {
          word_id = f2i->size();
          f2i->insert(std::make_pair(word, word_id));
        } else {
          word_id = f2i->at(word);
        }

        document.fv.push_back(std::make_pair(word_id, 1.0));
      }
    } else {
      size_t id = 0;
      char comma = 0;
      double value = 0.0;
      while (iss >> id >> comma >> value) {
        document.fv.push_back(std::make_pair(id, value));
      }
    }
    query->push_back(document);
  }

  return true;
}

template <class T>
int Run (T& ranking,
         const char* ranking_name,
         const Queries& train,
         const Query& test) {
  std::cout << ranking_name << std::endl;
  ranking.Train(train, 100);

  size_t score = 0;
  for (size_t i = 0; i < test.size(); ++i) {
    int result;
    result = ranking.Test(test[i].fv);
    if (test[i].level == result)
      ++score;
  }

  std::cout << "accuracy : " << score << " / " << test.size() << std::endl;
  std::cout << std::endl;
  return 0;
}

} //namespace
