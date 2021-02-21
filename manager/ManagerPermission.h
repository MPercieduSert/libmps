/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/08/2018
 */
#ifndef MANAGERPERMISSION_H
#define MANAGERPERMISSION_H

#include "ManagerSql.h"

namespace managerMPS {
/*! \ingroup groupeManager
 *\brief Classe template mère des différents manageurs de permissions code.
 */
template<class Ent> class AbstractManagerPermission : virtual public ManagerSql<Ent> {
protected:
    using ManagerSqlEnt = ManagerSql<Ent>;
    using ManagerSqlEnt::del;
public:
    using ManagerSqlEnt::ManagerSql;
    using ManagerSqlEnt::exists;
    using ManagerSqlEnt::get;
    using EntType = Ent;


    //! Destructeur.
    ~AbstractManagerPermission() override = default;

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes attributs (identifiant compris).
    bool sameInBdd(const Ent &ent) {
        if(isVirtual(entity))
            return !exists(entity);
        else
            return ManagerSqlEnt::sameInBdd(entity);
    }

    //! Teste s'il y a dans la base de donnée une entité d'identifiant id ayant exactement les mêmes attributs.
    bool sameInBdd(const Ent &ent, idt id) {
        if(isVirtual(entity))
            return !exists(Ent(id));
        else
            return ManagerSqlEnt::sameInBdd(entity,id);
    }

    //! Retourne le type du manager.
    virtual flag typeManager() const
        {return bmps::PermissionTypeManager;}

protected:
    //! Insert la nouvelle entité entity dans la base de donnée
    //! et assigne l'identifiant de l'entité insérée en base de donnée à entity.
    void add(Ent &ent) override {
        if(!isVirtual(entity))
            addParent(entity);
    }

    //! Insert la nouvelle entité entity dans la base de donnée.
    void add(const Ent &ent) override {
        if(!isVirtual(entity))
            addParent(entity);
    }

    //! Appelle la fonction d'insertion parent souhaitée.
    virtual void addParent(Ent &ent)
        {ManagerSqlEnt::add(entity);}

    //! Appelle la fonction d'insertion parent souhaitée.
    virtual void addParent(const Ent &ent)
        {ManagerSqlEnt::add(entity);}

    //! Test si l'entité doit exister dans la base de donnée.
    virtual bool isVirtual(const Ent entity) const = 0;

    //! Met à jour l'entité entity en base de donnée.
    void modify(const Ent &ent) override {
        if(isVirtual(entity))
            del(entity.id());
        else
            modifyParent(entity);
    }

    //! Met à jour l'entité entity en base de donnée d'identifiant id avec les valeurs d'entity.
    void modify(const Ent &ent, idt id) override {
        if(isVirtual(entity))
            del(id);
        else
            modifyParent(entity,id);
    }

    //! Appelle la fonction de modification parent souhaitée.
    virtual void modifyParent(const Ent &ent)
        {ManagerSqlEnt::modify(entity);}

    //! Appelle la fonction de modification parent souhaitée.
    virtual void modifyParent(const Ent &ent, idt id)
        {ManagerSqlEnt::modify(entity,id);}
};

/*! \ingroup groupeManager
 *\brief Classe template mère des différents manageurs de permissions num.
 */
template<class Ent> class ManagerPermissionNum : virtual public AbstractManagerPermission<Ent>
{
public:
    enum {NoExists = 0};
    using AbstractManagerPermission<Ent>::AbstractManagerPermission;

    //! Destructeur.
    ~ManagerPermissionNum() override = default;

protected:
    //! Test si l'entité doit exister dans la base de donnée.
    bool isVirtual(const Ent entity) const override
        {return entity.num() == NoExists;}
};

/*! \ingroup groupeManager
 *\brief Classe template mère des différents manageurs de permissions code.
 */
template<class Ent> class ManagerPermissionCode : virtual public AbstractManagerPermission<Ent>
{
public:
    using AbstractManagerPermission<Ent>::AbstractManagerPermission;

    //! Destructeur.
    ~ManagerPermissionCode() override = default;

protected:
    //! Test si l'entité doit exister dans la base de donnée.
    bool isVirtual(const Ent entity) const override
        {return !entity.code();}
};
}
#endif // MANAGERPERMISSION_H
