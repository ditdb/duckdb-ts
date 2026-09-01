#include "storage/ts_catalog.hpp"

#include "storage/ts_schema_entry.hpp"
#include "storage/ts_transaction.hpp"

#include "duckdb/parser/parsed_data/create_schema_info.hpp"
#include "duckdb/storage/database_size.hpp"
#include "duckdb/main/attached_database.hpp"
#include "duckdb/common/exception/binder_exception.hpp"
#include "duckdb/common/exception/transaction_exception.hpp"

#include <filesystem>

namespace duckdb {

TSCatalog::TSCatalog(AttachedDatabase &db_p, const string &path_p, TSOpenOptions options_p)
        : Catalog(db_p), path(path_p), options(std::move(options_p)) {
	namespace fs = std::filesystem;
	fs::path data_dir(path);
	try {
		auto target_data_dir = data_dir.is_absolute() ? data_dir : "";
		if (target_data_dir.empty()) {
                        target_data_dir = fs::path(db.StoredPath()).parent_path() / data_dir;
                }
                // Current work dir when target_data_dir is not absolute dir.
                auto canonical_path = fs::weakly_canonical(target_data_dir);
                if (!fs::exists(canonical_path) || !fs::is_directory(canonical_path)) {
                        fs::create_directories(canonical_path);
                }

                path = canonical_path.string();
	} catch (fs::filesystem_error const &ex) {
                throw InvalidInputException(ex.what());
        }
}

TSCatalog::~TSCatalog() {
}

void TSCatalog::Initialize(bool load_builtin) {
	CreateSchemaInfo info;
	auto schema = info.SchemaName();
        auto catalog = info.SchemaCatalog();
	main_schema = make_uniq<TSSchemaEntry>(*this, info);
}

optional_ptr<CatalogEntry> TSCatalog::CreateSchema(CatalogTransaction transaction, CreateSchemaInfo &info) {
        if (info.SchemaName() == Identifier::DefaultSchema()) {
		return main_schema.get();
	}
	throw BinderException("TS databases do not support creating new schemas");
}

void TSCatalog::ScanSchemas(ClientContext &context, std::function<void(SchemaCatalogEntry &)> callback) {
	callback(*main_schema);
}

optional_ptr<SchemaCatalogEntry> TSCatalog::LookupSchema(CatalogTransaction transaction,
                                                         const EntryLookupInfo &schema_lookup,
                                                         OnEntryNotFound if_not_found) {
	throw BinderException("TS databases only have a single schema - \"%s\"", std::string(DEFAULT_SCHEMA));
}

bool TSCatalog::InMemory() {
	return false;
}

string TSCatalog::GetDBPath() {
	return path;
}

void TSCatalog::DropSchema(ClientContext &context, DropInfo &info) {
	throw BinderException("TS databases do not support dropping schemas");
}

DatabaseSize TSCatalog::GetDatabaseSize(ClientContext &context) {
	DatabaseSize result;
	return result;
}

} // namespace duckdb
