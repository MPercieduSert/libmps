#include "ManagersPredef.h"

void ManagersPredef::enableCommentaire(const QString & commentaire, const QString & cibleCommentaire)
{
    //Commentaire
    InfoBdd infoCommentaire(commentaire,Commentaire::NbrAtt);
    infoCommentaire.setAttribut(Commentaire::Creation,"crea",bdd::TypeAttributBdd::DateTime);
    infoCommentaire.setAttribut(Commentaire::Modification,"modif",bdd::TypeAttributBdd::DateTime);
    infoCommentaire.setAttribut(Commentaire::Texte,"txt",bdd::TypeAttributBdd::Text,false);
    setManager(new ManagerSql<Commentaire>(infoCommentaire));
    setCible<Commentaire>(bdd::cibleId::Commentaire);

    //CibleCommentaire
    using Unique = CibleSimpleNumUniqueSql<CibleCommentaire>;
    InfoBdd infoCibleCom(cibleCommentaire,CibleCommentaire::NbrAtt,{Unique::NbrUnique});
    infoCibleCom.setAttribut(CibleCommentaire::IdCm,"idCm");
    infoCibleCom.setAttribut(CibleCommentaire::IdCible,"idCb");
    infoCibleCom.setAttribut(CibleCommentaire::Cible,"cb");
    infoCibleCom.setAttribut(CibleCommentaire::DateTime,"dt",bdd::TypeAttributBdd::DateTime);
    infoCibleCom.setAttribut(CibleCommentaire::Num,"num");
    infoCibleCom.setUnique(CibleCommentaire::IdCible,Unique::IdCibleUnique);
    infoCibleCom.setUnique(CibleCommentaire::Cible,Unique::CibleUnique);
    infoCibleCom.setUnique(CibleCommentaire::Num,Unique::NumUnique);
    infoCibleCom.setForeignKey(CibleCommentaire::IdCm,infoCommentaire);
    setManager(new ManagerSql<CibleCommentaire>(infoCibleCom,new Unique));
    setCible<CibleCommentaire>(bdd::cibleId::CibleCommentaire);

    m_commentaireEnabled = true;
}

void ManagersPredef::enableDonnee(const QString & donnee, const QString & arbreDonnee, const QString & cibleDonnee,
                                  const QString & cardDonnee,
                                  AbstractGestionAutorisation<Donnee> * gestion,
                                  AbstractGestionAutorisation<CibleDonnee> *gestionCilbeDonnee,
                                  AbstractGestionAutorisation<DonneeCard> *gestionCardDonnee)
{
    //Donnee
    auto infoArbre = infoBddArbre(arbreDonnee);
    using UniqueDonnee = IdProgUniqueSql<Donnee>;
    InfoBdd infoDonnee(donnee,Donnee::NbrAtt,{UniqueDonnee::NbrUnique});
    infoDonnee.setAttribut(Donnee::Nom,"nom",bdd::TypeAttributBdd::Text);
    infoDonnee.setAttribut(Donnee::Type,"tp");
    infoDonnee.setAttribut(Donnee::IdProg,"idPg",bdd::TypeAttributBdd::Integer,false);
    infoDonnee.setAttribut(Donnee::TpVal,"tpV");
    infoDonnee.setUnique(Donnee::IdProg,UniqueDonnee::IdProgUnique);
    infoDonnee.setForeignKey(Donnee::Id,infoArbre);
    if(gestion)
        setManager(new ManagerOfArbreModifControle<Donnee>(infoDonnee,infoArbre,gestion, new UniqueDonnee));
    else
        setManager(new ManagerOfArbre<Donnee>(infoDonnee,infoArbre, new UniqueDonnee));
    setCible<Donnee>(bdd::cibleId::Donnee);

    //CibleDonnee
    using UniqueCD = CibleNumUniqueSql<CibleDonnee>;
    InfoBdd infoCD(cibleDonnee,CibleDonnee::NbrAtt,{UniqueCD::NbrUnique});
    infoCD.setAttribut(CibleDonnee::IdDn,"idDn");
    infoCD.setAttribut(CibleDonnee::IdCible,"idCb");
    infoCD.setAttribut(CibleDonnee::Cible,"cb");
    infoCD.setAttribut(CibleDonnee::DateTime,"dt",bdd::TypeAttributBdd::DateTime);
    infoCD.setAttribut(CibleDonnee::Num,"num");
    infoCD.setAttribut(CibleDonnee::Valeur,"val",bdd::TypeAttributBdd::Blob,false);
    infoCD.setUnique(CibleDonnee::IdDn,UniqueCD::Id1Unique);
    infoCD.setUnique(CibleDonnee::IdCible,UniqueCD::IdCibleUnique);
    infoCD.setUnique(CibleDonnee::Cible,UniqueCD::CibleUnique);
    infoCD.setUnique(CibleDonnee::Num,UniqueCD::NumUnique);
    if(gestionCilbeDonnee)
        setManager(new ManagerOfModifControle<CibleDonnee>(infoCD,gestionCilbeDonnee, new UniqueCD));
    else
        setManager(new ManagerSql<CibleDonnee>(infoCD, new UniqueCD));
    setCible<CibleDonnee>(bdd::cibleId::CibleDonnee);

    //DonneeCard
    using UniqueCard = IdCibleUniqueSql<DonneeCard>;
    InfoBdd infoCard(cardDonnee,DonneeCard::NbrAtt,{UniqueCard::NbrUnique});
    infoCard.setAttribut(DonneeCard::IdDn,"idDn");
    infoCard.setAttribut(DonneeCard::Card,"cd");
    infoCard.setAttribut(DonneeCard::Exact,"ex",bdd::TypeAttributBdd::Bool);
    infoCard.setAttribut(DonneeCard::Cible,"cb");
    infoCard.setUnique(DonneeCard::IdDn,UniqueCard::Id1Unique);
    infoCard.setUnique(DonneeCard::Cible,UniqueCard::CibleUnique);
    infoCard.setForeignKey(DonneeCard::IdDn,infoDonnee);
    if(gestionCardDonnee)
        setManager(new ManagerOfModifControle<DonneeCard>(infoCard,gestionCardDonnee, new UniqueCard));
    else
        setManager(new ManagerSql<DonneeCard>(infoCard, new UniqueCard));
    setCible<DonneeCard>(bdd::cibleId::DonneeCard);

    m_donneeEnabled = true;
}

void ManagersPredef::enableHistorique(const QString & historique)
{
    using Unique = CibleSimpleNumUniqueSql<Historique>;
    InfoBdd info(historique,Historique::NbrAtt,{Unique::NbrUnique});
    info.setAttribut(Historique::IdCible,"idCb");
    info.setAttribut(Historique::Cible,"cb");
    info.setAttribut(Historique::IdEtat,"idEt");
    info.setAttribut(Historique::Etat,"et");
    info.setAttribut(Historique::Num,"num");
    info.setAttribut(Historique::DateTime,"dt",bdd::TypeAttributBdd::DateTime);
    info.setUnique(Historique::IdCible,Unique::IdCibleUnique);
    info.setUnique(Historique::Cible,Unique::CibleUnique);
    info.setUnique(Historique::Num,Unique::NumUnique);
    setManager(new ManagerSql<Historique>(info, new Unique));
    setCible<Historique>(bdd::cibleId::Historique);

    m_historiqueEnabled = true;
}

void ManagersPredef::enableMotCle(const QString & motCle, const QString & motCleArbre, const QString & cibleMotCle,
                  const QString & permissionMotCle,
                  AbstractGestionAutorisation<MotCle> * gestionMotCle,
                  AbstractGestionAutorisation<MotClePermission> * gestionPermission)
{
    //Mot Clé
    auto infoArbre = infoBddArbre(motCleArbre);
    InfoBdd infoMC(motCle,MotCle::NbrAtt);
    infoMC.setAttribut(MotCle::Nc,"nc",bdd::TypeAttributBdd::Text);
    infoMC.setAttribut(MotCle::Nom,"nm",bdd::TypeAttributBdd::Text);
    infoMC.setForeignKey(MotCle::Id,infoArbre);
    if(gestionMotCle)
        setManager(new ManagerOfArbreModifControle<MotCle>(infoMC,infoArbre,gestionMotCle));
    else
        setManager(new ManagerOfArbre<MotCle>(infoMC,infoArbre));
    setCible<MotCle>(bdd::cibleId::MotCle);

    // Cible Mot Clé
    using UniqueCible = CibleUniqueSql<CibleMotCle>;
    InfoBdd infoCible(cibleMotCle,CibleMotCle::NbrAtt,{UniqueCible::NbrUnique});
    infoCible.setAttribut(CibleMotCle::IdMC,"idMC");
    infoCible.setAttribut(CibleMotCle::IdCible,"idCb");
    infoCible.setAttribut(CibleMotCle::Cible,"cb");
    infoCible.setUnique(CibleMotCle::IdMC,UniqueCible::Id1Unique);
    infoCible.setUnique(CibleMotCle::IdCible,UniqueCible::IdCibleUnique);
    infoCible.setUnique(CibleMotCle::Cible,UniqueCible::CibleUnique);
    infoCible.setForeignKey(CibleMotCle::IdMC,infoMC);
    setManager(new ManagerSql<CibleMotCle>(infoCible, new UniqueCible));
    setCible<MotCle>(bdd::cibleId::CibleMotCle);

    // Permission
    using UniquePermission = IdCibleUniqueSql<MotClePermission>;
    InfoBdd infoPermission(permissionMotCle,MotClePermission::NbrAtt,{UniquePermission::NbrUnique});
    infoPermission.setAttribut(MotClePermission::IdMC,"idMC");
    infoPermission.setAttribut(MotClePermission::Cible,"cb");
    infoPermission.setAttribut(MotClePermission::Num,"num");
    infoPermission.setUnique(MotClePermission::IdMC,UniquePermission::Id1Unique);
    infoPermission.setUnique(MotClePermission::Cible,UniquePermission::CibleUnique);
    infoPermission.setForeignKey(MotClePermission::IdMC,infoMC);
    if(gestionPermission)
        setManager(new ManagerOfModifControlePermission<MotClePermission>(infoPermission,gestionPermission, new UniquePermission));
    else
        setManager(new ManagerOfPermission<MotClePermission>(infoPermission, new UniquePermission));
    setCible<MotClePermission>(bdd::cibleId::MotClePermission);

    m_motCleEnabled = true;
}

void ManagersPredef::enableRestriction(const QString & restriction)
{
    using Unique = CibleSimpleNumUniqueSql<Restriction>;
    InfoBdd info(restriction,Restriction::NbrAtt,{Unique::NbrUnique});
    info.setAttribut(Restriction::IdCible,"idCb");
    info.setAttribut(Restriction::Cible,"cb");
    info.setAttribut(Restriction::Num,"num");
    info.setUnique(Restriction::IdCible,Unique::IdCibleUnique);
    info.setUnique(Restriction::Cible,Unique::CibleUnique);
    info.setUnique(Restriction::Num,Unique::NumUnique);
    setManager(new ManagerSql<Restriction>(info,new Unique));
    setCible<Restriction>(bdd::cibleId::Restriction);

    m_restrictionModificationEnabled = true;
}

void ManagersPredef::enableTexte(const QString & texte, const QString & cibleTexte, const QString & source
                                 , const QString & texteSource)
{
    //Texte
    InfoBdd infoTexte(texte,Texte::NbrAtt);
    infoTexte.setAttribut(Texte::Creation,"crea",bdd::TypeAttributBdd::DateTime);
    infoTexte.setAttribut(Texte::Modification,"modif",bdd::TypeAttributBdd::DateTime);
    infoTexte.setAttribut(Texte::Texte,"txt",bdd::TypeAttributBdd::Text,false);
    setManager(new ManagerSql<Texte>(infoTexte));
    setCible<Texte>(bdd::cibleId::Texte);

    //CibleTexte
    using UniqueCible = CibleSimpleNumTypeUniqueSql<CibleTexte>;
    InfoBdd infoCible(cibleTexte,CibleTexte::NbrAtt,{UniqueCible::NbrUnique});
    infoCible.setAttribut(CibleTexte::IdTxt,"IdTxt");
    infoCible.setAttribut(CibleTexte::IdCible,"IdCb");
    infoCible.setAttribut(CibleTexte::Cible,"cb");
    infoCible.setAttribut(CibleTexte::Num,"num");
    infoCible.setAttribut(CibleTexte::Type,"tp");
    infoCible.setAttribut(CibleTexte::DateTime,"dt",bdd::TypeAttributBdd::DateTime);
    infoCible.setUnique(CibleTexte::IdCible,UniqueCible::IdCibleUnique);
    infoCible.setUnique(CibleTexte::Cible,UniqueCible::CibleUnique);
    infoCible.setUnique(CibleTexte::Num,UniqueCible::NumUnique);
    infoCible.setUnique(CibleTexte::Type,UniqueCible::TypeUnique);
    infoCible.setForeignKey(CibleTexte::IdTxt,infoTexte);
    setManager(new ManagerSql<CibleTexte>(infoCible, new UniqueCible));
    setCible<CibleTexte>(bdd::cibleId::CibleTexte);

    //Source
    using UniqueSource = NomUniqueSql<Source>;
    InfoBdd infoSource(source,Source::NbrAtt,{UniqueSource::NbrUnique});
    infoSource.setAttribut(Source::Nc,"nc",bdd::TypeAttributBdd::Text);
    infoSource.setAttribut(Source::Nom,"nm",bdd::TypeAttributBdd::Text);
    infoSource.setAttribut(Source::Type,"tp");
    infoSource.setUnique(Source::Nom,UniqueSource::NomUnique);
    setManager(new ManagerSql<Source>(infoSource, new UniqueSource));
    setCible<Source>(bdd::cibleId::Source);

    //SourceTexte
    using UniqueTS = RelationUniqueSql<TexteSource>;
    InfoBdd infoTS(texteSource,TexteSource::NbrAtt,{UniqueTS::NbrUnique});
    infoTS.setAttribut(TexteSource::IdSr,"idSr");
    infoTS.setAttribut(TexteSource::IdTxt,"idTxt");
    infoTS.setUnique(TexteSource::IdSr,UniqueTS::Id1Unique);
    infoTS.setUnique(TexteSource::IdTxt,UniqueTS::Id2Unique);
    infoTS.setForeignKey(TexteSource::IdSr,infoSource);
    infoTS.setForeignKey(TexteSource::IdTxt,infoTexte);
    setManager(new ManagerSql<TexteSource>(infoTS, new UniqueTS));
    setCible<TexteSource>(bdd::cibleId::TexteSource);

    m_texteEnabled = true;
}

void  ManagersPredef::enableType(const QString & typeEnt, const QString & permissionType,
                AbstractGestionAutorisation<Type> * gestionType,
                AbstractGestionAutorisation<TypePermission> * gestionPermission)
{
    //Type
    using UniqueType = IdProgNomParentUniqueSql<Type>;
    InfoBdd infoType(typeEnt,Type::NbrAtt,{UniqueType::NbrUnique_1,UniqueType::NbrUnique_2});
    infoType.setAttribut(Type::IdProg,"idPg",bdd::TypeAttributBdd::Integer,false);
    infoType.setAttribut(Type::Parent,"pt",bdd::TypeAttributBdd::Integer,false);
    infoType.setAttribut(Type::Nc,"nc",bdd::TypeAttributBdd::Text);
    infoType.setAttribut(Type::Nom,"nm",bdd::TypeAttributBdd::Text);
    infoType.setUnique(Type::IdProg,UniqueType::IdProgUnique,UniqueType::IdProgUniqueSet);
    infoType.setUnique(Type::Nom,UniqueType::NomUnique,UniqueType::NomUniqueSet);
    infoType.setUnique(Type::Parent,UniqueType::ParentUnique,UniqueType::ParentUniqueSet);
    infoType.setForeignKey(Type::Parent,infoType);
    if(gestionType)
        setManager(new ManagerOfArbreSimpleModifControle<Type>(infoType,gestionType,new UniqueType));
    else
        setManager(new ManagerOfArbreSimple<Type>(infoType,new UniqueType));
    setCible<Type>(bdd::cibleId::Type);

    //Permission
    using UniquePermission = IdCibleUniqueSql<TypePermission>;
    InfoBdd infoPermission(permissionType,TypePermission::NbrAtt,{UniquePermission::NbrUnique});
    infoPermission.setAttribut(TypePermission::IdTp,"idTp");
    infoPermission.setAttribut(TypePermission::Cible,"cb");
    infoPermission.setAttribut(TypePermission::Num,"num");
    infoPermission.setUnique(TypePermission::IdTp,UniquePermission::Id1Unique);
    infoPermission.setUnique(TypePermission::Cible,UniquePermission::CibleUnique);
    infoPermission.setForeignKey(TypePermission::IdTp,infoType);
    if(gestionPermission)
        setManager(new ManagerOfModifControlePermission<TypePermission>(infoPermission,gestionPermission, new UniquePermission));
    else
        setManager(new ManagerOfPermission<TypePermission>(infoPermission, new UniquePermission));
    setCible<TypePermission>(bdd::cibleId::TypePermission);

    m_typeEnabled = true;
}

void ManagersPredef::enableUtilisation(const QString &utilisation, const QString &usage, const QString & usageArbre,
                                       AbstractGestionAutorisation<Usage> * gestionUsage)
{
    //Usage
    auto infoArbre = infoBddArbre(usageArbre);
    using UniqueN = NomTypeUniqueSql<Usage>;
    InfoBdd infoN(usage,Usage::NbrAtt,{UniqueN::NbrUnique});
    infoN.setAttribut(Usage::Nc,"nc",bdd::TypeAttributBdd::Text);
    infoN.setAttribut(Usage::Nom,"nm",bdd::TypeAttributBdd::Text);
    infoN.setAttribut(Usage::Type,"tp");
    infoN.setUnique(Usage::Nom,UniqueN::NomUnique);
    infoN.setUnique(Usage::Type,UniqueN::TypeUnique);
    if(gestionUsage)
        setManager(new ManagerOfArbreModifControle<Usage>(infoN, infoArbre, gestionUsage, new UniqueN));
    else
        setManager(new ManagerOfArbre<Usage>(infoN,infoArbre, new UniqueN));
    setCible<Usage>(bdd::cibleId::Usage);

    //Utilisation
    using UniqueU = HistoriqueUniqueSql<Utilisation>;
    InfoBdd infoU(utilisation,Utilisation::NbrAtt,{UniqueU::NbrUnique});
    infoU.setAttribut(Utilisation::IdUs,"idUs",bdd::TypeAttributBdd::Integer,false);
    infoU.setAttribut(Utilisation::IdCible,"idCb");
    infoU.setAttribut(Utilisation::Cible,"cb");
    infoU.setAttribut(Utilisation::IdEtat,"idEt");
    infoU.setAttribut(Utilisation::Etat,"et");
    infoU.setAttribut(Utilisation::Num,"num");
    infoU.setAttribut(Utilisation::DateTime,"dt",bdd::TypeAttributBdd::DateTime);
    infoU.setUnique(Utilisation::IdCible,UniqueU::IdCibleUnique);
    infoU.setUnique(Utilisation::Cible,UniqueU::CibleUnique);
    infoU.setUnique(Utilisation::IdEtat,UniqueU::IdEtatUnique);
    infoU.setUnique(Utilisation::Etat,UniqueU::EtatUnique);
    infoU.setUnique(Utilisation::Num,UniqueU::NumUnique);
    infoU.setForeignKey(Utilisation::IdUs,infoN);
    setManager(new ManagerSql<Utilisation>(infoU, new UniqueU));
    setCible<Utilisation>(bdd::cibleId::Utilisation);

    m_utilisationEnabled = true;
}
