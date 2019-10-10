#include "Managers.h"

Managers::Managers(const int nbrEntity, const QString & versionTable,
                   const QMap<int,AbstractManager *> & managers,
                   const QSqlQuery & req)
      : m_nbrEntity(nbrEntity),
        m_requete(req),
        m_managers(nbrEntity,nullptr)
{
      using UniqueVersion = NumUniqueSql<VersionBdd>;
      InfoBdd infoVersion(versionTable,VersionBdd::NbrAtt,{UniqueVersion::NbrUnique});
      infoVersion.setAttribut(VersionBdd::Num,"num");
      infoVersion.setAttribut(VersionBdd::DateTime,"dt",bdd::TypeAttributBdd::DateTime);
      infoVersion.setUnique(VersionBdd::Num,UniqueVersion::NumUnique);
      m_managerVersion = new ManagerSql<VersionBdd>(infoVersion, new UniqueVersion());

      for(QMap<int, AbstractManager *>::const_iterator i = managers.cbegin(); i != managers.cend(); ++i)
          m_managers[i.key()]=i.value();
}
void Managers::creerVersion()
{
    m_managerVersion->creer();
    saveVersion(bdd::bddVersion::initiale);
}

VersionBdd Managers::getVersion()
{
    int max = m_managerVersion->fonctionAgrega(bdd::Max,VersionBdd::Num).toInt();
    VersionBdd ver(QDateTime(),max);
    m_managerVersion->getUnique(ver);
    return ver;
}

InfoBdd Managers::infoBddArbre(const QString & table) const
{
    using UniqueArbre = ArbreUniqueSql<Arbre>;
    InfoBdd infoArbre(table, Arbre::NbrAtt,{UniqueArbre::NbrUnique});
    infoArbre.setAttribut(Arbre::Parent,"pt",bdd::TypeAttributBdd::Integer,false);
    infoArbre.setAttribut(Arbre::Feuille,"fl",bdd::TypeAttributBdd::Bool,true);
    infoArbre.setAttribut(Arbre::Num,"num",bdd::TypeAttributBdd::Integer,true);
    infoArbre.setUnique(Arbre::Parent,UniqueArbre::ParentUnique);
    infoArbre.setUnique(Arbre::Num,UniqueArbre::NumUnique);
    infoArbre.setForeignKey(Arbre::Parent,infoArbre);
    return infoArbre;
}

int Managers::numVersion()
    {return m_managerVersion->fonctionAgrega(bdd::Max,VersionBdd::Num).toInt();}

void Managers::saveVersion(int num)
    {m_managerVersion->save(VersionBdd(QDateTime::currentDateTime(), num));}

void Managers::setRequete(const QSqlQuery & req)
{
    m_requete = req;
    AbstractManagerSql::setRequete(&m_requete);
}
