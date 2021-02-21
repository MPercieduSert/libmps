#include "InfoBdd.h"

using namespace bddMPS;

InfoBdd::InfoBdd(const QString &name, const QString &table, const std::map<post,QString> &att,
                const std::vector<Caract> &attCaract,
                const std::vector<std::vector<numt>> &attUnique,
                const std::map<post,QString> &foreignKeyName,
                const std::map<post,QString> &foreignKeyTable)
    : m_attSql(att.size()),
      m_attCaract(attCaract),
      m_attUnique(attUnique),
      m_foreignKeyName(foreignKeyName),
      m_foreignKeyTable(foreignKeyTable),
      m_name(name),
      m_table(table) {
    m_attSql[emps::entity::Id] = QString("ID");
    m_attSql[emps::entity::Id].squeeze();
    for(auto i = att.cbegin(); i != att.cend(); ++i) {
        if(i->first >= att.size())
            throw std::invalid_argument(QString("Indice d'attribut sql invalide de la table: ")
                                        .append(m_table).append(", attribut: ")
                                        .append(QString::number(i->first)).append("->")
                                        .append(i->second).toStdString());
        m_attSql[i->first] = i->second;
        m_attSql[i->first].squeeze();
    }
}

InfoBdd::InfoBdd(const QString &name, const QString &table, post nbr_att, const std::vector<post> &nbr_attUnique)
    : m_attSql(nbr_att),
      m_attCaract(nbr_att-1),
      m_attUnique(nbr_attUnique.size()),
      m_name(name),
      m_table(table) {
    m_attSql[emps::entity::Id] = QString("ID");
    m_attSql[emps::entity::Id].squeeze();
    for (szt i = 0; i != m_attUnique.size(); ++i) {
        m_attUnique[i].resize(nbr_attUnique[i]);
        m_attUnique[i].shrink_to_fit();
        m_attUnique.shrink_to_fit();
    }
}
