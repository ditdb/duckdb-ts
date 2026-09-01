#include "storage/ts_catalog.hpp"
#include "duckdb/common/exception/binder_exception.hpp"

namespace duckdb {

PhysicalOperator &TSCatalog::PlanInsert(ClientContext &context, PhysicalPlanGenerator &planner, LogicalInsert &op,
                                        optional_ptr<PhysicalOperator> plan) {
        throw BinderException("TS database unimplemented insert");
}
}
