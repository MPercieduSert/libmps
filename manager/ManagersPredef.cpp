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

    //CommentaireCible
    using Unique = CibleSimpleNumUniqueSql<CommentaireCible>;
    InfoBdd infoCibleCom(cibleCommentaire,CommentaireCible::NbrAtt,{Unique::NbrUnique});
    infoCibleCom.setAttribut(CommentaireCible::IdCommentaire,"idCm");
    infoCibleCom.setAttribut(CommentaireCible::IdCible,"idCb");
    infoCibleCom.setAttribut(CommentaireCible::Cible,"cb");
    infoCibleCom.setAttribut(CommentaireCible::DateTime,"dt",bdd::TypeAttributBdd::DateTime);
    infoCibleCom.setAttribut(CommentaireCible::Num,"num");
    infoCibleCom.setUnique(CommentaireCible::IdCible,Unique::IdCibleUnique);
    infoCibleCom.setUnique(CommentaireCible::Cible,Unique::CibleUnique);
    infoCibleCom.setUnique(CommentaireCible::Num,Unique::NumUnique);
    infoCibleCom.setForeignKey(CommentaireCible::IdCommentaire,infoCommentaire);
    setManager(new ManagerSql<CommentaireCible>(infoCibleCom,new Unique));
    setCible<CommentaireCible>(bdd::cibleId::CommentaireCible);

    m_commentaireEnabled = true;
}

void ManagersPredef::enableDonnee(const QString & donnee, const QString & arbreDonnee, const QString & cibleDonnee,
                                  const QString & cardDonnee,
                                  AbstractGestionAutorisation<Donnee> * gestion,
                                  AbstractGestionAutorisation<DonneeCible> *gestionCilbeDonnee,
                                  AbstractGestionAutorisation<DonneeCard> *gestionCardDonnee)
{
    //Donnee
    auto infoArbre = infoBddArbre(arbreDonnee);
    using UniqueDonnee = IdProgUniqueSql<Donnee>;
    InfoBdd infoDonnee(donnee,Donnee::NbrAtt,{UniqueDonnee::NbrUnique});
    infoDonnee.setAttribut(Donnee::Nom,"nom",bdd::TypeAttributBdd::Text);
    infoDonnee.setAttribut(Donnee::Type,"tp");
    infoDonnee.setAttribut(Donnee::TpVal,"tpV");
    infoDonnee.setAttribut(Donnee::IdProg,"idPg",bdd::TypeAttributBdd::Integer,false);
    infoDonnee.setUnique(Donnee::IdProg,UniqueDonnee::IdProgUnique);
    infoDonnee.setForeignKey(Donnee::Id,infoArbre);
    setTypeForeignKey<Donnee>(infoDonnee);
    if(gestion)
        setManager(new ManagerArbreModifControle<Donnee>(infoDonnee,infoArbre,gestion, new UniqueDonnee));
    else
        setManager(new ManagerArbre<Donnee>(infoDonnee,infoArbre, new UniqueDonnee));
    setCible<Donnee>(bdd::cibleId::Donnee);

    //DonneeCible
    using UniqueCD = CibleNumUniqueSql<DonneeCible>;
    InfoBdd infoCD(cibleDonnee,DonneeCible::NbrAtt,{UniqueCD::NbrUnique});
    infoCD.setAttribut(DonneeCible::IdDonnee,"idDn");
    infoCD.setAttribut(DonneeCible::IdCible,"idCb");
    infoCD.setAttribut(DonneeCible::Cible,"cb");
    infoCD.setAttribut(DonneeCible::DateTime,"dt",bdd::TypeAttributBdd::DateTime);
    infoCD.setAttribut(DonneeCible::Num,"num");
    infoCD.setAttribut(DonneeCible::Valeur,"val",bdd::TypeAttributBdd::Blob,false);
    infoCD.setUnique(DonneeCible::IdDonnee,UniqueCD::Id1Unique);
    infoCD.setUnique(DonneeCible::IdCible,UniqueCD::IdCibleUnique);
    infoCD.setUnique(DonneeCible::Cible,UniqueCD::CibleUnique);
    infoCD.setUnique(DonneeCible::Num,UniqueCD::NumUnique);
    if(gestionCilbeDonnee)
        setManager(new ManagerOfModifControle<DonneeCible>(infoCD,gestionCilbeDonnee, new UniqueCD));
    else
        setManager(new ManagerSql<DonneeCible>(infoCD, new UniqueCD));
    setCible<DonneeCible>(bdd::cibleId::DonneeCible);

    //DonneeCard
    using UniqueCard = IdCibleUniqueSql<DonneeCard>;
    InfoBdd infoCard(cardDonnee,DonneeCard::NbrAtt,{UniqueCard::NbrUnique});
    infoCard.setAttribut(DonneeCard::IdDonnee,"idDn");
    infoCard.setAttribut(DonneeCard::Card,"cd");
    infoCard.setAttribut(DonneeCard::Cible,"cb");
    infoCard.setAttribut(DonneeCard::Exact,"ex");
    infoCard.setUnique(DonneeCard::IdDonnee,UniqueCard::Id1Unique);
    infoCard.setUnique(DonneeCard::Cible,UniqueCard::CibleUnique);
    infoCard.setForeignKey(DonneeCard::IdDonnee,infoDonnee);
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
    info.setAttribut(Historique::IdEtat,"idEt");
    info.setAttribut(Historique::Cible,"cb");
    info.setAttribut(Historique::DateTime,"dt",bdd::TypeAttributBdd::DateTime);
    info.setAttribut(Historique::Etat,"et");
    info.setAttribut(Historique::Num,"num");
    info.setUnique(Historique::IdCible,Unique::IdCibleUnique);
    info.setUnique(Historique::Cible,Unique::CibleUnique);
    info.setUnique(Historique::Num,Unique::NumUnique);
    setManager(new ManagerSql<Historique>(info, new Unique));
    setCible<Historique>(bdd::cibleId::Historique);

    m_historiqueEnabled = true;
}

void ManagersPredef::enableMotCle(const QString & motCle, const QString & motCleArbre, const QString & cibleMotCle,
                                  const QString & cibleProg, const QString & permissionMotCle, const QString & permissionProg,
                                  AbstractGestionAutorisation<MotCle> * gestionMotCle,
                                  AbstractGestionAutorisation<MotClePermission> * gestionPermission,
                                  AbstractGestionAutorisation<MotProgPermission> * gestionProgPermission)
{
    //Mot Clé
    auto infoArbre = infoBddArbre(motCleArbre);
    InfoBdd infoMC(motCle,MotCle::NbrAtt);
    infoMC.setAttribut(MotCle::Nc,"nc",bdd::TypeAttributBdd::Text);
    infoMC.setAttribut(MotCle::Nom,"nm",bdd::TypeAttributBdd::Text);
    infoMC.setForeignKey(MotCle::Id,infoArbre);
    if(gestionMotCle)
        setManager(new ManagerArbreModifControle<MotCle>(infoMC,infoArbre,gestionMotCle));
    else
        setManager(new ManagerArbre<MotCle>(infoMC,infoArbre));
    setCible<MotCle>(bdd::cibleId::MotCle);

    // Cible Mot Clé
    using UniqueCible = CibleUniqueSql<MotCleCible>;
    InfoBdd infoCible(cibleMotCle,MotCleCible::NbrAtt,{UniqueCible::NbrUnique});
    infoCible.setAttribut(MotCleCible::IdMotCle,"idMC");
    infoCible.setAttribut(MotCleCible::IdCible,"idCb");
    infoCible.setAttribut(MotCleCible::Cible,"cb");
    infoCible.setUnique(MotCleCible::IdMotCle,UniqueCible::Id1Unique);
    infoCible.setUnique(MotCleCible::IdCible,UniqueCible::IdCibleUnique);
    infoCible.setUnique(MotCleCible::Cible,UniqueCible::CibleUnique);
    infoCible.setForeignKey(MotCleCible::IdMotCle,infoMC);
    setManager(new ManagerSql<MotCleCible>(infoCible, new UniqueCible));
    setCible<MotCleCible>(bdd::cibleId::MotCleCible);

    // Permission Mot Clé
    using UniquePermission = IdCibleUniqueSql<MotClePermission>;
    InfoBdd infoPermission(permissionMotCle,MotClePermission::NbrAtt,{UniquePermission::NbrUnique});
    infoPermission.setAttribut(MotClePermission::IdMotCle,"idMC");
    infoPermission.setAttribut(MotClePermission::Cible,"cb");
    infoPermission.setAttribut(MotClePermission::Code,"cd");
    infoPermission.setUnique(MotClePermission::IdMotCle,UniquePermission::Id1Unique);
    infoPermission.setUnique(MotClePermission::Cible,UniquePermission::CibleUnique);
    infoPermission.setForeignKey(MotClePermission::IdMotCle,infoMC);
    if(gestionPermission)
        setManager(new ManagerOfPermissionCodeModifControle<MotClePermission>(infoPermission,gestionPermission, new UniquePermission));
    else
        setManager(new ManagerOfPermissionCode<MotClePermission>(infoPermission, new UniquePermission));
    setCible<MotClePermission>(bdd::cibleId::MotClePermission);

    // Cible Mot Prog
    using UniqueProgCible = CibleUniqueSql<MotProgCible>;
    InfoBdd infoProgCible(cibleProg,MotProgCible::NbrAtt,{UniqueProgCible::NbrUnique});
    infoProgCible.setAttribut(MotProgCible::IdProg,"idPg");
    infoProgCible.setAttribut(MotProgCible::IdMotCle,"idMC");
    infoProgCible.setAttribut(MotProgCible::IdCible,"idCb");
    infoProgCible.setAttribut(MotProgCible::Cible,"cb");
    infoProgCible.setUnique(MotProgCible::IdProg,UniqueProgCible::Id1Unique);
    infoProgCible.setUnique(MotProgCible::IdCible,UniqueProgCible::IdCibleUnique);
    infoProgCible.setUnique(MotProgCible::Cible,UniqueProgCible::CibleUnique);
    infoProgCible.setForeignKey(MotProgCible::IdMotCle,infoMC);
    setManager(new ManagerSql<MotProgCible>(infoProgCible, new UniqueProgCible));
    setCible<MotProgCible>(bdd::cibleId::MotProgCible);

    // Permission Mot Prog
    using UniqueProgPermission = RelationCibleUniqueSql<MotProgPermission>;
    InfoBdd infoProgPermission(permissionProg,MotProgPermission::NbrAtt,{UniqueProgPermission::NbrUnique});
    infoProgPermission.setAttribut(MotProgPermission::IdProg,"idPg");
    infoProgPermission.setAttribut(MotProgPermission::IdMotCle,"idMC");
    infoProgPermission.setAttribut(MotProgPermission::Cible,"cb");
    infoProgPermission.setAttribut(MotProgPermission::Code,"cd");
    infoProgPermission.setUnique(MotProgPermission::IdProg,UniqueProgPermission::Id1Unique);
    infoProgPermission.setUnique(MotProgPermission::IdMotCle,UniqueProgPermission::Id2Unique);
    infoProgPermission.setUnique(MotProgPermission::Cible,UniqueProgPermission::CibleUnique);
    infoProgPermission.setForeignKey(MotProgPermission::IdMotCle,infoMC);
    if(gestionProgPermission)
        setManager(new ManagerOfPermissionCodeModifControle<MotProgPermission>(infoProgPermission,gestionProgPermission,
                                                                               new UniqueProgPermission));
    else
        setManager(new ManagerOfPermissionCode<MotProgPermission>(infoProgPermission, new UniqueProgPermission));
    setCible<MotProgPermission>(bdd::cibleId::MotProgPermission);

    m_motCleEnabled = true;
}

void ManagersPredef::enableRestriction(const QString & restriction)
{
    using Unique = CibleSimpleUniqueSql<Restriction>;
    InfoBdd info(restriction,Restriction::NbrAtt,{Unique::NbrUnique});
    info.setAttribut(Restriction::IdCible,"idCb");
    info.setAttribut(Restriction::Cible,"cb");
    info.setAttribut(Restriction::Code,"cd");
    info.setUnique(Restriction::IdCible,Unique::IdCibleUnique);
    info.setUnique(Restriction::Cible,Unique::CibleUnique);
    setManager(new ManagerOfPermissionCode<Restriction>(info,new Unique));
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

    //TexteCible
    using UniqueCible = CibleSimpleNumTypeUniqueSql<TexteCible>;
    InfoBdd infoCible(cibleTexte,TexteCible::NbrAtt,{UniqueCible::NbrUnique});
    infoCible.setAttribut(TexteCible::IdTexte,"IdTxt");
    infoCible.setAttribut(TexteCible::IdCible,"IdCb");
    infoCible.setAttribut(TexteCible::Cible,"cb");
    infoCible.setAttribut(TexteCible::DateTime,"dt",bdd::TypeAttributBdd::DateTime);
    infoCible.setAttribut(TexteCible::Num,"num");
    infoCible.setAttribut(TexteCible::Type,"tp");
    infoCible.setUnique(TexteCible::IdCible,UniqueCible::IdCibleUnique);
    infoCible.setUnique(TexteCible::Cible,UniqueCible::CibleUnique);
    infoCible.setUnique(TexteCible::Num,UniqueCible::NumUnique);
    infoCible.setUnique(TexteCible::Type,UniqueCible::TypeUnique);
    infoCible.setForeignKey(TexteCible::IdTexte,infoTexte);
    setTypeForeignKey<TexteCible>(infoCible);
    setManager(new ManagerSql<TexteCible>(infoCible, new UniqueCible));
    setCible<TexteCible>(bdd::cibleId::TexteCible);

    //Source
    using UniqueSource = NomUniqueSql<Source>;
    InfoBdd infoSource(source,Source::NbrAtt,{UniqueSource::NbrUnique});
    infoSource.setAttribut(Source::Nc,"nc",bdd::TypeAttributBdd::Text);
    infoSource.setAttribut(Source::Nom,"nm",bdd::TypeAttributBdd::Text);
    infoSource.setAttribut(Source::Type,"tp");
    infoSource.setUnique(Source::Nom,UniqueSource::NomUnique);
    setTypeForeignKey<Source>(infoSource);
    setManager(new ManagerSql<Source>(infoSource, new UniqueSource));
    setCible<Source>(bdd::cibleId::Source);

    //SourceTexte
    using UniqueTS = RelationUniqueSql<SourceTexte>;
    InfoBdd infoTS(texteSource,SourceTexte::NbrAtt,{UniqueTS::NbrUnique});
    infoTS.setAttribut(SourceTexte::IdSource,"idSr");
    infoTS.setAttribut(SourceTexte::IdTexte,"idTxt");
    infoTS.setUnique(SourceTexte::IdSource,UniqueTS::Id1Unique);
    infoTS.setUnique(SourceTexte::IdTexte,UniqueTS::Id2Unique);
    infoTS.setForeignKey(SourceTexte::IdSource,infoSource);
    infoTS.setForeignKey(SourceTexte::IdTexte,infoTexte);
    setManager(new ManagerSql<SourceTexte>(infoTS, new UniqueTS));
    setCible<SourceTexte>(bdd::cibleId::SourceTexte);

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
        setManager(new ManagerArbreSimpleModifControle<Type>(infoType,gestionType,new UniqueType));
    else
        setManager(new ManagerArbreSimple<Type>(infoType,new UniqueType));
    setCible<Type>(bdd::cibleId::Type);

    //Permission
    using UniquePermission = IdCibleUniqueSql<TypePermission>;
    InfoBdd infoPermission(permissionType,TypePermission::NbrAtt,{UniquePermission::NbrUnique});
    infoPermission.setAttribut(TypePermission::IdType,"idTp");
    infoPermission.setAttribut(TypePermission::Cible,"cb");
    infoPermission.setAttribut(TypePermission::Code,"cd");
    infoPermission.setUnique(TypePermission::IdType,UniquePermission::Id1Unique);
    infoPermission.setUnique(TypePermission::Cible,UniquePermission::CibleUnique);
    infoPermission.setForeignKey(TypePermission::IdType,infoType);
    if(gestionPermission)
        setManager(new ManagerOfPermissionCodeModifControle<TypePermission>(infoPermission,gestionPermission, new UniquePermission));
    else
        setManager(new ManagerOfPermissionCode<TypePermission>(infoPermission, new UniquePermission));
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
    infoN.setForeignKey(Usage::Id,infoArbre);
    setTypeForeignKey<Usage>(infoN);
    if(gestionUsage)
        setManager(new ManagerArbreModifControle<Usage>(infoN, infoArbre, gestionUsage, new UniqueN));
    else
        setManager(new ManagerArbre<Usage>(infoN,infoArbre, new UniqueN));
    setCible<Usage>(bdd::cibleId::Usage);

    //Utilisation
    using UniqueU = CibleSimpleNumUniqueSql<Utilisation>;
    InfoBdd infoU(utilisation,Utilisation::NbrAtt,{UniqueU::NbrUnique});
    infoU.setAttribut(Utilisation::IdUsage,"idUs",bdd::TypeAttributBdd::Integer);
    infoU.setAttribut(Utilisation::IdCible,"idCb");
    infoU.setAttribut(Utilisation::IdEtat,"idEt");
    infoU.setAttribut(Utilisation::Cible,"cb");
    infoU.setAttribut(Utilisation::DateTime,"dt",bdd::TypeAttributBdd::DateTime);
    infoU.setAttribut(Utilisation::Etat,"et");
    infoU.setAttribut(Utilisation::Num,"num");
    infoU.setUnique(Utilisation::IdCible,UniqueU::IdCibleUnique);
    infoU.setUnique(Utilisation::Cible,UniqueU::CibleUnique);
    infoU.setUnique(Utilisation::Num,UniqueU::NumUnique);
    infoU.setForeignKey(Utilisation::IdUsage,infoN);
    setManager(new ManagerSql<Utilisation>(infoU, new UniqueU));
    setCible<Utilisation>(bdd::cibleId::Utilisation);

    m_utilisationEnabled = true;
}
