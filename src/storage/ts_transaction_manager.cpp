#include "storage/ts_transaction_manager.hpp"
#include "duckdb/main/attached_database.hpp"

namespace duckdb {

TSTransactionManager::TSTransactionManager(AttachedDatabase &db_p, TSCatalog &ts_catalog)
    : TransactionManager(db_p), ts_catalog(ts_catalog) {
}

Transaction &TSTransactionManager::StartTransaction(ClientContext &context) {
	auto transaction = make_uniq<TSTransaction>(ts_catalog, *this, context);
	transaction->Start();
	auto &result = *transaction;
        lock_guard<mutex> l(transaction_lock);
	transactions[result] = std::move(transaction);
	return result;
}

ErrorData TSTransactionManager::CommitTransaction(ClientContext &context, Transaction &transaction) {
	auto &ts_transaction = transaction.Cast<TSTransaction>();
	ts_transaction.Commit();
        Close(transaction);
	return ErrorData();
}

void TSTransactionManager::RollbackTransaction(Transaction &transaction) {
	auto &ts_transaction = transaction.Cast<TSTransaction>();
	ts_transaction.Rollback();
        Close(transaction);
}

void TSTransactionManager::Close(Transaction &transaction) {
        unique_ptr<TSTransaction> close;
	{
		lock_guard<mutex> l(transaction_lock);
		auto entry = transactions.find(transaction);
		if (entry == transactions.end()) {
			return;
		}
		close = std::move(entry->second);
		transactions.erase(entry);
	}
}

void TSTransactionManager::Checkpoint(ClientContext &context, bool force) {
	auto &transaction = TSTransaction::Get(context, db.GetCatalog());
}

} // namespace duckdb
