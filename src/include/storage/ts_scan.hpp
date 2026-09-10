//===----------------------------------------------------------------------===//
//                         DuckDB
//
// ts_scanner.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/function/table_function.hpp"

namespace duckdb {

class TSScanFunction : public TableFunction {
public:
	TSScanFunction();
};

} // namespace duckdb
