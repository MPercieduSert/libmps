/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/08/2018
 */
#ifndef MANAGER_PERMISSION_H
#define MANAGER_PERMISSION_H

#include "manager_sql.h"

namespace mps {
namespace manager {
/*! \ingroup groupe_manager
 *\brief Classe template mère des différents manageurs de permissions code.
 */
template<class Ent> class abstract_manager_permission : virtual public manager_sql<Ent> {
protected:
    using ma_sql = manager_sql<Ent>;
    using ma_sql::del;
public:
    using ma_sql::manager_sql;
    using ma_sql::exists;
    using ma_sql::get;
    using ent_type = Ent;

    //! Destructeur.
    ~abstract_manager_permission() override = default;

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes attributs (identifiant compris).
    bool same_in_bdd(const Ent &ent) {
        if(is_virtual(ent))
            return !exists(ent);
        else
            return ma_sql::same_in_bdd(ent);
    }

    //! Teste s'il y a dans la base de donnée une entité d'identifiant id ayant exactement les mêmes attributs.
    bool same_in_bdd(const Ent &ent, idt id) {
        if(is_virtual(ent))
            return !exists(Ent(id));
        else
            return ma_sql::same_in_bdd(ent,id);
    }

    //! Retourne le type du manager.
    virtual flag type_manager() const
        {return b2d::Permission_Type_Manager;}

protected:
    //! Insert la nouvelle entité entity dans la base de donnée
    //! et assigne l'identifiant de l'entité insérée en base de donnée à ent.
    void add(Ent &ent) override {
        if(!is_virtual(ent))
            add_parent(ent);
    }

    //! Insert la nouvelle entité entity dans la base de donnée.
    void add(const Ent &ent) override {
        if(!is_virtual(ent))
            add_parent(ent);
    }

    //! Appelle la fonction d'insertion parent souhaitée.
    virtual void add_parent(Ent &ent)
        {ma_sql::add(ent);}

    //! Appelle la fonction d'insertion parent souhaitée.
    virtual void add_parent(const Ent &ent)
        {ma_sql::add(ent);}

    //! Test si l'entité doit exister dans la base de donnée.
    virtual bool is_virtual(const Ent &ent) const = 0;

    //! Met à jour l'entité entity en base de donnée.
    void modify(const Ent &ent) override {
        if(is_virtual(ent))
            del(ent.id());
        else
            modify_parent(ent);
    }

    //! Met à jour l'entité entity en base de donnée d'identifiant id avec les valeurs d'ent.
    void modify(const Ent &ent, idt id) override {
        if(is_virtual(ent))
            del(id);
        else
            modify_parent(ent,id);
    }

    //! Appelle la fonction de modification parent souhaitée.
    virtual void modify_parent(const Ent &ent)
        {ma_sql::modify(ent);}

    //! Appelle la fonction de modification parent souhaitée.
    virtual void modify_parent(const Ent &ent, idt id)
        {ma_sql::modify(ent,id);}
};

/*! \ingroup groupe_manager
 *\brief Classe template mère des différents manageurs de permissions num.
 */
template<class Ent> class manager_permission_num : virtual public abstract_manager_permission<Ent>
{
public:
    enum {No_Exists = 0};
    using abstract_manager_permission<Ent>::abstract_manager_permission;

    //! Destructeur.
    ~manager_permission_num() override = default;

protected:
    //! Test si l'entité doit exister dans la base de donnée.
    bool is_virtual(const Ent &ent) const override
        {return ent.num() == No_Exists;}
};

/*! \ingroup groupe_manager
 *\brief Classe template mère des différents manageurs de permissions code.
 */
template<class Ent> class manager_permission_code : virtual public abstract_manager_permission<Ent>
{
public:
    using abstract_manager_permission<Ent>::abstract_manager_permission;

    //! Destructeur.
    ~manager_permission_code() override = default;

protected:
    //! Test si l'entité doit exister dans la base de donnée.
    bool is_virtual(const Ent &ent) const override
        {return !ent.code();}
};
}}
#endif // MANAGER_PERMISSION_H
