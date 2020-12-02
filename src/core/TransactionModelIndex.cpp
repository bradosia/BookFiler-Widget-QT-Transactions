/*
 * @name BookFiler Widget - Transaction Widget
 * @version 1.00
 * @license MIT
 * @brief sqlite3 backend
 */

/*
   @brief A container for items of data supplied by the simple tree model.
*/

// C++
#include <iostream>

// Local Project
#include "TransactionModelIndex.hpp"

/*
 * bookfiler - widget
 */
namespace bookfiler {
namespace widget {

TransactionModelIndex::TransactionModelIndex() {}

TransactionModelIndex::~TransactionModelIndex() {}

int TransactionModelIndex::setParent(TransactionModelIndex *parentIndex_) {
  parentIndex = parentIndex_;
  return 0;
}
int TransactionModelIndex::run() {

    return 0;
}

} // namespace widget
} // namespace bookfiler
