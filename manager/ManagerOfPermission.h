/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/08/2018
 */
#ifndef MANAGEROFPERMISSION_H
#define MANAGEROFPERMISSION_H

#include "ManagerSql.h"

// Macro pour manageur.
//! \ingroup groupeManager
//! Coprs des deux methodes add.
#define ADD_PERMISSION if(entity.num() != NoExists) addParent(entity);

/*! \ingroup groupeManager
 * \brief Classe template mère des différents manageurs de permissions.
 */
template<class Ent> class ManagerOfPermission : virtual public ManagerSql<Ent>
{
public:
    enum {NoExists = 0};

protected:
    using ManagerSqlEnt = ManagerSql<Ent>;
    using ManagerSqlEnt::del;
public:
    using ManagerSqlEnt::exists;
    using ManagerSqlEnt::get;

    //! Constructeur
    ManagerOfPermission (const InfoBdd & info, AbstractUniqueSqlTemp<Ent> * unique = new NoUniqueSql<Ent>())
        : ManagerSql<Ent>(info, unique)
    {}

    //! Destructeur.
    ~ManagerOfPermission() override = default;

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes attributs (identifiant compris).
    bool sameInBdd(const Ent & entity)
    {
        if(entity.num() == NoExists)
            return !exists(entity);
        else
        {
            Ent entityT(entity.id());
            return get(entityT) ? entityT == entity : false;
        }
    }

    //! Teste s'il y a dans la base de donnée une entité d'identifiant id ayant exactement les mêmes attributs.
    bool sameInBdd(const Ent & entity, int id)
    {
        if(entity.num() == NoExists)
            return !exists(Ent(id));
        else
        {
            Ent entityT(id);
            if(!get(entityT))
                return false;
            entityT.setId(entity.id());
            return entityT == entity;
        }
    }

protected:
    /*//! Constructeur.
    ManagerOfPermission() {}*/

    //! Insert la nouvelle entité entity dans la base de donnée
    //! et assigne l'identifiant de l'entité insérée en base de donnée à entity.
    void add(Ent & entity) override
        {ADD_PERMISSION}

    //! Insert la nouvelle entité entity dans la base de donnée.
    void add(const Ent & entity) override
        {ADD_PERMISSION}

    //! Appelle la fonction d'insertion parent souhaitée.
    virtual void addParent(Ent & entity)
        {ManagerSqlEnt::add(entity);}

    //! Appelle la fonction d'insertion parent souhaitée.
    virtual void addParent(const Ent & entity)
        {ManagerSqlEnt::add(entity);}

    //! Met à jour l'entité entity en base de donnée.
    void modify(const Ent & entity) override
    {
        if(entity.num() != NoExists)
            modifyParent(entity);
        else
            del(entity.id());
    }

    //! Met à jour l'entité entity en base de donnée d'identifiant id avec les valeurs d'entity.
    void modify(const Ent & entity, int id) override
    {
        if(entity.num() != NoExists)
            modifyParent(entity, id);
        else
            del(id);
    }

    //! Appelle la fonction de modification parent souhaitée.
    virtual void modifyParent(const Ent & entity)
        {ManagerSqlEnt::modify(entity);}

    //! Appelle la fonction de modification parent souhaitée.
    virtual void modifyParent(const Ent & entity, int id)
        {ManagerSqlEnt::modify(entity,id);}
};

#endif // MANAGEROFPERMISSION_H
