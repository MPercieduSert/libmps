#include "managers_base.h"

using namespace mps;
using namespace manager;

managers_base::managers_base(const entidt nbr_entity, const QString &version_table,
                   std::map<entidt,std::unique_ptr<abstract_manager>> &&managers,
                   QSqlQuery &&req)
      : m_nbr_entity(nbr_entity),
        m_requete(std::move(req)),
        m_managers(nbr_entity) {
      using UniqueVersion = num_type_unique_sql<version_bdd>;
      info_bdd info_version("Version",version_table,version_bdd::Nbr_Att,{UniqueVersion::Nbr_Unique});
      info_version.set_attribut(version_bdd::Date_Time,"dt",b2d::type_attribut_bdd::Date_Time);
      info_version.set_attribut(version_bdd::Num,"num");
      info_version.set_attribut(version_bdd::Type,"tp");
      info_version.set_unique(version_bdd::Num,UniqueVersion::Num_Unique);
      info_version.set_unique(version_bdd::Type,UniqueVersion::Type_Unique);
      m_manager_version = std::make_unique<manager_sql<version_bdd>>(info_version, std::make_unique<UniqueVersion>());

      for(auto i = managers.begin(); i != managers.end(); ++i)
          m_managers[i->first] = std::move(i->second);
}

void managers_base::creer_version() {
    m_manager_version->creer();
    save_version(b2d::bdd_version::Creation,b2d::bdd_version::Library_Type);
}

bool managers_base::exists_version(idt type)
    {return m_manager_version->exists(version_bdd::Type,type);}

entities_base::version_bdd managers_base::get_version(idt type) {
    version_bdd ver(num_version(type),type);
    m_manager_version->get_unique(ver);
    return ver;
}

const info_bdd &managers_base::info(entidt id) const {
    if(valide(id))
            return m_managers[id]->info();
    else
        throw std::invalid_argument(QString("managers_base::info(entidt) manager d'indentifiant ")
                                    .append(QString::number(id)).append("invalide").toStdString());
}

info_bdd managers_base::info_bdd_arbre(const QString &table) const {
    using arbre = entities_base::arbre;
    using Uniquearbre = arbre_unique_sql<arbre>;
    info_bdd info_arbre(table,table, arbre::Nbr_Att,{Uniquearbre::Nbr_Unique});
    info_arbre.set_attribut(arbre::Parent,"pt",b2d::type_attribut_bdd::Integer,false);
    info_arbre.set_attribut(arbre::Feuille,"fl",b2d::type_attribut_bdd::Bool,true);
    info_arbre.set_attribut(arbre::Num,"num",b2d::type_attribut_bdd::Integer,true);
    info_arbre.set_unique(arbre::Parent,Uniquearbre::Parent_Unique);
    info_arbre.set_unique(arbre::Num,Uniquearbre::Num_Unique);
    info_arbre.set_foreign_key(arbre::Parent,info_arbre);
    return info_arbre;
}

std::unique_ptr<entity> managers_base::make_entity(const QString &ent) const {
    auto i = find(ent);
    if(i == m_nbr_entity)
        return nullptr;
    else
        return m_managers[i]->make_entity();
}

int managers_base::num_version(numt type)
    {return m_manager_version->fonction_agrega(b2d::Max,version_bdd::Num,version_bdd::Type,type).toInt();}

void managers_base::save_version(int num, numt type)
    {m_manager_version->save(version_bdd(num,type));}

void managers_base::set_requete(QSqlQuery &&req) {
    m_requete = std::move(req);
    abstract_manager_sql::set_requete(&m_requete);
}
