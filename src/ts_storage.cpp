#include "duckdb.hpp"

#include "ts_storage.hpp"
#include "storage/ts_catalog.hpp"
#include "storage/ts_transaction_manager.hpp"
#include "duckdb/parser/parsed_data/attach_info.hpp"
#include "duckdb/main/attached_database.hpp"
#include "duckdb/transaction/transaction_manager.hpp"
#include "duckdb/catalog/catalog_entry/schema_catalog_entry.hpp"
#include "duckdb/catalog/catalog_entry/table_catalog_entry.hpp"

namespace duckdb {

static unique_ptr<Catalog> AttachTS(optional_ptr<StorageExtensionInfo> storage_info, ClientContext &context,
                                    AttachedDatabase &db, const string &name, AttachInfo &info,
                                    AttachOptions &attach_options) {
	TSOpenOptions options;
	options.access_mode = attach_options.access_mode;

	return make_uniq<TSCatalog>(db, info.path, std::move(options));
}

static unique_ptr<TransactionManager> CreateTSTransactionManager(optional_ptr<StorageExtensionInfo> storage_info,
                                                                 AttachedDatabase &db, Catalog &catalog) {
	auto &ts_catalog = catalog.Cast<TSCatalog>();
	return make_uniq<TSTransactionManager>(db, ts_catalog);
}

TSStorageExtension::TSStorageExtension() {
	attach = AttachTS;
	create_transaction_manager = CreateTSTransactionManager;
}

} // namespace duckdb
