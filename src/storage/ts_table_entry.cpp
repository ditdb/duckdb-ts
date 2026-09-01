#include "storage/ts_catalog.hpp"
#include "storage/ts_table_entry.hpp"
#include "storage/ts_transaction.hpp"
#include "duckdb/storage/statistics/base_statistics.hpp"
#include "duckdb/storage/table_storage_info.hpp"

namespace duckdb {

TSTableEntry::TSTableEntry(Catalog &catalog, SchemaCatalogEntry &schema, CreateTableInfo &info,
                                   bool all_varchar)
    : TableCatalogEntry(catalog, schema, info) {
}

} // namespace duckdb
