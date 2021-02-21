/*Auteur: PERCIE DU SERT Maxime
 *Date: 14/08/2017
 */
#ifndef MANAGERARBRESIMPLEMODIFCONTROLE_H
#define MANAGERARBRESIMPLEMODIFCONTROLE_H

#include "ManagerModifControle.h"
#include "ManagerarbreSimple.h"

namespace managerMPS {
/*! \ingroup groupeManager
 *\brief Classe template mère des différents manageurs pour les entités de type arbre simple avec un controle de modification.
 */
template<class Ent> class ManagerarbreSimpleModifControle : public ManagerarbreSimple<Ent>,
                                                                      public ManagerModifControle<Ent> {
protected:
    using ManagerArb = ManagerarbreSimple<Ent>;
    using ManagerMC = ManagerModifControle<Ent>;

    using ManagerMC::modify;

public:
    using ManagerArb::get;
    using ManagerMC::testRestriction;

    //! Constructeur.
    ManagerarbreSimpleModifControle(const InfoBdd &info,
                                    std::unique_ptr<AbstractGestionRestriction<Ent>> &&gestionAutorisation
                                                = std::make_unique<AbstractGestionRestriction<Ent>>(),
                                      std::unique_ptr<AbstractUniqueSql_temp<Ent>> &&unique = std::make_unique<NoUniqueSql<Ent>>())
        : ManagerSql<Ent>(info,std::move(unique)),
          ManagerModifControle<Ent>(std::move(gestionAutorisation)) {}

    //! Destructeur.
    ~ManagerarbreSimpleModifControle() override = default;

    //! Supprime de la table en base de donnée l'entité d'identifiant id.
    bool del(idt id) override
        {return !testRestriction(id,bddMPS::Suppr) &&ManagerArb::del(id);}

    //! Enregistre l'entité entity en base de donnée.
    void save(Ent &ent) override
        {ManagerArb::save(entity);}

    //! Enregistre l'entité entity en base de donnée.
    void save(const Ent &ent) override
        {ManagerArb::save(entity);}

    //! Retourne le type du manager.
    virtual flag typeManager() const
        {return ManagerArb::typeManager() | ManagerMC::typeManager();}

protected:
    //! Constructeur.
    ManagerarbreSimpleModifControle(std::unique_ptr<AbstractGestionRestriction<Ent>> &&gestionAutorisation
                                    = std::make_unique<AbstractGestionRestriction<Ent>>())
        : ManagerModifControle<Ent>(std::move(gestionAutorisation))
    {}
};
}
#endif // MANAGERARBRESIMPLEMODIFCONTROLE_H
