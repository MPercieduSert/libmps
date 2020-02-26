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
    using ManagerArb = ManagerArbreSimple<Ent>;
    using ManagerMC = ManagerModifControle<Ent>;

    using ManagerMC::modify;

public:
    using ManagerArb::get;
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

    //! Supprime de la table en base de donnée l'entité d'identifiant id.
    bool del(idt id) override
        {return getAutorisation(id, bmps::Suppr) && ManagerArb::del(id);}

    //! Renvoie la liste des restrictions gérer par le gestionérer d'autorisarion.
    std::vector<bmps::autorisation> restriction() const override
        {return ManagerModifControle<Ent>::restriction();}

    //! Enregistre l'entité entity en base de donnée.
    void save(Ent & entity) override
        {ManagerArb::save(entity);}

    //! Enregistre l'entité entity en base de donnée.
    void save(const Ent & entity) override
        {ManagerArb::save(entity);}

    //! Retourne le type du manager.
    virtual unsigned typeManager() const
        {return ManagerArb::typeManager() | ManagerMC::typeManager();}

protected:
    //! Constructeur.
    ManagerArbreSimpleModifControle(std::unique_ptr<AbstractGestionAutorisation<Ent>> && gestionAutorisation
                                    = std::make_unique<GestionAutorisationNoRestrictif<Ent>>())
        : ManagerModifControle<Ent>(std::move(gestionAutorisation))
    {}
};
}
#endif // MANAGERARBRESIMPLEMODIFCONTROLE_H
