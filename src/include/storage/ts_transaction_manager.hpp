//===----------------------------------------------------------------------===//
//                         DuckDB
//
// storage/ts_transaction_manager.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "storage/ts_catalog.hpp"
#include "storage/ts_transaction.hpp"
#include "duckdb/transaction/transaction_manager.hpp"
#include "duckdb/common/reference_map.hpp"

namespace duckdb {

class TSTransactionManager : public TransactionManager {
public:
	TSTransactionManager(AttachedDatabase &db_p, TSCatalog &ts_catalog);
        ~TSTransactionManager() = default;

	Transaction &StartTransaction(ClientContext &context) override;
	ErrorData CommitTransaction(ClientContext &context, Transaction &transaction) override;
	void RollbackTransaction(Transaction &transaction) override;

	void Checkpoint(ClientContext &context, bool force = false) override;

private:
	void Close(Transaction &transaction);

private:
	TSCatalog &ts_catalog;
	mutex transaction_lock;
	reference_map_t<Transaction, unique_ptr<TSTransaction>> transactions;
};

} // namespace duckdb
