#include "storage/ts_catalog.hpp"
#include "storage/ts_schema_entry.hpp"
#include "storage/ts_table_entry.hpp"
#include "storage/ts_transaction.hpp"
#include "duckdb/catalog/dependency_list.hpp"
#include "duckdb/parser/parsed_data/create_table_info.hpp"
#include "duckdb/parser/parsed_data/create_view_info.hpp"
#include "duckdb/parser/parsed_data/create_index_info.hpp"
#include "duckdb/planner/parsed_data/bound_create_table_info.hpp"
#include "duckdb/parser/parsed_data/drop_info.hpp"
#include "duckdb/parser/constraints/list.hpp"
#include "duckdb/common/unordered_set.hpp"
#include "duckdb/parser/parsed_data/alter_info.hpp"
#include "duckdb/parser/parsed_data/alter_table_info.hpp"
#include "duckdb/parser/parsed_expression_iterator.hpp"
#include "duckdb/parser/expression/columnref_expression.hpp"

namespace duckdb {

TSSchemaEntry::TSSchemaEntry(Catalog &catalog, CreateSchemaInfo &info) : SchemaCatalogEntry(catalog, info) {
        catalog_map = make_uniq<TSCatalogMap>();
}

static TSTransaction &GetTSTransaction(CatalogTransaction transaction) {
	if (!transaction.transaction) {
		throw InternalException("No transaction!?");
	}
	return transaction.transaction->Cast<TSTransaction>();
}

optional_ptr<CatalogEntry> TSSchemaEntry::CreateTable(CatalogTransaction transaction, BoundCreateTableInfo &info) {
        auto &trans = GetTSTransaction(transaction);
        auto table_name = static_cast<const string&>(info.Base().GetTableName());
        auto table = make_uniq<TSTableEntry>(trans.GetCatalog(), *this, info);

        return catalog_map->InsertEntry(table_name, std::move(table));
        //throw BinderException("TS databases do not support creating tables");
}

optional_ptr<CatalogEntry> TSSchemaEntry::CreateFunction(CatalogTransaction transaction, CreateFunctionInfo &info) {
	throw BinderException("TS databases do not support creating functions");
}

optional_ptr<CatalogEntry> TSSchemaEntry::CreateIndex(CatalogTransaction transaction, CreateIndexInfo &info,
                                                      TableCatalogEntry &table) {
        throw BinderException("TS databases do not support creating indexes");
}

optional_ptr<CatalogEntry> TSSchemaEntry::CreateView(CatalogTransaction transaction, CreateViewInfo &info) {
        throw BinderException("TS databases do not support creating views");
}

optional_ptr<CatalogEntry> TSSchemaEntry::CreateSequence(CatalogTransaction transaction, CreateSequenceInfo &info) {
	throw BinderException("TS databases do not support creating sequences");
}

optional_ptr<CatalogEntry> TSSchemaEntry::CreateTableFunction(CatalogTransaction transaction,
                                                              CreateTableFunctionInfo &info) {
	throw BinderException("TS databases do not support creating table functions");
}

optional_ptr<CatalogEntry> TSSchemaEntry::CreateCopyFunction(CatalogTransaction transaction,
                                                             CreateCopyFunctionInfo &info) {
	throw BinderException("TS databases do not support creating copy functions");
}

optional_ptr<CatalogEntry> TSSchemaEntry::CreatePragmaFunction(CatalogTransaction transaction,
                                                               CreatePragmaFunctionInfo &info) {
	throw BinderException("TS databases do not support creating pragma functions");
}

optional_ptr<CatalogEntry> TSSchemaEntry::CreateCollation(CatalogTransaction transaction,
                                                          CreateCollationInfo &info) {
	throw BinderException("TS databases do not support creating collations");
}

optional_ptr<CatalogEntry> TSSchemaEntry::CreateType(CatalogTransaction transaction, CreateTypeInfo &info) {
	throw BinderException("TS databases do not support creating types");
}

void TSSchemaEntry::AlterTable(TSTransaction &sqlite_transaction, RenameTableInfo &info) {
}

void TSSchemaEntry::AlterTable(TSTransaction &sqlite_transaction, RenameColumnInfo &info) {
}

void TSSchemaEntry::AlterTable(TSTransaction &sqlite_transaction, AddColumnInfo &info) {
}

void TSSchemaEntry::AlterTable(TSTransaction &sqlite_transaction, RemoveColumnInfo &info) {
}

void TSSchemaEntry::Alter(CatalogTransaction catalog_transaction, AlterInfo &info) {
}

void TSSchemaEntry::Scan(ClientContext &context, CatalogType type,
                         const std::function<void(CatalogEntry &)> &callback) {
        auto &transaction = TSTransaction::Get(context, catalog);
	vector<string> entries;
	switch (type) {
	case CatalogType::TABLE_ENTRY:
		entries.emplace_back("t1");
		break;
        default:
		// no entries of this catalog type
		return;
	}
        for (auto &entry_name : entries) {
		callback(*GetEntry(GetCatalogTransaction(context), type, Identifier(entry_name)));
	}
}

void TSSchemaEntry::Scan(CatalogType type, const std::function<void(CatalogEntry &)> &callback) {
        throw InternalException("Scan");
}

void TSSchemaEntry::DropEntry(ClientContext &context, DropInfo &info) {
}

optional_ptr<CatalogEntry> TSSchemaEntry::LookupEntry(CatalogTransaction transaction,
                                                      const EntryLookupInfo &lookup_info) {
	switch (lookup_info.GetCatalogType()) {
        case CatalogType::TABLE_ENTRY:
                // duckdb_tables
		return catalog_map->GetEntry(lookup_info.GetEntryName());
	default:
		return nullptr;
	}
}

} // namespace duckdb
