/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef UNIQUE_SQL_BASE_H
#define UNIQUE_SQL_BASE_H

#include "abstract_unique_sql.h"

namespace mps {
namespace manager {
// Unique arbre
/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (num,parent).
 */
template<class Ent> class arbre_unique_sql : public simple_unique_sql<Ent> {
protected:
    using simple_unique_sql<Ent>::bind_value;
public:
    enum {Num_Unique,Parent_Unique,Nbr_Unique};
    CONSTR_DEFAUT(arbre_unique_sql)

    //!Destructeur.
    ~arbre_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        bind_value(Num_Unique,ent.num());
        bind_value(Parent_Unique,ent.parent());
    }
};

// Unique CibleSimple
/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (Id_Cible,Cible).
 */
template<class Ent> class cible_simple_unique_sql : public simple_unique_sql<Ent> {
protected:
    using simple_unique_sql<Ent>::bind_value;
public:
    enum {Id_Cible_Unique,Cible_Unique,Nbr_Unique};
    CONSTR_DEFAUT(cible_simple_unique_sql)

    //!Destructeur.
    ~cible_simple_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        bind_value(Id_Cible_Unique,ent.id_cible());
        bind_value(Cible_Unique,ent.cible());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triple (Id_Cible,Cible,Num).
 */
template<class Ent> class cible_simple_num_unique_sql : public cible_simple_unique_sql<Ent> {
protected:
    using cible_simple_unique_sql<Ent>::bind_value;
public:
    enum {Num_Unique = cible_simple_unique_sql<Ent>::Nbr_Unique, Nbr_Unique};
    CONSTR_DEFAUT(cible_simple_num_unique_sql)

    //!Destructeur.
    ~cible_simple_num_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        cible_simple_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Num_Unique,ent.num());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triple (Id_Cible,Cible,Num,Type).
 */
template<class Ent> class cible_simple_num_type_unique_sql : public cible_simple_num_unique_sql<Ent> {
protected:
    using cible_simple_num_unique_sql<Ent>::bind_value;
public:
    enum {Type_Unique = cible_simple_num_unique_sql<Ent>::Nbr_Unique, Nbr_Unique};
    CONSTR_DEFAUT(cible_simple_num_type_unique_sql)

    //!Destructeur.
    ~cible_simple_num_type_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        cible_simple_num_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Type_Unique,ent.type());
    }
};

// Unique Historique
/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le quadruplet (Id_Cible,Cible,Id_Etat,Etat).
 */
template<class Ent> class historique_unique_sql : public cible_simple_unique_sql<Ent> {
protected:
    using cible_simple_unique_sql<Ent>::bind_value;
public:
    enum {Id_Etat_Unique = cible_simple_unique_sql<Ent>::Nbr_Unique, Etat_Unique, Num_Unique, Nbr_Unique};
    CONSTR_DEFAUT(historique_unique_sql)

    //!Destructeur.
    ~historique_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        cible_simple_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Id_Etat_Unique,ent.Id_Etat());
        bind_value(Etat_Unique,ent.etat());
        bind_value(Num_Unique,ent.num());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triple (Id_Cible,Cible,ref).
 */
template<class Ent> class cible_simple_ref_unique_sql : public cible_simple_unique_sql<Ent> {
protected:
    using cible_simple_unique_sql<Ent>::bind_value;
public:
    enum {Ref_Unique = cible_simple_unique_sql<Ent>::Nbr_Unique, Nbr_Unique};
    CONSTR_DEFAUT(cible_simple_ref_unique_sql)

    //!Destructeur.
    ~cible_simple_ref_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        cible_simple_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Ref_Unique,ent.ref());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur id_1.
 */
template<class Ent> class id_unique_sql : public simple_unique_sql<Ent> {
protected:
    using simple_unique_sql<Ent>::bind_value;
public:
    enum {Id_1_Unique,Nbr_Unique};
    CONSTR_DEFAUT(id_unique_sql)

    //! Destructeur.
    ~id_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override
        {bind_value(Id_1_Unique,ent.id_1());}
};

// Unique Cible
/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (Id_1,Cible).
 */
template<class Ent> class id_cible_unique_sql : public id_unique_sql<Ent> {
protected:
    using id_unique_sql<Ent>::bind_value;
public:
    enum {Cible_Unique = id_unique_sql<Ent>::Nbr_Unique,Nbr_Unique};
    CONSTR_DEFAUT(id_cible_unique_sql)

    //!Destructeur.
    ~id_cible_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        id_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Cible_Unique,ent.cible());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triple (Id_1,Cible,Num).
 */
template<class Ent> class id_cible_num_unique_sql : public id_cible_unique_sql<Ent> {
protected:
    using id_cible_unique_sql<Ent>::bind_value;
public:
    enum {Num_Unique = id_cible_unique_sql<Ent>::Nbr_Unique, Nbr_Unique};
    CONSTR_DEFAUT(id_cible_num_unique_sql)

    //!Destructeur.
    ~id_cible_num_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        id_cible_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Num_Unique,ent.num());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triple (Id_1,Cible,ref).
 */
template<class Ent> class id_cible_ref_unique_sql : public id_cible_unique_sql<Ent> {
protected:
    using id_cible_unique_sql<Ent>::bind_value;
public:
    enum {Ref_Unique = id_cible_unique_sql<Ent>::Nbr_Unique, Nbr_Unique};
    CONSTR_DEFAUT(id_cible_ref_unique_sql)

    //!Destructeur.
    ~id_cible_ref_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        id_cible_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Ref_Unique,ent.ref());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triple (Id_1,Id_Cible,Cible).
 */
template<class Ent> class cible_unique_sql : public id_cible_unique_sql<Ent> {
protected:
    using id_cible_unique_sql<Ent>::bind_value;
public:
    enum {Id_Cible_Unique = id_cible_unique_sql<Ent>::Nbr_Unique, Nbr_Unique};
    CONSTR_DEFAUT(cible_unique_sql)

    //!Destructeur.
    ~cible_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        id_cible_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Id_Cible_Unique,ent.id_cible());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le 4-upplet (Id_1,Id_Cible,Cible,Date_Time).
 */
template<class Ent> class cible_date_time_unique_sql : public cible_unique_sql<Ent> {
protected:
    using cible_unique_sql<Ent>::bind_value;
public:
    enum {Date_Time_Unique = cible_unique_sql<Ent>::Nbr_Unique, Nbr_Unique};
    CONSTR_DEFAUT(cible_date_time_unique_sql)

    //!Destructeur.
    ~cible_date_time_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        cible_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Date_Time_Unique,ent.date_time());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le 4-upplet (Id_1,Id_Cible,Cible,Num).
 */
template<class Ent> class cible_num_unique_sql : public cible_unique_sql<Ent> {
protected:
    using cible_unique_sql<Ent>::bind_value;

public:
    enum {Num_Unique = cible_unique_sql<Ent>::Nbr_Unique, Nbr_Unique};
    CONSTR_DEFAUT(cible_num_unique_sql)

    //!Destructeur.
    ~cible_num_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        cible_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Num_Unique,ent.num());
    }
};

// Cible_null

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (Id_Cible,Cible).
 */
template<class Ent> class cible_null_unique_sql : public simple_unique_sql<Ent> {
protected:
    using simple_unique_sql<Ent>::bind_value;
public:
    enum {Id_Cible_Unique, Cible_Unique, Nbr_Unique};
    CONSTR_DEFAUT(cible_null_unique_sql)

    //!Destructeur.
    ~cible_null_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        bind_value(Id_Cible_Unique,ent.id_cible());
        bind_value(Cible_Unique,ent.cible());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le tripplet (Id_Cible,Cible,Num).
 */
template<class Ent> class cible_null_num_unique_sql : public cible_null_unique_sql<Ent> {
protected:
    using cible_null_unique_sql<Ent>::bind_value;
public:
    enum {Num_Unique = cible_null_unique_sql<Ent>::Nbr_Unique, Nbr_Unique};
    CONSTR_DEFAUT(cible_null_num_unique_sql)

    //!Destructeur.
    ~cible_null_num_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        cible_null_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Num_Unique,ent.num());
    }
};

// Unique Nom

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le nom.
 */
template<class Ent> class nom_unique_sql : public simple_unique_sql<Ent> {
protected:
    using simple_unique_sql<Ent>::bind_value;
public:
    enum {Nom_Unique,Nbr_Unique};
    CONSTR_DEFAUT(nom_unique_sql)

    //! Destructeur.
    ~nom_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override
        {bind_value(Nom_Unique,ent.nom());}
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (nc, nom).
 */
template<class Ent> class nom_nc_unique_sql : public nom_unique_sql<Ent> {
protected:
    using nom_unique_sql<Ent>::bind_value;
public:
    enum {Nc_Unique = nom_unique_sql<Ent>::Nbr_Unique};
    CONSTR_DEFAUT(nom_nc_unique_sql)

    //! Destructeur.
    ~nom_nc_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        nom_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Nc_Unique,ent.nc());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (nom, type).
 */
template<class Ent> class nom_type_unique_sql : public nom_unique_sql<Ent> {
protected:
    using nom_unique_sql<Ent>::bind_value;
public:
    enum {Type_Unique = nom_unique_sql<Ent>::Nbr_Unique,Nbr_Unique};
    CONSTR_DEFAUT(nom_type_unique_sql)

    //! Destructeur.
    ~nom_type_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        nom_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Type_Unique,ent.type());
    }
};

// Unique Relation

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple de clé (id_1,id_2).
 */
template<class Ent> class relation_unique_sql : public id_unique_sql<Ent> {
protected:
    using id_unique_sql<Ent>::bind_value;
public:
    enum {Id_2_Unique = id_unique_sql<Ent>::Nbr_Unique,Nbr_Unique};
    CONSTR_DEFAUT(relation_unique_sql)

    //! Destructeur.
    ~relation_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        id_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Id_2_Unique,ent.id_2());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triplet (id_1,id_2,cible).
 */
template<class Ent> class relation_cible_unique_sql : public relation_unique_sql<Ent> {
protected:
    using relation_unique_sql<Ent>::bind_value;
public:
    enum {Cible_Unique = relation_unique_sql<Ent>::Nbr_Unique,Nbr_Unique};
    CONSTR_DEFAUT(relation_cible_unique_sql)

    //! Destructeur.
    ~relation_cible_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        relation_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Cible_Unique,ent.cible());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triplet (id_1,id_2,date).
 */
template<class Ent> class relation_date_time_unique_sql : public relation_unique_sql<Ent> {
protected:
    using relation_unique_sql<Ent>::bind_value;
public:
    enum {Date_Time_Unique = relation_unique_sql<Ent>::Nbr_Unique,Nbr_Unique};
    CONSTR_DEFAUT(relation_date_time_unique_sql)

    //! Destructeur.
    ~relation_date_time_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        relation_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Date_Time_Unique,ent.date_time());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triplet (id_1,id_2,num).
 */
template<class Ent> class relation_num_unique_sql : public relation_unique_sql<Ent> {
protected:
    using relation_unique_sql<Ent>::bind_value;
public:
    enum {Num_Unique = relation_unique_sql<Ent>::Nbr_Unique,Nbr_Unique};
    CONSTR_DEFAUT(relation_num_unique_sql)

    //! Destructeur.
    ~relation_num_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        relation_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Num_Unique,ent.num());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le quadruplet (id_1,id_2,id_3,num).
 */
template<class Ent> class relation_trois_num_unique_sql : public relation_num_unique_sql<Ent> {
protected:
    using relation_num_unique_sql<Ent>::bind_value;
public:
    enum {Id_3_Unique = relation_num_unique_sql<Ent>::Nbr_Unique,Nbr_Unique};
    CONSTR_DEFAUT(relation_trois_num_unique_sql)

    //! Destructeur.
    ~relation_trois_num_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        relation_num_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Id_3_Unique,ent.id_3());
    }
};

// Unique Divers

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le num.
 */
template<class Ent> class num_unique_sql : public simple_unique_sql<Ent> {
protected:
    using simple_unique_sql<Ent>::bind_value;
public:
    enum {Num_Unique,Nbr_Unique};
    CONSTR_DEFAUT(num_unique_sql)

    //! Destructeur.
    ~num_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override
        {bind_value(Num_Unique,ent.num());}
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (num, type).
 */
template<class Ent> class num_type_unique_sql : public num_unique_sql<Ent> {
protected:
    using num_unique_sql<Ent>::bind_value;
public:
    enum {Type_Unique = num_unique_sql<Ent>::Nbr_Unique,Nbr_Unique};
    CONSTR_DEFAUT(num_type_unique_sql)

    //! Destructeur.
    ~num_type_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        num_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Type_Unique,ent.type());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (id_1,num).
 */
template<class Ent> class id_num_unique_sql : public id_unique_sql<Ent> {
protected:
    using id_unique_sql<Ent>::bind_value;
public:
    enum {Num_Unique = id_unique_sql<Ent>::Nbr_Unique,Nbr_Unique};
    CONSTR_DEFAUT(id_num_unique_sql)

    //! Destructeur.
    ~id_num_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        id_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Num_Unique,ent.num());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (id_1,type).
 */
template<class Ent> class id_type_unique_sql : public id_unique_sql<Ent> {
protected:
    using id_unique_sql<Ent>::bind_value;
public:
    enum {Type_Unique = id_unique_sql<Ent>::Nbr_Unique,Nbr_Unique};
    CONSTR_DEFAUT(id_type_unique_sql)

    //! Destructeur.
    ~id_type_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override {
        id_unique_sql<Ent>::bind_values_unique(ent);
        bind_value(Type_Unique,ent.type());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur l'attribut Id_Prog.
 */
template<class Ent> class id_prog_unique_sql : public simple_unique_sql<Ent> {
protected:
    using simple_unique_sql<Ent>::bind_value;
public:
    enum {Id_Prog_Unique ,Nbr_Unique};
    CONSTR_DEFAUT(id_prog_unique_sql)

    //! Destructeur.
    ~id_prog_unique_sql() override = default;
protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override
        {bind_value(Id_Prog_Unique,ent.Id_Prog());}
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant deux conditions d'unicité sur l'attribut Id_Prog et le nom.
 */
template<class Ent> class id_prog_nom_unique_sql : public double_unique_sql<Ent> {
protected:
    using double_unique_sql<Ent>::bind_value;
    using double_unique_sql<Ent>::bind_values_unique;
    using double_unique_sql<Ent>::bind_values_unique_2;

public:
    enum {Id_Prog_Unique, Nbr_Unique_1, Nom_Unique = Id_Prog_Unique, Nbr_Unique_2,
          Id_Prog_Unique_Set = double_unique_sql<Ent>::Unique_Set_1, Nom_Unique_Set = double_unique_sql<Ent>::Unique_Set_2};
    CONSTR_DEFAUT(id_prog_nom_unique_sql)

    //! Destructeur.
    ~id_prog_nom_unique_sql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override
        {bind_value(Id_Prog_Unique,ent.Id_Prog());}

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique_2(const Ent &ent) override
        {bind_value(Nom_Unique,ent.nom());}
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant deux conditions d'unicité sur l'attribut nom
 *et le 4-upplet (id_1,id_cible,cible,num).
 */
template<class Ent> class nom_cible_unique_sql : public double_unique_sql<Ent> {
protected:
    using double_unique_sql<Ent>::bind_value;
    using double_unique_sql<Ent>::bind_values_unique;
    using double_unique_sql<Ent>::bind_values_unique_2;

public:
    enum {Nom_Unique, Nbr_Unique_1,
          Id_1_Unique = Nom_Unique, Id_Cible_Unique, Cible_Unique, Num_Unique, Nbr_Unique_2,
          Nom_Unique_Set = double_unique_sql<Ent>::Unique_Set_1,
          Id_1_Unique_Set = double_unique_sql<Ent>::Unique_Set_2,
          Id_Cible_Unique_Set = Id_1_Unique_Set,
          Cible_Unique_Set = Id_1_Unique_Set,
          Num_Unique_Set = Id_1_Unique_Set};
    CONSTR_DEFAUT(nom_cible_unique_sql)

    //! Destructeur.
    ~nom_cible_unique_sql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override
        {bind_value(Nom_Unique, ent.nom());}

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique_2(const Ent &ent) override {
        bind_value(Id_1_Unique, ent.id_1());
        bind_value(Id_Cible_Unique, ent.id_cible());
        bind_value(Cible_Unique, ent.cible());
        bind_value(Num_Unique, ent.num());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant deux conditions d'unicité sur l'attribut Id_Prog et le couple (Parent, nom).
 */
template<class Ent> class id_prog_nom_parent_unique_sql : public id_prog_nom_unique_sql<Ent> {
protected:
    using id_prog_nom_unique_sql<Ent>::bind_value;
    using id_prog_nom_unique_sql<Ent>::bind_values_unique_2;

public:
    enum {Parent_Unique = id_prog_nom_unique_sql<Ent>::Nbr_Unique_2, Nbr_Unique_2,
          Parent_Unique_Set = id_prog_nom_unique_sql<Ent>::Nom_Unique_Set};
    CONSTR_DEFAUT(id_prog_nom_parent_unique_sql)

    //! Destructeur.
    ~id_prog_nom_parent_unique_sql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique_2(const Ent &ent) override {
        id_prog_nom_unique_sql<Ent>::bind_values_unique_2(ent);
        bind_value(Parent_Unique,ent.parent());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le texte.
 */
template<class Ent> class texte_unique_sql : public simple_unique_sql<Ent> {
protected:
    using simple_unique_sql<Ent>::bind_value;

public:
    enum {Texte_Unique,Nbr_Unique};
    CONSTR_DEFAUT(texte_unique_sql)

    //! Destructeur.
    ~texte_unique_sql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override
        {bind_value(Texte_Unique,ent.texte());}
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant deux conditions d'unicité (id_1,id_3) et (id_2,id_3) avec id_1 et id_2
 *n'étant pas simultanément nuls ou simultanément non nuls.
 */
template<class Ent> class id_relation_exact_one_not_null_unique_sql : public atts_relation_exact_one_not_null_unique_sql<Ent> {
protected:
    using AREONNU = atts_relation_exact_one_not_null_unique_sql<Ent>;
    using AREONNU::bind_value;

public:
    enum {Id_3_Unique = AREONNU::Nbr_Unique_1 ,Nbr_Unique_1, Nbr_Unique_2 = Nbr_Unique_1};
    CONSTR_DEFAUT(id_relation_exact_one_not_null_unique_sql)

    //! Destructeur.
    ~id_relation_exact_one_not_null_unique_sql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique_atts(const Ent &ent) override
        {bind_value(Id_3_Unique,ent.id_3());}
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur l'attribut ref.
 */
template<class Ent> class ref_unique_sql : public simple_unique_sql<Ent> {
protected:
    using simple_unique_sql<Ent>::bind_value;

public:
    enum {Ref_Unique ,Nbr_Unique};
    CONSTR_DEFAUT(ref_unique_sql)

    //! Destructeur.
    ~ref_unique_sql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override
        {bind_value(Ref_Unique,ent.ref());}
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant deux conditions d'unicité sur l'attribut ref et le couple (nom, type).
 */
template<class Ent> class ref_nom_type_unique_sql : public double_unique_sql<Ent> {
protected:
    using double_unique_sql<Ent>::bind_value;
    using double_unique_sql<Ent>::bind_values_unique;
    using double_unique_sql<Ent>::bind_values_unique_2;

public:
    enum {Ref_Unique, Nbr_Unique_1,
          Nom_Unique = Ref_Unique, Type_Unique, Nbr_Unique_2,
          Ref_Unique_Set = double_unique_sql<Ent>::Unique_Set_1,
          Nom_Unique_Set = double_unique_sql<Ent>::Unique_Set_2,
          Type_Unique_Set = double_unique_sql<Ent>::Unique_Set_2};
    CONSTR_DEFAUT(ref_nom_type_unique_sql)

    //! Destructeur.
    ~ref_nom_type_unique_sql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override
        {bind_value(Ref_Unique,ent.ref());}

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique_2(const Ent &ent) override {
        bind_value(Nom_Unique,ent.nom());
        bind_value(Type_Unique,ent.type());
    }
};

/*! \ingroup groupe_unique_sql
 *\brief Classe condition d'unicité pour les entités possédant deux conditions d'unicité sur l'attribut ref et le couple (nom, parent).
 */
template<class Ent> class ref_nom_parent_unique_sql : public double_unique_sql<Ent> {
protected:
    using double_unique_sql<Ent>::bind_value;
    using double_unique_sql<Ent>::bind_values_unique;
    using double_unique_sql<Ent>::bind_values_unique_2;

public:
    enum {Ref_Unique, Nbr_Unique_1,
          Nom_Unique = Ref_Unique, Parent_Unique, Nbr_Unique_2,
          Ref_Unique_Set = double_unique_sql<Ent>::Unique_Set_1,
          Nom_Unique_Set = double_unique_sql<Ent>::Unique_Set_2,
          Parent_Unique_Set = double_unique_sql<Ent>::Unique_Set_2};
    CONSTR_DEFAUT(ref_nom_parent_unique_sql)

    //! Destructeur.
    ~ref_nom_parent_unique_sql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique(const Ent &ent) override
        {bind_value(Ref_Unique,ent.ref());}

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bind_values_unique_2(const Ent &ent) override {
        bind_value(Nom_Unique,ent.nom());
        bind_value(Parent_Unique,ent.parent());
    }
};
}}
#endif // UNIQUE_SQL_BASE_H
