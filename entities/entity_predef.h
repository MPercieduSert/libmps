/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef ENTITY_PREDEF_H
#define ENTITY_PREDEF_H

#include "entity_cible.h"
#include "entity_divers.h"
#include "entity_relation.h"


/*! \ingroup groupe_entity
 *\brief Espace de noms contenant les enumérations sur les données.
 */
namespace donnee_info {
    //! Différents catégories de données.
    enum typeDn {Alpha = 0,
                Perso,
                ProgramationInterne,
                Contenu};

    //! Différents types des valeurs des données
    enum typeVal {No_Donnee = 0,
                  Int = 1,
                  String = 2,
                  Bool = 3,
                  Float = 4,
                  Double = 5,
                  Date = 6,
                  Date_Time = 7};

    //! Exact
    enum cardinalCode {Exact,
                       AuPlus,
                       AuMoins,
    };
}

namespace mps {
namespace entities {

// Entités de type prédéfinies
/*! \ingroup groupe_entity
 *\brief Ensemble des classes de la fonctionalité commentaire.
 */
namespace ensemble_commentaire {
using commentaire = entities_base::texte_entity<info_entity::Commentaire>;
ID1_ENTITY(commentaire_cible,cible_date_time_current_num,info_entity::Commentair_Cible,Commentaire,commentaire)
}
using namespace ensemble_commentaire;

/*! \ingroup groupe_entity
 *\brief Ensemble des classes de la fonctionalité donnee.
 */
namespace ensemble_donnee {
/*! \ingroup groupe_entity
 *\brief Représentation de l'entité donnee.
 */
class donnee : public entity_ids<info_entity::donnee,attribut::nom_type_attribut,
                                                     attribut::ref_attribut,
                                                     attribut::tp_val_attribut> {
protected:
    template<class T> using position_enum = position_enum<T,donnee>;
public:
    using eaid = entity_ids<info_entity::donnee,attribut::nom_type_attribut,
                                                attribut::ref_attribut,
                                                attribut::tp_val_attribut>;

    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut>::Position,
                   Nom = position_enum<nom_attribut>::Position,
                   Type = position_enum<type_attribut>::Position,
                   Ref = position_enum<ref_attribut>::Position,
                   Tp_Val = position_enum<tp_val_attribut>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    //! Identifiant des donnée hors de l'arbre.
    enum id_neg_donne_prog{
        Id_Prog_IdND = -1,      // permet de retrouver l'identifiant d'une entité grâce à un identifiant de programmation.
        Entity_IdND = -2,       // enregistrement d'entité en tant que donnée.
        Entity_Nature_IdND = -3,// contient la nature (sous forme numéro de cilbe) de l'entité enregistrée.
        Entity_Card_IdND = -4   // contient le cardinal maximale d'entitée pouvant être enregistrée.
    };

    using eaid::entity_id;
    BASE_ENTITY(donnee)

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    donnee(const QString &ref)
        {set_ref(ref);}

    //! Constructeur à partir des valeurs attributs.
    donnee(const QString &nom, idt type, int tp_val, idt id = 0)
        : eaid(id) {
        set_nom(nom);
        set_type(type);
        set_tp_val(tp_val);
    }

    donnee(const QString &nom, const QString &ref, idt type, int tp_val, idt id = 0)
        : donnee(nom,type,tp_val,id)
    {set_ref(ref);}
};

/*! \ingroup groupe_entity
 *\brief Représentation de l'entité donnee_card.
 */
class donnee_card : public entity_ids<info_entity::Donnee_Card,attribut::id_1_neg_attribut,
                                                              attribut::card_attribut,
                                                              attribut::exact_attribut,
                                                              attribut::cible_attribut> {
protected:
    template<class T> using position_enum = position_enum<T,donnee_card>;
public:
    using eaid = entity_ids<info_entity::Donnee_Card,attribut::id_1_neg_attribut,
                                                            attribut::card_attribut,
                                                            attribut::exact_attribut,
                                                            attribut::cible_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut>::Position,
                   Id_1 = position_enum<id_1_neg_attribut>::Position,
                   Card = position_enum<card_attribut>::Position,
                   Cible = position_enum<cible_attribut>::Position,
                   Exact = position_enum<exact_attribut>::Position,
                   Nbr_Att = eaid::Nbr_Att,
                   Id_Donnee = Id_1,};

    //! Cardinal.
    enum cardinal {Infini = 0};


    using eaid::entity_id;
    BASE_ENTITY(donnee_card)
    ALIAS_CLE_NEG(donnee,1)

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    donnee_card(int id_dn, int cible, idt id = 0)
        : eaid(id) {
        set_id_donnee(id_dn);
        set_cible(cible);
    }

    //! Constructeur à partir des valeurs attributs.
    donnee_card(int id_dn, int card, int cible, flag exact = 0, idt id = 0)
        : donnee_card(id_dn, cible, id) {
        set_card(card);
        set_exact(exact);
    }
};
ID1_ENTITY_NEG(donnee_cible,cible_neg_date_time_current_num_valeur_variant,info_entity::donnee_Cible,Donnee,donnee)
}
using namespace ensemble_donnee;

/*! \ingroup groupe_entity
 *\brief Ensemble des classes de la fonctionalité événement.
 */
namespace ensemble_evenement {
ID1_ENTITY(evenement_cible,cible_date_time_current_num_tp_val_valeur_variant,info_entity::Evenemen_Cible,Evenement,evenement)
/*! \ingroup groupe_entity
 *\brief Représentation de l'entité Evenement.
 */
class evenement : public entity_ids<info_entity::Evenement, attribut::id_1_attribut,
                                                            attribut::nc_nom_type_attribut,
                                                            attribut::code_attribut,
                                                            attribut::debut_fin_attribut,
                                                            attribut::creation_modification_attribut,
                                                            attribut::titre_attribut> {
protected:
    template<class T> using position_enum = position_enum<T,evenement>;
public:
    //! Code.
    enum code_flag : flag::flag_type {
        Aucun = 0x0,
        UnJour = 0x1,
        JourEntier = 0x2
    };
    using eaid = entity_ids<info_entity::Evenement, attribut::id_1_attribut,
                                                    attribut::nc_nom_type_attribut,
                                                    attribut::code_attribut,
                                                    attribut::debut_fin_attribut,
                                                    attribut::creation_modification_attribut,
                                                    attribut::titre_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut>::Position,
                       Id_1 = position_enum<id_1_attribut>::Position,
                       Code = position_enum<code_attribut>::Position,
                       Creation = position_enum<creation_attribut>::Position,
                       Debut = position_enum<debut_attribut>::Position,
                       Fin = position_enum<fin_attribut>::Position,
                       Modification = position_enum<modification_attribut>::Position,
                       Nc = position_enum<nc_attribut>::Position,
                       Nom = position_enum<nom_attribut>::Position,
                       Titre = position_enum<titre_attribut>::Position,
                       Type = position_enum<type_attribut>::Position,
                       Nbr_Att = eaid::Nbr_Att,
                       Id_Style = Id_1};
    using eaid::entity_id;
    BASE_ENTITY(evenement)
    ALIAS_CLE(style,1)

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    evenement(const QString &nom, idt id = 0)
        : eaid(id)
        {set_nom(nom);}

    //! Constructeur à partir des valeurs attributs.
    evenement(idt id_style, flag code, const QDateTime &creation, const QDateTime &debut, const QDateTime &fin,
              const QDateTime &modification, const QString &nc, const QString &nom, const QString &titre, idt type, idt id = 0)
        : evenement(nom,id) {
        set_id_style(id_style);
        set_code(code);
        set_creation(creation);
        set_debut(debut);
        set_fin(fin);
        set_modification(modification);
        set_nc(nc);
        set_nom(nom);
        set_titre(titre);
        set_type(type);
    }

    //! Teste si l'entité est valide.
    bool is_valid() const override
        {return eaid::is_valid() && debut() <= fin();}

};
/*! \ingroup groupe_entity
 *\brief Représentation de l'entité evenement_style.
 */
class evenement_style : public entity_ids<info_entity::Evenement_Style, attribut::bordure_attribut,
                                                                      attribut::couleur_bordure_attribut,
                                                                      attribut::couleur_fond_attribut,
                                                                      attribut::couleur_texte_attribut,
                                                                      attribut::couleur_titre_attribut,
                                                                      attribut::forme_attribut,
                                                                      attribut::nom_attribut,
                                                                      attribut::police_texte_attribut,
                                                                      attribut::police_titre_attribut,
                                                                      attribut::texture_attribut>{
protected:
    template<class T> using position_enum = position_enum<T,evenement_style>;
public:
    enum {Id_Style_Default = 1};
    //! Forme.
    enum forme_style {Rectangle,
                      Nbr_Forme_Style};
    using eaid = entity_ids<info_entity::Evenement_Style,attribut::bordure_attribut,
                                                        attribut::couleur_bordure_attribut,
                                                        attribut::couleur_fond_attribut,
                                                        attribut::couleur_texte_attribut,
                                                        attribut::couleur_titre_attribut,
                                                        attribut::forme_attribut,
                                                        attribut::nom_attribut,
                                                        attribut::police_texte_attribut,
                                                        attribut::police_titre_attribut,
                                                        attribut::texture_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut>::Position,
                       Bordure = position_enum<bordure_attribut>::Position,
                       Couleur_Bordure = position_enum<couleur_bordure_attribut>::Position,
                       Couleur_Fond = position_enum<couleur_fond_attribut>::Position,
                       Couleur_Texte = position_enum<couleur_texte_attribut>::Position,
                       Couleur_Titre = position_enum<couleur_titre_attribut>::Position,
                       Forme = position_enum<forme_attribut>::Position,
                       Nom = position_enum<nom_attribut>::Position,
                       Police_Texte = position_enum<police_texte_attribut>::Position,
                       Police_Titre = position_enum<police_titre_attribut>::Position,
                       Texture = position_enum<texture_attribut>::Position,
                       Nbr_Att = eaid::Nbr_Att};
    using eaid::entity_id;
    BASE_ENTITY(evenement_style)

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    evenement_style(const QString &nom, idt id = 0)
        : eaid(id)
        {set_nom(nom);}

    //! Constructeur à partir des valeurs attributs.
    evenement_style(int bordure, const QString &couleur_bordure, const QString &couleur_fond,
                   const QString &couleur_texte, const QString &couleur_titre,
                   int forme, const QString &nom, const QString &police_texte, const QString &police_titre,
                    enumt texture, idt id = 0)
        : evenement_style(nom,id) {
        set_bordure(bordure);
        set_couleur_bordure(couleur_bordure);
        set_couleur_fond(couleur_fond);
        set_couleur_texte(couleur_texte);
        set_couleur_titre(couleur_titre);
        set_forme(forme);
        set_police_texte(police_texte);
        set_police_titre(police_titre);
        set_texture(texture);
    }

    //! Converti un entier en string.
    static QString forme_string(int forme);
};
}
using namespace ensemble_evenement;

/*! \ingroup groupe_entity
 *\brief Ensemble des classes de la fonctionalité historique.
 */
namespace ensemble_historique {
using historique = entities_base::historique_entity<info_entity::Historique>;
}
using namespace ensemble_historique;

/*! \ingroup groupe_entity
 *\brief Ensemble des classes de la fonctionalité mot clé.
 */
namespace ensemble_mot_cle {
using mot_cle = entities_base::nc_nom_ref_entity<info_entity::Mot_Cle>;
ID1_ENTITY(mot_cle_cible,cible,info_entity::Mot_Cle_Cible,Mot_Cle,mot_cle)
ID1_ENTITY(mot_cle_permission,id_cible_code,info_entity::Mot_Cle_Permission,Mot_Cle,mot_cle)
ID1_ENTITY(mot_prog_cible,cible_ref,info_entity::Mot_Prog_Cible,Mot_Cle,mot_cle)
ID1_ENTITY(mot_prog_permission,id_cible_code_ref,info_entity::Mot_Prog_Permission,Mot_Cle,mot_cle)
}
using namespace ensemble_mot_cle;

/*! \ingroup groupe_entity
 *\brief Ensemble des classes de la fonctionalité restriction.
 */
namespace ensemble_restriction {
ID1_ENTITY(restriction,id_cible_code,info_entity::Restriction,Cible,cible)
}
using namespace ensemble_restriction;

/*! \ingroup groupe_entity
 *\brief Ensemble des classes de la fonctionalité texte.
 */
namespace ensemble_texte {
using source = entities_base::nc_nom_type_entity<info_entity::Source>;
RELATION_ENTITY(source_texte,relation,info_entity::source_texte,Source,source,Texte,texte)
using texte = entities_base::texte_entity<info_entity::Texte>;
ID1_ENTITY(texte_cible,cible_date_time_current_num_type,info_entity::Texte_Cible,Texte,texte)
}
using namespace ensemble_texte;

/*! \ingroup groupe_entity
 *\brief Ensemble des classes de la fonctionalité type.
 */
namespace ensemble_type {
using type = entities_base::arbre_simple_nc_nom_ref_entity<info_entity::Type>;
ID1_ENTITY(type_permission,id_cible_code,info_entity::Type_Permission,Type,type)
}
using namespace ensemble_type;

/*! \ingroup groupe_entity
 *\brief Ensemble des classes de la fonctionalité utilisation.
 */
namespace ensemble_utilisation {
using usage = entities_base::nc_nom_ref_type_entity<info_entity::Usage>;
ID1_ENTITY(utilisation,utilisation,info_entity::Utilisation,Usage,usage)
}
using namespace ensemble_utilisation;
}}
#endif // ENTITY_PREDEF_H
