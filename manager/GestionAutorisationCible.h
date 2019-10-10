/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/08/2018
 */
#ifndef GESTIONAUTORISATIONCIBLE_H
#define GESTIONAUTORISATIONCIBLE_H

#include <QList>
#include "AbstractGestionAutorisation.h"

/*! \ingroup groupeManager
 * \brief Classe template d'un gestionnaire d'autorisation de type cible.
 */
template<class Ent, class Restrict> class GestionAutorisationCible : public AbstractGestionAutorisation<Ent>
{
protected:
    const int m_cible;  //!< Identifiant de ciblage de l'entité Ent.
    AbstractManagerTemp<Restrict> * m_managerRestriction;   //!< Pointeur vers le gertionnaire des restriction.

public:
    //! Constructeur. (Ne posséde pas le manageur de restriction)
    GestionAutorisationCible(int cible, AbstractManagerTemp<Restrict> * managerRestriction = nullptr)
        : m_cible(cible), m_managerRestriction(managerRestriction)
    {}

    //! Destructeur.
    ~GestionAutorisationCible() override = default;

    //! Demande l'autorisation de modification pour une entité donnée.
    bool getAutorisation(const Ent & entity, bdd::Autorisation autorisation) override
        {return !m_managerRestriction->existsUnique(Restrict(entity.id(), m_cible, autorisation));}

    //! Demande l'autorisation de modification pour une entité d'identifiant id avec les valeurs de entity.
    bool getAutorisation(const Ent & /*entity*/, int id, bdd::Autorisation autorisation) override
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

    //! Accesseur du manageur de restriction.
    AbstractManagerTemp<Restrict> * managerRestriction() const
        {return m_managerRestriction;}

    //! Mutateur du manageur de restriction.
    void setManagerRestriction(AbstractManagerTemp<Restrict> * manager)
        {m_managerRestriction = manager;}
};

#endif // GESTIONAUTORISATIONCIBLE_H
