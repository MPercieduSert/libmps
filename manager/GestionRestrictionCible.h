/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/08/2018
 */
#ifndef GESTIONAUTORISATIONCIBLE_H
#define GESTIONAUTORISATIONCIBLE_H

#include "AbstractGestionRestriction.h"
#include "ManagerPermission.h"

namespace managerMPS {
/*! \ingroup groupeManager
 * \brief Classe template d'une classe abstraite de gestionnaire d'autorisation de type cible.
 */
template<class Ent, class Restrict> class AbstractGestionRestrictionCible : public AbstractGestionRestriction<Ent> {
protected:
    const szt m_cible;  //!< Identifiant de ciblage de l'entité Ent.
    AbstractManagerTemp<Restrict> & m_managerRestriction;   //!< Pointeur vers le gertionnaire des restriction.

public:
    //! Constructeur. (Ne posséde pas le manageur de restriction)
    AbstractGestionRestrictionCible(szt cible, AbstractManagerTemp<Restrict> & managerRestriction,
                                     flag restriction = bddMPS::Aucune)
        : AbstractGestionRestriction<Ent>(restriction), m_cible(cible), m_managerRestriction(managerRestriction)
    {}

    //! Destructeur.
    ~AbstractGestionRestrictionCible() override = default;

    //! Accesseur du manageur de restriction.
    AbstractManagerTemp<Restrict> & managerRestriction()
        {return m_managerRestriction;}

    //! Accesseur du manageur de restriction.
    const AbstractManagerTemp<Restrict> & managerRestriction() const
        {return m_managerRestriction;}

    //! Mutateur du manageur de restriction.
    void setManagerRestriction(AbstractManagerTemp<Restrict> & manager)
        {m_managerRestriction = manager;}
};

/*! \ingroup groupeManager
 * \brief Classe template d'un gestionnaire d'autorisation de type cible et code.
 */
template<class Ent, class Restrict> class GestionRestrictionCibleCode : public AbstractGestionRestrictionCible<Ent,Restrict> {
protected:
    using AbstractGestionRestrictionCible<Ent, Restrict>::m_cible;
    using AbstractGestionRestrictionCible<Ent, Restrict>::m_managerRestriction;

public:
    using AbstractGestionRestrictionCible<Ent,Restrict>::AbstractGestionRestrictionCible;

    //! Destructeur.
    ~GestionRestrictionCibleCode() override = default;

    //! Ajoute des restrictions de modification pour l'entité d'identifiant id.
    void addRestriction(idt id, flag restrict) override {
        AbstractGestionRestrictionCible<Ent,Restrict>::setRestriction(id,restrict);
        Restrict restriction(id, m_cible);
        if(m_managerRestriction.getUnique(restriction)) {
            if(~restriction.code() & restrict){
                restriction.setCode(restriction.code() |= restrict);
                m_managerRestriction.save(restriction);
            }
        }
        else if(restrict) {
            restriction.setCode(restrict);
            m_managerRestriction.save(restriction);
        }
    }

    //! Demande les restrictions de modification pour l'entité d'identifiant id.
    flag getRestriction(idt id) override {
        Restrict restrict(id,m_cible);
        if(m_managerRestriction.getUnique(restrict))
            return restrict.code();
        return bddMPS::Aucune;
    }

    //! Modifie les restrictions de modification pour l'entité d'identifiant id.
    void setRestriction(idt id, flag restrict) override {
        AbstractGestionRestrictionCible<Ent,Restrict>::setRestriction(id,restrict);
        Restrict restriction(id, m_cible);
        if(m_managerRestriction.getUnique(restriction)) {
            if(restriction.code() != restrict)
                m_managerRestriction.save(restriction);
        }
        else if(restrict) {
            restriction.setCode(restrict);
            m_managerRestriction.save(restriction);
        }
    }

    //! Teste la restriction de modification pour une entité d'identifiant id.
    bool testRestriction(idt id, flag restrict) override
        {return getRestriction(id) & restrict;}
};
}
#endif // GESTIONAUTORISATIONCIBLE_H
