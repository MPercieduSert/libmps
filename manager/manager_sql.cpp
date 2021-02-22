#include "manager_sql.h"

using namespace mps::manager;
const std::vector<QString> abstract_manager_sql::m_Fonction_agrega_string = {"max", "min", "avg", "count", "sum"};
const std::vector<QString> abstract_manager_sql::m_Condition_string = {"=", " is", "<", ">", "<=", ">=", "<>", " isnot"};
