#include "Managers.h"

using namespace managerMPS;

Managers::Managers(const szt nbrEntity, const QString & versionTable,
                   const std::map<szt,AbstractManager *> & managers,
                   const QSqlQuery & req)
      : m_nbrEntity(nbrEntity),
        m_requete(req),
        m_managers(nbrEntity,nullptr) {
      using UniqueVersion = NumUniqueSql<VersionBdd>;
      InfoBdd infoVersion(versionTable,VersionBdd::NbrAtt,{UniqueVersion::NbrUnique});
      infoVersion.setAttribut(VersionBdd::Num,"num");
      infoVersion.setAttribut(VersionBdd::DateTime,"dt",bmps::typeAttributBdd::DateTime);
      infoVersion.setUnique(VersionBdd::Num,UniqueVersion::NumUnique);
      m_managerVersion = new ManagerSql<VersionBdd>(infoVersion, new UniqueVersion());

      for(auto i = managers.cbegin(); i != managers.cend(); ++i)
          m_managers[i->first]=i->second;
}

void Managers::creerVersion() {
    m_managerVersion->creer();
    saveVersion(bmps::bddVersion::initiale);
}

entityBaseMPS::VersionBdd Managers::getVersion() {
    int max = m_managerVersion->fonctionAgrega(bmps::Max,VersionBdd::Num).toInt();
    VersionBdd ver(QDateTime(),max);
    m_managerVersion->getUnique(ver);
    return ver;
}

InfoBdd Managers::infoBddArbre(const QString & table) const {
    using Arbre = entityBaseMPS::Arbre;
    using UniqueArbre = ArbreUniqueSql<Arbre>;
    InfoBdd infoArbre(table, Arbre::NbrAtt,{UniqueArbre::NbrUnique});
    infoArbre.setAttribut(Arbre::Parent,"pt",bmps::typeAttributBdd::Integer,false);
    infoArbre.setAttribut(Arbre::Feuille,"fl",bmps::typeAttributBdd::Bool,true);
    infoArbre.setAttribut(Arbre::Num,"num",bmps::typeAttributBdd::Integer,true);
    infoArbre.setUnique(Arbre::Parent,UniqueArbre::ParentUnique);
    infoArbre.setUnique(Arbre::Num,UniqueArbre::NumUnique);
    infoArbre.setForeignKey(Arbre::Parent,infoArbre);
    return infoArbre;
}

int Managers::numVersion()
    {return m_managerVersion->fonctionAgrega(bmps::Max,VersionBdd::Num).toInt();}

void Managers::saveVersion(int num)
    {m_managerVersion->save(VersionBdd(QDateTime::currentDateTime(), num));}

void Managers::setRequete(const QSqlQuery & req) {
    m_requete = req;
    AbstractManagerSql::setRequete(&m_requete);
}
