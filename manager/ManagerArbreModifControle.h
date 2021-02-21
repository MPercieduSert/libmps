/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/08/2018
 */
#ifndef MANAGERARBREMODIFCONTROLE_H
#define MANAGERARBREMODIFCONTROLE_H

#include "ManagerModifControle.h"
#include "Managerarbre.h"

namespace managerMPS {
/*! \ingroup groupeManager
 *\brief Classe template mère des différents manageurs pour les entités de type arbre avec un controle de modification.
 */
template<class Ent> class ManagerarbreModifControle : public Managerarbre<Ent>,
                                                        public ManagerModifControle<Ent> {
protected:
    using ManagerArb = Managerarbre<Ent>;
    using ManagerMC = ManagerModifControle<Ent>;

    using ManagerArb::add;
    using ManagerMC::fermer;
    using ManagerMC::modify;
    using ManagerMC::ouvrir;

public:
    using ManagerArb::get;
    using ManagerMC::getRestriction;
    using ManagerMC::testRestriction;

    //! Constructeur.
    ManagerarbreModifControle(const InfoBdd &info, const InfoBdd &infoarbre,
                                std::unique_ptr<AbstractGestionRestriction<Ent>> &&gestionAutorisation
                                            = std::make_unique<AbstractGestionRestriction<Ent>>(),
                                std::unique_ptr<AbstractUniqueSql_temp<Ent>> &&unique = std::make_unique<NoUniqueSql<Ent>>());

    //! Destructeur.
    ~ManagerarbreModifControle() override = default;

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
    ManagerarbreModifControle(const InfoBdd &Infoarbre, std::unique_ptr<AbstractGestionRestriction<Ent>> &&gestionAutorisation
                                                            = std::make_unique<AbstractGestionRestriction<Ent>>());
};

template<class Ent> ManagerarbreModifControle<Ent>::ManagerarbreModifControle(const InfoBdd &info, const InfoBdd &infoarbre,
                                                      std::unique_ptr<AbstractGestionRestriction<Ent>> &&gestionAutorisation,
                                                      std::unique_ptr<AbstractUniqueSql_temp<Ent>> &&unique)
    : ManagerSql<Ent>(info,std::move(unique)), Managerarbre<Ent>(infoarbre),
      ManagerModifControle<Ent>(std::move(gestionAutorisation)) {}

template<class Ent> ManagerarbreModifControle<Ent>::ManagerarbreModifControle(const InfoBdd &infoarbre,
                                                            std::unique_ptr<AbstractGestionRestriction<Ent>> &&gestionAutorisation)
    : Managerarbre<Ent>(infoarbre), ManagerModifControle<Ent>(std::move(gestionAutorisation)) {}
}
#endif // MANAGERARBREMODIFCONTROLE_H
