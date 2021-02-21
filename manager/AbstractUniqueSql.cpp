#include "AbstractUniqueSql.h"

using namespace managerMPS;
using namespace bddMPS;

QString AbstractUniqueSql::uniqueString(const InfoBdd &info, numt num) const {
    QString sql("SELECT ID FROM ");
    sql.append(info.table()).append(" WHERE");
    auto numAttUnique = info.attributUnique(num);
    for(auto i = numAttUnique.cbegin(); i != numAttUnique.cend(); ++i)
        sql.append(" ").append(info.attribut(*i)).append("=? AND");
    sql.chop(3);
    sql.append("LIMIT 1");
    sql.squeeze();
    return sql;
}

QString AbstractUniqueSql::uniqueCreerString(const InfoBdd &info, numt num, bool numero) const {
    QString sql;
    sql.append(wordSqlString(wordSql::Constraint))
            .append(" UN").append(info.table()).append(numero ? "" : QString::number(num + 1))
            .append(" ").append(wordSqlString(wordSql::Unique)).append(" (");
    std::vector<numt> numAttUnique(info.attributUnique(num));
    for(auto i = numAttUnique.cbegin(); i != numAttUnique.cend(); ++i)
        sql.append(info.attribut(*i)).append(",");
    sql.chop(1);
    sql.append(")");
    return sql;
}
