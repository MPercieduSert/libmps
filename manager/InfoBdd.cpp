#include "InfoBdd.h"

using namespace bddMPS;

InfoBdd::InfoBdd(const QString & name, const QString & table, const std::map<post,QString> & att,
                const std::vector<Caract> & attCaract,
                const std::vector<std::vector<numt>> & attUnique,
                const std::map<post,QString> & foreignKeyName,
                const std::map<post,QString> & foreignKeyTable)
    : m_attSql(att.size()),
      m_attCaract(attCaract),
      m_attUnique(attUnique),
      m_foreignKeyName(foreignKeyName),
      m_foreignKeyTable(foreignKeyTable),
      m_name(name),
      m_table(table) {
    m_attSql[emps::Entity::Id] = QString("ID");
    m_attSql[emps::Entity::Id].squeeze();
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

InfoBdd::InfoBdd(const QString & name, const QString & table, post nbrAtt, const std::vector<post> &nbrAttUnique)
    : m_attSql(nbrAtt),
      m_attCaract(nbrAtt-1),
      m_attUnique(nbrAttUnique.size()),
      m_name(name),
      m_table(table) {
    m_attSql[emps::Entity::Id] = QString("ID");
    m_attSql[emps::Entity::Id].squeeze();
    for (szt i = 0; i != m_attUnique.size(); ++i) {
        m_attUnique[i].resize(nbrAttUnique[i]);
        m_attUnique[i].shrink_to_fit();
        m_attUnique.shrink_to_fit();
    }
}
