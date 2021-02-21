#include "Managers.h"

using namespace managerMPS;

Managers::Managers(const entidt nbrentity, const QString &versionTable,
                   std::map<entidt,std::unique_ptr<AbstractManager>> &&managers,
                   const QSqlQuery &req)
      : m_nbrentity(nbrentity),
        m_requete(req),
        m_managers(nbrentity) {
      using UniqueVersion = num_typeUniqueSql<version_bdd>;
      InfoBdd infoVersion("Version",versionTable,version_bdd::Nbr_Att,{UniqueVersion::NbrUnique});
      infoVersion.set_attribut(version_bdd::Date_Time,"dt",bmps::typeAttributBdd::Date_Time);
      infoVersion.set_attribut(version_bdd::Num,"num");
      infoVersion.set_attribut(version_bdd::Type,"tp");
      infoVersion.setUnique(version_bdd::Num,UniqueVersion::NumUnique);
      infoVersion.setUnique(version_bdd::Type,UniqueVersion::TypeUnique);
      m_managerVersion = std::make_unique<ManagerSql<version_bdd>>(infoVersion, std::make_unique<UniqueVersion>());

      for(auto i = managers.begin(); i != managers.end(); ++i)
          m_managers[i->first] = std::move(i->second);
}

void Managers::creerVersion() {
    m_managerVersion->creer();
    saveVersion(bmps::bddVersion::Creation,bmps::bddVersion::LibraryType);
}

bool Managers::existsVersion(idt type)
    {return m_managerVersion->exists(version_bdd::Type,type);}

entities_base::version_bdd Managers::getVersion(idt type) {
    version_bdd ver(numVersion(type),type);
    m_managerVersion->getUnique(ver);
    return ver;
}

const InfoBdd &Managers::info(entidt id) const {
    if(valide(id))
            return m_managers[id]->info();
    else
        throw std::invalid_argument(QString("Managers::info(entidt) manager d'indentifiant ")
                                    .append(QString::number(id)).append("invalide").toStdString());
}

InfoBdd Managers::infoBddarbre(const QString &table) const {
    using arbre = entities_base::arbre;
    using Uniquearbre = arbreUniqueSql<arbre>;
    InfoBdd infoarbre(table,table, arbre::Nbr_Att,{Uniquearbre::NbrUnique});
    infoarbre.set_attribut(arbre::Parent,"pt",bmps::typeAttributBdd::Integer,false);
    infoarbre.set_attribut(arbre::Feuille,"fl",bmps::typeAttributBdd::Bool,true);
    infoarbre.set_attribut(arbre::Num,"num",bmps::typeAttributBdd::Integer,true);
    infoarbre.setUnique(arbre::Parent,Uniquearbre::ParentUnique);
    infoarbre.setUnique(arbre::Num,Uniquearbre::NumUnique);
    infoarbre.setForeignKey(arbre::Parent,infoarbre);
    return infoarbre;
}

std::unique_ptr<entity> Managers::makeentity(const QString &ent) const {
    auto i = find(entity);
    if(i == m_nbrentity)
        return nullptr;
    else
        return m_managers[i]->makeentity();
}

int Managers::numVersion(numt type)
    {return m_managerVersion->fonctionAgrega(bmps::Max,version_bdd::Num,version_bdd::Type,type).toInt();}

void Managers::saveVersion(int num, numt type)
    {m_managerVersion->save(version_bdd(num,type));}

void Managers::setRequete(const QSqlQuery &req) {
    m_requete = req;
    AbstractManagerSql::setRequete(&m_requete);
}

//bmps::Table Managers::table(entidt id) const {
//    if(id < m_nbrentity) {
//        if(m_managers[id]){
//            bmps::Table tab;
//            tab.name = m_managers[id]->name();
//            tab.nameBdd = m_managers[id]->info().table();
//            tab.Names_attributs = m_managers[id]->Names_attributs();
//            tab.foreignKeys = m_managers[id]->info().foreignKey();
//            return tab;
//        }
//        else
//            return bmps::Table();
//    }
//    else
//        throw std::invalid_argument("Identifiant d'entité invalide dans la fonction get.");
//}
