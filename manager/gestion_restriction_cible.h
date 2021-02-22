/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/08/2018
 */
#ifndef GESTION_AUTORISATION_CIBLE_H
#define GESTION_AUTORISATION_CIBLE_H

#include "abstract_gestion_restriction.h"
#include "manager_permission.h"

namespace mps {
namespace manager {
/*! \ingroup groupe_manager
 *\brief Classe template d'une classe abstraite de gestionnaire d'autorisation de type cible.
 */
template<class Ent, class Restrict> class abstract_gestion_restriction_cible : public abstract_gestion_restriction<Ent> {
protected:
    const entidt m_cible;  //!< Identifiant de ciblage de l'entité Ent.
    abstract_manager_temp<Restrict> &m_manager_restriction;   //!< Pointeur vers le gertionnaire des restriction.

public:
    //! Constructeur. (Ne posséde pas le manageur de restriction)
    abstract_gestion_restriction_cible(entidt cible, abstract_manager_temp<Restrict> &manager_restriction,
                                     flag restriction = b2d::Aucune)
        : abstract_gestion_restriction<Ent>(restriction), m_cible(cible), m_manager_restriction(manager_restriction)
    {}

    //! Destructeur.
    ~abstract_gestion_restriction_cible() override = default;

    //! Accesseur du manageur de restriction.
    abstract_manager_temp<Restrict> &manager_restriction()
        {return m_manager_restriction;}

    //! Accesseur du manageur de restriction.
    const abstract_manager_temp<Restrict> &manager_restriction() const
        {return m_manager_restriction;}

    //! Mutateur du manageur de restriction.
    void set_manager_restriction(abstract_manager_temp<Restrict> &manager)
        {m_manager_restriction = manager;}
};

/*! \ingroup groupe_manager
 *\brief Classe template d'un gestionnaire d'autorisation de type cible et code.
 */
template<class Ent, class Restrict> class gestion_restriction_cible_code : public abstract_gestion_restriction_cible<Ent,Restrict> {
protected:
    using abstract_gestion_restriction_cible<Ent, Restrict>::m_cible;
    using abstract_gestion_restriction_cible<Ent, Restrict>::m_manager_restriction;

public:
    using abstract_gestion_restriction_cible<Ent,Restrict>::abstract_gestion_restriction_cible;

    //! Destructeur.
    ~gestion_restriction_cible_code() override = default;

    //! Ajoute des restrictions de modification pour l'entité d'identifiant id.
    void add_restriction(idt id, flag restrict) override {
        abstract_gestion_restriction_cible<Ent,Restrict>::set_restriction(id,restrict);
        Restrict restriction(id, m_cible);
        if(m_manager_restriction.get_unique(restriction)) {
            if(~restriction.code() &restrict){
                restriction.set_code(restriction.code() |= restrict);
                m_manager_restriction.save(restriction);
            }
        }
        else if(restrict) {
            restriction.set_code(restrict);
            m_manager_restriction.save(restriction);
        }
    }

    //! Demande les restrictions de modification pour l'entité d'identifiant id.
    flag get_restriction(idt id) override {
        Restrict restrict(id,m_cible);
        if(m_manager_restriction.get_unique(restrict))
            return restrict.code();
        return b2d::Aucune;
    }

    //! Modifie les restrictions de modification pour l'entité d'identifiant id.
    void set_restriction(idt id, flag restrict) override {
        abstract_gestion_restriction_cible<Ent,Restrict>::set_restriction(id,restrict);
        Restrict restriction(id, m_cible);
        if(m_manager_restriction.get_unique(restriction)) {
            if(restriction.code() != restrict)
                m_manager_restriction.save(restriction);
        }
        else if(restrict) {
            restriction.set_code(restrict);
            m_manager_restriction.save(restriction);
        }
    }

    //! Teste la restriction de modification pour une entité d'identifiant id.
    bool test_restriction(idt id, flag restrict) override
        {return get_restriction(id) &restrict;}
};
}}
#endif // GESTIONAUTORISATIONCIBLE_H
