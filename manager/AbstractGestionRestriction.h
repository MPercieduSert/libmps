/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef ABSTRACTGESTIONRESTRICTION_H
#define ABSTRACTGESTIONRESTRICTION_H

#include "AbstractManager.h"

namespace managerMPS {
/*! \ingroup groupeManager
 * \brief Classe template abstraite mère des différents gestionnaire d'autorisation.
 */
template<class Ent> class AbstractGestionRestriction {
protected:
    flag m_restriction;  //! Ensemble des restrictions gérée par le gestionère d'autorisation.
public:
    //! Constructeur.
    AbstractGestionRestriction(flag restriction = bddMPS::Aucune)
        : m_restriction(restriction) {}

    //!Destructeur.
    virtual ~AbstractGestionRestriction() = default;

    //! Ajoute des restrictions gérées par le gestionnaire.
    void addEnableRestriction(flag restriction)
        {m_restriction |= restriction;}

    //! Ajoute des restrictions de modification pour une entité donnée.
    virtual void addRestriction(idt /*id*/, flag restrict) {
        if(restrict & ~m_restriction)
            throw std::invalid_argument(QString("void AbstractGestionRestriction<").append(Ent::Name())
                                        .append(">::addRestriction(idt,flag))\n ")
                                        .append("restriction non gérée par le gestionére d'autorisation :")
                                        .append(QString::number(restrict.value())).toStdString());
    }

    //! Accesseur des restrictions gérées par le gestionnaire.
    flag enableRestriction() const
        {return m_restriction;}

    //! Demande les restrictions de modification pour l'entité d'identifiant id.
    virtual flag getRestriction(idt /*id*/)
        {return bddMPS::Aucune;}

    //! Mutateur des restrictions gérées par le gestionnaire.
    void setEnableRestriction(flag restriction)
        {m_restriction = restriction;}

    //! Modifie les restrictions de modification pour une entité donnée.
    virtual void setRestriction(idt /*id*/, flag restrict) {
        if(restrict & ~m_restriction)
            throw std::invalid_argument(QString("void AbstractGestionRestriction<").append(Ent::Name())
                                        .append(">::setRestriction(idt,flag))\n ")
                                        .append("restriction non gérée par le gestionére d'autorisation :")
                                        .append(QString::number(restrict.value())).toStdString());
    }

    //! Teste la restriction de modification pour une entité d'identifiant id.
    virtual bool testRestriction(idt /*id*/, flag /*restrict*/)
        {return false;}

    //! Teste la restriction de modification pour une entité d'identifiant id avec les valeur de entity.
    virtual bool testRestriction(idt id, flag restrict, const Ent & /*entity*/)
        {return testRestriction(id,restrict);}
};
}
#endif // ABSTRACTGESTIONRESTRICTION_H
