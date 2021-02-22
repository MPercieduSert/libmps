/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef ABSTRACT_GESTION_RESTRICTION_H
#define ABSTRACT_GESTION_RESTRICTION_H

#include "abstract_manager.h"

namespace mps {
namespace manager {
/*! \ingroup groupe_manager
 *\brief Classe template abstraite mère des différents gestionnaire d'autorisation.
 */
template<class Ent> class abstract_gestion_restriction {
protected:
    flag m_restriction;  //! Ensemble des restrictions gérée par le gestionère d'autorisation.
public:
    //! Constructeur.
    abstract_gestion_restriction(flag restriction = b2d::Aucune)
        : m_restriction(restriction) {}

    //!Destructeur.
    virtual ~abstract_gestion_restriction() = default;

    //! Ajoute des restrictions gérées par le gestionnaire.
    void add_enable_restriction(flag restriction)
        {m_restriction |= restriction;}

    //! Ajoute des restrictions de modification pour une entité donnée.
    virtual void add_restriction(idt /*id*/, flag restrict) {
        if(restrict &~m_restriction)
            throw std::invalid_argument(QString("void abstract_gestion_restriction<").append(Ent::Name())
                                        .append(">::add_restriction(idt,flag))\n ")
                                        .append("restriction non gérée par le gestionére d'autorisation :")
                                        .append(QString::number(restrict.value())).toStdString());
    }

    //! Accesseur des restrictions gérées par le gestionnaire.
    flag enable_restriction() const
        {return m_restriction;}

    //! Demande les restrictions de modification pour l'entité d'identifiant id.
    virtual flag get_restriction(idt /*id*/)
        {return b2d::Aucune;}

    //! Mutateur des restrictions gérées par le gestionnaire.
    void set_enable_restriction(flag restriction)
        {m_restriction = restriction;}

    //! Modifie les restrictions de modification pour une entité donnée.
    virtual void set_restriction(idt /*id*/, flag restrict) {
        if(restrict &~m_restriction)
            throw std::invalid_argument(QString("void abstract_gestion_restriction<").append(Ent::Name())
                                        .append(">::set_restriction(idt,flag))\n ")
                                        .append("restriction non gérée par le gestionére d'autorisation :")
                                        .append(QString::number(restrict.value())).toStdString());
    }

    //! Teste la restriction de modification pour une entité d'identifiant id.
    virtual bool test_restriction(idt /*id*/, flag /*restrict*/)
        {return false;}

    //! Teste la restriction de modification pour une entité d'identifiant id avec les valeur de ent.
    virtual bool test_restriction(idt id, flag restrict, const Ent &/*ent*/)
        {return test_restriction(id,restrict);}
};
}}
#endif // ABSTRACT_GESTION_RESTRICTION_H
