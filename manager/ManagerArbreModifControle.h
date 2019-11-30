/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/08/2018
 */
#ifndef MANAGERARBREMODIFCONTROLE_H
#define MANAGERARBREMODIFCONTROLE_H

#include "ManagerModifControle.h"
#include "ManagerArbre.h"

namespace managerMPS {
/*! \ingroup groupeManager
 * \brief Classe template mère des différents manageurs pour les entités de type arbre avec un controle de modification.
 */
template<class Ent> class ManagerArbreModifControle : public ManagerArbre<Ent>,
                                                        public ManagerModifControle<Ent> {
protected:
    using ManagerForArbreEnt = ManagerArbre<Ent>;
    using ManagerMC = ManagerModifControle<Ent>;

    using ManagerForArbreEnt::add;
    using ManagerMC::fermer;
    using ManagerMC::modify;
    using ManagerMC::ouvrir;

public:
    using ManagerForArbreEnt::del;
    using ManagerForArbreEnt::get;
    using ManagerForArbreEnt::save;
    using ManagerMC::getAutorisation;
    using ManagerMC::getRestriction;
    using ManagerMC::setAutorisation;
    using ManagerMC::setRestriction;

    //! Constructeur.
    ManagerArbreModifControle(const InfoBdd & info, const InfoBdd & infoArbre,
                                AbstractGestionAutorisation<Ent> * gestionAutorisation = new GestionAutorisationNoRestrictif<Ent>(),
                                AbstractUniqueSqlTemp<Ent> * unique = new NoUniqueSql<Ent>());

    //! Destructeur.
    ~ManagerArbreModifControle() override = default;

    /*//! Supprime de la table en base de donnée l'entité d'identifiant id.
    bool del(idt id)
        {return getAutorisation(Ent(id), bmps::Suppr) && ManagerForArbreEnt::del(id);}*/

    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle autorisation de modification.
    void save(Ent & entity, bmps::autorisation autorisation, bool bb = false) override
        {ManagerMC::save(entity, autorisation, bb);}

    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle autorisation de modification..
    void save(const Ent & entity, bmps::autorisation autorisation, bool bb) override {
        ouvrir();
        save(entity);
        fermer();
        setAutorisation(entity, autorisation, bb);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles autorisations de modification.
    void save(Ent & entity, const std::map<bmps::autorisation,bool> & autorisations) override
        {ManagerMC::save(entity, autorisations);}

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles autorisation de modification..
    void save(const Ent & entity, const std::map<bmps::autorisation,bool> & autorisations) override {
        ouvrir();
        save(entity);
        fermer();
        setAutorisation(entity, autorisations);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles restriction de modification.
    void save(Ent & entity, const std::vector<bmps::autorisation> restriction) override {
        ouvrir();
        save(entity);
        fermer();
        setRestriction(entity, restriction);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles restrictions de modification..
    void save(const Ent & entity, const std::vector<bmps::autorisation> restriction) override {
        ouvrir();
        save(entity);
        fermer();
        setRestriction(entity, restriction);
    }

protected:
    //! Constructeur.
    ManagerArbreModifControle(const InfoBdd & InfoArbre,
                                AbstractGestionAutorisation<Ent> * gestionAutorisation = new GestionAutorisationNoRestrictif<Ent>());
};

template<class Ent> ManagerArbreModifControle<Ent>::ManagerArbreModifControle(const InfoBdd & info, const InfoBdd & infoArbre,
                                                                                  AbstractGestionAutorisation<Ent> * gestionAutorisation,
                                                                                  AbstractUniqueSqlTemp<Ent> * unique)
    : ManagerSql<Ent>(info,unique), ManagerArbre<Ent>(info, infoArbre, unique),
      ManagerModifControle<Ent>(info, gestionAutorisation, unique) {}

template<class Ent> ManagerArbreModifControle<Ent>::ManagerArbreModifControle(const InfoBdd & infoArbre,
                                                                                  AbstractGestionAutorisation<Ent> * gestionAutorisation)
    : ManagerArbre<Ent>(infoArbre), ManagerModifControle<Ent>(gestionAutorisation) {}
}
#endif // MANAGERARBREMODIFCONTROLE_H
