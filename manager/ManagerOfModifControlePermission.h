/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/08/2018
 */
#ifndef MANAGEROFMODIFCONTROLEPERMISSION_H
#define MANAGEROFMODIFCONTROLEPERMISSION_H

#include "ManagerOfModifControle.h"
#include "ManagerOfPermission.h"

/*! \ingroup groupeManager
 * \brief Classe template des différents manageurs à modification controlée de permission.
 */
template<class Ent> class ManagerOfModifControlePermission : public ManagerOfPermission<Ent>,
                                                                           public ManagerOfModifControle<Ent>
{
protected:
    using ManagerPermission = ManagerOfPermission<Ent>;
    using ManagerMC = ManagerOfModifControle<Ent>;

    using ManagerPermission::add;

public:
    using ManagerPermission::sameInBdd;
    using ManagerMC::del;

    //! Constructeur
    ManagerOfModifControlePermission (const InfoBdd & info, AbstractGestionAutorisation<Ent> * gestionAutorisation = new GestionAutorisationNoRestrictif<Ent>(),
                                      AbstractUniqueSqlTemp<Ent> * unique = new NoUniqueSql<Ent>())
        : ManagerSql<Ent>(info, unique), ManagerOfPermission<Ent> (info,unique),
          ManagerOfModifControle<Ent>(info, gestionAutorisation, unique)
    {}

    //! Destructeur.
    ~ManagerOfModifControlePermission() override = default;

protected:
    /*//! Constructeur.
    ManagerOfModifControlePermission() = default;*/

    //! Appelle la fonction d'insertion parent souhaitée.
    void addParent(Ent & entity) override
        {ManagerMC::add(entity);}

    //! Appelle la fonction d'insertion parent souhaitée.
    void addParent(const Ent & entity) override
        {ManagerMC::add(entity);}

    //! Supprime de la table en base de donnée l'entité d'identifiant id.
    bool del(int id) override;

    //! Réimplemente modify.
    void modify(const Ent & entity) override
        {ManagerPermission::modify(entity);}

    //! Réimplemente modify.
    void modify(const Ent & entity, int id) override
        {ManagerPermission::modify(entity,id);}

    //! Appelle la fonction de modification parent souhaitée.
    void modifyParent(const Ent & entity) override
        {ManagerMC::modify(entity);}

    //! Appelle la fonction de modification parent souhaitée.
    void modifyParent(const Ent & entity, int id) override
        {ManagerMC::modify(entity,id);}
};

template<class Ent> bool ManagerOfModifControlePermission<Ent>::del(int id)
    {return ManagerMC::del(id);}
#endif // MANAGEROFMODIFCONTROLEPERMISSION_H
