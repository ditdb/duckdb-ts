#include "storage/ts_scan.hpp"
#include "duckdb/catalog/catalog.hpp"
#include "duckdb/catalog/entry_lookup_info.hpp"


namespace duckdb {

struct TSGlobalState : public GlobalTableFunctionState {
	explicit TSGlobalState(idx_t max_threads) : max_threads(max_threads) {
	}

	mutex lock;
	idx_t position = 0;
	idx_t max_threads;
	idx_t rows_per_group = 0;

	idx_t MaxThreads() const override {
		return max_threads;
	}
};

struct TSLocalState : public LocalTableFunctionState {
	vector<column_t> column_ids;
	//! The amount of rows we scanned as part of this row group
	idx_t scan_count = 1;

	~TSLocalState() {
	}
};

static idx_t TSMaxThreads(ClientContext &context, const FunctionData *bind_data_p) {
        return 1;
}

static unique_ptr<GlobalTableFunctionState> TSInitGlobalState(ClientContext &context,
                                                              TableFunctionInitInput &input) {
	auto result = make_uniq<TSGlobalState>(TSMaxThreads(context, input.bind_data.get()));
	result->position = 0;
        auto &bind_data = input.bind_data->Cast<TSBindData>();

	return std::move(result);
}

static unique_ptr<LocalTableFunctionState>
TSInitLocalState(ExecutionContext &context, TableFunctionInitInput &input, GlobalTableFunctionState *global_state) {
	auto result = make_uniq<TSLocalState>();
        auto &bind_data = input.bind_data->CastNoConst<TSBindData>();
	auto &gstate = global_state->Cast<TSGlobalState>();

	return std::move(result);
}

static void TSScan(ClientContext &context, TableFunctionInput &data, DataChunk &output) {
        static int count  = 0;
	auto &lstate = data.local_state->Cast<TSLocalState>();
	auto &gstate = data.global_state->Cast<TSGlobalState>();
	auto &bind_data = data.bind_data->Cast<TSBindData>();

        if (count != 0) {
                count = 0;
                output.SetChildCardinality(0);
                return;
        }

        auto &out_vec = output.data[0];
        FlatVector::GetDataMutable<int32_t>(out_vec)[0] = 10;
        output.SetChildCardinality(1);
        count++;
        return;
}

TSScanFunction::TSScanFunction()
        : TableFunction("ts_scan", {LogicalType::VARCHAR, LogicalType::VARCHAR}, TSScan, nullptr,
                        TSInitGlobalState, TSInitLocalState) {
}

} // namespace duckdb
