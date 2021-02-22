#include "abstract_unique_sql.h"

using namespace mps::b2d;
using namespace mps::manager;


QString abstract_unique_sql::unique_string(const info_bdd &info, numt num) const {
    QString sql("SELECT ID FROM ");
    sql.append(info.table()).append(" WHERE");
    auto numAttUnique = info.attribut_unique(num);
    for(auto i = numAttUnique.cbegin(); i != numAttUnique.cend(); ++i)
        sql.append(" ").append(info.attribut(*i)).append("=? AND");
    sql.chop(3);
    sql.append("LIMIT 1");
    sql.squeeze();
    return sql;
}

QString abstract_unique_sql::unique_creer_string(const info_bdd &info, numt num, bool numero) const {
    QString sql;
    sql.append(word_sql_string(word_sql::Constraint))
            .append(" UN").append(info.table()).append(numero ? "" : QString::number(num + 1))
            .append(" ").append(word_sql_string(word_sql::Unique)).append(" (");
    std::vector<numt> numAttUnique(info.attribut_unique(num));
    for(auto i = numAttUnique.cbegin(); i != numAttUnique.cend(); ++i)
        sql.append(info.attribut(*i)).append(",");
    sql.chop(1);
    sql.append(")");
    return sql;
}
