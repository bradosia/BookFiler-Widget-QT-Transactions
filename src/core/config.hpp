/*
 * @name BookFiler Widget - Transaction Widget
 * @version 1.00
 * @license MIT
 * @brief sqlite3 backend
 */

#ifndef BOOKFILER_WIDGET_TRANSACTION_WIDGET_CONFIG_H
#define BOOKFILER_WIDGET_TRANSACTION_WIDGET_CONFIG_H

#define BOOKFILER_WIDGET_TRANSACTION_WIDGET_MODEL_SET_DATA 0
#define BOOKFILER_WIDGET_TRANSACTION_WIDGET_MODEL_PARENT 0
#define BOOKFILER_WIDGET_TRANSACTION_WIDGET_MODEL_ROW_COUNT 0
#define BOOKFILER_WIDGET_TRANSACTION_WIDGET_MODEL_COLUMN_COUNT 0
#define BOOKFILER_WIDGET_TRANSACTION_WIDGET_MODEL_DATA 0
#define BOOKFILER_WIDGET_TRANSACTION_WIDGET_MODEL_INDEX 0

// C++
#include <string>

/* boost 1.72.0
 * License: Boost Software License (similar to BSD and MIT)
 */
#define BOOST_ALLOW_DEPRECATED_HEADERS

namespace bookfiler {
namespace widget {

static std::string moduleName = "BookFiler Widget";
static std::string moduleCode = "bookfiler::widget";

} // namespace widget
} // namespace bookfiler

#endif // BOOKFILER_WIDGET_TRANSACTION_WIDGET_CONFIG_H
