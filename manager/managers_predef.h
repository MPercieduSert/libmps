/*Auteur: PERCIE DU SERT Maxime
 *Date: 16/08/2019
 */
#ifndef MANAGERS_PREDEF_H
#define MANAGERS_PREDEF_H

#include "manager_arbre_modif_controle.h"
#include "manager_arbre_simple_modif_controle.h"
#include "manager_permission_modif_controle.h"
#include "managers_base.h"
#include "entity_predef.h"

namespace mps {
namespace b2d {
    //! Ensembles prédéfinis pris en charge.
    enum ensemble_manager_flag : flag::flag_type {
        Ensemble_Disable = 0x0,
        Commentaire_Enable = 0x1,
        Donnee_Enable = 0x2,
        Evenement_Enable = 0x4,
        Historique_Enable = 0x8,
        Mot_Cle_Enable = 0x10,
        Restriction_Enable = 0x20,
        Texte_Enable = 0x40,
        Type_Enable = 0x80,
        Utilisation_Enable = 0x100
    };

    //! Associe un numéro de cible.
    namespace cible_id  {
        //! Numéro de cible des types de base.
        enum cible_neg{Configuration = -1};

        //! Numéro de cible des entités prédéfinies.
        enum entity_predef{Commentaire,
                          Commentaire_Cible,
                          Donnee,
                          Donnee_Card,
                          Donnee_Cible,
                          Evenement,
                          Evenement_Cible,
                          Evenement_Style,
                          Historique,
                          Mot_Cle,
                          Mot_Cle_Cible,
                          Mot_Cle_Permission,
                          Mot_Prog_Cible,
                          Mot_Prog_Permission,
                          Restriction,
                          Source,
                          Texte,
                          Texte_Cible,
                          Source_Texte,
                          Type,
                          Type_Permission,
                          Utilisation,
                          Usage,
                          Nbr_Cible_Predef};
    }

    namespace nature_id {
        //! Idantifiant de nature des types de base.
        enum type_base {Vide = -1,
              Int = -2,
              String = -3,
              Bool = -4,
              Float = -5,
              Double = -6,
              Date = -7,
              Date_Time = -8,
              Variant = -9};

        //! Idantifiant de nature des entités prédéfinies.
        enum entity_predef{Commentaire,
                          Commentaire_Cible,
                          Donnee,
                          Donnee_Card,
                          Donnee_Cible,
                          Evenement,
                          Evenement_Cible,
                          Evenement_Style,
                          Historique,
                          Mot_Cle,
                          Mot_Cle_Cible,
                          Mot_Cle_Permission,
                          Mot_Prog_Cible,
                          Mot_Prog_Permission,
                          Restriction,
                          Source,
                          Texte,
                          Texte_Cible,
                          Source_Texte,
                          Type,
                          Type_Permission,
                          Utilisation,
                          Usage,
                          Nbr_Cible_Predef};
    }
}

namespace manager {
/*! \ingroup groupe_file
 * \brief Classe mère des managers utilisant des entités prédéfinies.
 */
class managers_predef : public managers_base {
protected:
    flag m_ensemble_enable = b2d::Ensemble_Disable;  //!< Prise en charge d'ensembles.
    std::vector<int> m_cible_array;                  //!< Tableau de correspondance ID -> Cible.
    std::vector<int> m_cible_nbr_att_array;          //!< Tableau de correspondance cible -> nombre d'attributs.

public:
    //! Constructeur.
    managers_predef(entidt nbr_entity, entidt nbr_cible, const QString &version_table,
                    std::map<entidt,std::unique_ptr<abstract_manager>> &&managers
                        = std::map<entidt,std::unique_ptr<abstract_manager>>(),
                    const QSqlQuery &req = QSqlQuery())
        : managers_base (nbr_entity,version_table,std::move(managers),req),
          m_cible_array(nbr_entity,b2d::nature_id::Vide),
          m_cible_nbr_att_array(nbr_cible,0) {}

    //! Destructeur.
    ~managers_predef() = default;

    template<class Ent> int cible() const
        {return m_cible_array[Ent::ID];}

    int cible(entidt id) const
        {return id < m_nbr_entity ? m_cible_array[id] : b2d::nature_id::Vide;}

    //! Renvoie le nombre d'attributs d'une entité à partir du numéro de cible.
    int nbr_att_cible(int cible) const {
        if(cible == b2d::nature_id::Vide || cible >= static_cast<int>(m_cible_nbr_att_array.size()))
            return 0;
        if(cible < b2d::nature_id::Vide)
            return 1;
        return m_cible_nbr_att_array[static_cast<unsigned>(cible)];
    }

    //! Teste la prise en charge d'un ensemble donnée
    bool ensemble_enable(b2d::ensemble_manager_flag flag) const noexcept
        {return m_ensemble_enable.test(flag);}

protected:
    //! Mise en place de la prise en charge des commentaires.
    void enable_commentaire(const QString &table, const QString &cible_table);
    
    //! Mise en place de la prise en charge des donnees.
    void enable_donnee(const QString &table, const QString &arbre_table, const QString &cible_table, const QString &card_table,
                      std::unique_ptr<abstract_gestion_restriction<entities::donnee>> && gestion_donnee
                        = std::make_unique<abstract_gestion_restriction<entities::donnee>>(),
                      std::unique_ptr<abstract_gestion_restriction<entities::donnee_cible>> && gestion_cilbe_donnee
                        = std::make_unique<abstract_gestion_restriction<entities::donnee_cible>>(),
                      std::unique_ptr<abstract_gestion_restriction<entities::donnee_card>> && gestion_card_donnee
                        = std::make_unique<abstract_gestion_restriction<entities::donnee_card>>());

    //! Mise en place de la prise en charge des evenements.
    void enable_evenement(const QString &table, const QString &cible_table, const QString &table_style);

    //! Mise en place de la prise en charge des historiques.
    void enable_historique(const QString &table);

    //! Mise en place de la prise en charge des mots Clé.
    void enable_mot_cle(const QString &table, const QString &arbre_table, const QString &cible_table,
                      const QString &prog_cible_table, const QString &permission_table, const QString &prog_permission_table,
                      std::unique_ptr<abstract_gestion_restriction<entities::mot_cle>> && gestionmot_cle
                        = std::make_unique<abstract_gestion_restriction<entities::mot_cle>>(),
                      std::unique_ptr<abstract_gestion_restriction<entities::mot_cle_permission>> && gestion_permission
                        = std::make_unique<abstract_gestion_restriction<entities::mot_cle_permission>>(),
                      std::unique_ptr<abstract_gestion_restriction<entities::mot_prog_permission>> && gestion_prog_permission
                        = std::make_unique<abstract_gestion_restriction<entities::mot_prog_permission>>());

    //! Mise en place de la prise en charge des restrictions de modification.
    void enable_restriction(const QString &table);

    //! Mise en place de la prise en charge des textes.
    void enable_texte(const QString &table, const QString &cible_table,
                      const QString &source_table, const QString &texte_source_table);

    //! Mise en place de la prise en charge des types.
    void enable_type(const QString &table, const QString &permission_table,
                    std::unique_ptr<abstract_gestion_restriction<entities::type>> && gestion_type
                        = std::make_unique<abstract_gestion_restriction<entities::type>>(),
                    std::unique_ptr<abstract_gestion_restriction<entities::type_permission>> && gestion_permission
                        = std::make_unique<abstract_gestion_restriction<entities::type_permission>>());

    //! Mise en place de la prise en charge des utilisations.
    void enable_utilisation(const QString &table, const QString &usage_table, const QString &arbre_table,
                           std::unique_ptr<abstract_gestion_restriction<entities::usage>> &&gestion_usage
                            = std::make_unique<abstract_gestion_restriction<entities::usage>>());

    //! Mutateur du tableau de correspondance ID -> Cible.
    template<class Ent> void set_cible(int cible) {
        m_cible_array[Ent::ID] = cible;
        m_cible_nbr_att_array[cible] = Ent::Nbr_Att;
    }

    void set_ensemble_enable(b2d::ensemble_manager_flag flag) noexcept
        {m_ensemble_enable |= flag;}

    //! Si les types sont pris en charge, déclare l'attribut type comme clé étrangère sur la table Type.
    template<class Ent> void set_type_foreign_key(info_bdd &info);
};

template<> inline int managers_predef::cible<int>() const
    {return b2d::nature_id::Int;}

template<> inline int managers_predef::cible<QString>() const
    {return b2d::nature_id::String;}

template<> inline int managers_predef::cible<bool>() const
    {return b2d::nature_id::Bool;}

template<> inline int managers_predef::cible<float>() const
    {return b2d::nature_id::Float;}

template<> inline int managers_predef::cible<double>() const
    {return b2d::nature_id::Double;}

template<> inline int managers_predef::cible<QDate>() const
    {return b2d::nature_id::Date;}

template<> inline int managers_predef::cible<QDateTime>() const
    {return b2d::nature_id::Date_Time;}

template<> inline int managers_predef::cible<QVariant>() const
    {return b2d::nature_id::Variant;}

template<class Ent> void managers_predef::set_type_foreign_key(info_bdd &info) {
    if(ensemble_enable(b2d::Type_Enable))
        info.set_foreign_key(Ent::Type,get(entities::type::ID).info());
}
}}
#endif // MANAGERS_PREDEF_H
