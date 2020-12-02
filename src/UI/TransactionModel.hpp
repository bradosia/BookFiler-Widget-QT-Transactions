/*
 * @name BookFiler Widget - Transaction Widget
 * @version 1.00
 * @license MIT
 * @brief sqlite3 backend
 */

#ifndef BOOKFILER_WIDGET_TRANSACTION_MODEL_H
#define BOOKFILER_WIDGET_TRANSACTION_MODEL_H

// config
#include "../core/config.hpp"

// C++
#include <iostream>
#include <memory>
#include <queue>

/* boost 1.72.0
 * License: Boost Software License (similar to BSD and MIT)
 */
#include <boost/signals2.hpp>

/* QT 5.13.2
 * License: LGPLv3
 */
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

// Local Project
#include "../core/TransactionModelIndex.hpp"

/*
 * bookfiler - widget
 */
namespace bookfiler {
namespace widget {

/*
 * @brief Provides a simple tree model to show how to create and use
 * hierarchical models.
 */
class TransactionModel : public QAbstractItemModel {
  Q_OBJECT
private:
  std::shared_ptr<sqlite3> database;
  std::string tableName, idColumn, parentColumn, viewRootId;
  std::vector<QVariant> headerList;
  boost::signals2::signal<void(std::vector<std::string>,
                               std::vector<std::string>,
                               std::vector<std::string>)>
      updateSignal;

public:
  TransactionModel(QObject *parent = nullptr);
  ~TransactionModel();

  /* Sets the database to use for the model.
   * @param database mysqlite3 database that this tree widget will be synced
   * with
   * @param tableName the table name
   * @param idColumn The name of the ID column in the database table.
   * for example the sql schema may be: "guid" text(32) NOT NULL
   * * @param idColumn The name of the parent ID column in the database table.
   * for example the sql schema may be: "parent_guid" text(32)
   * @return 0 on success, else error code
   */
  int setData(std::shared_ptr<sqlite3> database, std::string tableName,
              std::string idColumn, std::string parentColumn);

  /* @param id the view root. "*" to view all rows with a NULL parent
   * @return 0 on success, else error code
   */
  int setRoot(std::string id);

  /* Called when the sqlite3 database is updated by another widget, thread, or
   * process. Internally, this method will need to ask the model for the list of
   * QModelIndex that need to be updated.
   * @param addedIdList a list of id that were added. Only the
   * row id provided was added, not the children, unless the child id is
   * also listed
   * @param updatedIdList a list of id that were updated. Only the
   * row id provided was updated, not the children, unless the child id is
   * also listed
   * @param deletedIdList a list of id that were deleted. Only the
   * row id provided was deleted, not the children, unless the child id is
   * also listed
   * @return 0 on success, else error code
   */
  int updateIdHint(std::vector<std::string> addedIdList,
                   std::vector<std::string> updatedIdList,
                   std::vector<std::string> deletedIdList);
  /* Connect a function that will be signaled when the database is updated by
   * this widget
   * @param addedIdList a list of id that were added. Only the
   * row id provided was added, not the children, unless the child id is
   * also listed
   * @param updatedIdList a list of id that were updated. Only the
   * row id provided was updated, not the children, unless the child id is
   * also listed
   * @param deletedIdList a list of id that were deleted. Only the
   * row id provided was deleted, not the children, unless the child id is
   * also listed
   * @return 0 on success, else error code
   */
  int connectUpdateIdHint(
      std::function<void(std::vector<std::string>, std::vector<std::string>,
                         std::vector<std::string>)>);

  /* The vector representation of an SQL "ORDER BY" clause.
   * For example the initialized object:
   * {{"Country","ASC"},{"CustomerName","DESC"}}
   * is converted to the following internally
   * ORDER BY Country ASC, CustomerName DESC;
   * Does not update the view. You should update the view after calling this.
   * @param sortOrderList A list of orders to sort by
   * @return 0 on success, else error code
   */
  int setSort(std::vector<std::pair<std::string, std::string>> sortOrderList);

  /* The vector representation of an SQL "WHERE" clause.
   * For example the initialized object:
   * {{"name","Josephine","="},{"description","funny","match"}}
   * is converted to the following internally
   * WHERE `name`='Josephine' AND `description` MATCH 'funny'
   * the matching methid may be:
   * "=" exact match
   * "match" full-text search
   * "auto" exact match for integers and full text search for strings
   * Does not update the view. You should update the view after calling this.
   * @param sortOrderList A list of orders to sort by
   * @return 0 on success, else error code
   */
  int setFilter(std::vector<std::tuple<std::string, std::string, std::string>>
                    sortOrderList);

  /* Essential QAbstractItemModel methods
   *
   * https://doc.qt.io/qt-5/qabstractitemmodel.html
   * When subclassing QAbstractItemModel, at the very least you must implement
   * index(), parent(), rowCount(), columnCount(), and data().
   * These functions are used in all read-only models, and form the basis of
   * editable models.
   */
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  /* Copy and move operations methods
   */
  Qt::DropActions supportedDropActions() const override;
  bool removeRows(int row, int count,
                  const QModelIndex &parent = QModelIndex()) override;
  /* Other QAbstractItemModel methods
   */
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;
};

} // namespace widget
} // namespace bookfiler

#endif // BOOKFILER_WIDGET_TRANSACTION_MODEL_H
