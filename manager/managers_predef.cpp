#include "managers_predef.h"

using namespace  mps::manager;
using namespace  mps::entities;

void managers_predef::enable_commentaire(const QString &table, const QString &cible_table) {
    //Commentaire
    info_bdd info_commentaire("commentaire",table,commentaire::Nbr_Att);
    info_commentaire.set_attribut(commentaire::Creation,"crea",b2d::type_attribut_bdd::Date_Time);
    info_commentaire.set_attribut(commentaire::Modification,"modif",b2d::type_attribut_bdd::Date_Time);
    info_commentaire.set_attribut(commentaire::Texte,"txt",b2d::type_attribut_bdd::Text,false);
    set_manager<commentaire>(std::make_unique<manager_sql<commentaire>>(info_commentaire));
    set_cible<commentaire>(b2d::cible_id::Commentaire);

    //commentaire_cible
    using unique_cible_com = cible_num_unique_sql<commentaire_cible>;
    info_bdd info_cible_com(commentaire_cible::Name(),cible_table,commentaire_cible::Nbr_Att,{unique_cible_com::Nbr_Unique});
    info_cible_com.set_attribut(commentaire_cible::Id_Commentaire,"idCm");
    info_cible_com.set_attribut(commentaire_cible::Id_Cible,"idCb");
    info_cible_com.set_attribut(commentaire_cible::Cible,"cb");
    info_cible_com.set_attribut(commentaire_cible::Date_Time,"dt",b2d::type_attribut_bdd::Date_Time);
    info_cible_com.set_attribut(commentaire_cible::Num,"num");
    info_cible_com.set_unique(commentaire_cible::Id_Cible,unique_cible_com::Id_Cible_Unique);
    info_cible_com.set_unique(commentaire_cible::Cible,unique_cible_com::Cible_Unique);
    info_cible_com.set_unique(commentaire_cible::Num,unique_cible_com::Num_Unique);
    info_cible_com.set_foreign_key(commentaire_cible::Id_Commentaire,info_commentaire);
    info_cible_com.set_foreign_key(commentaire_cible::Id_Cible,info_bdd());
    set_manager<commentaire_cible>(std::make_unique<manager_sql<commentaire_cible>>(info_cible_com,
                                                                                    std::make_unique<unique_cible_com>()));
    set_cible<commentaire_cible>(b2d::cible_id::Commentaire_Cible);

    set_ensemble_enable(b2d::Commentaire_Enable);
}

void managers_predef::enable_donnee(const QString &table, const QString &arbre_table, const QString &cible_table,
                                  const QString &card_table,
                                  std::unique_ptr<abstract_gestion_restriction<donnee>> && gestion_donnee,
                                  std::unique_ptr<abstract_gestion_restriction<donnee_cible>> &&gestion_cilbe_donnee,
                                  std::unique_ptr<abstract_gestion_restriction<donnee_card>> &&gestion_card_donnee) {
    //donnee
    auto info_arbre = info_bdd_arbre(arbre_table);
    using unique_donnee = ref_unique_sql<donnee>;
    info_bdd info_donnee(donnee::Name(),table,donnee::Nbr_Att,{unique_donnee::Nbr_Unique});
    info_donnee.set_attribut(donnee::Nom,"nom",b2d::type_attribut_bdd::Text);
    info_donnee.set_attribut(donnee::Type,"tp");
    info_donnee.set_attribut(donnee::Tp_Val,"tpV");
    info_donnee.set_attribut(donnee::Ref,"ref",b2d::type_attribut_bdd::Text,false);
    info_donnee.set_unique(donnee::Ref,unique_donnee::Ref_Unique);
    info_donnee.set_foreign_key(donnee::Id,info_arbre);
    set_type_foreign_key<donnee>(info_donnee);
    if(gestion_donnee)
        set_manager<donnee>(std::make_unique<manager_arbre_modif_controle<donnee>>(info_donnee,info_arbre,std::move(gestion_donnee),
                                                                               std::make_unique<unique_donnee>()));
    else
        set_manager<donnee>(std::make_unique<manager_arbre<donnee>>(info_donnee,info_arbre, std::make_unique<unique_donnee>()));
    set_cible<donnee>(b2d::cible_id::Donnee);

    //donnee_cible
    using unique_cd = cible_num_unique_sql<donnee_cible>;
    info_bdd info_cd(donnee_cible::Name(),cible_table,donnee_cible::Nbr_Att,{unique_cd::Nbr_Unique});
    info_cd.set_attribut(donnee_cible::Id_Donnee,"idDn");
    info_cd.set_attribut(donnee_cible::Id_Cible,"idCb");
    info_cd.set_attribut(donnee_cible::Cible,"cb");
    info_cd.set_attribut(donnee_cible::Date_Time,"dt",b2d::type_attribut_bdd::Date_Time);
    info_cd.set_attribut(donnee_cible::Num,"num");
    info_cd.set_attribut(donnee_cible::Valeur,"val",b2d::type_attribut_bdd::Blob,false);
    info_cd.set_unique(donnee_cible::Id_Donnee,unique_cd::Id_1_Unique);
    info_cd.set_unique(donnee_cible::Id_Cible,unique_cd::Id_Cible_Unique);
    info_cd.set_unique(donnee_cible::Cible,unique_cd::Cible_Unique);
    info_cd.set_unique(donnee_cible::Num,unique_cd::Num_Unique);
    if(gestion_cilbe_donnee)
        set_manager<donnee_cible>(std::make_unique<manager_modif_controle<donnee_cible>>(info_cd,std::move(gestion_cilbe_donnee),
                                                                                    std::make_unique<unique_cd>()));
    else
        set_manager<donnee_cible>(std::make_unique<manager_sql<donnee_cible>>(info_cd, std::make_unique<unique_cd>()));
    set_cible<donnee_cible>(b2d::cible_id::Donnee_Cible);

    //donnee_card
    using unique_card = id_cible_unique_sql<donnee_card>;
    info_bdd info_card(donnee_card::Name(),card_table,donnee_card::Nbr_Att,{unique_card::Nbr_Unique});
    info_card.set_attribut(donnee_card::Id_Donnee,"idDn");
    info_card.set_attribut(donnee_card::Card,"cd");
    info_card.set_attribut(donnee_card::Cible,"cb");
    info_card.set_attribut(donnee_card::Exact,"ex");
    info_card.set_unique(donnee_card::Id_Donnee,unique_card::Id_1_Unique);
    info_card.set_unique(donnee_card::Cible,unique_card::Cible_Unique);
    info_card.set_foreign_key(donnee_card::Id_Donnee,info_donnee);
    if(gestion_card_donnee)
        set_manager<donnee_card>(std::make_unique<manager_modif_controle<donnee_card>>(info_card,std::move(gestion_card_donnee),
                                                                                  std::make_unique<unique_card>()));
    else
        set_manager<donnee_card>(std::make_unique<manager_sql<donnee_card>>(info_card, std::make_unique<unique_card>()));
    set_cible<donnee_card>(b2d::cible_id::Donnee_Card);

    set_ensemble_enable(b2d::Donnee_Enable);
}

void managers_predef::enable_evenement(const QString &table, const QString &cible_table, const QString &table_style) {
    // evenement_style
    using unique_style = nom_unique_sql<evenement_style>;
    info_bdd info_style(evenement_style::Name(),table_style,evenement_style::Nbr_Att,{unique_style::Nbr_Unique});
    info_style.set_attribut(evenement_style::Bordure,"bd");
    info_style.set_attribut(evenement_style::Couleur_Bordure,"cb",b2d::type_attribut_bdd::Text);
    info_style.set_attribut(evenement_style::Couleur_Fond,"cf",b2d::type_attribut_bdd::Text);
    info_style.set_attribut(evenement_style::Couleur_Texte,"cte",b2d::type_attribut_bdd::Text);
    info_style.set_attribut(evenement_style::Couleur_Titre,"cti",b2d::type_attribut_bdd::Text);
    info_style.set_attribut(evenement_style::Forme,"fm");
    info_style.set_attribut(evenement_style::Nom,"nm",b2d::type_attribut_bdd::Text);
    info_style.set_attribut(evenement_style::Police_Texte,"pte",b2d::type_attribut_bdd::Text);
    info_style.set_attribut(evenement_style::Police_Titre,"pti",b2d::type_attribut_bdd::Text);
    info_style.set_attribut(evenement_style::Texture,"tx");
    info_style.set_unique(evenement_style::Nom,unique_style::Nom_Unique);
    set_manager<evenement_style>(std::make_unique<manager_sql<evenement_style>>(info_style, std::make_unique<unique_style>()));
    set_cible<evenement_style>(b2d::cible_id::Evenement_Style);

    // Evenement
    using unique_evenement = nom_unique_sql<evenement>;
    info_bdd info_evenement(evenement::Name(),table,evenement::Nbr_Att,{unique_evenement::Nbr_Unique});
    info_evenement.set_attribut(evenement::Id_Style,"idSt");
    info_evenement.set_attribut(evenement::Code,"cd");
    info_evenement.set_attribut(evenement::Creation,"cr",b2d::type_attribut_bdd::Date_Time);
    info_evenement.set_attribut(evenement::Debut,"deb",b2d::type_attribut_bdd::Date_Time);
    info_evenement.set_attribut(evenement::Fin,"fn",b2d::type_attribut_bdd::Date_Time);
    info_evenement.set_attribut(evenement::Modification,"modif",b2d::type_attribut_bdd::Date_Time);
    info_evenement.set_attribut(evenement::Nc,"nc",b2d::type_attribut_bdd::Text);
    info_evenement.set_attribut(evenement::Nom,"nm",b2d::type_attribut_bdd::Text);
    info_evenement.set_attribut(evenement::Titre,"tii",b2d::type_attribut_bdd::Text);
    info_evenement.set_attribut(evenement::Type,"tp");
    info_evenement.set_unique(evenement::Nom,unique_evenement::Nom_Unique);
    info_evenement.set_foreign_key(evenement::Id_Style,info_style);
    set_type_foreign_key<evenement>(info_evenement);
    set_manager<evenement>(std::make_unique<manager_sql<evenement>>(info_evenement, std::make_unique<unique_evenement>()));
    set_cible<evenement>(b2d::cible_id::Evenement);

    // evenement_cible
    using unique_cible = cible_num_unique_sql<evenement_cible>;
    info_bdd info_cible("evenement_cible",cible_table,evenement_cible::Nbr_Att,{unique_cible::Nbr_Unique});
    info_cible.set_attribut(evenement_cible::Id_Evenement,"idEv");
    info_cible.set_attribut(evenement_cible::Id_Cible,"idCb");
    info_cible.set_attribut(evenement_cible::Cible,"cb");
    info_cible.set_attribut(evenement_cible::Date_Time,"dt",b2d::type_attribut_bdd::Date_Time);
    info_cible.set_attribut(evenement_cible::Num,"num");
    info_cible.set_attribut(evenement_cible::Tp_Val,"tpV");
    info_cible.set_attribut(evenement_cible::Valeur,"val",b2d::type_attribut_bdd::Blob);
    info_cible.set_unique(evenement_cible::Id_Evenement,unique_cible::Id_1_Unique);
    info_cible.set_unique(evenement_cible::Id_Cible,unique_cible::Id_Cible_Unique);
    info_cible.set_unique(evenement_cible::Cible,unique_cible::Cible_Unique);
    info_cible.set_unique(evenement_cible::Num,unique_cible::Num_Unique);
    info_cible.set_foreign_key(evenement_cible::Id_Evenement,info_evenement);
    set_manager<evenement_cible>(std::make_unique<manager_sql<evenement_cible>>(info_cible, std::make_unique<unique_cible>()));
    set_cible<evenement_cible>(b2d::cible_id::Evenement_Cible);

    set_ensemble_enable(b2d::Evenement_Enable);
}

void managers_predef::enable_historique(const QString &table) {
    using unique = cible_simple_num_unique_sql<historique>;
    info_bdd info(historique::Name(),table,historique::Nbr_Att,{unique::Nbr_Unique});
    info.set_attribut(historique::Id_Cible,"idCb");
    info.set_attribut(historique::Id_Etat,"idEt");
    info.set_attribut(historique::Cible,"cb");
    info.set_attribut(historique::Date_Time,"dt",b2d::type_attribut_bdd::Date_Time);
    info.set_attribut(historique::Etat,"et");
    info.set_attribut(historique::Num,"num");
    info.set_unique(historique::Id_Cible,unique::Id_Cible_Unique);
    info.set_unique(historique::Cible,unique::Cible_Unique);
    info.set_unique(historique::Num,unique::Num_Unique);
    set_manager<historique>(std::make_unique<manager_sql<historique>>(info, std::make_unique<unique>()));
    set_cible<historique>(b2d::cible_id::Historique);

    set_ensemble_enable(b2d::Historique_Enable);
}

void managers_predef::enable_mot_cle(const QString &table, const QString &arbre_table, const QString &cible_table,
                                  const QString &prog_cible_table, const QString &permission_table,
                                  const QString &prog_permission_table,
                                  std::unique_ptr<abstract_gestion_restriction<mot_cle>> && gestionmot_cle,
                                  std::unique_ptr<abstract_gestion_restriction<mot_cle_permission>> && gestion_permission,
                                  std::unique_ptr<abstract_gestion_restriction<mot_prog_permission>> && gestion_prog_permission) {
    //Mot Clé
    auto info_arbre = info_bdd_arbre(arbre_table);
    using unique_mc = ref_unique_sql<mot_cle>;
    info_bdd info_mc("mot_cle",table,mot_cle::Nbr_Att,{unique_mc::Nbr_Unique});
    info_mc.set_attribut(mot_cle::Nc,"nc",b2d::type_attribut_bdd::Text);
    info_mc.set_attribut(mot_cle::Nom,"nm",b2d::type_attribut_bdd::Text);
    info_mc.set_attribut(mot_cle::Ref,"ref",b2d::type_attribut_bdd::Text,false);
    info_mc.set_unique(mot_cle::Ref,unique_mc::Ref_Unique);
    info_mc.set_foreign_key(mot_cle::Id,info_arbre);
    if(gestionmot_cle)
        set_manager<mot_cle>(std::make_unique<manager_arbre_modif_controle<mot_cle>>(info_mc,info_arbre,std::move(gestionmot_cle),
                                                                               std::make_unique<unique_mc>()));
    else
        set_manager<mot_cle>(std::make_unique<manager_arbre<mot_cle>>(info_mc,info_arbre,std::make_unique<unique_mc>()));
    set_cible<mot_cle>(b2d::cible_id::Mot_Cle);

    // Cible Mot Clé
    using unique_cible = cible_unique_sql<mot_cle_cible>;
    info_bdd info_cible(mot_cle_cible::Name(),cible_table,mot_cle_cible::Nbr_Att,{unique_cible::Nbr_Unique});
    info_cible.set_attribut(mot_cle_cible::Id_Mot_Cle,"idMC");
    info_cible.set_attribut(mot_cle_cible::Id_Cible,"idCb");
    info_cible.set_attribut(mot_cle_cible::Cible,"cb");
    info_cible.set_unique(mot_cle_cible::Id_Mot_Cle,unique_cible::Id_1_Unique);
    info_cible.set_unique(mot_cle_cible::Id_Cible,unique_cible::Id_Cible_Unique);
    info_cible.set_unique(mot_cle_cible::Cible,unique_cible::Cible_Unique);
    info_cible.set_foreign_key(mot_cle_cible::Id_Mot_Cle,info_mc);
    set_manager<mot_cle_cible>(std::make_unique<manager_sql<mot_cle_cible>>(info_cible, std::make_unique<unique_cible>()));
    set_cible<mot_cle_cible>(b2d::cible_id::Mot_Cle_Cible);

    // Permission Mot Clé
    using unique_permission = id_cible_unique_sql<mot_cle_permission>;
    info_bdd info_permission(mot_cle_permission::Name(),permission_table,mot_cle_permission::Nbr_Att,
        {unique_permission::Nbr_Unique});
    info_permission.set_attribut(mot_cle_permission::Id_Mot_Cle,"idMC");
    info_permission.set_attribut(mot_cle_permission::Cible,"cb");
    info_permission.set_attribut(mot_cle_permission::Code,"cd");
    info_permission.set_unique(mot_cle_permission::Id_Mot_Cle,unique_permission::Id_1_Unique);
    info_permission.set_unique(mot_cle_permission::Cible,unique_permission::Cible_Unique);
    info_permission.set_foreign_key(mot_cle_permission::Id_Mot_Cle,info_mc);
    if(gestion_permission)
        set_manager<mot_cle_permission>(std::make_unique<manager_permission_code_modif_controle<mot_cle_permission>>(
                                            info_permission,
                                            std::move(gestion_permission),
                                            std::make_unique<unique_permission>()));
    else
        set_manager<mot_cle_permission>(std::make_unique<manager_permission_code<mot_cle_permission>>(
                                            info_permission,
                                            std::make_unique<unique_permission>()));
    set_cible<mot_cle_permission>(b2d::cible_id::Mot_Cle_Permission);

    // Cible Mot Prog
    using unique_prog_cible = cible_simple_ref_unique_sql<mot_prog_cible>;
    info_bdd info_prog_cible(mot_prog_cible::Name(),prog_cible_table,mot_prog_cible::Nbr_Att,{unique_prog_cible::Nbr_Unique});
    info_prog_cible.set_attribut(mot_prog_cible::Ref,"ref",b2d::type_attribut_bdd::Text,false);
    info_prog_cible.set_attribut(mot_prog_cible::Id_Mot_Cle,"idMC");
    info_prog_cible.set_attribut(mot_prog_cible::Id_Cible,"idCb");
    info_prog_cible.set_attribut(mot_prog_cible::Cible,"cb");
    info_prog_cible.set_unique(mot_prog_cible::Ref,unique_prog_cible::Ref_Unique);
    info_prog_cible.set_unique(mot_prog_cible::Id_Cible,unique_prog_cible::Id_Cible_Unique);
    info_prog_cible.set_unique(mot_prog_cible::Cible,unique_prog_cible::Cible_Unique);
    info_prog_cible.set_foreign_key(mot_prog_cible::Id_Mot_Cle,info_mc);
    set_manager<mot_prog_cible>(std::make_unique<manager_sql<mot_prog_cible>>(info_prog_cible,
                                                                              std::make_unique<unique_prog_cible>()));
    set_cible<mot_prog_cible>(b2d::cible_id::Mot_Prog_Cible);

    // Permission Mot Prog
    using unique_prog_permission = id_cible_ref_unique_sql<mot_prog_permission>;
    info_bdd info_prog_permission(mot_prog_permission::Name(),prog_permission_table,mot_prog_permission::Nbr_Att,
        {unique_prog_permission::Nbr_Unique});
    info_prog_permission.set_attribut(mot_prog_permission::Ref,"ref",b2d::type_attribut_bdd::Text,false);
    info_prog_permission.set_attribut(mot_prog_permission::Id_Mot_Cle,"idMC");
    info_prog_permission.set_attribut(mot_prog_permission::Cible,"cb");
    info_prog_permission.set_attribut(mot_prog_permission::Code,"cd");
    info_prog_permission.set_unique(mot_prog_permission::Ref,unique_prog_permission::Ref_Unique);
    info_prog_permission.set_unique(mot_prog_permission::Id_Mot_Cle,unique_prog_permission::Id_1_Unique);
    info_prog_permission.set_unique(mot_prog_permission::Cible,unique_prog_permission::Cible_Unique);
    info_prog_permission.set_foreign_key(mot_prog_permission::Id_Mot_Cle,info_mc);
    if(gestion_prog_permission)
        set_manager<mot_prog_permission>(
                    std::make_unique<manager_permission_code_modif_controle<mot_prog_permission>>(
                        info_prog_permission,
                        std::move(gestion_prog_permission),
                        std::make_unique<unique_prog_permission>()));
    else
        set_manager<mot_prog_permission>(std::make_unique<manager_permission_code<mot_prog_permission>>(
                                             info_prog_permission,
                                             std::make_unique<unique_prog_permission>()));
    set_cible<mot_prog_permission>(b2d::cible_id::Mot_Prog_Permission);

    set_ensemble_enable(b2d::Mot_Cle_Enable);
}

void managers_predef::enable_restriction(const QString &table) {
    using unique = cible_simple_unique_sql<restriction>;
    info_bdd info("restriction",table,restriction::Nbr_Att,{unique::Nbr_Unique});
    info.set_attribut(restriction::Id_Cible,"idCb");
    info.set_attribut(restriction::Cible,"cb");
    info.set_attribut(restriction::Code,"cd");
    info.set_unique(restriction::Id_Cible,unique::Id_Cible_Unique);
    info.set_unique(restriction::Cible,unique::Cible_Unique);
    set_manager<restriction>(std::make_unique<manager_permission_code<restriction>>(info,std::make_unique<unique>()));
    set_cible<restriction>(b2d::cible_id::Restriction);

    set_ensemble_enable(b2d::Restriction_Enable);
}

void managers_predef::enable_texte(const QString &table, const QString &cible_table,
                                   const QString &source_table, const QString &texte_source_table) {
    //Texte
    info_bdd info_texte("texte",table,texte::Nbr_Att);
    info_texte.set_attribut(texte::Creation,"crea",b2d::type_attribut_bdd::Date_Time);
    info_texte.set_attribut(texte::Modification,"modif",b2d::type_attribut_bdd::Date_Time);
    info_texte.set_attribut(texte::Texte,"txt",b2d::type_attribut_bdd::Text,false);
    set_manager<texte>(std::make_unique<manager_sql<texte>>(info_texte));
    set_cible<texte>(b2d::cible_id::Texte);

    //texte_cible
    using unique_cible = cible_simple_num_type_unique_sql<texte_cible>;
    info_bdd info_cible(texte_cible::Name(),cible_table,texte_cible::Nbr_Att,{unique_cible::Nbr_Unique});
    info_cible.set_attribut(texte_cible::Id_Texte,"IdTxt");
    info_cible.set_attribut(texte_cible::Id_Cible,"IdCb");
    info_cible.set_attribut(texte_cible::Cible,"cb");
    info_cible.set_attribut(texte_cible::Date_Time,"dt",b2d::type_attribut_bdd::Date_Time);
    info_cible.set_attribut(texte_cible::Num,"num");
    info_cible.set_attribut(texte_cible::Type,"tp");
    info_cible.set_unique(texte_cible::Id_Cible,unique_cible::Id_Cible_Unique);
    info_cible.set_unique(texte_cible::Cible,unique_cible::Cible_Unique);
    info_cible.set_unique(texte_cible::Num,unique_cible::Num_Unique);
    info_cible.set_unique(texte_cible::Type,unique_cible::Type_Unique);
    info_cible.set_foreign_key(texte_cible::Id_Texte,info_texte);
    set_type_foreign_key<texte_cible>(info_cible);
    set_manager<texte_cible>(std::make_unique<manager_sql<texte_cible>>(info_cible, std::make_unique<unique_cible>()));
    set_cible<texte_cible>(b2d::cible_id::Texte_Cible);

    //Source
    using unique_source = nom_unique_sql<source>;
    info_bdd info_source("Source",source_table,source::Nbr_Att,{unique_source::Nbr_Unique});
    info_source.set_attribut(source::Nc,"nc",b2d::type_attribut_bdd::Text);
    info_source.set_attribut(source::Nom,"nm",b2d::type_attribut_bdd::Text);
    info_source.set_attribut(source::Type,"tp");
    info_source.set_unique(source::Nom,unique_source::Nom_Unique);
    set_type_foreign_key<source>(info_source);
    set_manager<source>(std::make_unique<manager_sql<source>>(info_source, std::make_unique<unique_source>()));
    set_cible<source>(b2d::cible_id::Source);

    //source_texte
    using unique_ts = relation_unique_sql<source_texte>;
    info_bdd info_ts(source_texte::Name(),texte_source_table,source_texte::Nbr_Att,{unique_ts::Nbr_Unique});
    info_ts.set_attribut(source_texte::Id_Source,"idSr");
    info_ts.set_attribut(source_texte::Id_Texte,"idTxt");
    info_ts.set_unique(source_texte::Id_Source,unique_ts::Id_1_Unique);
    info_ts.set_unique(source_texte::Id_Texte,unique_ts::Id_2_Unique);
    info_ts.set_foreign_key(source_texte::Id_Source,info_source);
    info_ts.set_foreign_key(source_texte::Id_Texte,info_texte);
    set_manager<source_texte>(std::make_unique<manager_sql<source_texte>>(info_ts, std::make_unique<unique_ts>()));
    set_cible<source_texte>(b2d::cible_id::Source_Texte);

    set_ensemble_enable(b2d::Texte_Enable);
}

void  managers_predef::enable_type(const QString &table, const QString &permission_table,
                std::unique_ptr<abstract_gestion_restriction<type>> &&gestion_type,
                std::unique_ptr<abstract_gestion_restriction<type_permission>> &&gestion_permission) {
    //Type
    using unique_type = ref_nom_parent_unique_sql<type>;
    info_bdd info_type("type",table,type::Nbr_Att,{unique_type::Nbr_Unique_1,unique_type::Nbr_Unique_2});
    info_type.set_attribut(type::Ref,"ref",b2d::type_attribut_bdd::Text,false);
    info_type.set_attribut(type::Parent,"pt",b2d::type_attribut_bdd::Integer,false);
    info_type.set_attribut(type::Nc,"nc",b2d::type_attribut_bdd::Text);
    info_type.set_attribut(type::Nom,"nm",b2d::type_attribut_bdd::Text);
    info_type.set_unique(type::Ref,unique_type::Ref_Unique,unique_type::Ref_Unique_Set);
    info_type.set_unique(type::Nom,unique_type::Nom_Unique,unique_type::Nom_Unique_Set);
    info_type.set_unique(type::Parent,unique_type::Parent_Unique,unique_type::Parent_Unique_Set);
    info_type.set_foreign_key(type::Parent,info_type);
    if(gestion_type)
        set_manager<type>(std::make_unique<manager_arbre_simple_modif_controle<type>>(info_type,std::move(gestion_type),
                                                                                 std::make_unique<unique_type>()));
    else
        set_manager<type>(std::make_unique<manager_arbre_simple<type>>(info_type,std::make_unique<unique_type>()));
    set_cible<type>(b2d::cible_id::Type);

    //Permission
    using unique_permission = id_cible_unique_sql<type_permission>;
    info_bdd info_permission(type_permission::Name(),permission_table,type_permission::Nbr_Att,{unique_permission::Nbr_Unique});
    info_permission.set_attribut(type_permission::Id_Type,"idTp");
    info_permission.set_attribut(type_permission::Cible,"cb");
    info_permission.set_attribut(type_permission::Code,"cd");
    info_permission.set_unique(type_permission::Id_Type,unique_permission::Id_1_Unique);
    info_permission.set_unique(type_permission::Cible,unique_permission::Cible_Unique);
    info_permission.set_foreign_key(type_permission::Id_Type,info_type);
    if(gestion_permission)
        set_manager<type_permission>(
                    std::make_unique<manager_permission_code_modif_controle<type_permission>>(
                        info_permission,std::move(gestion_permission),
                        std::make_unique<unique_permission>()));
    else
        set_manager<type_permission>(std::make_unique<manager_permission_code<type_permission>>(
                                         info_permission,
                                         std::make_unique<unique_permission>()));
    set_cible<type_permission>(b2d::cible_id::Type_Permission);

    set_ensemble_enable(b2d::Type_Enable);
}

void managers_predef::enable_utilisation(const QString &table, const QString &usage_table, const QString &arbre_table,
                                       std::unique_ptr<abstract_gestion_restriction<usage>> && gestion_usage) {
    //Usage
    auto info_arbre = info_bdd_arbre(arbre_table);
    using unique_usage = ref_nom_type_unique_sql<usage>;
    info_bdd info_usage("usage",usage_table,usage::Nbr_Att,{unique_usage::Nbr_Unique_1,unique_usage::Nbr_Unique_2});
    info_usage.set_attribut(usage::Nc,"nc",b2d::type_attribut_bdd::Text);
    info_usage.set_attribut(usage::Nom,"nm",b2d::type_attribut_bdd::Text);
    info_usage.set_attribut(usage::Ref,"ref",b2d::type_attribut_bdd::Text,false);
    info_usage.set_attribut(usage::Type,"tp");
    info_usage.set_unique(usage::Ref,unique_usage::Ref_Unique,unique_usage::Type_Unique_Set);
    info_usage.set_unique(usage::Nom,unique_usage::Nom_Unique,unique_usage::Nom_Unique_Set);
    info_usage.set_unique(usage::Type,unique_usage::Type_Unique,unique_usage::Type_Unique_Set);
    info_usage.set_foreign_key(usage::Id,info_arbre);
    set_type_foreign_key<usage>(info_usage);
    if(gestion_usage)
        set_manager<usage>(std::make_unique<manager_arbre_modif_controle<usage>>(info_usage, info_arbre, std::move(gestion_usage),
                                                                             std::make_unique<unique_usage>()));
    else
        set_manager<usage>(std::make_unique<manager_arbre<usage>>(info_usage,info_arbre, std::make_unique<unique_usage>()));
    set_cible<usage>(b2d::cible_id::Usage);

    //Utilisation
    using unique_util = cible_num_unique_sql<utilisation>;
    info_bdd info_util(utilisation::Name(),table,utilisation::Nbr_Att,{unique_util::Nbr_Unique});
    info_util.set_attribut(utilisation::Id_Usage,"idUs",b2d::type_attribut_bdd::Integer);
    info_util.set_attribut(utilisation::Id_Cible,"idCb");
    info_util.set_attribut(utilisation::Id_Etat,"idEt");
    info_util.set_attribut(utilisation::Cible,"cb");
    info_util.set_attribut(utilisation::Date_Time,"dt",b2d::type_attribut_bdd::Date_Time);
    info_util.set_attribut(utilisation::Etat,"et");
    info_util.set_attribut(utilisation::Num,"num");
    info_util.set_unique(utilisation::Id_Cible,unique_util::Id_Cible_Unique);
    info_util.set_unique(utilisation::Cible,unique_util::Cible_Unique);
    info_util.set_unique(utilisation::Num,unique_util::Num_Unique);
    info_util.set_foreign_key(utilisation::Id_Usage,info_usage);
    set_manager<utilisation>(std::make_unique<manager_sql<utilisation>>(info_util, std::make_unique<unique_util>()));
    set_cible<utilisation>(b2d::cible_id::Utilisation);

    set_ensemble_enable(b2d::Utilisation_Enable);
}
