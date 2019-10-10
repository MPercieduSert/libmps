#include "InfoBdd.h"

InfoBdd::InfoBdd(const QString & table, const QMap<int,QString> & att,
        const QVector<Caract> & attCaract,
        const QVector<QVector<int>> & attUnique,
        const QMap<int,QString> & foreignKey)
    : m_attSql(att.count()),
      m_attCaract(attCaract),
      m_attUnique(attUnique),
      m_foreignKey(foreignKey),
      m_table(table)
{
    m_attSql[Entity::Id] = QString("ID");
    m_attSql[Entity::Id].squeeze();
    for(QMap<int,QString>::const_iterator i = att.cbegin(); i != att.cend(); ++i)
    {
        if(i.key() <= 0 || i.key() >= att.count())
            throw std::invalid_argument(QString("Indice d'attribut sql invalide de la table: ")
                                        .append(m_table).append(", attribut: ")
                                        .append(QString::number(i.key())).append("->")
                                        .append(i.value()).toStdString());
        m_attSql[i.key()] = i.value();
        m_attSql[i.key()].squeeze();
    }
}

InfoBdd::InfoBdd(const QString & table, int nbrAtt, const QVector<int> &nbrAttUnique)
    : m_attSql(nbrAtt),
      m_attCaract(nbrAtt-1),
      m_attUnique(nbrAttUnique.size()),
      m_table(table)
{
    m_attSql[Entity::Id] = QString("ID");
    m_attSql[Entity::Id].squeeze();
    for (auto i = 0; i != m_attUnique.size(); ++i)
    {
        m_attUnique[i].resize(nbrAttUnique.at(i));
        m_attUnique[i].squeeze();
        m_attUnique.squeeze();
    }
}
