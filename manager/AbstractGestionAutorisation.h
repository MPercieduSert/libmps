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
protected:
    std::vector<bmps::autorisation> m_restriction;  //! Ensemble des restrictions gérée par le gestionère d'autorisation.
public:
    //! Constructeur.
    AbstractGestionAutorisation(const std::vector<bmps::autorisation> & restriction = std::vector<bmps::autorisation>())
        : m_restriction(restriction) {}

    //!Destructeur.
    virtual ~AbstractGestionAutorisation() = default;

    //! Ajoute une restriction gérée par le gestionère.
    void addRestriction(bmps::autorisation restriction)
        {m_restriction.push_back(restriction);}

    //! Ajoute des restrictions gérées par le gestionère.
    //void addRestriction(const std::vector<bmps::autorisation> & restriction)
    //    {m_restriction += restriction;}

    //! Demande l'autorisation de modification pour l'entité d'identifiant id.
    virtual bool getAutorisation(idt id, bmps::autorisation autorisation) = 0;

    //! Demande l'autorisation de modification pour une entité d'identifiant id avec les valeur de entity.
    virtual bool getAutorisation(idt id, bmps::autorisation autorisation, const Ent & entity) = 0;

    //! Demande la liste des restrictions de modification pour l'entité d'identifiant id.
    virtual std::vector<unsigned> getRestriction(idt id) = 0;

    //! Renvoie la liste des restrictions gérer par le manageur.
    const std::vector<bmps::autorisation> & restriction() const
        {return m_restriction;}

    //! Modifie une autorisation de modification pour une entité donnée.
    virtual void setAutorisation(idt /*id*/, bmps::autorisation autorisation, bool bb) {
        if(!bb && std::find(m_restriction.cbegin(),m_restriction.cend(),autorisation) == m_restriction.cend())
            throw std::invalid_argument(QString("void AbstractGestionAutorisation<").append(Ent::Name())
                                        .append(">::setAutorisation(idt id, bmps::autorisation autorisation, bool bb))\n ")
                                        .append("restriction non gérée par le gestionére d'autorisation :")
                                        .append(QString::number(autorisation)).toStdString());
    }

//    //! Modifie les autorisations de modification pour une entité donnée.
//    virtual void setAutorisation(const Ent & entity, std::map<bmps::autorisation,bool> autorisations);

//    //! Ajoute les restrictions de modification pour une entité donnée.
//    virtual void setRestriction(const Ent & entity, std::vector<bmps::autorisation> restrictions);
};

//template<class Ent> void AbstractGestionAutorisation<Ent>
//                            ::setAutorisation(const Ent & entity, std::map<bmps::autorisation,bool> autorisations) {
//    for(auto i = autorisations.cbegin(); i != autorisations.cend(); i++)
//        setAutorisation(entity, i->first, i->second);
//}

//template<class Ent> void AbstractGestionAutorisation<Ent>::setRestriction(const Ent & entity, std::vector<bmps::autorisation> restrictions) {
//    for(auto i = restrictions.cbegin(); i != restrictions.cend(); i++)
//        setAutorisation(entity, *i, false);
//}

/*! \ingroup groupeManager
 * \brief Classe template implémentant une gestionnaire d'autorisation non restrictif.
 */
template<class Ent> class GestionAutorisationNoRestrictif : public AbstractGestionAutorisation<Ent>
{
public:
    GestionAutorisationNoRestrictif() = default;

    //! Destructeur.
    ~GestionAutorisationNoRestrictif() override = default;

    //! Demande l'autorisation de modification pour l'entité d'identifiant id.
    bool getAutorisation(idt /*id*/, bmps::autorisation /*autorisation*/) override
        {return true;}

    //! Demande l'autorisation de modification pour une entité d'identifiant id avec les valeur de entity.
    bool getAutorisation(szt /*id*/, bmps::autorisation /*autorisation*/, const Ent & /*entity*/) override
        {return true;}

    //! Demande la liste des restrictions de modification pour l'entité d'identifiant id.
    std::vector<unsigned> getRestriction(idt /*id*/) override
        {return std::vector<unsigned>();}

//    //! Modifie les autorisations de modification pour une entité donnée.
//    void setAutorisation(const Ent & /*entity*/, std::map<bmps::autorisation,bool> /*autorisations*/) override;

//    //! Ajoute les restrictions de modification pour une entité donnée.
//    void setRestriction(const Ent & /*entity*/, std::vector<bmps::autorisation> /*restrictions*/) override
//        {}
};

//template<class Ent> void GestionAutorisationNoRestrictif<Ent>::setAutorisation(const Ent & /*entity*/, std::map<bmps::autorisation,bool> /*autorisations*/)
//    {}
}
#endif // ABSTRACTGESTIONAUTORISATION_H
