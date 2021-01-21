#include "ManagersPredef.h"

using namespace  managerMPS;
using namespace  entityMPS;

void ManagersPredef::enableCommentaire(const QString & commentaire, const QString & cibleCommentaire) {
    //Commentaire
    InfoBdd infoCommentaire("Commentaire",commentaire,Commentaire::NbrAtt);
    infoCommentaire.setAttribut(Commentaire::Creation,"crea",bmps::typeAttributBdd::DateTime);
    infoCommentaire.setAttribut(Commentaire::Modification,"modif",bmps::typeAttributBdd::DateTime);
    infoCommentaire.setAttribut(Commentaire::Texte,"txt",bmps::typeAttributBdd::Text,false);
    setManager<Commentaire>(std::make_unique<ManagerSql<Commentaire>>(infoCommentaire));
    setCible<Commentaire>(bmps::cibleId::Commentaire);

    //CommentaireCible
    using Unique = CibleSimpleNumUniqueSql<CommentaireCible>;
    InfoBdd infoCibleCom(CommentaireCible::Name(),cibleCommentaire,CommentaireCible::NbrAtt,{Unique::NbrUnique});
    infoCibleCom.setAttribut(CommentaireCible::IdCommentaire,"idCm");
    infoCibleCom.setAttribut(CommentaireCible::IdCible,"idCb");
    infoCibleCom.setAttribut(CommentaireCible::Cible,"cb");
    infoCibleCom.setAttribut(CommentaireCible::DateTime,"dt",bmps::typeAttributBdd::DateTime);
    infoCibleCom.setAttribut(CommentaireCible::Num,"num");
    infoCibleCom.setUnique(CommentaireCible::IdCible,Unique::IdCibleUnique);
    infoCibleCom.setUnique(CommentaireCible::Cible,Unique::CibleUnique);
    infoCibleCom.setUnique(CommentaireCible::Num,Unique::NumUnique);
    infoCibleCom.setForeignKey(CommentaireCible::IdCommentaire,infoCommentaire);
    infoCibleCom.setForeignKey(CommentaireCible::IdCible,InfoBdd());
    setManager<CommentaireCible>(std::make_unique<ManagerSql<CommentaireCible>>(infoCibleCom,std::make_unique<Unique>()));
    setCible<CommentaireCible>(bmps::cibleId::CommentaireCible);

    setEnsembleEnable(bmps::CommentaireEnable);
}

void ManagersPredef::enableDonnee(const QString & donnee, const QString & arbreDonnee, const QString & cibleDonnee,
                                  const QString & cardDonnee,
                                  std::unique_ptr<AbstractGestionRestriction<Donnee>> &&  gestion,
                                  std::unique_ptr<AbstractGestionRestriction<DonneeCible>> && gestionCilbeDonnee,
                                  std::unique_ptr<AbstractGestionRestriction<DonneeCard>> && gestionCardDonnee) {
    //Donnee
    auto infoArbre = infoBddArbre(arbreDonnee);
    using UniqueDonnee = RefUniqueSql<Donnee>;
    InfoBdd infoDonnee(Donnee::Name(),donnee,Donnee::NbrAtt,{UniqueDonnee::NbrUnique});
    infoDonnee.setAttribut(Donnee::Nom,"nom",bmps::typeAttributBdd::Text);
    infoDonnee.setAttribut(Donnee::Type,"tp");
    infoDonnee.setAttribut(Donnee::TpVal,"tpV");
    infoDonnee.setAttribut(Donnee::Ref,"ref",bmps::typeAttributBdd::Text,false);
    infoDonnee.setUnique(Donnee::Ref,UniqueDonnee::RefUnique);
    infoDonnee.setForeignKey(Donnee::Id,infoArbre);
    setTypeForeignKey<Donnee>(infoDonnee);
    if(gestion)
        setManager<Donnee>(std::make_unique<ManagerArbreModifControle<Donnee>>(infoDonnee,infoArbre,std::move(gestion),
                                                                               std::make_unique<UniqueDonnee>()));
    else
        setManager<Donnee>(std::make_unique<ManagerArbre<Donnee>>(infoDonnee,infoArbre, std::make_unique<UniqueDonnee>()));
    setCible<Donnee>(bmps::cibleId::Donnee);

    //DonneeCible
    using UniqueCD = CibleNumUniqueSql<DonneeCible>;
    InfoBdd infoCD(DonneeCible::Name(),cibleDonnee,DonneeCible::NbrAtt,{UniqueCD::NbrUnique});
    infoCD.setAttribut(DonneeCible::IdDonnee,"idDn");
    infoCD.setAttribut(DonneeCible::IdCible,"idCb");
    infoCD.setAttribut(DonneeCible::Cible,"cb");
    infoCD.setAttribut(DonneeCible::DateTime,"dt",bmps::typeAttributBdd::DateTime);
    infoCD.setAttribut(DonneeCible::Num,"num");
    infoCD.setAttribut(DonneeCible::Valeur,"val",bmps::typeAttributBdd::Blob,false);
    infoCD.setUnique(DonneeCible::IdDonnee,UniqueCD::Id1Unique);
    infoCD.setUnique(DonneeCible::IdCible,UniqueCD::IdCibleUnique);
    infoCD.setUnique(DonneeCible::Cible,UniqueCD::CibleUnique);
    infoCD.setUnique(DonneeCible::Num,UniqueCD::NumUnique);
    if(gestionCilbeDonnee)
        setManager<DonneeCible>(std::make_unique<ManagerModifControle<DonneeCible>>(infoCD,std::move(gestionCilbeDonnee),
                                                                                    std::make_unique<UniqueCD>()));
    else
        setManager<DonneeCible>(std::make_unique<ManagerSql<DonneeCible>>(infoCD, std::make_unique<UniqueCD>()));
    setCible<DonneeCible>(bmps::cibleId::DonneeCible);

    //DonneeCard
    using UniqueCard = IdCibleUniqueSql<DonneeCard>;
    InfoBdd infoCard(DonneeCard::Name(),cardDonnee,DonneeCard::NbrAtt,{UniqueCard::NbrUnique});
    infoCard.setAttribut(DonneeCard::IdDonnee,"idDn");
    infoCard.setAttribut(DonneeCard::Card,"cd");
    infoCard.setAttribut(DonneeCard::Cible,"cb");
    infoCard.setAttribut(DonneeCard::Exact,"ex");
    infoCard.setUnique(DonneeCard::IdDonnee,UniqueCard::Id1Unique);
    infoCard.setUnique(DonneeCard::Cible,UniqueCard::CibleUnique);
    infoCard.setForeignKey(DonneeCard::IdDonnee,infoDonnee);
    if(gestionCardDonnee)
        setManager<DonneeCard>(std::make_unique<ManagerModifControle<DonneeCard>>(infoCard,std::move(gestionCardDonnee),
                                                                                  std::make_unique<UniqueCard>()));
    else
        setManager<DonneeCard>(std::make_unique<ManagerSql<DonneeCard>>(infoCard, std::make_unique<UniqueCard>()));
    setCible<DonneeCard>(bmps::cibleId::DonneeCard);

    setEnsembleEnable(bmps::DonneeEnable);
}

void ManagersPredef::enableEvenement(const QString & evenement, const QString & evenementCible, const QString & evenementStyle) {
    // EvenementStyle
    using UniqueStyle = NomUniqueSql<EvenementStyle>;
    InfoBdd infoStyle(EvenementStyle::Name(),evenementStyle,EvenementStyle::NbrAtt,{UniqueStyle::NbrUnique});
    infoStyle.setAttribut(EvenementStyle::Bordure,"bd");
    infoStyle.setAttribut(EvenementStyle::CouleurBordure,"cb",bmps::typeAttributBdd::Text);
    infoStyle.setAttribut(EvenementStyle::CouleurFond,"cf",bmps::typeAttributBdd::Text);
    infoStyle.setAttribut(EvenementStyle::CouleurTexte,"cte",bmps::typeAttributBdd::Text);
    infoStyle.setAttribut(EvenementStyle::CouleurTitre,"cti",bmps::typeAttributBdd::Text);
    infoStyle.setAttribut(EvenementStyle::Forme,"fm");
    infoStyle.setAttribut(EvenementStyle::Nom,"nm",bmps::typeAttributBdd::Text);
    infoStyle.setAttribut(EvenementStyle::PoliceTexte,"pte",bmps::typeAttributBdd::Text);
    infoStyle.setAttribut(EvenementStyle::PolicceTitre,"pti",bmps::typeAttributBdd::Text);
    infoStyle.setAttribut(EvenementStyle::Texture,"tx");
    infoStyle.setUnique(EvenementStyle::Nom,UniqueStyle::NomUnique);
    setManager<EvenementStyle>(std::make_unique<ManagerSql<EvenementStyle>>(infoStyle, std::make_unique<UniqueStyle>()));
    setCible<EvenementStyle>(bmps::cibleId::EvenementStyle);

    // Evenement
    using UniqueEvenement = NomUniqueSql<Evenement>;
    InfoBdd infoEvenement(Evenement::Name(),evenement,Evenement::NbrAtt,{UniqueEvenement::NbrUnique});
    infoEvenement.setAttribut(Evenement::IdStyle,"idSt");
    infoEvenement.setAttribut(Evenement::Code,"cd");
    infoEvenement.setAttribut(Evenement::Creation,"cr",bmps::typeAttributBdd::DateTime);
    infoEvenement.setAttribut(Evenement::Debut,"deb",bmps::typeAttributBdd::DateTime);
    infoEvenement.setAttribut(Evenement::Fin,"fn",bmps::typeAttributBdd::DateTime);
    infoEvenement.setAttribut(Evenement::Modification,"modif",bmps::typeAttributBdd::DateTime);
    infoEvenement.setAttribut(Evenement::Nc,"nc",bmps::typeAttributBdd::Text);
    infoEvenement.setAttribut(Evenement::Nom,"nm",bmps::typeAttributBdd::Text);
    infoEvenement.setAttribut(Evenement::Titre,"tii",bmps::typeAttributBdd::Text);
    infoEvenement.setAttribut(Evenement::Type,"tp");
    infoEvenement.setUnique(Evenement::Nom,UniqueEvenement::NomUnique);
    infoEvenement.setForeignKey(Evenement::IdStyle,infoStyle);
    setTypeForeignKey<Evenement>(infoEvenement);
    setManager<Evenement>(std::make_unique<ManagerSql<Evenement>>(infoEvenement, std::make_unique<UniqueEvenement>()));
    setCible<Evenement>(bmps::cibleId::Evenement);

    // EvenementCible
    using UniqueCible = CibleNumUniqueSql<EvenementCible>;
    InfoBdd infoCible("EvenementCible",evenementCible,EvenementCible::NbrAtt,{UniqueCible::NbrUnique});
    infoCible.setAttribut(EvenementCible::IdEvenement,"idEv");
    infoCible.setAttribut(EvenementCible::IdCible,"idCb");
    infoCible.setAttribut(EvenementCible::Cible,"cb");
    infoCible.setAttribut(EvenementCible::DateTime,"dt",bmps::typeAttributBdd::DateTime);
    infoCible.setAttribut(EvenementCible::Num,"num");
    infoCible.setAttribut(EvenementCible::TpVal,"tpV");
    infoCible.setAttribut(EvenementCible::Valeur,"val",bmps::typeAttributBdd::Blob);
    infoCible.setUnique(EvenementCible::IdEvenement,UniqueCible::Id1Unique);
    infoCible.setUnique(EvenementCible::IdCible,UniqueCible::IdCibleUnique);
    infoCible.setUnique(EvenementCible::Cible,UniqueCible::CibleUnique);
    infoCible.setUnique(EvenementCible::Num,UniqueCible::NumUnique);
    infoCible.setForeignKey(EvenementCible::IdEvenement,infoEvenement);
    setManager<EvenementCible>(std::make_unique<ManagerSql<EvenementCible>>(infoCible, std::make_unique<UniqueCible>()));
    setCible<EvenementCible>(bmps::cibleId::EvenementCible);

    setEnsembleEnable(bmps::EvenementEnable);
}

void ManagersPredef::enableHistorique(const QString & historique) {
    using Unique = CibleSimpleNumUniqueSql<Historique>;
    InfoBdd info(Historique::Name(),historique,Historique::NbrAtt,{Unique::NbrUnique});
    info.setAttribut(Historique::IdCible,"idCb");
    info.setAttribut(Historique::IdEtat,"idEt");
    info.setAttribut(Historique::Cible,"cb");
    info.setAttribut(Historique::DateTime,"dt",bmps::typeAttributBdd::DateTime);
    info.setAttribut(Historique::Etat,"et");
    info.setAttribut(Historique::Num,"num");
    info.setUnique(Historique::IdCible,Unique::IdCibleUnique);
    info.setUnique(Historique::Cible,Unique::CibleUnique);
    info.setUnique(Historique::Num,Unique::NumUnique);
    setManager<Historique>(std::make_unique<ManagerSql<Historique>>(info, std::make_unique<Unique>()));
    setCible<Historique>(bmps::cibleId::Historique);

    setEnsembleEnable(bmps::HistoriqueEnable);
}

void ManagersPredef::enableMotCle(const QString & motCle, const QString & motCleArbre, const QString & cibleMotCle,
                                  const QString & cibleProg, const QString & permissionMotCle, const QString & permissionProg,
                                  std::unique_ptr<AbstractGestionRestriction<MotCle>> &&  gestionMotCle,
                                  std::unique_ptr<AbstractGestionRestriction<MotClePermission>> &&  gestionPermission,
                                  std::unique_ptr<AbstractGestionRestriction<MotProgPermission>> &&  gestionProgPermission) {
    //Mot Clé
    auto infoArbre = infoBddArbre(motCleArbre);
    using UniqueMC = RefUniqueSql<MotCle>;
    InfoBdd infoMC("MotCle",motCle,MotCle::NbrAtt,{UniqueMC::NbrUnique});
    infoMC.setAttribut(MotCle::Nc,"nc",bmps::typeAttributBdd::Text);
    infoMC.setAttribut(MotCle::Nom,"nm",bmps::typeAttributBdd::Text);
    infoMC.setAttribut(MotCle::Ref,"ref",bmps::typeAttributBdd::Text,false);
    infoMC.setUnique(MotCle::Ref,UniqueMC::RefUnique);
    infoMC.setForeignKey(MotCle::Id,infoArbre);
    if(gestionMotCle)
        setManager<MotCle>(std::make_unique<ManagerArbreModifControle<MotCle>>(infoMC,infoArbre,std::move(gestionMotCle),
                                                                               std::make_unique<UniqueMC>()));
    else
        setManager<MotCle>(std::make_unique<ManagerArbre<MotCle>>(infoMC,infoArbre,std::make_unique<UniqueMC>()));
    setCible<MotCle>(bmps::cibleId::MotCle);

    // Cible Mot Clé
    using UniqueCible = CibleUniqueSql<MotCleCible>;
    InfoBdd infoCible(MotCleCible::Name(),cibleMotCle,MotCleCible::NbrAtt,{UniqueCible::NbrUnique});
    infoCible.setAttribut(MotCleCible::IdMotCle,"idMC");
    infoCible.setAttribut(MotCleCible::IdCible,"idCb");
    infoCible.setAttribut(MotCleCible::Cible,"cb");
    infoCible.setUnique(MotCleCible::IdMotCle,UniqueCible::Id1Unique);
    infoCible.setUnique(MotCleCible::IdCible,UniqueCible::IdCibleUnique);
    infoCible.setUnique(MotCleCible::Cible,UniqueCible::CibleUnique);
    infoCible.setForeignKey(MotCleCible::IdMotCle,infoMC);
    setManager<MotCleCible>(std::make_unique<ManagerSql<MotCleCible>>(infoCible, std::make_unique<UniqueCible>()));
    setCible<MotCleCible>(bmps::cibleId::MotCleCible);

    // Permission Mot Clé
    using UniquePermission = IdCibleUniqueSql<MotClePermission>;
    InfoBdd infoPermission(MotClePermission::Name(),permissionMotCle,MotClePermission::NbrAtt,{UniquePermission::NbrUnique});
    infoPermission.setAttribut(MotClePermission::IdMotCle,"idMC");
    infoPermission.setAttribut(MotClePermission::Cible,"cb");
    infoPermission.setAttribut(MotClePermission::Code,"cd");
    infoPermission.setUnique(MotClePermission::IdMotCle,UniquePermission::Id1Unique);
    infoPermission.setUnique(MotClePermission::Cible,UniquePermission::CibleUnique);
    infoPermission.setForeignKey(MotClePermission::IdMotCle,infoMC);
    if(gestionPermission)
        setManager<MotClePermission>(std::make_unique<ManagerPermissionCodeModifControle<MotClePermission>>(infoPermission,
                                                                                                   std::move(gestionPermission),
                                                                                                   std::make_unique<UniquePermission>()));
    else
        setManager<MotClePermission>(std::make_unique<ManagerPermissionCode<MotClePermission>>(infoPermission,
                                                                                               std::make_unique<UniquePermission>()));
    setCible<MotClePermission>(bmps::cibleId::MotClePermission);

    // Cible Mot Prog
    using UniqueProgCible = CibleSimpleRefUniqueSql<MotProgCible>;
    InfoBdd infoProgCible(MotProgCible::Name(),cibleProg,MotProgCible::NbrAtt,{UniqueProgCible::NbrUnique});
    infoProgCible.setAttribut(MotProgCible::Ref,"ref",bddMPS::typeAttributBdd::Text,false);
    infoProgCible.setAttribut(MotProgCible::IdMotCle,"idMC");
    infoProgCible.setAttribut(MotProgCible::IdCible,"idCb");
    infoProgCible.setAttribut(MotProgCible::Cible,"cb");
    infoProgCible.setUnique(MotProgCible::Ref,UniqueProgCible::RefUnique);
    infoProgCible.setUnique(MotProgCible::IdCible,UniqueProgCible::IdCibleUnique);
    infoProgCible.setUnique(MotProgCible::Cible,UniqueProgCible::CibleUnique);
    infoProgCible.setForeignKey(MotProgCible::IdMotCle,infoMC);
    setManager<MotProgCible>(std::make_unique<ManagerSql<MotProgCible>>(infoProgCible, std::make_unique<UniqueProgCible>()));
    setCible<MotProgCible>(bmps::cibleId::MotProgCible);

    // Permission Mot Prog
    using UniqueProgPermission = IdCibleRefUniqueSql<MotProgPermission>;
    InfoBdd infoProgPermission(MotProgPermission::Name(),permissionProg,MotProgPermission::NbrAtt,{UniqueProgPermission::NbrUnique});
    infoProgPermission.setAttribut(MotProgPermission::Ref,"ref",bddMPS::typeAttributBdd::Text,false);
    infoProgPermission.setAttribut(MotProgPermission::IdMotCle,"idMC");
    infoProgPermission.setAttribut(MotProgPermission::Cible,"cb");
    infoProgPermission.setAttribut(MotProgPermission::Code,"cd");
    infoProgPermission.setUnique(MotProgPermission::Ref,UniqueProgPermission::RefUnique);
    infoProgPermission.setUnique(MotProgPermission::IdMotCle,UniqueProgPermission::Id1Unique);
    infoProgPermission.setUnique(MotProgPermission::Cible,UniqueProgPermission::CibleUnique);
    infoProgPermission.setForeignKey(MotProgPermission::IdMotCle,infoMC);
    if(gestionProgPermission)
        setManager<MotProgPermission>(
                    std::make_unique<ManagerPermissionCodeModifControle<MotProgPermission>>(infoProgPermission,
                                                                                            std::move(gestionProgPermission),
                                                                                            std::make_unique<UniqueProgPermission>()));
    else
        setManager<MotProgPermission>(std::make_unique<ManagerPermissionCode<MotProgPermission>>(infoProgPermission,
                                                                                                 std::make_unique<UniqueProgPermission>()));
    setCible<MotProgPermission>(bmps::cibleId::MotProgPermission);

    setEnsembleEnable(bmps::MotCleEnable);
}

void ManagersPredef::enableRestriction(const QString & restriction) {
    using Unique = CibleSimpleUniqueSql<Restriction>;
    InfoBdd info("Restriction",restriction,Restriction::NbrAtt,{Unique::NbrUnique});
    info.setAttribut(Restriction::IdCible,"idCb");
    info.setAttribut(Restriction::Cible,"cb");
    info.setAttribut(Restriction::Code,"cd");
    info.setUnique(Restriction::IdCible,Unique::IdCibleUnique);
    info.setUnique(Restriction::Cible,Unique::CibleUnique);
    setManager<Restriction>(std::make_unique<ManagerPermissionCode<Restriction>>(info,std::make_unique<Unique>()));
    setCible<Restriction>(bmps::cibleId::Restriction);

    setEnsembleEnable(bmps::RestrictionEnable);
}

void ManagersPredef::enableTexte(const QString & texte, const QString & cibleTexte, const QString & source
                                 , const QString & texteSource)
{
    //Texte
    InfoBdd infoTexte("Texte",texte,Texte::NbrAtt);
    infoTexte.setAttribut(Texte::Creation,"crea",bmps::typeAttributBdd::DateTime);
    infoTexte.setAttribut(Texte::Modification,"modif",bmps::typeAttributBdd::DateTime);
    infoTexte.setAttribut(Texte::Texte,"txt",bmps::typeAttributBdd::Text,false);
    setManager<Texte>(std::make_unique<ManagerSql<Texte>>(infoTexte));
    setCible<Texte>(bmps::cibleId::Texte);

    //TexteCible
    using UniqueCible = CibleSimpleNumTypeUniqueSql<TexteCible>;
    InfoBdd infoCible(TexteCible::Name(),cibleTexte,TexteCible::NbrAtt,{UniqueCible::NbrUnique});
    infoCible.setAttribut(TexteCible::IdTexte,"IdTxt");
    infoCible.setAttribut(TexteCible::IdCible,"IdCb");
    infoCible.setAttribut(TexteCible::Cible,"cb");
    infoCible.setAttribut(TexteCible::DateTime,"dt",bmps::typeAttributBdd::DateTime);
    infoCible.setAttribut(TexteCible::Num,"num");
    infoCible.setAttribut(TexteCible::Type,"tp");
    infoCible.setUnique(TexteCible::IdCible,UniqueCible::IdCibleUnique);
    infoCible.setUnique(TexteCible::Cible,UniqueCible::CibleUnique);
    infoCible.setUnique(TexteCible::Num,UniqueCible::NumUnique);
    infoCible.setUnique(TexteCible::Type,UniqueCible::TypeUnique);
    infoCible.setForeignKey(TexteCible::IdTexte,infoTexte);
    setTypeForeignKey<TexteCible>(infoCible);
    setManager<TexteCible>(std::make_unique<ManagerSql<TexteCible>>(infoCible, std::make_unique<UniqueCible>()));
    setCible<TexteCible>(bmps::cibleId::TexteCible);

    //Source
    using UniqueSource = NomUniqueSql<Source>;
    InfoBdd infoSource("Source",source,Source::NbrAtt,{UniqueSource::NbrUnique});
    infoSource.setAttribut(Source::Nc,"nc",bmps::typeAttributBdd::Text);
    infoSource.setAttribut(Source::Nom,"nm",bmps::typeAttributBdd::Text);
    infoSource.setAttribut(Source::Type,"tp");
    infoSource.setUnique(Source::Nom,UniqueSource::NomUnique);
    setTypeForeignKey<Source>(infoSource);
    setManager<Source>(std::make_unique<ManagerSql<Source>>(infoSource, std::make_unique<UniqueSource>()));
    setCible<Source>(bmps::cibleId::Source);

    //SourceTexte
    using UniqueTS = RelationUniqueSql<SourceTexte>;
    InfoBdd infoTS(SourceTexte::Name(),texteSource,SourceTexte::NbrAtt,{UniqueTS::NbrUnique});
    infoTS.setAttribut(SourceTexte::IdSource,"idSr");
    infoTS.setAttribut(SourceTexte::IdTexte,"idTxt");
    infoTS.setUnique(SourceTexte::IdSource,UniqueTS::Id1Unique);
    infoTS.setUnique(SourceTexte::IdTexte,UniqueTS::Id2Unique);
    infoTS.setForeignKey(SourceTexte::IdSource,infoSource);
    infoTS.setForeignKey(SourceTexte::IdTexte,infoTexte);
    setManager<SourceTexte>(std::make_unique<ManagerSql<SourceTexte>>(infoTS, std::make_unique<UniqueTS>()));
    setCible<SourceTexte>(bmps::cibleId::SourceTexte);

    setEnsembleEnable(bmps::TexteEnable);
}

void  ManagersPredef::enableType(const QString & typeEnt, const QString & permissionType,
                std::unique_ptr<AbstractGestionRestriction<Type>> &&  gestionType,
                std::unique_ptr<AbstractGestionRestriction<TypePermission>> &&  gestionPermission)
{
    //Type
    using UniqueType = RefNomParentUniqueSql<Type>;
    InfoBdd infoType("Type",typeEnt,Type::NbrAtt,{UniqueType::NbrUnique_1,UniqueType::NbrUnique_2});
    infoType.setAttribut(Type::Ref,"ref",bmps::typeAttributBdd::Text,false);
    infoType.setAttribut(Type::Parent,"pt",bmps::typeAttributBdd::Integer,false);
    infoType.setAttribut(Type::Nc,"nc",bmps::typeAttributBdd::Text);
    infoType.setAttribut(Type::Nom,"nm",bmps::typeAttributBdd::Text);
    infoType.setUnique(Type::Ref,UniqueType::RefUnique,UniqueType::RefUniqueSet);
    infoType.setUnique(Type::Nom,UniqueType::NomUnique,UniqueType::NomUniqueSet);
    infoType.setUnique(Type::Parent,UniqueType::ParentUnique,UniqueType::ParentUniqueSet);
    infoType.setForeignKey(Type::Parent,infoType);
    if(gestionType)
        setManager<Type>(std::make_unique<ManagerArbreSimpleModifControle<Type>>(infoType,std::move(gestionType),
                                                                                 std::make_unique<UniqueType>()));
    else
        setManager<Type>(std::make_unique<ManagerArbreSimple<Type>>(infoType,std::make_unique<UniqueType>()));
    setCible<Type>(bmps::cibleId::Type);

    //Permission
    using UniquePermission = IdCibleUniqueSql<TypePermission>;
    InfoBdd infoPermission(TypePermission::Name(),permissionType,TypePermission::NbrAtt,{UniquePermission::NbrUnique});
    infoPermission.setAttribut(TypePermission::IdType,"idTp");
    infoPermission.setAttribut(TypePermission::Cible,"cb");
    infoPermission.setAttribut(TypePermission::Code,"cd");
    infoPermission.setUnique(TypePermission::IdType,UniquePermission::Id1Unique);
    infoPermission.setUnique(TypePermission::Cible,UniquePermission::CibleUnique);
    infoPermission.setForeignKey(TypePermission::IdType,infoType);
    if(gestionPermission)
        setManager<TypePermission>(
                    std::make_unique<ManagerPermissionCodeModifControle<TypePermission>>(infoPermission,std::move(gestionPermission),
                                                                                         std::make_unique<UniquePermission>()));
    else
        setManager<TypePermission>(std::make_unique<ManagerPermissionCode<TypePermission>>(infoPermission,
                                                                                           std::make_unique<UniquePermission>()));
    setCible<TypePermission>(bmps::cibleId::TypePermission);

    setEnsembleEnable(bmps::TypeEnable);
}

void ManagersPredef::enableUtilisation(const QString &utilisation, const QString &usage, const QString & usageArbre,
                                       std::unique_ptr<AbstractGestionRestriction<Usage>> &&  gestionUsage)
{
    //Usage
    auto infoArbre = infoBddArbre(usageArbre);
    using UniqueN = RefNomTypeUniqueSql<Usage>;
    InfoBdd infoN("Usage",usage,Usage::NbrAtt,{UniqueN::NbrUnique_1,UniqueN::NbrUnique_2});
    infoN.setAttribut(Usage::Nc,"nc",bmps::typeAttributBdd::Text);
    infoN.setAttribut(Usage::Nom,"nm",bmps::typeAttributBdd::Text);
    infoN.setAttribut(Usage::Ref,"ref",bmps::typeAttributBdd::Text,false);
    infoN.setAttribut(Usage::Type,"tp");
    infoN.setUnique(Usage::Ref,UniqueN::RefUnique,UniqueN::TypeUniqueSet);
    infoN.setUnique(Usage::Nom,UniqueN::NomUnique,UniqueN::NomUniqueSet);
    infoN.setUnique(Usage::Type,UniqueN::TypeUnique,UniqueN::TypeUniqueSet);
    infoN.setForeignKey(Usage::Id,infoArbre);
    setTypeForeignKey<Usage>(infoN);
    if(gestionUsage)
        setManager<Usage>(std::make_unique<ManagerArbreModifControle<Usage>>(infoN, infoArbre, std::move(gestionUsage),
                                                                             std::make_unique<UniqueN>()));
    else
        setManager<Usage>(std::make_unique<ManagerArbre<Usage>>(infoN,infoArbre, std::make_unique<UniqueN>()));
    setCible<Usage>(bmps::cibleId::Usage);

    //Utilisation
    using UniqueU = CibleSimpleNumUniqueSql<Utilisation>;
    InfoBdd infoU(Utilisation::Name(),utilisation,Utilisation::NbrAtt,{UniqueU::NbrUnique});
    infoU.setAttribut(Utilisation::IdUsage,"idUs",bmps::typeAttributBdd::Integer);
    infoU.setAttribut(Utilisation::IdCible,"idCb");
    infoU.setAttribut(Utilisation::IdEtat,"idEt");
    infoU.setAttribut(Utilisation::Cible,"cb");
    infoU.setAttribut(Utilisation::DateTime,"dt",bmps::typeAttributBdd::DateTime);
    infoU.setAttribut(Utilisation::Etat,"et");
    infoU.setAttribut(Utilisation::Num,"num");
    infoU.setUnique(Utilisation::IdCible,UniqueU::IdCibleUnique);
    infoU.setUnique(Utilisation::Cible,UniqueU::CibleUnique);
    infoU.setUnique(Utilisation::Num,UniqueU::NumUnique);
    infoU.setForeignKey(Utilisation::IdUsage,infoN);
    setManager<Utilisation>(std::make_unique<ManagerSql<Utilisation>>(infoU, std::make_unique<UniqueU>()));
    setCible<Utilisation>(bmps::cibleId::Utilisation);

    setEnsembleEnable(bmps::UtilisationEnable);
}
