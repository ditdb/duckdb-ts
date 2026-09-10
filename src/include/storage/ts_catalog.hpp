//===----------------------------------------------------------------------===//
//                         DuckDB
//
// storage/ts_catalog.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/catalog/catalog.hpp"
#include "ts_options.hpp"

namespace duckdb {
class TSSchemaEntry;

class TSCatalog : public Catalog {
public:
	explicit TSCatalog(AttachedDatabase &db_p, const string &path, TSOpenOptions options);
	~TSCatalog();

	string attach_path;
        string database_name;
	TSOpenOptions options;

public:
	void Initialize(bool load_builtin) override;
	string GetCatalogType() override {
		return "ts";
	}

	optional_ptr<CatalogEntry> CreateSchema(CatalogTransaction transaction, CreateSchemaInfo &info) override;

	void ScanSchemas(ClientContext &context, std::function<void(SchemaCatalogEntry &)> callback) override;

	optional_ptr<SchemaCatalogEntry> LookupSchema(CatalogTransaction transaction, const EntryLookupInfo &schema_lookup,
	                                              OnEntryNotFound if_not_found) override;

	TSSchemaEntry &GetMainSchema() {
		return *main_schema;
	}

        PhysicalOperator &PlanCreateTableAs(ClientContext &context, PhysicalPlanGenerator &planner, LogicalCreateTable &op,
	                                    PhysicalOperator &plan) override;
	PhysicalOperator &PlanInsert(ClientContext &context, PhysicalPlanGenerator &planner, LogicalInsert &op,
	                             optional_ptr<PhysicalOperator> plan) override;
	PhysicalOperator &PlanDelete(ClientContext &context, PhysicalPlanGenerator &planner, LogicalDelete &op,
	                             PhysicalOperator &plan) override;
	PhysicalOperator &PlanUpdate(ClientContext &context, PhysicalPlanGenerator &planner, LogicalUpdate &op,
	                             PhysicalOperator &plan) override;

	DatabaseSize GetDatabaseSize(ClientContext &context) override;

        bool InMemory() override;
	string GetDBPath() override;

private:
	void DropSchema(ClientContext &context, DropInfo &info) override;

private:
	unique_ptr<TSSchemaEntry> main_schema;
        unique_ptr<TSSchemaEntry> current_schema;
};

} // namespace duckdb
