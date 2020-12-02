/*
 * @name BookFiler Widget - Transaction Widget
 * @version 1.00
 * @license MIT
 * @brief sqlite3 backend
 */

#ifndef BOOKFILER_WIDGET_TRANSACTION_MODEL_INDEX_H
#define BOOKFILER_WIDGET_TRANSACTION_MODEL_INDEX_H

// config
#include "config.hpp"

/* sqlite3 3.33.0
 * License: PublicDomain
 */
#include <sqlite3.h>

/* QT 5.13.2
 * License: LGPLv3
 */
#include <QComboBox>
#include <QStyledItemDelegate>
#include <QVariant>
#include <QVector>

/*
 * bookfiler - widget
 */
namespace bookfiler {
namespace widget {
/*
 * @brief A container for items of data supplied by the simple tree model.
 */
class TransactionModelIndex {
private:
  TransactionModelIndex *parentIndex;

public:
  TransactionModelIndex();
  ~TransactionModelIndex();

  int setParent(TransactionModelIndex *parentIndex);
  int run();
};

} // namespace widget
} // namespace bookfiler

#endif // BOOKFILER_WIDGET_TRANSACTION_MODEL_INDEX_H
