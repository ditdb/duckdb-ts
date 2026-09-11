//===----------------------------------------------------------------------===//
//                         DuckDB
//
// ts_scanner.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "duckdb/function/table_function.hpp"
#include "duckdb/common/types.hpp"

namespace duckdb {

struct TSBindData : public TableFunctionData {
	string file_name;
	string table_name;

	vector<string> names;
	vector<LogicalType> types;
	vector<Value> params;

	optional_idx rows_per_group = 122880;

	optional_ptr<TableCatalogEntry> table;
};

class TSScanFunction : public TableFunction {
public:
	TSScanFunction();
};

} // namespace duckdb
