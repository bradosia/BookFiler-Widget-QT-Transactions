/*
 * @name BookFiler Widget - Transaction Widget
 * @version 1.00
 * @license MIT
 * @brief sqlite3 backend
 */

/* QT 5.13.2
 * License: LGPLv3
 */
#include <QStringList>

// Local Project
#include "TransactionModel.hpp"

/*
 * bookfiler - widget
 */
namespace bookfiler {
namespace widget {

TransactionModel::TransactionModel(QObject *parent) : QAbstractItemModel(parent) {}

TransactionModel::~TransactionModel() {}

/* Custom methods
 *
 *
 *
 *
 */

int TransactionModel::setData(std::shared_ptr<sqlite3> database_,
                       std::string tableName_, std::string idColumn_,
                       std::string parentColumn_) {
  char *errMessage = 0;
  int rc = 0;

  // Set sqlite database information
  database = database_;
  tableName = tableName_;
  idColumn = idColumn_;
  parentColumn = parentColumn_;

  /* Get the table headers */
  std::string sqlQuery =
      "SELECT name FROM pragma_table_info('" + tableName + "');";

  sqlite3_stmt *stmt = nullptr;
  rc = sqlite3_prepare_v2(database.get(), sqlQuery.c_str(), -1, &stmt, nullptr);
  if (rc != SQLITE_OK)
    return rc;

  int rowCount = 0;
  rc = sqlite3_step(stmt);
  while (rc != SQLITE_DONE && rc != SQLITE_OK) {
    rowCount++;
    int colCount = sqlite3_column_count(stmt);
    for (int colIndex = 0; colIndex < colCount; colIndex++) {
      int type = sqlite3_column_type(stmt, colIndex);
      const char *columnName = sqlite3_column_name(stmt, colIndex);
      const unsigned char *valChar = sqlite3_column_text(stmt, colIndex);
      headerList.push_back(reinterpret_cast<const char *>(valChar));
#if BOOKFILER_WIDGET_TRANSACTION_WIDGET_MODEL_SET_DATA
      std::cout << "row: " << rowCount << ", col: " << colCount
                << ", colName: " << columnName << ", valChar: " << valChar
                << std::endl;
#endif
    }
    rc = sqlite3_step(stmt);
  }

  rc = sqlite3_finalize(stmt);
  if (rc != 0) {
    return rc;
  }
  return 0;
}

int TransactionModel::setRoot(std::string id) {
  viewRootId = id;
  return 0;
}

int TransactionModel::updateIdHint(std::vector<std::string> addedIdList,
                            std::vector<std::string> updatedIdList,
                            std::vector<std::string> deletedIdList) {
  return 0;
}
int TransactionModel::connectUpdateIdHint(
    std::function<void(std::vector<std::string>, std::vector<std::string>,
                       std::vector<std::string>)>
        slot) {
  updateSignal.connect(slot);
  return 0;
}

/* Base methods for the view
 *
 *
 *
 */

int TransactionModel::columnCount(const QModelIndex &parent) const {
#if BOOKFILER_WIDGET_TRANSACTION_WIDGET_MODEL_COLUMN_COUNT
  std::cout << "TransactionModel::columnCount row: " << parent.row()
            << ", col: " << parent.column();
  if (parent.internalPointer()) {
    std::string parentId =
        *static_cast<std::string *>(parent.internalPointer());
    std::cout << ", parentId: " << parentId;
  }
  std::cout << std::endl;
#endif
  return headerList.size();
}

QVariant TransactionModel::data(const QModelIndex &index, int role) const {
#if BOOKFILER_WIDGET_TRANSACTION_WIDGET_MODEL_DATA
  std::cout << "TransactionModel::data index.row: " << index.row()
            << ", index.col: " << index.column();
  if (index.internalPointer()) {
    std::string indexId = *static_cast<std::string *>(index.internalPointer());
    std::cout << ", parentId: " << indexId;
  }
  std::cout << std::endl;
#endif

  if (!index.isValid())
    return QVariant();

  std::string childId = *static_cast<std::string *>(index.internalPointer());

  // Normal data display
  if (role == Qt::DisplayRole) {
    // return item->data(index.column());
  }
  // Data displayed in the edit box
  else if (role == Qt::EditRole) {
    // return item->data(index.column());
  }

  // for all else
  return QVariant();
}

Qt::ItemFlags TransactionModel::flags(const QModelIndex &index) const {
#if BOOKFILER_WIDGET_TRANSACTION_WIDGET_MODEL_DATA
  std::cout << "TransactionModel::flags index.row: " << index.row()
            << ", index.col: " << index.column();
  if (index.internalPointer()) {
    std::string indexId = *static_cast<std::string *>(index.internalPointer());
    std::cout << ", indexId: " << indexId;
  }
  std::cout << std::endl;
#endif
  if (!index.isValid())
    return Qt::NoItemFlags;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable |
         Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

QVariant TransactionModel::headerData(int section, Qt::Orientation orientation,
                               int role) const {
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    return headerList.at(section);
  }

  return QVariant();
}

QModelIndex TransactionModel::index(int row, int column,
                             const QModelIndex &parent) const {
  char *errMessage = 0;
  int rc = 0;
  std::string parentId;
  std::string *childId;

#if BOOKFILER_WIDGET_TRANSACTION_WIDGET_MODEL_DATA
  std::cout << "TransactionModel::index parent.row: " << parent.row()
            << ", parent.col: " << parent.column();
  std::cout << ", row: " << row << ", col: " << column;
  if (parent.internalPointer()) {
    parentId = *static_cast<std::string *>(parent.internalPointer());
    std::cout << ", parentId: " << parentId;
  }
  std::cout << std::endl;
#endif

  if (!hasIndex(row, column, parent))
    return QModelIndex();

  if (!parent.isValid())
    parentId = viewRootId;
  else
    parentId = *static_cast<std::string *>(parent.internalPointer());

  // Get the parentID from the SELECT of the id
  std::string sqlQuery = "SELECT `" + idColumn + "` FROM `" + tableName;
  if (parentId != "*") {
    sqlQuery.append(" WHERE `" + parentColumn + "`='" + parentId + "'");
  }
  sqlQuery.append("` LIMIT " + std::to_string(row) + ",1;");

  sqlite3_stmt *stmt = nullptr;
  rc = sqlite3_prepare_v2(database.get(), sqlQuery.c_str(), -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    return QModelIndex();
  }

  rc = sqlite3_step(stmt);
  while (rc != SQLITE_DONE && rc != SQLITE_OK) {
    int colCount = sqlite3_column_count(stmt);
    for (int colIndex = 0; colIndex < colCount; colIndex++) {
      const unsigned char *valChar = sqlite3_column_text(stmt, colIndex);
      childId = new std::string(reinterpret_cast<const char *>(valChar));
    }
    rc = sqlite3_step(stmt);
  }

  rc = sqlite3_finalize(stmt);
  if (rc != 0) {
    return QModelIndex();
    ;
  }

#if BOOKFILER_WIDGET_TRANSACTION_WIDGET_MODEL_ROW_COUNT
  std::cout << "TransactionModel::index childId: " << *childId << std::endl;
#endif

  if (childId)
    return createIndex(row, column, childId);
  return QModelIndex();
}

QModelIndex TransactionModel::parent(const QModelIndex &index) const {
  char *errMessage = 0;
  int rc = 0;
  std::string *parentId = nullptr;
  std::string indexId;

#if BOOKFILER_WIDGET_TRANSACTION_WIDGET_MODEL_DATA
  std::cout << "TransactionModel::parent row: " << index.row()
            << ", col: " << index.column();
  if (index.internalPointer()) {
    indexId = *static_cast<std::string *>(index.internalPointer());
    std::cout << ", indexId: " << indexId;
  }
  std::cout << std::endl;
#endif

  if (!index.isValid())
    return QModelIndex();

  // Get the parentID from the SELECT of the id
  std::string sqlQuery = "SELECT `" + parentColumn + "` FROM `" + tableName +
                         "` WHERE `" + idColumn + "`='" + indexId + "';";

  sqlite3_stmt *stmt = nullptr;
  rc = sqlite3_prepare_v2(database.get(), sqlQuery.c_str(), -1, &stmt, nullptr);
  if (rc != SQLITE_OK)
    return QModelIndex();

  rc = sqlite3_step(stmt);
  while (rc != SQLITE_DONE && rc != SQLITE_OK) {
    int colCount = sqlite3_column_count(stmt);
    for (int colIndex = 0; colIndex < colCount; colIndex++) {
      int type = sqlite3_column_type(stmt, colIndex);
      if (type == SQLITE_NULL) {
        break;
      }
      const unsigned char *valChar = sqlite3_column_text(stmt, colIndex);
      parentId = new std::string(reinterpret_cast<const char *>(valChar));
    }
    rc = sqlite3_step(stmt);
  }

  rc = sqlite3_finalize(stmt);
  if (rc != 0) {
    return QModelIndex();
  }

#if BOOKFILER_WIDGET_TRANSACTION_WIDGET_MODEL_ROW_COUNT
  if (parentId) {
    std::cout << "TransactionModel::parent parentId: " << *parentId << std::endl;
  }
#endif

  if (!parentId)
    return QModelIndex();

  return createIndex(0, 0, parentId);
}

int TransactionModel::rowCount(const QModelIndex &parent) const {
  char *errMessage = 0;
  int rc = 0;
  int rowCountRet = 0;
  std::string parentId;

#if BOOKFILER_WIDGET_TRANSACTION_WIDGET_MODEL_DATA
  std::cout << "TransactionModel::rowCount parent.row: " << parent.row()
            << ", parent.col: " << parent.column();
  if (parent.internalPointer()) {
    parentId = *static_cast<std::string *>(parent.internalPointer());
    std::cout << ", parentId: " << parentId;
  }
  std::cout << std::endl;
#endif

  if (parent.column() > 0)
    return 0;

  if (!parent.isValid())
    parentId = viewRootId;
  else
    parentId = *static_cast<std::string *>(parent.internalPointer());

#if BOOKFILER_WIDGET_TRANSACTION_WIDGET_MODEL_ROW_COUNT
  std::cout << "TransactionModel::rowCount isValid: "
            << (parent.isValid() ? "true" : "false")
            << ", parentId: " << parentId << std::endl;
#endif

  // Get the parentID from the SELECT of the id
  std::string sqlQuery = "SELECT COUNT(*) FROM `" + tableName + "`";
  if (parentId != "*") {
    sqlQuery.append(" WHERE `" + parentColumn + "`='" + parentId + "'");
  }
  sqlQuery.append(";");

  sqlite3_stmt *stmt = nullptr;
  rc = sqlite3_prepare_v2(database.get(), sqlQuery.c_str(), -1, &stmt, nullptr);
  if (rc != SQLITE_OK)
    return 0;

  rc = sqlite3_step(stmt);
  while (rc != SQLITE_DONE && rc != SQLITE_OK) {
    int colCount = sqlite3_column_count(stmt);
    for (int colIndex = 0; colIndex < colCount; colIndex++) {
      rowCountRet = sqlite3_column_int(stmt, colIndex);
    }
    rc = sqlite3_step(stmt);
  }

  rc = sqlite3_finalize(stmt);
  if (rc != 0) {
    return 0;
  }

#if BOOKFILER_WIDGET_TRANSACTION_WIDGET_MODEL_ROW_COUNT
  std::cout << "TransactionModel::rowCount rowCountRet: " << rowCountRet << std::endl;
#endif

  return rowCountRet;
}

bool TransactionModel::setData(const QModelIndex &index, const QVariant &value,
                        int role) {
  if (role) {
    // TODO
  }
  return true;
}

Qt::DropActions TransactionModel::supportedDropActions() const {
  return Qt::CopyAction | Qt::MoveAction;
}

bool TransactionModel::removeRows(int row, int count, const QModelIndex &parent) {
  // TODO
  return true;
}

} // namespace widget
} // namespace bookfiler
