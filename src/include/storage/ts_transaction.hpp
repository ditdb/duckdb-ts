//===----------------------------------------------------------------------===//
//                         DuckDB
//
// storage/ts_transaction.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/transaction/transaction.hpp"
#include "duckdb/common/case_insensitive_map.hpp"
#include "duckdb/common/mutex.hpp"
#include "duckdb/common/atomic.hpp"

namespace duckdb {
class TSCatalog;
class TSTableEntry;

class TSTransaction : public Transaction {
public:
	TSTransaction(TSCatalog &ts_catalog, TransactionManager &manager, ClientContext &context);
	~TSTransaction() override;

	void Start();
	void Commit();
	void Rollback();

	static TSTransaction &Get(ClientContext &context, Catalog &catalog);

private:
	TSCatalog &ts_catalog;
};

} // namespace duckdb
