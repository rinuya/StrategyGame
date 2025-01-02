#ifndef SRC_SELECTABLE_HPP_
#define SRC_SELECTABLE_HPP_

#include <string>
#include <vector>
#include <iostream>

class Selectable {
 public:
  /**
   * @brief Construct a new Selectable object
   *
   * @param name
   */
  explicit Selectable(std::string name) : name_(name), label_("") {}

  /**
   * @brief Destroy the Selectable object
   *
   */
  virtual ~Selectable() {}

  /**
   * get this selectable's name.
   * @return This selectable's name.
   **/
  std::string getName() const { return name_; }

  /**
   * @brief Get the selectable's label
   *
   * @return std::string
   */
  std::string getLabel() const { return label_; }

  /**
   * @brief Get the selectable's list of information
   *
   * @return std::vector<std::string>
   */
  virtual std::vector<std::string> getInfo() const {
    std::vector<std::string> lines;
    lines.push_back(getName());
    return lines;
  }

  /**
   * @brief Set this selectable's label.
   *
   * @param label
   */
  void setLabel(const std::string label) { label_ = label; }

 private:
  std::string name_;
  std::string label_;
};

#endif  // SRC_SELECTABLE_HPP_
