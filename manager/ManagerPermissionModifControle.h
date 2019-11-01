/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/08/2018
 */
#ifndef MANAGERPERMISSIONMODIFCONTROLE_H
#define MANAGERPERMISSIONMODIFCONTROLE_H

#include "ManagerModifControle.h"
#include "ManagerPermission.h"

/*! \ingroup groupeManager
 * \brief Classe template des différents manageurs à modification controlée de permission.
 */
template<class ManagerOfPerm> class ManagerOfPermissionModifControle : public ManagerOfPerm,
                                                                           public ManagerOfModifControle<typename ManagerOfPerm::EntType>
{
protected:
    using Ent = typename ManagerOfPerm::EntType;
    using ManagerMC = ManagerOfModifControle<Ent>;
    using ManagerOfPerm::add;

public:
    using ManagerOfPerm::sameInBdd;
    using ManagerMC::del;

    //! Constructeur
    ManagerOfPermissionModifControle (const InfoBdd & info,
                                      AbstractGestionAutorisation<Ent> * gestionAutorisation = new GestionAutorisationNoRestrictif<Ent>(),
                                      AbstractUniqueSqlTemp<Ent> * unique = new NoUniqueSql<Ent>())
        : ManagerSql<Ent>(info, unique), ManagerOfPerm(info,unique),
          ManagerOfModifControle<Ent>(info, gestionAutorisation, unique)
    {}

    //! Destructeur.
    ~ManagerOfPermissionModifControle() override = default;

protected:
    /*//! Constructeur.
    ManagerOfPermissionModifControle() = default;*/

    //! Appelle la fonction d'insertion parent souhaitée.
    void addParent(Ent & entity) override
        {ManagerMC::add(entity);}

    //! Appelle la fonction d'insertion parent souhaitée.
    void addParent(const Ent & entity) override
        {ManagerMC::add(entity);}

    //! Supprime de la table en base de donnée l'entité d'identifiant id.
    bool del(idt id) override;

    //! Réimplemente modify.
    void modify(const Ent & entity) override
        {ManagerOfPerm::modify(entity);}

    //! Réimplemente modify.
    void modify(const Ent & entity, idt id) override
        {ManagerOfPerm::modify(entity,id);}

    //! Appelle la fonction de modification parent souhaitée.
    void modifyParent(const Ent & entity) override
        {ManagerMC::modify(entity);}

    //! Appelle la fonction de modification parent souhaitée.
    void modifyParent(const Ent & entity, idt id) override
        {ManagerMC::modify(entity,id);}
};

template<class Ent> bool ManagerOfPermissionModifControle<Ent>::del(idt id)
    {return ManagerMC::del(id);}

//! Manager de permission de type num à modification controlé.
template<class Ent> using ManagerOfPermissionNumModifControle = ManagerOfPermissionModifControle<ManagerOfPermissionNum<Ent>>;
//! Manager de permission de type code à modification controlé.
template<class Ent> using ManagerOfPermissionCodeModifControle = ManagerOfPermissionModifControle<ManagerOfPermissionCode<Ent>>;
#endif // MANAGERPERMISSIONMODIFCONTROLE_H
