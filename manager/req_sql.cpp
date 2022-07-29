#include "req_sql.h"

using namespace mps::manager;

QSqlQuery *req_sql::m_requete = nullptr;

void req_sql::aff_error() const {
    QSqlError err(m_requete->lastError());
    if(err.isValid())
        throw std::invalid_argument(err.text().append("\n"+err.nativeErrorCode()+"\n"+m_requete->lastQuery()).toStdString());
}

QString req_sql::type_attribut_sql_string(b2d::type_attribut_bdd n) {
    using namespace b2d;
    switch (n) {
    case type_attribut_bdd::Primary:
        return "integer primary key";
    case type_attribut_bdd::Bool:
    case type_attribut_bdd::Integer:
        return "integer";
    case type_attribut_bdd::Date_Time:
    case type_attribut_bdd::Date:
    case type_attribut_bdd::Text:
        return "text";
    case type_attribut_bdd::Variant:
    case type_attribut_bdd::Blob:
        return "blob";
    case type_attribut_bdd::Double:
    case type_attribut_bdd::Real:
        return "real";
    case type_attribut_bdd::Numeric:
        return "numeric";
    default:
        throw std::invalid_argument("Invalid argument dans type_attribut_sql_string");
    }
}

QString req_sql::word_sql_string(b2d::word_sql n) {
    using namespace b2d;
    switch (n) {
    case word_sql::Create:
        return "CREATE TABLE";
    case word_sql::Not_null:
        return "not null";
    case word_sql::Constraint:
        return "constraint";
    case word_sql::Unique:
        return "unique";
    case word_sql::Foreign:
        return "foreign key";
    case word_sql::Ref:
        return "references";
    default:
        throw std::invalid_argument("Invalid argument dans word_sql_string");
    }
}
