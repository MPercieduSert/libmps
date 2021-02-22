/*Auteur: PERCIE DU SERT Maxime
 *Date: 14/08/2017
 */
#ifndef MANAGER_ARBRE_SIMPLE_MODIF_CONTROLE_H
#define MANAGER_ARBRE_SIMPLE_MODIF_CONTROLE_H

#include "manager_modif_controle.h"
#include "manager_arbre_simple.h"

namespace mps {
namespace manager {
/*! \ingroup groupeManager
 *\brief Classe template mère des différents manageurs pour les entités de type arbre simple avec un controle de modification.
 */
template<class Ent> class manager_arbre_simple_modif_controle
        : public manager_arbre_simple<Ent>, public manager_modif_controle<Ent> {
protected:
    using ma_arb = manager_arbre_simple<Ent>;
    using ma_mc = manager_modif_controle<Ent>;

    using ma_mc::modify;

public:
    using ma_arb::get;
    using ma_mc::test_restriction;

    //! Constructeur.
    manager_arbre_simple_modif_controle(const info_bdd &info,
                                        std::unique_ptr<abstract_gestion_restriction<Ent>> &&gestion_autorisation
                                            = std::make_unique<abstract_gestion_restriction<Ent>>(),
                                        std::unique_ptr<abstract_unique_sql_temp<Ent>> &&unique
                                            = std::make_unique<no_unique_sql<Ent>>())
        : manager_sql<Ent>(info,std::move(unique)),
          manager_modif_controle<Ent>(std::move(gestion_autorisation)) {}

    //! Destructeur.
    ~manager_arbre_simple_modif_controle() override = default;

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
    manager_arbre_simple_modif_controle(std::unique_ptr<abstract_gestion_restriction<Ent>> &&gestion_autorisation
                                    = std::make_unique<abstract_gestion_restriction<Ent>>())
        : manager_modif_controle<Ent>(std::move(gestion_autorisation))
    {}
};
}}
#endif // MANAGERARBRESIMPLEMODIFCONTROLE_H
