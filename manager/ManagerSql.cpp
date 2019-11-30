#include "ManagerSql.h"

using namespace managerMPS;
const std::vector<QString> AbstractManagerSql::m_fonctionAgregaString = {"max", "min", "avg", "count", "sum"};
const std::vector<QString> AbstractManagerSql::m_conditionString = {"=", " is", "<", ">", "<=", ">=", "<>", " isnot"};
//const std::vector<QString> AbstractManagerSql::m_fonctionAgregaString = (std::vector<QString>()<<QString("max")<<QString("min")<<QString("avg")<<QString("count")<<QString("sum"));
//const std::vector<QString> AbstractManagerSql::m_conditionString = std::vector<QString>()<<QString("=")<<QString(" is")<<QString("<")<<QString(">")<<QString("<=")<<QString(">=")<<QString("<>")<<QString(" isnot");
//const std::vector<QString> AbstractManagerSql::m_conditionNullString = std::vector<QString>()<<QString("isnull")<<QString("notnull");
//constexpr std::array<const char *, bmps::NbrAgrega> AbstractManagerSql::m_fonctionAgregaString {{"max","min","avg","count","sum"}};
