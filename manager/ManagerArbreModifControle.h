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
    using ManagerArb = ManagerArbre<Ent>;
    using ManagerMC = ManagerModifControle<Ent>;

    using ManagerArb::add;
    using ManagerMC::fermer;
    using ManagerMC::modify;
    using ManagerMC::ouvrir;

public:
    using ManagerArb::del;
    using ManagerArb::get;
    using ManagerMC::getAutorisation;

    //! Constructeur.
    ManagerArbreModifControle(const InfoBdd & info, const InfoBdd & infoArbre,
                                std::unique_ptr<AbstractGestionAutorisation<Ent>> && gestionAutorisation
                                            = std::make_unique<GestionAutorisationNoRestrictif<Ent>>(),
                                std::unique_ptr<AbstractUniqueSqlTemp<Ent>> && unique = std::make_unique<NoUniqueSql<Ent>>());

    //! Destructeur.
    ~ManagerArbreModifControle() override = default;

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


    /*//! Supprime de la table en base de donnée l'entité d'identifiant id.
    bool del(idt id)
        {return getAutorisation(Ent(id), bmps::Suppr) && ManagerArb::del(id);}*/

//    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle autorisation de modification.
//    void save(Ent & entity, bmps::autorisation autorisation, bool bb = false) override
//        {ManagerMC::save(entity, autorisation, bb);}

//    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle autorisation de modification..
//    void save(const Ent & entity, bmps::autorisation autorisation, bool bb) override {
//        ouvrir();
//        save(entity);
//        fermer();
//        setAutorisation(entity, autorisation, bb);
//    }

//    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles autorisations de modification.
//    void save(Ent & entity, const std::map<bmps::autorisation,bool> & autorisations) override
//        {ManagerMC::save(entity, autorisations);}

//    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles autorisation de modification..
//    void save(const Ent & entity, const std::map<bmps::autorisation,bool> & autorisations) override {
//        ouvrir();
//        save(entity);
//        fermer();
//        setAutorisation(entity, autorisations);
//    }

//    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles restriction de modification.
//    void save(Ent & entity, const std::vector<bmps::autorisation> restriction) override {
//        ouvrir();
//        save(entity);
//        fermer();
//        setRestriction(entity, restriction);
//    }

//    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles restrictions de modification..
//    void save(const Ent & entity, const std::vector<bmps::autorisation> restriction) override {
//        ouvrir();
//        save(entity);
//        fermer();
//        setRestriction(entity, restriction);
//    }

    //! Retourne le type du manager.
    virtual unsigned typeManager() const
        {return ManagerArb::typeManager() | ManagerMC::typeManager();}

protected:
    //! Constructeur.
    ManagerArbreModifControle(const InfoBdd & InfoArbre, std::unique_ptr<AbstractGestionAutorisation<Ent>> && gestionAutorisation
                                                            = std::make_unique<GestionAutorisationNoRestrictif<Ent>>());
};

template<class Ent> ManagerArbreModifControle<Ent>::ManagerArbreModifControle(const InfoBdd & info, const InfoBdd & infoArbre,
                                                      std::unique_ptr<AbstractGestionAutorisation<Ent>> && gestionAutorisation,
                                                      std::unique_ptr<AbstractUniqueSqlTemp<Ent>> && unique)
    : ManagerSql<Ent>(info,std::move(unique)), ManagerArbre<Ent>(infoArbre),
      ManagerModifControle<Ent>(std::move(gestionAutorisation)) {}

template<class Ent> ManagerArbreModifControle<Ent>::ManagerArbreModifControle(const InfoBdd & infoArbre,
                                                            std::unique_ptr<AbstractGestionAutorisation<Ent>> && gestionAutorisation)
    : ManagerArbre<Ent>(infoArbre), ManagerModifControle<Ent>(std::move(gestionAutorisation)) {}
}
#endif // MANAGERARBREMODIFCONTROLE_H
