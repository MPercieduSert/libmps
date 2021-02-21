#include "ManagersPredef.h"

using namespace  managerMPS;
using namespace  entities;

void ManagersPredef::enableCommentaire(const QString &commentaire, const QString &cibleCommentaire) {
    //Commentaire
    InfoBdd infoCommentaire("Commentaire",commentaire,Commentaire::Nbr_Att);
    infoCommentaire.set_attribut(Commentaire::Creation,"crea",bmps::typeAttributBdd::Date_Time);
    infoCommentaire.set_attribut(Commentaire::Modification,"modif",bmps::typeAttributBdd::Date_Time);
    infoCommentaire.set_attribut(Commentaire::Texte,"txt",bmps::typeAttributBdd::Text,false);
    setManager<Commentaire>(std::make_unique<ManagerSql<Commentaire>>(infoCommentaire));
    set_cible<Commentaire>(bmps::cibleId::Commentaire);

    //commentaire_cible
    using Unique = cible_numUniqueSql<commentaire_cible>;
    InfoBdd infoCibleCom(commentaire_cible::Name(),cibleCommentaire,commentaire_cible::Nbr_Att,{Unique::NbrUnique});
    infoCibleCom.set_attribut(commentaire_cible::IdCommentaire,"idCm");
    infoCibleCom.set_attribut(commentaire_cible::Id_Cible,"idCb");
    infoCibleCom.set_attribut(commentaire_cible::Cible,"cb");
    infoCibleCom.set_attribut(commentaire_cible::Date_Time,"dt",bmps::typeAttributBdd::Date_Time);
    infoCibleCom.set_attribut(commentaire_cible::Num,"num");
    infoCibleCom.setUnique(commentaire_cible::Id_Cible,Unique::Id_CibleUnique);
    infoCibleCom.setUnique(commentaire_cible::Cible,Unique::CibleUnique);
    infoCibleCom.setUnique(commentaire_cible::Num,Unique::NumUnique);
    infoCibleCom.setForeignKey(commentaire_cible::IdCommentaire,infoCommentaire);
    infoCibleCom.setForeignKey(commentaire_cible::Id_Cible,InfoBdd());
    setManager<commentaire_cible>(std::make_unique<ManagerSql<commentaire_cible>>(infoCibleCom,std::make_unique<Unique>()));
    set_cible<commentaire_cible>(bmps::cibleId::commentaire_cible);

    setEnsembleEnable(bmps::CommentaireEnable);
}

void ManagersPredef::enabledonnee(const QString &donnee, const QString &arbredonnee, const QString &cibledonnee,
                                  const QString &carddonnee,
                                  std::unique_ptr<AbstractGestionRestriction<donnee>> && gestion,
                                  std::unique_ptr<AbstractGestionRestriction<donnee_cible>> &&gestionCilbedonnee,
                                  std::unique_ptr<AbstractGestionRestriction<donnee_card>> &&gestionCarddonnee) {
    //donnee
    auto infoarbre = infoBddarbre(arbredonnee);
    using Uniquedonnee = RefUniqueSql<donnee>;
    InfoBdd infodonnee(donnee::Name(),donnee,donnee::Nbr_Att,{Uniquedonnee::NbrUnique});
    infodonnee.set_attribut(donnee::Nom,"nom",bmps::typeAttributBdd::Text);
    infodonnee.set_attribut(donnee::Type,"tp");
    infodonnee.set_attribut(donnee::Tp_Val,"tpV");
    infodonnee.set_attribut(donnee::Ref,"ref",bmps::typeAttributBdd::Text,false);
    infodonnee.setUnique(donnee::Ref,Uniquedonnee::RefUnique);
    infodonnee.setForeignKey(donnee::Id,infoarbre);
    set_typeForeignKey<donnee>(infodonnee);
    if(gestion)
        setManager<donnee>(std::make_unique<ManagerarbreModifControle<donnee>>(infodonnee,infoarbre,std::move(gestion),
                                                                               std::make_unique<Uniquedonnee>()));
    else
        setManager<donnee>(std::make_unique<Managerarbre<donnee>>(infodonnee,infoarbre, std::make_unique<Uniquedonnee>()));
    set_cible<donnee>(bmps::cibleId::donnee);

    //donnee_cible
    using UniqueCD = cible_numUniqueSql<donnee_cible>;
    InfoBdd infoCD(donnee_cible::Name(),cibledonnee,donnee_cible::Nbr_Att,{UniqueCD::NbrUnique});
    infoCD.set_attribut(donnee_cible::Iddonnee,"idDn");
    infoCD.set_attribut(donnee_cible::Id_Cible,"idCb");
    infoCD.set_attribut(donnee_cible::Cible,"cb");
    infoCD.set_attribut(donnee_cible::Date_Time,"dt",bmps::typeAttributBdd::Date_Time);
    infoCD.set_attribut(donnee_cible::Num,"num");
    infoCD.set_attribut(donnee_cible::Valeur,"val",bmps::typeAttributBdd::Blob,false);
    infoCD.setUnique(donnee_cible::Iddonnee,UniqueCD::Id_1Unique);
    infoCD.setUnique(donnee_cible::Id_Cible,UniqueCD::Id_CibleUnique);
    infoCD.setUnique(donnee_cible::Cible,UniqueCD::CibleUnique);
    infoCD.setUnique(donnee_cible::Num,UniqueCD::NumUnique);
    if(gestionCilbedonnee)
        setManager<donnee_cible>(std::make_unique<ManagerModifControle<donnee_cible>>(infoCD,std::move(gestionCilbedonnee),
                                                                                    std::make_unique<UniqueCD>()));
    else
        setManager<donnee_cible>(std::make_unique<ManagerSql<donnee_cible>>(infoCD, std::make_unique<UniqueCD>()));
    set_cible<donnee_cible>(bmps::cibleId::donnee_cible);

    //donnee_card
    using UniqueCard = Id_CibleUniqueSql<donnee_card>;
    InfoBdd infoCard(donnee_card::Name(),carddonnee,donnee_card::Nbr_Att,{UniqueCard::NbrUnique});
    infoCard.set_attribut(donnee_card::Iddonnee,"idDn");
    infoCard.set_attribut(donnee_card::Card,"cd");
    infoCard.set_attribut(donnee_card::Cible,"cb");
    infoCard.set_attribut(donnee_card::Exact,"ex");
    infoCard.setUnique(donnee_card::Iddonnee,UniqueCard::Id_1Unique);
    infoCard.setUnique(donnee_card::Cible,UniqueCard::CibleUnique);
    infoCard.setForeignKey(donnee_card::Iddonnee,infodonnee);
    if(gestionCarddonnee)
        setManager<donnee_card>(std::make_unique<ManagerModifControle<donnee_card>>(infoCard,std::move(gestionCarddonnee),
                                                                                  std::make_unique<UniqueCard>()));
    else
        setManager<donnee_card>(std::make_unique<ManagerSql<donnee_card>>(infoCard, std::make_unique<UniqueCard>()));
    set_cible<donnee_card>(bmps::cibleId::donnee_card);

    setEnsembleEnable(bmps::donneeEnable);
}

void ManagersPredef::enableEvenement(const QString &evenement, const QString &evenementCible, const QString &evenementStyle) {
    // evenement_style
    using UniqueStyle = NomUniqueSql<evenement_style>;
    InfoBdd infoStyle(evenement_style::Name(),evenementStyle,evenement_style::Nbr_Att,{UniqueStyle::NbrUnique});
    infoStyle.set_attribut(evenement_style::Bordure,"bd");
    infoStyle.set_attribut(evenement_style::Couleur_Bordure,"cb",bmps::typeAttributBdd::Text);
    infoStyle.set_attribut(evenement_style::Couleur_Fond,"cf",bmps::typeAttributBdd::Text);
    infoStyle.set_attribut(evenement_style::Couleur_Texte,"cte",bmps::typeAttributBdd::Text);
    infoStyle.set_attribut(evenement_style::Couleur_Titre,"cti",bmps::typeAttributBdd::Text);
    infoStyle.set_attribut(evenement_style::Forme,"fm");
    infoStyle.set_attribut(evenement_style::Nom,"nm",bmps::typeAttributBdd::Text);
    infoStyle.set_attribut(evenement_style::Police_Texte,"pte",bmps::typeAttributBdd::Text);
    infoStyle.set_attribut(evenement_style::Police_Titre,"pti",bmps::typeAttributBdd::Text);
    infoStyle.set_attribut(evenement_style::Texture,"tx");
    infoStyle.setUnique(evenement_style::Nom,UniqueStyle::NomUnique);
    setManager<evenement_style>(std::make_unique<ManagerSql<evenement_style>>(infoStyle, std::make_unique<UniqueStyle>()));
    set_cible<evenement_style>(bmps::cibleId::evenement_style);

    // Evenement
    using UniqueEvenement = NomUniqueSql<Evenement>;
    InfoBdd infoEvenement(Evenement::Name(),evenement,Evenement::Nbr_Att,{UniqueEvenement::NbrUnique});
    infoEvenement.set_attribut(Evenement::IdStyle,"idSt");
    infoEvenement.set_attribut(Evenement::Code,"cd");
    infoEvenement.set_attribut(Evenement::Creation,"cr",bmps::typeAttributBdd::Date_Time);
    infoEvenement.set_attribut(Evenement::Debut,"deb",bmps::typeAttributBdd::Date_Time);
    infoEvenement.set_attribut(Evenement::Fin,"fn",bmps::typeAttributBdd::Date_Time);
    infoEvenement.set_attribut(Evenement::Modification,"modif",bmps::typeAttributBdd::Date_Time);
    infoEvenement.set_attribut(Evenement::Nc,"nc",bmps::typeAttributBdd::Text);
    infoEvenement.set_attribut(Evenement::Nom,"nm",bmps::typeAttributBdd::Text);
    infoEvenement.set_attribut(Evenement::Titre,"tii",bmps::typeAttributBdd::Text);
    infoEvenement.set_attribut(Evenement::Type,"tp");
    infoEvenement.setUnique(Evenement::Nom,UniqueEvenement::NomUnique);
    infoEvenement.setForeignKey(Evenement::IdStyle,infoStyle);
    set_typeForeignKey<Evenement>(infoEvenement);
    setManager<Evenement>(std::make_unique<ManagerSql<Evenement>>(infoEvenement, std::make_unique<UniqueEvenement>()));
    set_cible<Evenement>(bmps::cibleId::Evenement);

    // evenement_cible
    using UniqueCible = cible_numUniqueSql<evenement_cible>;
    InfoBdd infoCible("evenement_cible",evenementCible,evenement_cible::Nbr_Att,{UniqueCible::NbrUnique});
    infoCible.set_attribut(evenement_cible::IdEvenement,"idEv");
    infoCible.set_attribut(evenement_cible::Id_Cible,"idCb");
    infoCible.set_attribut(evenement_cible::Cible,"cb");
    infoCible.set_attribut(evenement_cible::Date_Time,"dt",bmps::typeAttributBdd::Date_Time);
    infoCible.set_attribut(evenement_cible::Num,"num");
    infoCible.set_attribut(evenement_cible::Tp_Val,"tpV");
    infoCible.set_attribut(evenement_cible::Valeur,"val",bmps::typeAttributBdd::Blob);
    infoCible.setUnique(evenement_cible::IdEvenement,UniqueCible::Id_1Unique);
    infoCible.setUnique(evenement_cible::Id_Cible,UniqueCible::Id_CibleUnique);
    infoCible.setUnique(evenement_cible::Cible,UniqueCible::CibleUnique);
    infoCible.setUnique(evenement_cible::Num,UniqueCible::NumUnique);
    infoCible.setForeignKey(evenement_cible::IdEvenement,infoEvenement);
    setManager<evenement_cible>(std::make_unique<ManagerSql<evenement_cible>>(infoCible, std::make_unique<UniqueCible>()));
    set_cible<evenement_cible>(bmps::cibleId::evenement_cible);

    setEnsembleEnable(bmps::EvenementEnable);
}

void ManagersPredef::enableHistorique(const QString &historique) {
    using Unique = cible_numUniqueSql<Historique>;
    InfoBdd info(Historique::Name(),historique,Historique::Nbr_Att,{Unique::NbrUnique});
    info.set_attribut(Historique::Id_Cible,"idCb");
    info.set_attribut(Historique::Id_Etat,"idEt");
    info.set_attribut(Historique::Cible,"cb");
    info.set_attribut(Historique::Date_Time,"dt",bmps::typeAttributBdd::Date_Time);
    info.set_attribut(Historique::Etat,"et");
    info.set_attribut(Historique::Num,"num");
    info.setUnique(Historique::Id_Cible,Unique::Id_CibleUnique);
    info.setUnique(Historique::Cible,Unique::CibleUnique);
    info.setUnique(Historique::Num,Unique::NumUnique);
    setManager<Historique>(std::make_unique<ManagerSql<Historique>>(info, std::make_unique<Unique>()));
    set_cible<Historique>(bmps::cibleId::Historique);

    setEnsembleEnable(bmps::HistoriqueEnable);
}

void ManagersPredef::enablemot_cle(const QString &motCle, const QString &motClearbre, const QString &ciblemot_cle,
                                  const QString &cibleProg, const QString &permissionmot_cle, const QString &permissionProg,
                                  std::unique_ptr<AbstractGestionRestriction<mot_cle>> && gestionmot_cle,
                                  std::unique_ptr<AbstractGestionRestriction<mot_cle_permission>> && gestionPermission,
                                  std::unique_ptr<AbstractGestionRestriction<mot_prog_permission>> && gestionProgPermission) {
    //Mot Clé
    auto infoarbre = infoBddarbre(motClearbre);
    using UniqueMC = RefUniqueSql<mot_cle>;
    InfoBdd infoMC("mot_cle",motCle,mot_cle::Nbr_Att,{UniqueMC::NbrUnique});
    infoMC.set_attribut(mot_cle::Nc,"nc",bmps::typeAttributBdd::Text);
    infoMC.set_attribut(mot_cle::Nom,"nm",bmps::typeAttributBdd::Text);
    infoMC.set_attribut(mot_cle::Ref,"ref",bmps::typeAttributBdd::Text,false);
    infoMC.setUnique(mot_cle::Ref,UniqueMC::RefUnique);
    infoMC.setForeignKey(mot_cle::Id,infoarbre);
    if(gestionmot_cle)
        setManager<mot_cle>(std::make_unique<ManagerarbreModifControle<mot_cle>>(infoMC,infoarbre,std::move(gestionmot_cle),
                                                                               std::make_unique<UniqueMC>()));
    else
        setManager<mot_cle>(std::make_unique<Managerarbre<mot_cle>>(infoMC,infoarbre,std::make_unique<UniqueMC>()));
    set_cible<mot_cle>(bmps::cibleId::mot_cle);

    // Cible Mot Clé
    using UniqueCible = CibleUniqueSql<mot_cle_cible>;
    InfoBdd infoCible(mot_cle_cible::Name(),ciblemot_cle,mot_cle_cible::Nbr_Att,{UniqueCible::NbrUnique});
    infoCible.set_attribut(mot_cle_cible::Idmot_cle,"idMC");
    infoCible.set_attribut(mot_cle_cible::Id_Cible,"idCb");
    infoCible.set_attribut(mot_cle_cible::Cible,"cb");
    infoCible.setUnique(mot_cle_cible::Idmot_cle,UniqueCible::Id_1Unique);
    infoCible.setUnique(mot_cle_cible::Id_Cible,UniqueCible::Id_CibleUnique);
    infoCible.setUnique(mot_cle_cible::Cible,UniqueCible::CibleUnique);
    infoCible.setForeignKey(mot_cle_cible::Idmot_cle,infoMC);
    setManager<mot_cle_cible>(std::make_unique<ManagerSql<mot_cle_cible>>(infoCible, std::make_unique<UniqueCible>()));
    set_cible<mot_cle_cible>(bmps::cibleId::mot_cle_cible);

    // Permission Mot Clé
    using UniquePermission = Id_CibleUniqueSql<mot_cle_permission>;
    InfoBdd infoPermission(mot_cle_permission::Name(),permissionmot_cle,mot_cle_permission::Nbr_Att,{UniquePermission::NbrUnique});
    infoPermission.set_attribut(mot_cle_permission::Idmot_cle,"idMC");
    infoPermission.set_attribut(mot_cle_permission::Cible,"cb");
    infoPermission.set_attribut(mot_cle_permission::Code,"cd");
    infoPermission.setUnique(mot_cle_permission::Idmot_cle,UniquePermission::Id_1Unique);
    infoPermission.setUnique(mot_cle_permission::Cible,UniquePermission::CibleUnique);
    infoPermission.setForeignKey(mot_cle_permission::Idmot_cle,infoMC);
    if(gestionPermission)
        setManager<mot_cle_permission>(std::make_unique<ManagerPermissionCodeModifControle<mot_cle_permission>>(infoPermission,
                                                                                                   std::move(gestionPermission),
                                                                                                   std::make_unique<UniquePermission>()));
    else
        setManager<mot_cle_permission>(std::make_unique<ManagerPermissionCode<mot_cle_permission>>(infoPermission,
                                                                                               std::make_unique<UniquePermission>()));
    set_cible<mot_cle_permission>(bmps::cibleId::mot_cle_permission);

    // Cible Mot Prog
    using UniqueProgCible = CibleSimpleRefUniqueSql<mot_prog_cible>;
    InfoBdd infoProgCible(mot_prog_cible::Name(),cibleProg,mot_prog_cible::Nbr_Att,{UniqueProgCible::NbrUnique});
    infoProgCible.set_attribut(mot_prog_cible::Ref,"ref",bddMPS::typeAttributBdd::Text,false);
    infoProgCible.set_attribut(mot_prog_cible::Idmot_cle,"idMC");
    infoProgCible.set_attribut(mot_prog_cible::Id_Cible,"idCb");
    infoProgCible.set_attribut(mot_prog_cible::Cible,"cb");
    infoProgCible.setUnique(mot_prog_cible::Ref,UniqueProgCible::RefUnique);
    infoProgCible.setUnique(mot_prog_cible::Id_Cible,UniqueProgCible::Id_CibleUnique);
    infoProgCible.setUnique(mot_prog_cible::Cible,UniqueProgCible::CibleUnique);
    infoProgCible.setForeignKey(mot_prog_cible::Idmot_cle,infoMC);
    setManager<mot_prog_cible>(std::make_unique<ManagerSql<mot_prog_cible>>(infoProgCible, std::make_unique<UniqueProgCible>()));
    set_cible<mot_prog_cible>(bmps::cibleId::mot_prog_cible);

    // Permission Mot Prog
    using UniqueProgPermission = Id_cible_refUniqueSql<mot_prog_permission>;
    InfoBdd infoProgPermission(mot_prog_permission::Name(),permissionProg,mot_prog_permission::Nbr_Att,{UniqueProgPermission::NbrUnique});
    infoProgPermission.set_attribut(mot_prog_permission::Ref,"ref",bddMPS::typeAttributBdd::Text,false);
    infoProgPermission.set_attribut(mot_prog_permission::Idmot_cle,"idMC");
    infoProgPermission.set_attribut(mot_prog_permission::Cible,"cb");
    infoProgPermission.set_attribut(mot_prog_permission::Code,"cd");
    infoProgPermission.setUnique(mot_prog_permission::Ref,UniqueProgPermission::RefUnique);
    infoProgPermission.setUnique(mot_prog_permission::Idmot_cle,UniqueProgPermission::Id_1Unique);
    infoProgPermission.setUnique(mot_prog_permission::Cible,UniqueProgPermission::CibleUnique);
    infoProgPermission.setForeignKey(mot_prog_permission::Idmot_cle,infoMC);
    if(gestionProgPermission)
        setManager<mot_prog_permission>(
                    std::make_unique<ManagerPermissionCodeModifControle<mot_prog_permission>>(infoProgPermission,
                                                                                            std::move(gestionProgPermission),
                                                                                            std::make_unique<UniqueProgPermission>()));
    else
        setManager<mot_prog_permission>(std::make_unique<ManagerPermissionCode<mot_prog_permission>>(infoProgPermission,
                                                                                                 std::make_unique<UniqueProgPermission>()));
    set_cible<mot_prog_permission>(bmps::cibleId::mot_prog_permission);

    setEnsembleEnable(bmps::mot_cleEnable);
}

void ManagersPredef::enableRestriction(const QString &restriction) {
    using Unique = CibleSimpleUniqueSql<Restriction>;
    InfoBdd info("Restriction",restriction,Restriction::Nbr_Att,{Unique::NbrUnique});
    info.set_attribut(Restriction::Id_Cible,"idCb");
    info.set_attribut(Restriction::Cible,"cb");
    info.set_attribut(Restriction::Code,"cd");
    info.setUnique(Restriction::Id_Cible,Unique::Id_CibleUnique);
    info.setUnique(Restriction::Cible,Unique::CibleUnique);
    setManager<Restriction>(std::make_unique<ManagerPermissionCode<Restriction>>(info,std::make_unique<Unique>()));
    set_cible<Restriction>(bmps::cibleId::Restriction);

    setEnsembleEnable(bmps::RestrictionEnable);
}

void ManagersPredef::enableTexte(const QString &texte, const QString &cibleTexte, const QString &source
                                 , const QString &texteSource)
{
    //Texte
    InfoBdd infoTexte("Texte",texte,Texte::Nbr_Att);
    infoTexte.set_attribut(Texte::Creation,"crea",bmps::typeAttributBdd::Date_Time);
    infoTexte.set_attribut(Texte::Modification,"modif",bmps::typeAttributBdd::Date_Time);
    infoTexte.set_attribut(Texte::Texte,"txt",bmps::typeAttributBdd::Text,false);
    setManager<Texte>(std::make_unique<ManagerSql<Texte>>(infoTexte));
    set_cible<Texte>(bmps::cibleId::Texte);

    //texte_cible
    using UniqueCible = cible_num_typeUniqueSql<texte_cible>;
    InfoBdd infoCible(texte_cible::Name(),cibleTexte,texte_cible::Nbr_Att,{UniqueCible::NbrUnique});
    infoCible.set_attribut(texte_cible::IdTexte,"IdTxt");
    infoCible.set_attribut(texte_cible::Id_Cible,"IdCb");
    infoCible.set_attribut(texte_cible::Cible,"cb");
    infoCible.set_attribut(texte_cible::Date_Time,"dt",bmps::typeAttributBdd::Date_Time);
    infoCible.set_attribut(texte_cible::Num,"num");
    infoCible.set_attribut(texte_cible::Type,"tp");
    infoCible.setUnique(texte_cible::Id_Cible,UniqueCible::Id_CibleUnique);
    infoCible.setUnique(texte_cible::Cible,UniqueCible::CibleUnique);
    infoCible.setUnique(texte_cible::Num,UniqueCible::NumUnique);
    infoCible.setUnique(texte_cible::Type,UniqueCible::TypeUnique);
    infoCible.setForeignKey(texte_cible::IdTexte,infoTexte);
    set_typeForeignKey<texte_cible>(infoCible);
    setManager<texte_cible>(std::make_unique<ManagerSql<texte_cible>>(infoCible, std::make_unique<UniqueCible>()));
    set_cible<texte_cible>(bmps::cibleId::texte_cible);

    //Source
    using UniqueSource = NomUniqueSql<Source>;
    InfoBdd infoSource("Source",source,Source::Nbr_Att,{UniqueSource::NbrUnique});
    infoSource.set_attribut(Source::Nc,"nc",bmps::typeAttributBdd::Text);
    infoSource.set_attribut(Source::Nom,"nm",bmps::typeAttributBdd::Text);
    infoSource.set_attribut(Source::Type,"tp");
    infoSource.setUnique(Source::Nom,UniqueSource::NomUnique);
    set_typeForeignKey<Source>(infoSource);
    setManager<Source>(std::make_unique<ManagerSql<Source>>(infoSource, std::make_unique<UniqueSource>()));
    set_cible<Source>(bmps::cibleId::Source);

    //source_texte
    using UniqueTS = RelationUniqueSql<source_texte>;
    InfoBdd infoTS(source_texte::Name(),texteSource,source_texte::Nbr_Att,{UniqueTS::NbrUnique});
    infoTS.set_attribut(source_texte::IdSource,"idSr");
    infoTS.set_attribut(source_texte::IdTexte,"idTxt");
    infoTS.setUnique(source_texte::IdSource,UniqueTS::Id_1Unique);
    infoTS.setUnique(source_texte::IdTexte,UniqueTS::Id_2Unique);
    infoTS.setForeignKey(source_texte::IdSource,infoSource);
    infoTS.setForeignKey(source_texte::IdTexte,infoTexte);
    setManager<source_texte>(std::make_unique<ManagerSql<source_texte>>(infoTS, std::make_unique<UniqueTS>()));
    set_cible<source_texte>(bmps::cibleId::source_texte);

    setEnsembleEnable(bmps::TexteEnable);
}

void  ManagersPredef::enableType(const QString &typeEnt, const QString &permissionType,
                std::unique_ptr<AbstractGestionRestriction<Type>> && gestionType,
                std::unique_ptr<AbstractGestionRestriction<type_permission>> && gestionPermission)
{
    //Type
    using UniqueType = RefNomParentUniqueSql<Type>;
    InfoBdd infoType("Type",typeEnt,Type::Nbr_Att,{UniqueType::NbrUnique_1,UniqueType::NbrUnique_2});
    infoType.set_attribut(Type::Ref,"ref",bmps::typeAttributBdd::Text,false);
    infoType.set_attribut(Type::Parent,"pt",bmps::typeAttributBdd::Integer,false);
    infoType.set_attribut(Type::Nc,"nc",bmps::typeAttributBdd::Text);
    infoType.set_attribut(Type::Nom,"nm",bmps::typeAttributBdd::Text);
    infoType.setUnique(Type::Ref,UniqueType::RefUnique,UniqueType::RefUniqueSet);
    infoType.setUnique(Type::Nom,UniqueType::NomUnique,UniqueType::NomUniqueSet);
    infoType.setUnique(Type::Parent,UniqueType::ParentUnique,UniqueType::ParentUniqueSet);
    infoType.setForeignKey(Type::Parent,infoType);
    if(gestionType)
        setManager<Type>(std::make_unique<Managerarbre_simpleModifControle<Type>>(infoType,std::move(gestionType),
                                                                                 std::make_unique<UniqueType>()));
    else
        setManager<Type>(std::make_unique<Managerarbre_simple<Type>>(infoType,std::make_unique<UniqueType>()));
    set_cible<Type>(bmps::cibleId::Type);

    //Permission
    using UniquePermission = Id_CibleUniqueSql<type_permission>;
    InfoBdd infoPermission(type_permission::Name(),permissionType,type_permission::Nbr_Att,{UniquePermission::NbrUnique});
    infoPermission.set_attribut(type_permission::id_type,"idTp");
    infoPermission.set_attribut(type_permission::Cible,"cb");
    infoPermission.set_attribut(type_permission::Code,"cd");
    infoPermission.setUnique(type_permission::id_type,UniquePermission::Id_1Unique);
    infoPermission.setUnique(type_permission::Cible,UniquePermission::CibleUnique);
    infoPermission.setForeignKey(type_permission::id_type,infoType);
    if(gestionPermission)
        setManager<type_permission>(
                    std::make_unique<ManagerPermissionCodeModifControle<type_permission>>(infoPermission,std::move(gestionPermission),
                                                                                         std::make_unique<UniquePermission>()));
    else
        setManager<type_permission>(std::make_unique<ManagerPermissionCode<type_permission>>(infoPermission,
                                                                                           std::make_unique<UniquePermission>()));
    set_cible<type_permission>(bmps::cibleId::type_permission);

    setEnsembleEnable(bmps::TypeEnable);
}

void ManagersPredef::enableUtilisation(const QString &utilisation, const QString &usage, const QString &usagearbre,
                                       std::unique_ptr<AbstractGestionRestriction<Usage>> && gestionUsage)
{
    //Usage
    auto infoarbre = infoBddarbre(usagearbre);
    using UniqueN = Refnom_typeUniqueSql<Usage>;
    InfoBdd infoN("Usage",usage,Usage::Nbr_Att,{UniqueN::NbrUnique_1,UniqueN::NbrUnique_2});
    infoN.set_attribut(Usage::Nc,"nc",bmps::typeAttributBdd::Text);
    infoN.set_attribut(Usage::Nom,"nm",bmps::typeAttributBdd::Text);
    infoN.set_attribut(Usage::Ref,"ref",bmps::typeAttributBdd::Text,false);
    infoN.set_attribut(Usage::Type,"tp");
    infoN.setUnique(Usage::Ref,UniqueN::RefUnique,UniqueN::TypeUniqueSet);
    infoN.setUnique(Usage::Nom,UniqueN::NomUnique,UniqueN::NomUniqueSet);
    infoN.setUnique(Usage::Type,UniqueN::TypeUnique,UniqueN::TypeUniqueSet);
    infoN.setForeignKey(Usage::Id,infoarbre);
    set_typeForeignKey<Usage>(infoN);
    if(gestionUsage)
        setManager<Usage>(std::make_unique<ManagerarbreModifControle<Usage>>(infoN, infoarbre, std::move(gestionUsage),
                                                                             std::make_unique<UniqueN>()));
    else
        setManager<Usage>(std::make_unique<Managerarbre<Usage>>(infoN,infoarbre, std::make_unique<UniqueN>()));
    set_cible<Usage>(bmps::cibleId::Usage);

    //Utilisation
    using UniqueU = cible_numUniqueSql<Utilisation>;
    InfoBdd infoU(Utilisation::Name(),utilisation,Utilisation::Nbr_Att,{UniqueU::NbrUnique});
    infoU.set_attribut(Utilisation::IdUsage,"idUs",bmps::typeAttributBdd::Integer);
    infoU.set_attribut(Utilisation::Id_Cible,"idCb");
    infoU.set_attribut(Utilisation::Id_Etat,"idEt");
    infoU.set_attribut(Utilisation::Cible,"cb");
    infoU.set_attribut(Utilisation::Date_Time,"dt",bmps::typeAttributBdd::Date_Time);
    infoU.set_attribut(Utilisation::Etat,"et");
    infoU.set_attribut(Utilisation::Num,"num");
    infoU.setUnique(Utilisation::Id_Cible,UniqueU::Id_CibleUnique);
    infoU.setUnique(Utilisation::Cible,UniqueU::CibleUnique);
    infoU.setUnique(Utilisation::Num,UniqueU::NumUnique);
    infoU.setForeignKey(Utilisation::IdUsage,infoN);
    setManager<Utilisation>(std::make_unique<ManagerSql<Utilisation>>(infoU, std::make_unique<UniqueU>()));
    set_cible<Utilisation>(bmps::cibleId::Utilisation);

    setEnsembleEnable(bmps::UtilisationEnable);
}
