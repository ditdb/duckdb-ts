#include "storage/ts_transaction.hpp"
#include "storage/ts_catalog.hpp"
#include "storage/ts_schema_entry.hpp"
#include "storage/ts_table_entry.hpp"
#include "duckdb/parser/parsed_data/create_table_info.hpp"
#include "duckdb/parser/parsed_data/create_view_info.hpp"
#include "duckdb/catalog/catalog_entry/index_catalog_entry.hpp"
#include "duckdb/catalog/catalog_entry/view_catalog_entry.hpp"
#include "duckdb/parser/parsed_data/create_view_info.hpp"
#include "duckdb/parser/statement/create_statement.hpp"
#include "duckdb/parser/parser.hpp"
#include "duckdb/parser/parsed_expression_iterator.hpp"
#include "duckdb/parser/expression/columnref_expression.hpp"
#include "duckdb/common/file_system.hpp"

namespace duckdb {

TSTransaction::TSTransaction(TSCatalog &ts_catalog, TransactionManager &manager, ClientContext &context)
    : Transaction(manager, context), ts_catalog(ts_catalog) {
}

TSTransaction::~TSTransaction() {
}

void TSTransaction::Start() {
}

void TSTransaction::Commit() {
}

void TSTransaction::Rollback() {
}

TSTransaction &TSTransaction::Get(ClientContext &context, Catalog &catalog) {
	return Transaction::Get(context, catalog).Cast<TSTransaction>();
}

} // namespace duckdb
