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

//! \ingroup groupeManager
//! Milieu du coprs des deux methodes save.
#define SAVE_MODIF_MILIEU }else \
            throw std::invalid_argument(messageErreursUnique(entity).append("\n Erreur d'unicité : " \
                    "il existe déjà dans la base de donnée une entité ayant les mêmes valeurs d'attributs unique " \
                    "que cette nouvelle entité.").toStdString());} \
    else \
        {if(!sameInBdd(entity)) \
            {if(existsUnique(entity) <= bmps::Meme) \
                modify(entity); \
            else \
                throw std::invalid_argument(messageErreursUnique(entity).append("\n Erreur d'unicité :  " \
                    "il existe déjà dans la base de donnée une entité ayant les mêmes valeurs d'attributs unique " \
                    "que l'entité modifiée.").toStdString());}

            //! \ingroup groupeManager
            //! Milieu du coprs des deux methodes save.
#define SAVE_MODIF_FIN }} else \
    throw std::invalid_argument(messageErreurs(entity).append("\n Erreur de validité").toStdString());

namespace managerMPS {
/*! \ingroup groupeManager
 * \brief Classe template mère des différents manageurs à modification controlée.
 */
template<class Ent> class BaseManagerModifControle {
protected:
    bool m_bypass = false;  //!< Booléen permettant de courtcircuité le gestionnaire d'autorisation.
    AbstractGestionAutorisation<Ent> * m_gestionAutorisation;   //!< Gestionnaire d'autorisation
public:
    //! Constructeur. (Posséde le gestionnaire d'autorisation.)
    BaseManagerModifControle(AbstractGestionAutorisation<Ent> * gestionAutorisation)
        : m_gestionAutorisation(gestionAutorisation) {}

    //! Destructeur.
    virtual ~BaseManagerModifControle();

    //! Accesseur du gestionnaire d'autorisations.
    AbstractGestionAutorisation<Ent> * gestionAutorisation() const
        {return m_gestionAutorisation;}

    //! Demande l'autorisation de modification pour une entité donnée.
    virtual bool getAutorisation(const Ent & entity, bmps::autorisation autorisation)
        {return m_gestionAutorisation->getAutorisation(entity, autorisation) || m_bypass;}

    //! Demande l'autorisation de modification pour une entité d'identifiant id avec les valeurs de entity.
    virtual bool getAutorisation(const Ent & entity, idt id, bmps::autorisation autorisation)
        {return m_gestionAutorisation->getAutorisation(entity, id, autorisation) || m_bypass;}

    //! Demande la liste des restrictions de modification pour une entité donnée.
    virtual std::vector<int> getRestriction(const Ent & entity)
        {return m_gestionAutorisation->getRestriction(entity);}

    //! Modifie une autorisation de modification pour une entité donnée.
    virtual void setAutorisation(const Ent & entity, bmps::autorisation autorisation, bool bb)
        {m_gestionAutorisation->setAutorisation(entity, autorisation, bb);}

    //! Modifie les autorisations de modification pour une entité donnée.
    virtual void setAutorisation(const Ent & entity, std::map<bmps::autorisation,bool> autorisations)
        {m_gestionAutorisation->setAutorisation(entity, autorisations);}

    //! Mutateur du gestionnaire d'autorisations.
    void setGestionAutorisation(AbstractGestionAutorisation<Ent> * gestionAutorisation)
        {m_gestionAutorisation = gestionAutorisation;}

    //! Ajoute les restrictions de modification pour une entité donnée.
    virtual void setRestriction(const Ent & entity, std::vector<bmps::autorisation> restrictions)
        {m_gestionAutorisation->setRestriction(entity, restrictions);}

protected:
    //! Met en place un court circuit du gestionnaire d'autorisations.
    void ouvrir()
        {m_bypass = true;}

    //! Met fin au court circuit du gestionnaire d'autorisations.
    void fermer()
        {m_bypass = false;}
};

template<class Ent> BaseManagerModifControle<Ent>::~BaseManagerModifControle()
    {delete m_gestionAutorisation;}

/*! \ingroup groupeManager
 * \brief Classe template manageurs à modification controlée.
 */
template<class Ent> class ManagerModifControle : public virtual ManagerSql<Ent>, public BaseManagerModifControle<Ent> {
protected:
    using ManagerSqlEnt = ManagerSql<Ent>;
    using ManagerBMC = BaseManagerModifControle<Ent>;

    using ManagerSqlEnt::add;
    using ManagerSqlEnt::messageErreurs;
    using ManagerSqlEnt::messageErreursUnique;
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
    using ManagerBMC::getAutorisation;
    using ManagerBMC::getRestriction;
    using ManagerBMC::setAutorisation;
    using ManagerBMC::setRestriction;

    //! Constructeur
    ManagerModifControle (const InfoBdd & info,
                            AbstractGestionAutorisation<Ent> * gestionAutorisation = new GestionAutorisationNoRestrictif<Ent>(),
                            AbstractUniqueSqlTemp<Ent> * unique = new NoUniqueSql<Ent>())
        : ManagerSql<Ent>(info, unique), BaseManagerModifControle<Ent>(gestionAutorisation) {}

    //! Destructeur.
    ~ManagerModifControle() override = default;

    /*//! Supprime de la table en base de donnée l'entité d'identifiant id.
    bool del(idt id) override;*/

    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle autorisation de modification.
    void save(Ent & entity, bmps::autorisation autorisation, bool bb = false) override {
        ouvrir();
        save(entity);
        fermer();
        setAutorisation(entity, autorisation, bb);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle autorisation de modification.
    void save(const Ent & entity, bmps::autorisation autorisation, bool bb = false) override {
        SAVE_MODIF_DEBUT
        setAutorisation(ent, autorisation, bb);
        SAVE_MODIF_MILIEU
        setAutorisation(entity, autorisation, bb);
        SAVE_MODIF_FIN
    }

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles autorisation de modification..
    void save(Ent & entity, const std::map<bmps::autorisation,bool> & autorisations) override {
        ouvrir();
        save(entity);
        fermer();
        setAutorisation(entity, autorisations);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles autorisations de modification.
    void save(const Ent & entity, const std::map<bmps::autorisation,bool> & autorisations) override {
        SAVE_MODIF_DEBUT
        setAutorisation(ent, autorisations);
        SAVE_MODIF_MILIEU
        setAutorisation(entity, autorisations);
        SAVE_MODIF_FIN
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
        SAVE_MODIF_DEBUT
        setRestriction(ent, restriction);
        SAVE_MODIF_MILIEU
        setRestriction(entity, restriction);
        SAVE_MODIF_FIN
    }

protected:
    /*//! Constructeur.
    ManagerModifControle(AbstractGestionAutorisation<Ent> * gestionAutorisation = new GestionAutorisationNoRestrictif<Ent>())
        : BaseManagerModifControle<Ent>(gestionAutorisation)
        {}*/

    //! Met à jour l'entité entity en base de donnée.
    void modify(const Ent & entity) override {
        if(getAutorisation(entity, bmps::autorisation::Modif))
            ManagerSqlEnt::modify(entity);
        else
            throw std::invalid_argument(QString("Erreur d'autorisation de modification' : "
                                                "l'entité suivante n'est pas modifiable:\n")
                                        .append(entity.affiche()).toStdString());
    }

    //! Met à jour l'entité entity en base de donnée d'identifiant id avec les valeurs d'entity.
    void modify(const Ent & entity, idt id) override {
        if(getAutorisation(entity, id, bmps::autorisation::Modif))
            ManagerSqlEnt::modify(entity,id);
        else
            throw std::invalid_argument(QString("Erreur d'autorisation de modification' : "
                                                "l'entité suivante n'est pas modifiable:\n")
                                        .append(entity.affiche()).toStdString());
    }
};

/*template<class Ent> bool ManagerModifControle<Ent>::del(idt id)
{
    if(getAutorisation(Ent(id), bmps::Suppr))
        return ManagerSqlEnt::del(id);
    else
        return false;
}*/
}
#endif // MANAGERMODIFCONTROLE_H
