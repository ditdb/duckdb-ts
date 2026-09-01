#include "storage/ts_catalog.hpp"
#include "duckdb/common/exception/binder_exception.hpp"

namespace duckdb {

PhysicalOperator &TSCatalog::PlanDelete(ClientContext &context, PhysicalPlanGenerator &planner, LogicalDelete &op,
                                        PhysicalOperator &plan) {
        throw BinderException("TS database unimplemented delete");
}

}
