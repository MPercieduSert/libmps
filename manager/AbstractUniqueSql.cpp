#include "AbstractUniqueSql.h"

QString AbstractUniqueSql::uniqueString(const InfoBdd & info, int num) const
{
    QString sql("SELECT ID FROM ");
    sql.append(info.table()).append(" WHERE");
    QVector<int> numAttUnique(info.attributUnique(num));
    for(QVector<int>::const_iterator i = numAttUnique.cbegin(); i != numAttUnique.cend(); ++i)
        sql.append(" ").append(info.attribut(*i)).append("=? AND");
    sql.chop(3);
    sql.append("LIMIT 1");
    sql.squeeze();
    return sql;
}

QString AbstractUniqueSql::uniqueCreerString(const InfoBdd & info, int num, bool numero) const
{
    using namespace bdd;
    QString sql;
    sql.append(wordSqlString(wordSql::Constraint))
            .append(" UN").append(info.table()).append(numero ? "" : QString::number(num + 1))
            .append(" ").append(wordSqlString(wordSql::Unique)).append(" (");
    QVector<int> numAttUnique(info.attributUnique(num));
    for(QVector<int>::const_iterator i = numAttUnique.cbegin(); i != numAttUnique.cend(); ++i)
        sql.append(info.attribut(*i)).append(",");
    sql.chop(1);
    sql.append(")");
    return sql;
}
