/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/08/2018
 */
#ifndef MANAGERMODIFCONTROLE_H
#define MANAGERMODIFCONTROLE_H

#include "AbstractGestionAutorisation.h"
#include "ManagerSql.h"

//! \ingroup groupeManager
//! Début du coprs des deux methodes save.
#define SAVE_MODIF_DEBUT if(entity.isValid()) \
    {if(entity.isNew()) \
        {if(existsUnique(entity) == bmps::Aucun) \
            {Ent ent(entity); \
            add(ent);

namespace managerMPS {
/*! \ingroup groupeManager
 * \brief Classe template mère des différents manageurs à modification controlée.
 */
template<class Ent> class BaseManagerModifControle {
protected:
    bool m_bypass = false;  //!< Booléen permettant de courtcircuité le gestionnaire d'autorisation.
    std::unique_ptr<AbstractGestionAutorisation<Ent>>  m_gestionAutorisation;   //!< Gestionnaire d'autorisation
public:
    //! Constructeur. (Posséde le gestionnaire d'autorisation.)
    BaseManagerModifControle(std::unique_ptr<AbstractGestionAutorisation<Ent>> &&  gestionAutorisation)
        : m_gestionAutorisation(std::move(gestionAutorisation)) {}

    //! Destructeur.
    virtual ~BaseManagerModifControle() = default;

    //! Accesseur du gestionnaire d'autorisations.
    AbstractGestionAutorisation<Ent> & gestionAutorisation() const
        {return *m_gestionAutorisation;}

    //! Demande l'autorisation de modification pour l'entité d'identifiant id.
    virtual bool getAutorisation(idt id, bmps::autorisation autorisation)
        {return m_gestionAutorisation->getAutorisation(id, autorisation) || m_bypass;}

    //! Demande l'autorisation de modification pour une entité d'identifiant id avec les valeurs de entity.
    virtual bool getAutorisation(idt id, bmps::autorisation autorisation, const Ent & entity)
        {return m_gestionAutorisation->getAutorisation(id, autorisation, entity) || m_bypass;}

    //! Demande la liste des restrictions de modification pour l'entité didentifiant id.
    virtual std::vector<unsigned> getRestriction(idt id)
        {return m_gestionAutorisation->getRestriction(id);}

    //! Renvoie la liste des restrictions gérer par le gestionérer d'autorisarion.
    virtual std::vector<bmps::autorisation> restriction() const
        {return m_gestionAutorisation->restriction();}

    //! Modifie une autorisation de modification pour l'entité d'identifiant id.
    virtual void setAutorisation(idt id, bmps::autorisation autorisation, bool bb)
        {m_gestionAutorisation->setAutorisation(id, autorisation, bb);}

    //! Mutateur du gestionnaire d'autorisations.
    void setGestionAutorisation(AbstractGestionAutorisation<Ent> * gestionAutorisation)
        {m_gestionAutorisation = gestionAutorisation;}

protected:
    //! Met en place un court circuit du gestionnaire d'autorisations.
    void ouvrir()
        {m_bypass = true;}

    //! Met fin au court circuit du gestionnaire d'autorisations.
    void fermer()
        {m_bypass = false;}
};

/*! \ingroup groupeManager
 * \brief Classe template manageurs à modification controlée.
 */
template<class Ent> class ManagerModifControle : public virtual ManagerSql<Ent>, public BaseManagerModifControle<Ent> {
protected:
    using ManagerSqlEnt = ManagerSql<Ent>;
    using ManagerBMC = BaseManagerModifControle<Ent>;

    using ManagerBMC::m_gestionAutorisation;

    using ManagerSqlEnt::add;
    using ManagerSqlEnt::messageErreurs;
    using ManagerSqlEnt::table;
    using ManagerBMC::ouvrir;
    using ManagerBMC::fermer;

public:
    using ManagerSqlEnt::existsUnique;
    using ManagerSqlEnt::exists;
    using ManagerSqlEnt::del;
    using ManagerSqlEnt::get;
    using ManagerSqlEnt::sameInBdd;
    using ManagerSqlEnt::save;

    //! Constructeur
    ManagerModifControle (const InfoBdd & info,
                          std::unique_ptr<AbstractGestionAutorisation<Ent>> && gestionAutorisation
                                      = std::make_unique<GestionAutorisationNoRestrictif<Ent>>(),
                          std::unique_ptr<AbstractUniqueSqlTemp<Ent>> && unique = std::make_unique<NoUniqueSql<Ent>>())
        : ManagerSql<Ent>(info,std::move(unique)), BaseManagerModifControle<Ent>(std::move(gestionAutorisation)) {
        m_gestionAutorisation->addRestriction(bmps::Modif);
        m_gestionAutorisation->addRestriction(bmps::Suppr);
    }
    //! Destructeur.
    ~ManagerModifControle() override = default;

    //! Supprime de la table en base de donnée l'entité d'identifiant id.
    bool del(idt id) override
        {return getAutorisation(id, bmps::Suppr) &&  ManagerSqlEnt::del(id);}

    //! Demande l'autorisation de modification pour l'entité d'identifiant id.
    bool getAutorisation(idt id, bmps::autorisation autorisation) override
        {return ManagerBMC::getAutorisation(id, autorisation);}

    //! Demande l'autorisation de modification pour une entité d'identifiant id avec les valeurs de entity.
    bool getAutorisation(idt id, bmps::autorisation autorisation, const Ent & entity) override
        {return ManagerBMC::getAutorisation(id, autorisation, entity);}

    //! Demande la liste des restrictions de modification pour l'entité d'identifiant id.
    std::vector<unsigned> getRestriction(idt id) override
        {return ManagerBMC::getRestriction(id);}

//    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle autorisation de modification.
//    void save(Ent & entity, bmps::autorisation autorisation, bool bb = false) override {
//        ouvrir();
//        save(entity);
//        fermer();
//        setAutorisation(entity, autorisation, bb);
//    }

//    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle autorisation de modification.
//    void save(const Ent & entity, bmps::autorisation autorisation, bool bb = false) override {
//        SAVE_MODIF_DEBUT
//        setAutorisation(ent, autorisation, bb);
//        SAVE_MODIF_MILIEU
//        setAutorisation(entity, autorisation, bb);
//        SAVE_MODIF_FIN
//    }

//    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles autorisation de modification..
//    void save(Ent & entity, const std::map<bmps::autorisation,bool> & autorisations) override {
//        ouvrir();
//        save(entity);
//        fermer();
//        setAutorisation(entity, autorisations);
//    }

//    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles autorisations de modification.
//    void save(const Ent & entity, const std::map<bmps::autorisation,bool> & autorisations) override {
//        SAVE_MODIF_DEBUT
//        setAutorisation(ent, autorisations);
//        SAVE_MODIF_MILIEU
//        setAutorisation(entity, autorisations);
//        SAVE_MODIF_FIN
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
//        SAVE_MODIF_DEBUT
//        setRestriction(ent, restriction);
//        SAVE_MODIF_MILIEU
//        setRestriction(entity, restriction);
//        SAVE_MODIF_FIN
//    }

    //! Renvoie la liste des restrictions gérer par le manageur.
    std::vector<bmps::autorisation> restriction() const override
        {return ManagerBMC::restriction();}

    //! Modifie une autorisation de modification pour l'entité d'identifiant id.
    void setAutorisation(idt id, bmps::autorisation autorisation, bool bb) override
        {ManagerBMC::setAutorisation(id, autorisation, bb);}

    //! Retourne le type du manager.
    virtual unsigned typeManager() const
        {return bmps::ModifControleTypeManager;}

protected:
    //! Constructeur.
    ManagerModifControle(std::unique_ptr<AbstractGestionAutorisation<Ent>> && gestionAutorisation
                         = std::make_unique<GestionAutorisationNoRestrictif<Ent>>())
        : BaseManagerModifControle<Ent>(std::move(gestionAutorisation)) {
        m_gestionAutorisation->addRestriction(bmps::Modif);
        m_gestionAutorisation->addRestriction(bmps::Suppr);
    }


    //! Met à jour l'entité entity en base de donnée.
    void modify(const Ent & entity) override {
        if(getAutorisation(entity.id(), bmps::autorisation::Modif))
            ManagerSqlEnt::modify(entity);
        else
            throw std::invalid_argument(QString("Erreur d'autorisation de modification' : "
                                                "l'entité suivante n'est pas modifiable:\n")
                                        .append(entity.affiche()).toStdString());
    }

    //! Met à jour l'entité entity en base de donnée d'identifiant id avec les valeurs d'entity.
    void modify(const Ent & entity, idt id) override {
        if(getAutorisation(id, bmps::autorisation::Modif, entity))
            ManagerSqlEnt::modify(entity,id);
        else
            throw std::invalid_argument(QString("Erreur d'autorisation de modification' : "
                                                "l'entité suivante n'est pas modifiable:\n")
                                        .append(entity.affiche()).toStdString());
    }

    //! Enregistre l'entité entity en base en ignorant les restrictions.
    void saveByPass(Ent & entity) override {
        ouvrir();
        save(entity);
        fermer();
    }
};
}
#endif // MANAGERMODIFCONTROLE_H
