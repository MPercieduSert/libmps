#include "Managers.h"

using namespace managerMPS;

Managers::Managers(const szt nbrEntity, const QString & versionTable,
                   std::map<szt,std::unique_ptr<AbstractManager>> && managers,
                   const QSqlQuery & req)
      : m_nbrEntity(nbrEntity),
        m_requete(req),
        m_managers(nbrEntity) {
      using UniqueVersion = NumTypeUniqueSql<VersionBdd>;
      InfoBdd infoVersion("Version",versionTable,VersionBdd::NbrAtt,{UniqueVersion::NbrUnique});
      infoVersion.setAttribut(VersionBdd::DateTime,"dt",bmps::typeAttributBdd::DateTime);
      infoVersion.setAttribut(VersionBdd::Num,"num");
      infoVersion.setAttribut(VersionBdd::Type,"tp");
      infoVersion.setUnique(VersionBdd::Num,UniqueVersion::NumUnique);
      infoVersion.setUnique(VersionBdd::Type,UniqueVersion::TypeUnique);
      m_managerVersion = std::make_unique<ManagerSql<VersionBdd>>(infoVersion, std::make_unique<UniqueVersion>());

      for(auto i = managers.begin(); i != managers.end(); ++i)
          m_managers[i->first] = std::move(i->second);
}

void Managers::creerVersion() {
    m_managerVersion->creer();
    saveVersion(bmps::bddVersion::Creation,bmps::bddVersion::LibraryType);
}

bool Managers::existsVersion(idt type)
    {return m_managerVersion->exists(VersionBdd::Type,type);}

entityBaseMPS::VersionBdd Managers::getVersion(idt type) {
    VersionBdd ver(numVersion(type),type);
    m_managerVersion->getUnique(ver);
    return ver;
}

const InfoBdd & Managers::info(szt id) const {
    if(valide(id))
            return m_managers[id]->info();
    else
        throw std::invalid_argument(QString("Managers::info(szt) manager d'indentifiant ")
                                    .append(QString::number(id)).append("invalide").toStdString());
}

InfoBdd Managers::infoBddArbre(const QString & table) const {
    using Arbre = entityBaseMPS::Arbre;
    using UniqueArbre = ArbreUniqueSql<Arbre>;
    InfoBdd infoArbre(table,table, Arbre::NbrAtt,{UniqueArbre::NbrUnique});
    infoArbre.setAttribut(Arbre::Parent,"pt",bmps::typeAttributBdd::Integer,false);
    infoArbre.setAttribut(Arbre::Feuille,"fl",bmps::typeAttributBdd::Bool,true);
    infoArbre.setAttribut(Arbre::Num,"num",bmps::typeAttributBdd::Integer,true);
    infoArbre.setUnique(Arbre::Parent,UniqueArbre::ParentUnique);
    infoArbre.setUnique(Arbre::Num,UniqueArbre::NumUnique);
    infoArbre.setForeignKey(Arbre::Parent,infoArbre);
    return infoArbre;
}

std::unique_ptr<Entity> Managers::makeEntity(const QString & entity) const {
    auto i = find(entity);
    if(i == m_nbrEntity)
        return nullptr;
    else
        return m_managers[i]->makeEntity();
}

int Managers::numVersion(idt type)
    {return m_managerVersion->fonctionAgrega(bmps::Max,VersionBdd::Num,VersionBdd::Type,type).toInt();}

void Managers::saveVersion(int num, idt type)
    {m_managerVersion->save(VersionBdd(num,type));}

void Managers::setRequete(const QSqlQuery & req) {
    m_requete = req;
    AbstractManagerSql::setRequete(&m_requete);
}

//bmps::Table Managers::table(szt id) const {
//    if(id < m_nbrEntity) {
//        if(m_managers[id]){
//            bmps::Table tab;
//            tab.name = m_managers[id]->name();
//            tab.nameBdd = m_managers[id]->info().table();
//            tab.namesAttributs = m_managers[id]->namesAttributs();
//            tab.foreignKeys = m_managers[id]->info().foreignKey();
//            return tab;
//        }
//        else
//            return bmps::Table();
//    }
//    else
//        throw std::invalid_argument("Identifiant d'entité invalide dans la fonction get.");
//}
