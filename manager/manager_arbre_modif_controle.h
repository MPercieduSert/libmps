/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/08/2018
 */
#ifndef MANAGER_ARBRE_MODIF_CONTROLE_H
#define MANAGER_ARBRE_MODIF_CONTROLE_H

#include "manager_modif_controle.h"
#include "manager_arbre.h"

namespace mps {
namespace manager {
/*! \ingroup groupeManager
 *\brief Classe template mère des différents manageurs pour les entités de type arbre avec un controle de modification.
 */
template<class Ent> class manager_arbre_modif_controle
        : public manager_arbre<Ent>, public manager_modif_controle<Ent> {
protected:
    using ma_arb = manager_arbre<Ent>;
    using ma_mc = manager_modif_controle<Ent>;

    using ma_arb::add;
    using ma_mc::fermer;
    using ma_mc::modify;
    using ma_mc::ouvrir;

public:
    using ma_arb::get;
    using ma_mc::get_restriction;
    using ma_mc::test_restriction;

    //! Constructeur.
    manager_arbre_modif_controle(const info_bdd &info, const info_bdd &info_arbre,
                                std::unique_ptr<abstract_gestion_restriction<Ent>> &&gestion_autorisation
                                            = std::make_unique<abstract_gestion_restriction<Ent>>(),
                                std::unique_ptr<abstract_unique_sql_temp<Ent>> &&unique
                                            = std::make_unique<no_unique_sql<Ent>>());

    //! Destructeur.
    ~manager_arbre_modif_controle() override = default;

    //! Supprime de la table en base de donnée l'entité d'identifiant id.
    bool del(idt id) override
        {return !test_restriction(id,b2d::Suppr) &&ma_arb::del(id);}

    //! Enregistre l'entité entity en base de donnée.
    void save(Ent &ent) override
        {ma_arb::save(ent);}

    //! Enregistre l'entité entity en base de donnée.
    void save(const Ent &ent) override
        {ma_arb::save(ent);}

    //! Retourne le type du manager.
    virtual flag type_manager() const
        {return ma_arb::type_manager() | ma_mc::type_manager();}

protected:
    //! Constructeur.
    manager_arbre_modif_controle(const info_bdd &info_arbre,
                                 std::unique_ptr<abstract_gestion_restriction<Ent>> &&gestion_autorisation
                                    = std::make_unique<abstract_gestion_restriction<Ent>>());
};

template<class Ent> manager_arbre_modif_controle<Ent>::manager_arbre_modif_controle(const info_bdd &info, const info_bdd &info_arbre,
                                                      std::unique_ptr<abstract_gestion_restriction<Ent>> &&gestion_autorisation,
                                                      std::unique_ptr<abstract_unique_sql_temp<Ent>> &&unique)
    : manager_sql<Ent>(info,std::move(unique)), manager_arbre<Ent>(info_arbre),
      manager_modif_controle<Ent>(std::move(gestion_autorisation)) {}

template<class Ent> manager_arbre_modif_controle<Ent>::manager_arbre_modif_controle(const info_bdd &info_arbre,
                                std::unique_ptr<abstract_gestion_restriction<Ent>> &&gestion_autorisation)
    : manager_arbre<Ent>(info_arbre), manager_modif_controle<Ent>(std::move(gestion_autorisation)) {}
}}
#endif // MANAGER_ARBRE_MODIF_CONTROLE_H
