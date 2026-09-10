//===----------------------------------------------------------------------===//
//                         DuckDB
//
// storage/ts_catalog_map.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/catalog/catalog_entry.hpp"
#include "duckdb/common/mutex.hpp"
#include "duckdb/common/case_insensitive_map.hpp"

namespace duckdb {

class TSCatalogMap {
public:
	optional_ptr<CatalogEntry> InsertEntry(const string &entry_name, unique_ptr<CatalogEntry> entry);
	optional_ptr<CatalogEntry> GetEntry(const string &entry_name);
	void EraseEntry(const string &entry_name);

private:
	mutex lock;
	case_insensitive_map_t<unique_ptr<CatalogEntry>> catalog_entries;
};

} // namespace duckdb
