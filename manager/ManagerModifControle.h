/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/08/2018
 */
#ifndef MANAGERMODIFCONTROLE_H
#define MANAGERMODIFCONTROLE_H

#include "AbstractGestionRestriction.h"
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
    bool m_bypass = false;  //!< Booléen permettant de courtcircuité le gestionnaire de restriction.
    std::unique_ptr<AbstractGestionRestriction<Ent>>  m_gestionRestriction;   //!< Gestionnaire de restriction.
public:
    //! Constructeur. (Posséde le gestionnaire de restriction.)
    BaseManagerModifControle(std::unique_ptr<AbstractGestionRestriction<Ent>> &&  gestionRestriction)
        : m_gestionRestriction(std::move(gestionRestriction)) {}

    //! Destructeur.
    virtual ~BaseManagerModifControle() = default;

    //! Accesseur du gestionnaire de restriction.
    AbstractGestionRestriction<Ent> & gestionRestriction() const
        {return *m_gestionRestriction;}

    //! Ajoute des restrictions gérées par le gestionnaire.
    void addEnableRestriction(flag restriction)
        {m_gestionRestriction->addEnableRestriction(restriction);}

    //! Ajoute des restrictions de modification pour une entité donnée.
    virtual void addRestriction(idt id, flag restrict)
        {m_gestionRestriction->addRestriction(id,restrict);}

    //! Renvoie la liste des restrictions gérer par le gestionérer d'autorisarion.
    virtual flag enableRestriction() const
        {return m_gestionRestriction->enableRestriction();}

    //! Demande les restrictions de modification pour l'entité d'identifiant id.
    virtual flag getRestriction(idt id)
        {return m_bypass ? bddMPS::Aucune : m_gestionRestriction->getRestriction(id);}

    //! Mutateur des restrictions gérées par le gestionnaire.
    void setEnableRestriction(flag restriction)
        {m_gestionRestriction->setEnableRestriction(restriction);}

    //! Mutateur du gestionnaire d'autorisations.
    void setGestionRestriction(AbstractGestionRestriction<Ent> * gestionRestriction)
        {m_gestionRestriction = gestionRestriction;}

    //! Modifie les restrictions de modification pour une entité donnée.
    virtual void setRestriction(idt id, flag restrict)
        {m_gestionRestriction->setRestriction(id,restrict);}

    //! Teste la restriction de modification pour une entité d'identifiant id.
    virtual bool testRestriction(idt id, flag restrict)
        {return m_bypass ? true : m_gestionRestriction->testRestriction(id, restrict);}

    //! Teste la restriction de modification pour une entité d'identifiant id avec les valeurs de entity.
    virtual bool testRestriction(idt id, flag restrict, const Ent & entity)
        {return m_bypass ? true : m_gestionRestriction->testRestriction(id, restrict, entity);}

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

    using ManagerBMC::m_gestionRestriction;

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
                          std::unique_ptr<AbstractGestionRestriction<Ent>> && gestionRestriction
                                      = std::make_unique<AbstractGestionRestriction<Ent>>(),
                          std::unique_ptr<AbstractUniqueSqlTemp<Ent>> && unique = std::make_unique<NoUniqueSql<Ent>>())
        : ManagerSql<Ent>(info,std::move(unique)), BaseManagerModifControle<Ent>(std::move(gestionRestriction))
            {m_gestionRestriction->setEnableRestriction(bmps::ModifSupr);}

    //! Destructeur.
    ~ManagerModifControle() override = default;

    //! Ajoute des restrictions de modification pour l'entité d'identifiant id.
    void addRestriction(idt id, flag restrict) override
        {ManagerBMC::addRestriction(id, restrict);}


    //! Supprime de la table en base de donnée l'entité d'identifiant id.
    bool del(idt id) override
        {return testRestriction(id,bddMPS::Suppr) &&  ManagerSqlEnt::del(id);}

    //! Demande les restriction de modification pour l'entité d'identifiant id.
    flag getRestriction(idt id) override
        {return ManagerBMC::getRestriction(id);}

    //! Renvoie la liste des restrictions gérées par le manageur.
    flag enableRestriction() const override
        {return ManagerBMC::enableRestriction();}

    //! Modifie les restrictions de modification pour l'entité d'identifiant id.
    void setRestriction(idt id, flag restrict) override
        {ManagerBMC::setRestriction(id, restrict);}

    //! Teste la restriction de modification pour une entité d'identifiant id.
    bool testRestriction(idt id, flag restrict) override
        {return ManagerBMC::testRestriction(id, restrict);}

    //! Teste la restriction de modification pour une entité d'identifiant id avec les valeurs de entity.
    bool testRestriction(idt id, flag restrict, const Ent & entity) override
        {return ManagerBMC::testRestriction(id, restrict, entity);}

    //! Retourne le type du manager.
    virtual flag typeManager() const
        {return bmps::ModifControleTypeManager;}

protected:
    //! Constructeur.
    ManagerModifControle(std::unique_ptr<AbstractGestionRestriction<Ent>> && gestionRestriction
                         = std::make_unique<AbstractGestionRestriction<Ent>>())
        : BaseManagerModifControle<Ent>(std::move(gestionRestriction))
        {m_gestionRestriction->setEnableRestriction(bmps::ModifSupr);}

    //! Met à jour l'entité entity en base de donnée.
    void modify(const Ent & entity) override {
        if(testRestriction(entity.id(), bmps::Modif,entity))
            ManagerSqlEnt::modify(entity);
        else
            throw std::invalid_argument(QString("Erreur d'autorisation de modification' : "
                                                "l'entité suivante n'est pas modifiable:\n")
                                        .append(entity.affiche()).toStdString());
    }

    //! Met à jour l'entité entity en base de donnée d'identifiant id avec les valeurs d'entity.
    void modify(const Ent & entity, idt id) override {
        if(testRestriction(id, bmps::Modif, entity))
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
