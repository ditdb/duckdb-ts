#include "storage/ts_table_entry.hpp"
#include "storage/ts_scan.hpp"
#include "storage/ts_catalog.hpp"
#include "storage/ts_transaction.hpp"
#include "duckdb/storage/statistics/base_statistics.hpp"
#include "duckdb/storage/table_storage_info.hpp"

namespace duckdb {

TSTableEntry::TSTableEntry(Catalog &catalog, SchemaCatalogEntry &schema, BoundCreateTableInfo &info)
        : TableCatalogEntry(catalog, schema, info.Base()), columns(std::move(info.Base().columns)) {
}

const ColumnList &TSTableEntry::GetColumns() const {
        return columns;
}

unique_ptr<BaseStatistics> TSTableEntry::GetStatistics(ClientContext &context, column_t column_id) {
        return nullptr;
}

TableFunction TSTableEntry::GetScanFunction(ClientContext &context, unique_ptr<FunctionData> &bind_data) {
        auto ts_bind_data = make_uniq<TSBindData>();
        for (auto &col : columns.Logical()) {
		ts_bind_data->names.emplace_back(col.GetName().GetIdentifierName());
		ts_bind_data->types.push_back(col.GetType());
	}
        ts_bind_data->table = this;

        bind_data = std::move(ts_bind_data);
        return static_cast<TableFunction>(TSScanFunction());
}

TableStorageInfo TSTableEntry::GetStorageInfo(ClientContext &context) {
        TableStorageInfo result;
        return result;
}

void TSTableEntry::BindUpdateConstraints(Binder &binder, LogicalGet &get, LogicalProjection &proj,
                                         LogicalUpdate &update, ClientContext &context) {
        return;
}

} // namespace duckdb
