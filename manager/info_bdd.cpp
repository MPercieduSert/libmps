#include "info_bdd.h"

using namespace mps::b2d;
using entity = mps::entities::entity;

info_bdd::info_bdd(const QString &name, const QString &table, const std::map<post,QString> &att,
                const std::vector<caract> &attcaract,
                const std::vector<std::vector<numt>> &att_unique,
                const std::map<post,QString> &foreign_key_name,
                const std::map<post,QString> &foreign_key_table)
    : m_att_sql(att.size()),
      m_att_caract(attcaract),
      m_att_unique(att_unique),
      m_foreign_key_name(foreign_key_name),
      m_foreign_key_table(foreign_key_table),
      m_name(name),
      m_table(table) {
    m_att_sql[entity::Id] = QString("ID");
    m_att_sql[entity::Id].squeeze();
    for(auto it = att.cbegin(); it != att.cend(); ++it) {
        if(it->first >= att.size())
            throw std::invalid_argument(QString("Indice d'attribut sql invalide de la table: ")
                                        .append(m_table).append(", attribut: ")
                                        .append(QString::number(it->first)).append("->")
                                        .append(it->second).toStdString());
        m_att_sql[it->first] = it->second;
        m_att_sql[it->first].squeeze();
    }
}

info_bdd::info_bdd(const QString &name, const QString &table, post nbr_att, const std::vector<post> &nbr_att_unique)
    : m_att_sql(nbr_att),
      m_att_caract(nbr_att-1),
      m_att_unique(nbr_att_unique.size()),
      m_name(name),
      m_table(table) {
    m_att_sql[entity::Id] = QString("ID");
    m_att_sql[entity::Id].squeeze();
    for (szt i = 0; i != m_att_unique.size(); ++i) {
        m_att_unique[i].resize(nbr_att_unique[i]);
        m_att_unique[i].shrink_to_fit();
        m_att_unique.shrink_to_fit();
    }
}
