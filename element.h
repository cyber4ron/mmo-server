#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <string>

class element {
 public:
  element() { }
  element(const std::string& id) : ele_id_(id) { }
  virtual ~element() {}
  inline void set_ele_id(const std::string& id) { ele_id_ = id; }
  inline const std::string& ele_id() { return ele_id_; }
 private:
  std::string ele_id_;
};

#endif