/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef ABSTRACTGESTIONAUTORISATION_H
#define ABSTRACTGESTIONAUTORISATION_H

#include "AbstractManager.h"

namespace managerMPS {
/*! \ingroup groupeManager
 * \brief Classe template abstraite mère des différents gestionnaire d'autorisation.
 */
template<class Ent> class AbstractGestionAutorisation {
public:
    //! Constructeur.
    AbstractGestionAutorisation() = default;

    //!Destructeur.
    virtual ~AbstractGestionAutorisation() = default;

    //! Demande l'autorisation de modification pour une entité donnée.
    virtual bool getAutorisation(const Ent & entity, bmps::autorisation autorisation) = 0;

    //! Demande l'autorisation de modification pour une entité d'identifiant id avec les valeur de entity.
    virtual bool getAutorisation(const Ent & entity, idt id, bmps::autorisation autorisation) = 0;

    //! Demande la liste des restrictions de modification pour une entité donnée.
    virtual std::vector<int> getRestriction(const Ent & entity) = 0;

    //! Modifie une autorisation de modification pour une entité donnée.
    virtual void setAutorisation(const Ent & entity, bmps::autorisation autorisation, bool bb) = 0;

    //! Modifie les autorisations de modification pour une entité donnée.
    virtual void setAutorisation(const Ent & entity, std::map<bmps::autorisation,bool> autorisations);

    //! Ajoute les restrictions de modification pour une entité donnée.
    virtual void setRestriction(const Ent & entity, std::vector<bmps::autorisation> restrictions);
};

template<class Ent> void AbstractGestionAutorisation<Ent>
                            ::setAutorisation(const Ent & entity, std::map<bmps::autorisation,bool> autorisations) {
    for(auto i = autorisations.cbegin(); i != autorisations.cend(); i++)
        setAutorisation(entity, i->first, i->second);
}

template<class Ent> void AbstractGestionAutorisation<Ent>::setRestriction(const Ent & entity, std::vector<bmps::autorisation> restrictions) {
    for(auto i = restrictions.cbegin(); i != restrictions.cend(); i++)
        setAutorisation(entity, *i, false);
}

/*! \ingroup groupeManager
 * \brief Classe template implémentant une gestionnaire d'autorisation non restrictif.
 */
template<class Ent> class GestionAutorisationNoRestrictif : public AbstractGestionAutorisation<Ent>
{
public:
    CONSTR_DEFAUT(GestionAutorisationNoRestrictif)

    //! Destructeur.
    ~GestionAutorisationNoRestrictif() override = default;

    //! Demande l'autorisation de modification pour une entité donnée.
    bool getAutorisation(const Ent & /*entity*/, bmps::autorisation /*autorisation*/) override
        {return true;}

    //! Demande l'autorisation de modification pour une entité d'identifiant id avec les valeur de entity.
    bool getAutorisation(const Ent & /*entity*/, szt /*id*/, bmps::autorisation /*autorisation*/) override
        {return true;}

    //! Demande la liste des restrictions de modification pour une entité donnée.
    std::vector<int> getRestriction(const Ent & /*entity*/) override
        {return std::vector<int>();}

    //! Modifie une autorisation de modification pour une entité donnée.
    void setAutorisation(const Ent & /*entity*/, bmps::autorisation /*autorisation*/, bool /*bb*/) override
        {}

    //! Modifie les autorisations de modification pour une entité donnée.
    void setAutorisation(const Ent & /*entity*/, std::map<bmps::autorisation,bool> /*autorisations*/) override;

    //! Ajoute les restrictions de modification pour une entité donnée.
    void setRestriction(const Ent & /*entity*/, std::vector<bmps::autorisation> /*restrictions*/) override
        {}
};

template<class Ent> void GestionAutorisationNoRestrictif<Ent>::setAutorisation(const Ent & /*entity*/, std::map<bmps::autorisation,bool> /*autorisations*/)
    {}
}
#endif // ABSTRACTGESTIONAUTORISATION_H
