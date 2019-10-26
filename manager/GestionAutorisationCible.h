/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/08/2018
 */
#ifndef GESTIONAUTORISATIONCIBLE_H
#define GESTIONAUTORISATIONCIBLE_H

#include <QList>
#include "AbstractGestionAutorisation.h"
#include "ManagerOfPermission.h"

/*! \ingroup groupeManager
 * \brief Classe template d'une classe abstraite de gestionnaire d'autorisation de type cible.
 */
template<class Ent, class Restrict> class AbstractGestionAutorisationCible : public AbstractGestionAutorisation<Ent>
{
protected:
    const int m_cible;  //!< Identifiant de ciblage de l'entité Ent.
    AbstractManagerTemp<Restrict> * m_managerRestriction;   //!< Pointeur vers le gertionnaire des restriction.

public:
    //! Constructeur. (Ne posséde pas le manageur de restriction)
    AbstractGestionAutorisationCible(int cible, AbstractManagerTemp<Restrict> * managerRestriction = nullptr)
        : m_cible(cible), m_managerRestriction(managerRestriction)
    {}

    //! Destructeur.
    ~AbstractGestionAutorisationCible() override = default;

    //! Accesseur du manageur de restriction.
    AbstractManagerOfPermission<Restrict> * managerRestriction() const
        {return m_managerRestriction;}

    //! Mutateur du manageur de restriction.
    void setManagerRestriction(AbstractManagerOfPermission<Restrict> * manager)
        {m_managerRestriction = manager;}
};

/*! \ingroup groupeManager
 * \brief Classe template d'un gestionnaire d'autorisation de type cible et num.
 */
template<class Ent, class Restrict> class GestionAutorisationCibleNum : public AbstractGestionAutorisationCible<Ent, Restrict>
{
protected:
    using AbstractGestionAutorisationCible<Ent, Restrict>::m_cible;
    using AbstractGestionAutorisationCible<Ent, Restrict>::m_managerRestriction;
public:
    //! Constructeur. (Ne posséde pas le manageur de restriction)
    GestionAutorisationCibleNum(int cible, AbstractUniqueSqlTemp<Restrict> * managerRestriction = nullptr)
        : AbstractGestionAutorisationCible<Ent, Restrict>(cible, managerRestriction)
    {}

    //! Destructeur.
    ~GestionAutorisationCibleNum() override = default;

    //! Demande l'autorisation de modification pour une entité donnée.
    bool getAutorisation(const Ent & entity, bdd::Autorisation autorisation) override
        {return !m_managerRestriction->existsUnique(Restrict(entity.id(), m_cible, autorisation));}

    //! Demande l'autorisation de modification pour une entité d'identifiant id avec les valeurs de entity.
    bool getAutorisation(const Ent & /*entity*/, idt id, bdd::Autorisation autorisation) override
        {return !m_managerRestriction->existsUnique(Restrict(id, m_cible, autorisation));}

    //! Demande la liste des restrictions de modification pour une entité donnée.
    QList<int> getRestriction(const Ent & entity) override
    {
        ListPtr<Restrict> listeRestrict(m_managerRestriction->getList(Restrict::IdCible, entity.id(), Restrict::Cible, m_cible, Restrict:: Num));
        QList<int> listeNum;
        for(typename ListPtr<Restrict>::iterator i = listeRestrict.begin(); i != listeRestrict.end(); i++)
            listeNum.append((*i).num());
        return listeNum;
    }

    //! Modifie une autorisation de modification pour une entité donnée.
    void setAutorisation(const Ent & entity, bdd::Autorisation autorisation, bool bb) override
    {
        Restrict restriction(entity.id(), m_cible, autorisation);
        bool existRestric = m_managerRestriction->existsUnique(restriction);
        if(bb)
        {
            if(existRestric)
                m_managerRestriction->del(restriction.id());
        }
        else
        {
            if(!existRestric)
                m_managerRestriction->save(restriction);
        }
    }
};

/*! \ingroup groupeManager
 * \brief Classe template d'un gestionnaire d'autorisation de type cible et code.
 */
template<class Ent, class Restrict> class GestionAutorisationCibleCode : public AbstractGestionAutorisationCible<Ent,Restrict>
{
protected:
    using AbstractGestionAutorisationCible<Ent, Restrict>::m_cible;
    using AbstractGestionAutorisationCible<Ent, Restrict>::m_managerRestriction;

public:
    //! Constructeur. (Ne posséde pas le manageur de restriction)
    GestionAutorisationCibleCode(int cible, AbstractManagerTemp<Restrict> * managerRestriction = nullptr)
        : AbstractGestionAutorisationCible<Ent, Restrict>(cible, managerRestriction)
    {}

    //! Destructeur.
    ~GestionAutorisationCibleCode() override = default;

    //! Demande l'autorisation de modification pour une entité donnée.
    bool getAutorisation(const Ent & entity, bdd::Autorisation autorisation) override
    {
        Restrict restrict(entity.id(),m_cible);
        if(!(m_managerRestriction->getUnique(restrict)))
            return true;
        return !restrict.in(autorisation);
    }

    //! Demande l'autorisation de modification pour une entité d'identifiant id avec les valeurs de entity.
    bool getAutorisation(const Ent & /*entity*/, idt id, bdd::Autorisation autorisation) override
        {return getAutorisation(Ent(id),autorisation);}

    //! Demande la liste des restrictions de modification pour une entité donnée.
    QList<int> getRestriction(const Ent & entity) override
    {
        Restrict restrict(entity.id(),m_cible);
        return m_managerRestriction->getUnique(restrict)? restrict.list(): QList<int>();
    }

    //! Modifie une autorisation de modification pour une entité donnée.
    void setAutorisation(const Ent & entity, bdd::Autorisation autorisation, bool bb) override
    {
        Restrict restriction(entity.id(), m_cible);
        if(m_managerRestriction->getUnique(restriction))
        {
            if(restriction.in(autorisation) != bb)
            {
                if(bb)
                    restriction.del(autorisation);
                else
                    restriction.add(autorisation);
                m_managerRestriction->save(restriction);
            }
        }
        else if(!bb)
        {
            restriction.add(autorisation);
            m_managerRestriction->save(restriction);
        }
    }

    //! Accesseur du manageur de restriction.
    AbstractManagerTemp<Restrict> * managerRestriction() const
        {return m_managerRestriction;}

    //! Mutateur du manageur de restriction.
    void setManagerRestriction(AbstractManagerTemp<Restrict> * manager)
        {m_managerRestriction = manager;}
};

#endif // GESTIONAUTORISATIONCIBLE_H
