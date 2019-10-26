/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef ABSTRACTGESTIONAUTORISATION_H
#define ABSTRACTGESTIONAUTORISATION_H

#include <QList>
#include <QMap>
#include "AbstractManager.h"

/*! \ingroup groupeManager
 * \brief Classe template abstraite mère des différents gestionnaire d'autorisation.
 */
template<class Ent> class AbstractGestionAutorisation
{
public:
    //! Constructeur.
    AbstractGestionAutorisation() = default;

    //!Destructeur.
    virtual ~AbstractGestionAutorisation() = default;

    //! Demande l'autorisation de modification pour une entité donnée.
    virtual bool getAutorisation(const Ent & entity, bdd::Autorisation autorisation) = 0;

    //! Demande l'autorisation de modification pour une entité d'identifiant id avec les valeur de entity.
    virtual bool getAutorisation(const Ent & entity, unsigned id, bdd::Autorisation autorisation) = 0;

    //! Demande la liste des restrictions de modification pour une entité donnée.
    virtual QList<int> getRestriction(const Ent & entity) = 0;

    //! Modifie une autorisation de modification pour une entité donnée.
    virtual void setAutorisation(const Ent & entity, bdd::Autorisation autorisation, bool bb) = 0;

    //! Modifie les autorisations de modification pour une entité donnée.
    virtual void setAutorisation(const Ent & entity, QMap<bdd::Autorisation,bool> autorisations);

    //! Ajoute les restrictions de modification pour une entité donnée.
    virtual void setRestriction(const Ent & entity, QList<bdd::Autorisation> restrictions);
};

template<class Ent> void AbstractGestionAutorisation<Ent>::setAutorisation(const Ent & entity, QMap<bdd::Autorisation,bool> autorisations)
{
    for(QMap<bdd::Autorisation,bool>::const_iterator i = autorisations.cbegin(); i != autorisations.cend(); i++)
        setAutorisation(entity, i.key(), i.value());
}

template<class Ent> void AbstractGestionAutorisation<Ent>::setRestriction(const Ent & entity, QList<bdd::Autorisation> restrictions)
{
    for(QList<bdd::Autorisation>::const_iterator i = restrictions.cbegin(); i != restrictions.cend(); i++)
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
    bool getAutorisation(const Ent & /*entity*/, bdd::Autorisation /*autorisation*/) override
        {return true;}

    //! Demande l'autorisation de modification pour une entité d'identifiant id avec les valeur de entity.
    bool getAutorisation(const Ent & /*entity*/, int /*id*/, bdd::Autorisation /*autorisation*/) override
        {return true;}


    //! Demande la liste des restrictions de modification pour une entité donnée.
    QList<int> getRestriction(const Ent & /*entity*/) override
        {return QList<int>();}


    //! Modifie une autorisation de modification pour une entité donnée.
    void setAutorisation(const Ent & /*entity*/, bdd::Autorisation /*autorisation*/, bool /*bb*/) override
        {}

    //! Modifie les autorisations de modification pour une entité donnée.
    void setAutorisation(const Ent & /*entity*/, QMap<bdd::Autorisation,bool> /*autorisations*/) override;

    //! Ajoute les restrictions de modification pour une entité donnée.
    void setRestriction(const Ent & /*entity*/, QList<bdd::Autorisation> /*restrictions*/) override
        {}
};

template<class Ent> void GestionAutorisationNoRestrictif<Ent>::setAutorisation(const Ent & /*entity*/, QMap<bdd::Autorisation,bool> /*autorisations*/)
    {}

#endif // ABSTRACTGESTIONAUTORISATION_H
