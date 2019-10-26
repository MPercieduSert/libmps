/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/08/2018
 */
#ifndef MANAGEROFPERMISSION_H
#define MANAGEROFPERMISSION_H

#include "ManagerSql.h"

/*! \ingroup groupeManager
 * \brief Classe template mère des différents manageurs de permissions code.
 */
template<class Ent> class AbstractManagerOfPermission : virtual public ManagerSql<Ent>
{
protected:
    using ManagerSqlEnt = ManagerSql<Ent>;
    using ManagerSqlEnt::del;
public:
    using ManagerSqlEnt::ManagerSql;
    using ManagerSqlEnt::exists;
    using ManagerSqlEnt::get;
    using EntType = Ent;


    //! Destructeur.
    ~AbstractManagerOfPermission() override = default;

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes attributs (identifiant compris).
    bool sameInBdd(const Ent & entity)
    {
        if(isVirtual(entity))
            return !exists(entity);
        else
            return ManagerSqlEnt::sameInBdd(entity);
    }

    //! Teste s'il y a dans la base de donnée une entité d'identifiant id ayant exactement les mêmes attributs.
    bool sameInBdd(const Ent & entity, idt id)
    {
        if(isVirtual(entity))
            return !exists(Ent(id));
        else
            return ManagerSqlEnt::sameInBdd(entity,id);
    }

protected:
    /*//! Constructeur.
    ManagerOfPermission() {}*/

    //! Insert la nouvelle entité entity dans la base de donnée
    //! et assigne l'identifiant de l'entité insérée en base de donnée à entity.
    void add(Ent & entity) override
    {
        if(!isVirtual(entity))
            addParent(entity);
    }

    //! Insert la nouvelle entité entity dans la base de donnée.
    void add(const Ent & entity) override
    {
        if(!isVirtual(entity))
            addParent(entity);
    }

    //! Appelle la fonction d'insertion parent souhaitée.
    virtual void addParent(Ent & entity)
        {ManagerSqlEnt::add(entity);}

    //! Appelle la fonction d'insertion parent souhaitée.
    virtual void addParent(const Ent & entity)
        {ManagerSqlEnt::add(entity);}

    //! Test si l'entité doit exister dans la base de donnée.
    virtual bool isVirtual(const Ent entity) const = 0;

    //! Met à jour l'entité entity en base de donnée.
    void modify(const Ent & entity) override
    {
        if(isVirtual(entity))
            del(entity.id());
        else
            modifyParent(entity);
    }

    //! Met à jour l'entité entity en base de donnée d'identifiant id avec les valeurs d'entity.
    void modify(const Ent & entity, idt id) override
    {
        if(isVirtual(entity))
            del(id);
        else
            modifyParent(entity,id);
    }

    //! Appelle la fonction de modification parent souhaitée.
    virtual void modifyParent(const Ent & entity)
        {ManagerSqlEnt::modify(entity);}

    //! Appelle la fonction de modification parent souhaitée.
    virtual void modifyParent(const Ent & entity, idt id)
        {ManagerSqlEnt::modify(entity,id);}
};

/*! \ingroup groupeManager
 * \brief Classe template mère des différents manageurs de permissions num.
 */
template<class Ent> class ManagerOfPermissionNum : virtual public AbstractManagerOfPermission<Ent>
{
public:
    enum {NoExists = 0};
    using AbstractManagerOfPermission<Ent>::AbstractManagerOfPermission;

    //! Destructeur.
    ~ManagerOfPermissionNum() override = default;

protected:
    //! Test si l'entité doit exister dans la base de donnée.
    bool isVirtual(const Ent entity) const override
        {return entity.num() == NoExists;}
};

/*! \ingroup groupeManager
 * \brief Classe template mère des différents manageurs de permissions code.
 */
template<class Ent> class ManagerOfPermissionCode : virtual public AbstractManagerOfPermission<Ent>
{
public:
    enum {NoExists = bdd::Autorisation::Toute};
    using AbstractManagerOfPermission<Ent>::AbstractManagerOfPermission;

    //! Destructeur.
    ~ManagerOfPermissionCode() override = default;

protected:
    //! Test si l'entité doit exister dans la base de donnée.
    bool isVirtual(const Ent entity) const override
        {return entity.code() == NoExists;}
};
#endif // MANAGEROFPERMISSION_H
