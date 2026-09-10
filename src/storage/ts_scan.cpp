#include "storage/ts_scan.hpp"

namespace duckdb {

TSScanFunction::TSScanFunction()
    : TableFunction("ts_scan", {LogicalType::VARCHAR, LogicalType::VARCHAR}, nullptr, nullptr,
                    nullptr, nullptr) {
}

} // namespace duckdb
