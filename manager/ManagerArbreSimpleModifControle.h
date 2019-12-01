/*Auteur: PERCIE DU SERT Maxime
 *Date: 14/08/2017
 */
#ifndef MANAGERARBRESIMPLEMODIFCONTROLE_H
#define MANAGERARBRESIMPLEMODIFCONTROLE_H

#include "ManagerModifControle.h"
#include "ManagerArbreSimple.h"

namespace managerMPS {
/*! \ingroup groupeManager
 * \brief Classe template mère des différents manageurs pour les entités de type arbre simple avec un controle de modification.
 */
template<class Ent> class ManagerArbreSimpleModifControle : public ManagerArbreSimple<Ent>,
                                                                      public ManagerModifControle<Ent> {
protected:
    using ManagerForArbreEnt = ManagerArbreSimple<Ent>;
    using ManagerMC = ManagerModifControle<Ent>;

    using ManagerMC::modify;

public:
    using ManagerForArbreEnt::get;
    using ManagerMC::save;
    using ManagerMC::getAutorisation;

    //! Constructeur.
    ManagerArbreSimpleModifControle(const InfoBdd & info,
                                    std::unique_ptr<AbstractGestionAutorisation<Ent>> && gestionAutorisation
                                                = std::make_unique<GestionAutorisationNoRestrictif<Ent>>(),
                                      std::unique_ptr<AbstractUniqueSqlTemp<Ent>> && unique = std::make_unique<NoUniqueSql<Ent>>())
        : ManagerSql<Ent>(info,std::move(unique)),
          ManagerModifControle<Ent>(std::move(gestionAutorisation)) {}

    //! Destructeur.
    ~ManagerArbreSimpleModifControle() override = default;

    /*//! Supprime de la table en base de donnée l'entité d'identifiant id.
    bool del(idt id) override
        {return getAutorisation(Ent(id), bmps::Suppr) && ManagerForArbreEnt::del(id);}*/

    //! Enregistre l'entité entity en base de donnée.
    void save(Ent & entity) override
        {ManagerForArbreEnt::save(entity);}

    //! Enregistre l'entité entity en base de donnée.
    void save(const Ent & entity) override
        {ManagerForArbreEnt::save(entity);}

protected:
    //! Constructeur.
    ManagerArbreSimpleModifControle(std::unique_ptr<AbstractGestionAutorisation<Ent>> && gestionAutorisation
                                    = std::make_unique<GestionAutorisationNoRestrictif<Ent>>())
        : ManagerModifControle<Ent>(std::move(gestionAutorisation))
    {}
};
}
#endif // MANAGERARBRESIMPLEMODIFCONTROLE_H