#include "ManagerSql.h"

const QVector<QString> AbstractManagerSql::m_fonctionAgregaString = (QVector<QString>()<<QString("max")<<QString("min")<<QString("avg")<<QString("count")<<QString("sum"));
const QVector<QString> AbstractManagerSql::m_conditionString = QVector<QString>()<<QString("=")<<QString(" is")<<QString("<")<<QString(">")<<QString("<=")<<QString(">=")<<QString("<>")<<QString(" isnot");
//const QVector<QString> AbstractManagerSql::m_conditionNullString = QVector<QString>()<<QString("isnull")<<QString("notnull");
//constexpr std::array<const char *, bdd::NbrAgrega> AbstractManagerSql::m_fonctionAgregaString {{"max","min","avg","count","sum"}};
