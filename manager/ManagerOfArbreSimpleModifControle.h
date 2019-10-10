/*Auteur: PERCIE DU SERT Maxime
 *Date: 14/08/2017
 */
#ifndef MANAGEROFARBRESIMPLEMODIFCONTROLE_H
#define MANAGEROFARBRESIMPLEMODIFCONTROLE_H

#include "ManagerOfModifControle.h"
#include "ManagerOfArbreSimple.h"

/*! \ingroup groupeManager
 * \brief Classe template mère des différents manageurs pour les entités de type arbre simple avec un controle de modification.
 */
template<class Ent> class ManagerOfArbreSimpleModifControle : public ManagerOfArbreSimple<Ent>,
                                                                      public ManagerOfModifControle<Ent>
{
protected:
    using ManagerArbreEnt = ManagerOfArbreSimple<Ent>;
    using ManagerMC = ManagerOfModifControle<Ent>;

    using ManagerMC::modify;

public:
    using ManagerArbreEnt::get;
    using ManagerMC::save;
    using ManagerMC::getAutorisation;

    //! Constructeur.
    ManagerOfArbreSimpleModifControle(const InfoBdd & info,
                                      AbstractGestionAutorisation<Ent> * gestionAutorisation
                                                        = new GestionAutorisationNoRestrictif<Ent>(),
                                      AbstractUniqueSqlTemp<Ent> * unique = new NoUniqueSql<Ent>())
        : ManagerSql<Ent>(info,unique), ManagerOfArbreSimple<Ent> (info, unique),
          ManagerOfModifControle<Ent>(info, gestionAutorisation, unique)
    {}

    //! Destructeur.
    ~ManagerOfArbreSimpleModifControle() override = default;

    /*//! Supprime de la table en base de donnée l'entité d'identifiant id.
    bool del(int id) override
        {return getAutorisation(Ent(id), bdd::Suppr) && ManagerArbreEnt::del(id);}*/

    //! Enregistre l'entité entity en base de donnée.
    void save(Ent & entity) override
        {ManagerArbreEnt::save(entity);}

    //! Enregistre l'entité entity en base de donnée.
    void save(const Ent & entity) override
        {ManagerArbreEnt::save(entity);}

protected:
    /*//! Constructeur.
    ManagerOfArbreSimpleModifControle(AbstractGestionAutorisation<Ent> *gestionAutorisation)
        : ManagerOfModifControle<Ent>(gestionAutorisation)
    {}*/
};

#endif // MANAGEROFARBRESIMPLEMODIFCONTROLE_H
