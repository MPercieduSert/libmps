#include "ReqSql.h"

using namespace managerMPS;

QSqlQuery *ReqSql::m_requete = nullptr;

void ReqSql::affError() const {
    QSqlError err(m_requete->lastError());
    if(err.is_valid())
        throw std::invalid_argument(err.text().append("\n"+err.nativeErrorCode()+"\n"+m_requete->lastQuery()).toStdString());
}

QString ReqSql::typeAttributSqlString(bmps::typeAttributBdd n) {
    using namespace bmps;
    switch (n) {
    case typeAttributBdd::Primary:
        return "integer primary key";
    case typeAttributBdd::Bool:
    case typeAttributBdd::Integer:
        return "integer";
    case typeAttributBdd::Date_Time:
    case typeAttributBdd::Date:
    case typeAttributBdd::Text:
        return "text";
    case typeAttributBdd::Variant:
    case typeAttributBdd::Blob:
        return "blob";
    case typeAttributBdd::Double:
    case typeAttributBdd::Real:
        return "real";
    case typeAttributBdd::Numeric:
        return "numeric";
    default:
        throw std::invalid_argument("Invalid argument dans typeAttributSqlString");
    }
}

QString ReqSql::wordSqlString(bmps::wordSql n) {
    using namespace bmps;
    switch (n) {
    case wordSql::Create:
        return "CREATE TABLE";
    case wordSql::Not_null:
        return "not null";
    case wordSql::Constraint:
        return "constraint";
    case wordSql::Unique:
        return "unique";
    case wordSql::Foreign:
        return "foreign key";
    case wordSql::Ref:
        return "references";
    default:
        throw std::invalid_argument("Invalid argument dans wordSqlString");
    }
}
