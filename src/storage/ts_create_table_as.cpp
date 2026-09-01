#include "storage/ts_catalog.hpp"
#include "duckdb/common/exception/binder_exception.hpp"

namespace duckdb {

PhysicalOperator &TSCatalog::PlanCreateTableAs(ClientContext &context, PhysicalPlanGenerator &planner,
                                               LogicalCreateTable &op, PhysicalOperator &plan) {
        throw BinderException("TS databases do not support creating table as");
}

}
