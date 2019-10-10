#include "ReqSql.h"

QSqlQuery * ReqSql::m_requete = nullptr;

void ReqSql::affError() const
{
    QSqlError err(m_requete->lastError());
    if(err.isValid())
        throw std::invalid_argument(err.text().append("\n"+err.nativeErrorCode()+m_requete->lastQuery()).toStdString());
}

QString ReqSql::typeAttributSqlString(bdd::TypeAttributBdd n)
{
    using namespace bdd;
    switch (n)
    {
    case TypeAttributBdd::Primary:
        return "integer primary key";
    case TypeAttributBdd::Bool:
    case TypeAttributBdd::Integer:
        return "integer";
    case TypeAttributBdd::DateTime:
    case TypeAttributBdd::Date:
    case TypeAttributBdd::Text:
        return "text";
    case TypeAttributBdd::Variant:
    case TypeAttributBdd::Blob:
        return "blob";
    case TypeAttributBdd::Double:
    case TypeAttributBdd::Real:
        return "real";
    case TypeAttributBdd::Numeric:
        return "numeric";
    default:
        throw std::invalid_argument("Invalid argument dans typeAttributSqlString");
    }
}

QString ReqSql::wordSqlString(bdd::wordSql n)
{
    using namespace bdd;
    switch (n)
    {
    case wordSql::Create:
        return "CREATE TABLE";
    case wordSql::NotNull:
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
