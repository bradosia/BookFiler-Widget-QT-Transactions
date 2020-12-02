# Development Summary

I need a QT5 widget written in C++. Only the widget implementation needs to be written, not an app. The widget is a financial transaction manager. There are two main components to this widget:
* The view, which inherits from `QTreeView` or `QTableView` or any `QWidget` you think works best. This part needs to be designed to look like the reference image.
* The model, which inherits from `QAbstractItemModel`. This widget uses `sqlite3`.

![/reference/widget-tasklist-subtasks.png](/reference/transactions-1.png?raw=true)

# View component

## How does sorting work?

The user should be able to click the headers labeled: Date, Num, Description, Notes, Transfer, R, L, Total Increase, Tot Decrease to toggle ascending and descending order. The user can sort by multiple columns by clicking one column, then clocking another so that data is sorted first by the last clicked column, then the second to last one. The `AbstractItemModel` will have a method to call with the list of columns to sort by.

If the user creates a custom sort, then the balance column will be grayed out and blank. This is because balance column only makes sense when transactions are sorted by date.

```cpp
// Query a new order with the model
// Uses {columnName, order} pairs for the sort
std::vector<std::pair> sortOrderList{{"date","ASC"},{"totalIncrease","DESC"}};
abstractItemModelPtr->setSort(sortOrderList);
```

Then the model will signal to the transaction view widget when the data is received and the view needs to update.

## Why are there two rows for the header?

The header has two rows. The header should be visible, even when scrolling through the transactions. Columns should able to be re-arranged by dragging the headers to another spot on the header bar.

## Cell selection

The user should be able to select cells by clicking on them. They are able to select many cells by holding alt or shift while selecting. When copying and pasting a selection to a text pad, it should paste the tab delimited values of the copied cells.

## How transactions are updated?

The widget view is updated when the controller calls `update(...)`

# Model component

The Tree Model is similar to `QSqlTableModel`, except it is specialized to use `sqlite3`. The reason `sqlite3` is used and not `QSqlDatabase` is because other modules this widget is used with won't be linked against QT, so a `sqlite3` database is passed between modules instead.

The header file written for the `TransactionModel` documents how the methods should work: [/src/UI/TransactionModel.hpp](/src/UI/TransactionModel.hpp)

There is already a partial implementation for the transaction model to give an example of how the column names are pulled out of a `sqlite3` database table. Database query results need to somehow be cached in the model. One design idea may be to cache queries in a custom table index that will be used with the `QModelIndex`. A mostly empty file for this is at [/src/core/TransactionModelIndex.hpp](/src/core/TransactionModelIndex.hpp).

## Sorting and Filtering Implementation

Most of the time, in memory `sqlite3` databases will be used so using sql queries to refetch the `sqlite3` database table should be quick. Similar to `QSqlTableModel`, the `TreeModel` also has `setSort` and `setFilter` methods, but the usage is different. See the header file [/src/UI/TransactionModel.hpp](/src/UI/TransactionModel.hpp) for my notes on these methods.

## What does the `sqlite3` table look like?

Here is the create statement for the table.
```sql
CREATE TABLE "transactionTest" (
	"guid"	TEXT NOT NULL,
	"date"	TEXT,
	"num"	TEXT,
	"description"	TEXT,
	"notes"	TEXT,
	"transfer"	TEXT,
	"R"	TEXT,
	"L"	TEXT,
	"Tot Increase"	INTEGER,
	"Tot Decrease"	INTEGER,
	"Balance"	INTEGER,
	PRIMARY KEY("guid")
)
```

# Libraries, Compiler, and compatability

The Libraries used are QT, Boost, and sqlite3. Other libraries need to be approved before use.

Program must compile on Windows and Linux. For Windows, use MinGW for compiling. For Linux use GCC. Use cmake as the build scipt. Personally, the IDE I use is QT Creator, but any should work as long as you can build with cmake.

# Collaboration

To begin development, you should fork this repository and begin implementing the source files at [/src/](/src). Make a pull request or zip up your forked repository and send it to me when delivering work.

## Coding Standards
Always use the standard library when possible. Use `std::shared_ptr` and `std::unique_ptr` instead of raw pointers whenever possible, except when creating widgets since the the QT UI will Immediately take control of the widget. use `boost` if some method does not exist in standard library. Finally use `QT5` as the last option. For example use `std::string` instead of `QString` so the code is more portable. Use `QString` only when necessary to pass to a QT function.

Separate all graphical GUI code into the `/src/UI/` directory. Anything with `QT` should be in the `/src/UI/` directory. All logic and non-GUI code goes into the `/src/core/` directory.

* Use camel case
* Use C++17 best coding practices
* Use the standard library when possible

## Deliverables

* Clean and commented code that follows the general design already provided and discussed in the readme.
* Widget works and looks as the photos reference images show.
* Make a working example [/src_example/example00/main.cpp](/src_example/example00/main.cpp).
* transaction cells are selectable and copyable as tab separated values in the clipboard.
* Columns can be re-ordered
* Headers can toggle the sort order

