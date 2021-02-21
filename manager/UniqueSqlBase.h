/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef UNIQUESQLBASE_H
#define UNIQUESQLBASE_H

#include "AbstractUniqueSql.h"

namespace managerMPS {
// Unique arbre
/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (num,parent).
 */
template<class Ent> class arbreUniqueSql : public SimpleUniqueSql<Ent> {
protected:
    using SimpleUniqueSql<Ent>::bindValue;

public:
    enum {NumUnique,ParentUnique,NbrUnique};
    CONSTR_DEFAUT(arbreUniqueSql)

    //!Destructeur.
    ~arbreUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &ent) override {
        bindValue(NumUnique,entity.num());
        bindValue(ParentUnique,entity.parent());
    }
};

// Unique CibleSimple
/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (Id_Cible,Cible).
 */
template<class Ent> class CibleSimpleUniqueSql : public SimpleUniqueSql<Ent> {
protected:
    using SimpleUniqueSql<Ent>::bindValue;

public:
    enum {Id_CibleUnique,CibleUnique,NbrUnique};
    CONSTR_DEFAUT(CibleSimpleUniqueSql)

    //!Destructeur.
    ~CibleSimpleUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &ent) override {
        bindValue(Id_CibleUnique,entity.id_cible());
        bindValue(CibleUnique,entity.cible());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triple (Id_Cible,Cible,Num).
 */
template<class Ent> class cible_numUniqueSql : public CibleSimpleUniqueSql<Ent> {
protected:
    using CibleSimpleUniqueSql<Ent>::bindValue;

public:
    enum {NumUnique = CibleSimpleUniqueSql<Ent>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(cible_numUniqueSql)

    //!Destructeur.
    ~cible_numUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &ent) override {
        CibleSimpleUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(NumUnique,entity.num());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triple (Id_Cible,Cible,Num,Type).
 */
template<class Ent> class cible_num_typeUniqueSql : public cible_numUniqueSql<Ent> {
protected:
    using cible_numUniqueSql<Ent>::bindValue;

public:
    enum {TypeUnique = cible_numUniqueSql<Ent>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(cible_num_typeUniqueSql)

    //!Destructeur.
    ~cible_num_typeUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &ent) override {
        cible_numUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(TypeUnique,entity.type());
    }
};

// Unique Historique
/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le quadruplet (Id_Cible,Cible,Id_Etat,Etat).
 */
template<class Ent> class HistoriqueUniqueSql : public CibleSimpleUniqueSql<Ent> {
protected:
    using CibleSimpleUniqueSql<Ent>::bindValue;

public:
    enum {Id_EtatUnique = CibleSimpleUniqueSql<Ent>::NbrUnique, EtatUnique, NumUnique, NbrUnique};
    CONSTR_DEFAUT(HistoriqueUniqueSql)

    //!Destructeur.
    ~HistoriqueUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &ent) override {
        CibleSimpleUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(Id_EtatUnique,entity.Id_Etat());
        bindValue(EtatUnique,entity.etat());
        bindValue(NumUnique,entity.num());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triple (Id_Cible,Cible,ref).
 */
template<class Ent> class CibleSimpleRefUniqueSql : public CibleSimpleUniqueSql<Ent> {
protected:
    using CibleSimpleUniqueSql<Ent>::bindValue;

public:
    enum {RefUnique = CibleSimpleUniqueSql<Ent>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(CibleSimpleRefUniqueSql)

    //!Destructeur.
    ~CibleSimpleRefUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &ent) override {
        CibleSimpleUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(RefUnique,entity.ref());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur id_1.
 */
template<class Ent> class IdUniqueSql : public SimpleUniqueSql<Ent> {
protected:
    using SimpleUniqueSql<Ent>::bindValue;

public:
    enum {Id_1Unique,NbrUnique};
    CONSTR_DEFAUT(IdUniqueSql)

    //! Destructeur.
    ~IdUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override
        {bindValue(Id_1Unique,entity.id_1());}
};

// Unique Cible
/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (Id_1,Cible).
 */
template<class Ent> class Id_CibleUniqueSql : public IdUniqueSql<Ent> {
protected:
    using IdUniqueSql<Ent>::bindValue;

public:
    enum {CibleUnique = IdUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(Id_CibleUniqueSql)

    //!Destructeur.
    ~Id_CibleUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &ent) override {
        IdUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(CibleUnique,entity.cible());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triple (Id_1,Cible,Num).
 */
template<class Ent> class id_cible_numUniqueSql : public Id_CibleUniqueSql<Ent> {
protected:
    using Id_CibleUniqueSql<Ent>::bindValue;

public:
    enum {NumUnique = Id_CibleUniqueSql<Ent>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(id_cible_numUniqueSql)

    //!Destructeur.
    ~id_cible_numUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &ent) override {
        Id_CibleUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(NumUnique,entity.num());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triple (Id_1,Cible,ref).
 */
template<class Ent> class Id_cible_refUniqueSql : public Id_CibleUniqueSql<Ent> {
protected:
    using Id_CibleUniqueSql<Ent>::bindValue;

public:
    enum {RefUnique = Id_CibleUniqueSql<Ent>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(Id_cible_refUniqueSql)

    //!Destructeur.
    ~Id_cible_refUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &ent) override {
        Id_CibleUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(RefUnique,entity.ref());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triple (Id_1,Id_Cible,Cible).
 */
template<class Ent> class CibleUniqueSql : public Id_CibleUniqueSql<Ent> {
protected:
    using Id_CibleUniqueSql<Ent>::bindValue;

public:
    enum {Id_CibleUnique = Id_CibleUniqueSql<Ent>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(CibleUniqueSql)

    //!Destructeur.
    ~CibleUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &ent) override {
        Id_CibleUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(Id_CibleUnique,entity.id_cible());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le 4-upplet (Id_1,Id_Cible,Cible,Date_Time).
 */
template<class Ent> class cible_date_timeUniqueSql : public CibleUniqueSql<Ent> {
protected:
    using CibleUniqueSql<Ent>::bindValue;

public:
    enum {Date_TimeUnique = CibleUniqueSql<Ent>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(cible_date_timeUniqueSql)

    //!Destructeur.
    ~cible_date_timeUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &ent) override {
        CibleUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(Date_TimeUnique,entity.date_time());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le 4-upplet (Id_1,Id_Cible,Cible,Num).
 */
template<class Ent> class cible_numUniqueSql : public CibleUniqueSql<Ent> {
protected:
    using CibleUniqueSql<Ent>::bindValue;

public:
    enum {NumUnique = CibleUniqueSql<Ent>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(cible_numUniqueSql)

    //!Destructeur.
    ~cible_numUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &ent) override {
        CibleUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(NumUnique,entity.num());
    }
};

// Cible _null

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (Id_Cible,Cible).
 */
template<class Ent> class cible_nullUniqueSql : public SimpleUniqueSql<Ent> {
protected:
    using SimpleUniqueSql<Ent>::bindValue;

public:
    enum {Id_CibleUnique, CibleUnique, NbrUnique};
    CONSTR_DEFAUT(cible_nullUniqueSql)

    //!Destructeur.
    ~cible_nullUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &ent) override {
        bindValue(Id_CibleUnique,entity.id_cible());
        bindValue(CibleUnique,entity.cible());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le tripplet (Id_Cible,Cible,Num).
 */
template<class Ent> class cible_null_numUniqueSql : public cible_nullUniqueSql<Ent> {
protected:
    using cible_nullUniqueSql<Ent>::bindValue;

public:
    enum {NumUnique = cible_nullUniqueSql<Ent>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(cible_null_numUniqueSql)

    //!Destructeur.
    ~cible_null_numUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &ent) override {
        cible_nullUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(NumUnique,entity.num());
    }
};

// Unique Nom

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le nom.
 */
template<class Ent> class NomUniqueSql : public SimpleUniqueSql<Ent> {
protected:
    using SimpleUniqueSql<Ent>::bindValue;

public:
    enum {NomUnique,NbrUnique};
    CONSTR_DEFAUT(NomUniqueSql)

    //! Destructeur.
    ~NomUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override
        {bindValue(NomUnique,entity.nom());}
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (nc, nom).
 */
template<class Ent> class NomNcUniqueSql : public NomUniqueSql<Ent> {
protected:
    using NomUniqueSql<Ent>::bindValue;

public:
    enum {NcUnique = NomUniqueSql<Ent>::NbrUnique};
    CONSTR_DEFAUT(NomNcUniqueSql)

    //! Destructeur.
    ~NomNcUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override {
        NomUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(NcUnique,entity.nc());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (nom, type).
 */
template<class Ent> class nom_typeUniqueSql : public NomUniqueSql<Ent> {
protected:
    using NomUniqueSql<Ent>::bindValue;

public:
    enum {TypeUnique = NomUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(nom_typeUniqueSql)

    //! Destructeur.
    ~nom_typeUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override {
        NomUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(TypeUnique,entity.type());
    }
};

// Unique Relation

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple de clé (id_1,id_2).
 */
template<class Ent> class RelationUniqueSql : public IdUniqueSql<Ent> {
protected:
    using IdUniqueSql<Ent>::bindValue;

public:
    enum {Id_2Unique = IdUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(RelationUniqueSql)

    //! Destructeur.
    ~RelationUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override {
        IdUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(Id_2Unique,entity.id_2());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triplet (id_1,id_2,cible).
 */
template<class Ent> class relation_cibleUniqueSql : public RelationUniqueSql<Ent> {
protected:
    using RelationUniqueSql<Ent>::bindValue;

public:
    enum {CibleUnique = RelationUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(relation_cibleUniqueSql)

    //! Destructeur.
    ~relation_cibleUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override {
        RelationUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(CibleUnique,entity.cible());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triplet (id_1,id_2,date).
 */
template<class Ent> class relation_date_timeUniqueSql : public RelationUniqueSql<Ent> {
protected:
    using RelationUniqueSql<Ent>::bindValue;

public:
    enum {Date_TimeUnique = RelationUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(relation_date_timeUniqueSql)

    //! Destructeur.
    ~relation_date_timeUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override {
        RelationUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(Date_TimeUnique,entity.date_time());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triplet (id_1,id_2,num).
 */
template<class Ent> class relation_numUniqueSql : public RelationUniqueSql<Ent> {
protected:
    using RelationUniqueSql<Ent>::bindValue;

public:
    enum {NumUnique = RelationUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(relation_numUniqueSql)

    //! Destructeur.
    ~relation_numUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override {
        RelationUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(NumUnique,entity.num());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le quadruplet (id_1,id_2,id_3,num).
 */
template<class Ent> class RelationTroisNumUniqueSql : public relation_numUniqueSql<Ent> {
protected:
    using relation_numUniqueSql<Ent>::bindValue;

public:
    enum {id_3Unique = relation_numUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(RelationTroisNumUniqueSql)

    //! Destructeur.
    ~RelationTroisNumUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override {
        relation_numUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(id_3Unique,entity.id_3());
    }
};

// Unique Divers

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le num.
 */
template<class Ent> class NumUniqueSql : public SimpleUniqueSql<Ent> {
protected:
    using SimpleUniqueSql<Ent>::bindValue;

public:
    enum {NumUnique,NbrUnique};
    CONSTR_DEFAUT(NumUniqueSql)

    //! Destructeur.
    ~NumUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override
        {bindValue(NumUnique,entity.num());}
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (num, type).
 */
template<class Ent> class num_typeUniqueSql : public NumUniqueSql<Ent> {
protected:
    using NumUniqueSql<Ent>::bindValue;

public:
    enum {TypeUnique = NumUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(num_typeUniqueSql)

    //! Destructeur.
    ~num_typeUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override {
        NumUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(TypeUnique,entity.type());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (id_1,num).
 */
template<class Ent> class IdNumUniqueSql : public IdUniqueSql<Ent> {
protected:
    using IdUniqueSql<Ent>::bindValue;

public:
    enum {NumUnique = IdUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(IdNumUniqueSql)

    //! Destructeur.
    ~IdNumUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override {
        IdUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(NumUnique,entity.num());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (id_1,type).
 */
template<class Ent> class id_typeUniqueSql : public IdUniqueSql<Ent> {
protected:
    using IdUniqueSql<Ent>::bindValue;

public:
    enum {TypeUnique = IdUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(id_typeUniqueSql)

    //! Destructeur.
    ~id_typeUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override {
        IdUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(TypeUnique,entity.type());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur l'attribut Id_Prog.
 */
template<class Ent> class Id_ProgUniqueSql : public SimpleUniqueSql<Ent> {
protected:
    using SimpleUniqueSql<Ent>::bindValue;

public:
    enum {Id_ProgUnique ,NbrUnique};
    CONSTR_DEFAUT(Id_ProgUniqueSql)

    //! Destructeur.
    ~Id_ProgUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override
        {bindValue(Id_ProgUnique,entity.Id_Prog());}
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant deux conditions d'unicité sur l'attribut Id_Prog et le nom.
 */
template<class Ent> class Id_ProgNomUniqueSql : public DoubleUniqueSql<Ent> {
protected:
    using DoubleUniqueSql<Ent>::bindValue;
    using DoubleUniqueSql<Ent>::bindValuesUnique;
    using DoubleUniqueSql<Ent>::bindValuesUnique_2;

public:
    enum {Id_ProgUnique, NbrUnique_1, NomUnique = Id_ProgUnique, NbrUnique_2,
          Id_ProgUniqueSet = DoubleUniqueSql<Ent>::UniqueSet1, NomUniqueSet = DoubleUniqueSql<Ent>::UniqueSet2};
    CONSTR_DEFAUT(Id_ProgNomUniqueSql)

    //! Destructeur.
    ~Id_ProgNomUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override
        {bindValue(Id_ProgUnique,entity.Id_Prog());}

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique_2(const Ent &entity) override
        {bindValue(NomUnique,entity.nom());}
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant deux conditions d'unicité sur l'attribut nom
 *et le 4-upplet (id_1,id_cible,cible,num).
 */
template<class Ent> class NomCibleUniqueSql : public DoubleUniqueSql<Ent> {
protected:
    using DoubleUniqueSql<Ent>::bindValue;
    using DoubleUniqueSql<Ent>::bindValuesUnique;
    using DoubleUniqueSql<Ent>::bindValuesUnique_2;

public:
    enum {NomUnique, NbrUnique_1,
          Id_1Unique = NomUnique, Id_CibleUnique, CibleUnique, NumUnique, NbrUnique_2,
          NomUniqueSet = DoubleUniqueSql<Ent>::UniqueSet1,
          Id_1UniqueSet = DoubleUniqueSql<Ent>::UniqueSet2,
          Id_CibleUniqueSet = Id_1UniqueSet,
          CibleUniqueSet = Id_1UniqueSet,
          NumUniqueSet = Id_1UniqueSet};
    CONSTR_DEFAUT(NomCibleUniqueSql)

    //! Destructeur.
    ~NomCibleUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override
        {bindValue(NomUnique, entity.nom());}

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique_2(const Ent &entity) override {
        bindValue(Id_1Unique, entity.id_1());
        bindValue(Id_CibleUnique, entity.id_cible());
        bindValue(CibleUnique, entity.cible());
        bindValue(NumUnique, entity.num());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant deux conditions d'unicité sur l'attribut Id_Prog et le couple (Parent, nom).
 */
template<class Ent> class Id_ProgNomParentUniqueSql : public Id_ProgNomUniqueSql<Ent> {
protected:
    using Id_ProgNomUniqueSql<Ent>::bindValue;
    using Id_ProgNomUniqueSql<Ent>::bindValuesUnique_2;

public:
    enum {ParentUnique = Id_ProgNomUniqueSql<Ent>::NbrUnique_2, NbrUnique_2, ParentUniqueSet = Id_ProgNomUniqueSql<Ent>::NomUniqueSet};
    CONSTR_DEFAUT(Id_ProgNomParentUniqueSql)

    //! Destructeur.
    ~Id_ProgNomParentUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique_2(const Ent &entity) override {
        Id_ProgNomUniqueSql<Ent>::bindValuesUnique_2(entity);
        bindValue(ParentUnique,entity.parent());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le texte.
 */
template<class Ent> class TexteUniqueSql : public SimpleUniqueSql<Ent> {
protected:
    using SimpleUniqueSql<Ent>::bindValue;

public:
    enum {TexteUnique,NbrUnique};
    CONSTR_DEFAUT(TexteUniqueSql)

    //! Destructeur.
    ~TexteUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override
        {bindValue(TexteUnique,entity.texte());}
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant deux conditions d'unicité (id_1,id_3) et (id_2,id_3) avec id_1 et id_2
 *n'étant pas simultanément nuls ou simultanément non nuls.
 */
template<class Ent> class Idrelation_exact_one_not_nullUniqueSql : public Attsrelation_exact_one_not_nullUniqueSql<Ent> {
protected:
    using AREONNU = Attsrelation_exact_one_not_nullUniqueSql<Ent>;
    using AREONNU::bindValue;

public:
    enum {id_3Unique = AREONNU::NbrUnique_1 ,NbrUnique_1, NbrUnique_2 = NbrUnique_1};
    CONSTR_DEFAUT(Idrelation_exact_one_not_nullUniqueSql)

    //! Destructeur.
    ~Idrelation_exact_one_not_nullUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUniqueAtts(const Ent &entity) override
        {bindValue(id_3Unique,entity.id_3());}
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur l'attribut ref.
 */
template<class Ent> class RefUniqueSql : public SimpleUniqueSql<Ent> {
protected:
    using SimpleUniqueSql<Ent>::bindValue;

public:
    enum {RefUnique ,NbrUnique};
    CONSTR_DEFAUT(RefUniqueSql)

    //! Destructeur.
    ~RefUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override
        {bindValue(RefUnique,entity.ref());}
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant deux conditions d'unicité sur l'attribut ref et le couple (nom, type).
 */
template<class Ent> class Refnom_typeUniqueSql : public DoubleUniqueSql<Ent> {
protected:
    using DoubleUniqueSql<Ent>::bindValue;
    using DoubleUniqueSql<Ent>::bindValuesUnique;
    using DoubleUniqueSql<Ent>::bindValuesUnique_2;

public:
    enum {RefUnique, NbrUnique_1, NomUnique = RefUnique, TypeUnique, NbrUnique_2,
          RefUniqueSet = DoubleUniqueSql<Ent>::UniqueSet1,
          NomUniqueSet = DoubleUniqueSql<Ent>::UniqueSet2,
          TypeUniqueSet = DoubleUniqueSql<Ent>::UniqueSet2};
    CONSTR_DEFAUT(Refnom_typeUniqueSql)

    //! Destructeur.
    ~Refnom_typeUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override
        {bindValue(RefUnique,entity.ref());}

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique_2(const Ent &entity) override {
        bindValue(NomUnique,entity.nom());
        bindValue(TypeUnique,entity.type());
    }
};

/*! \ingroup groupeUniqueSql
 *\brief Classe condition d'unicité pour les entités possédant deux conditions d'unicité sur l'attribut ref et le couple (nom, parent).
 */
template<class Ent> class RefNomParentUniqueSql : public DoubleUniqueSql<Ent> {
protected:
    using DoubleUniqueSql<Ent>::bindValue;
    using DoubleUniqueSql<Ent>::bindValuesUnique;
    using DoubleUniqueSql<Ent>::bindValuesUnique_2;

public:
    enum {RefUnique, NbrUnique_1, NomUnique = RefUnique, ParentUnique, NbrUnique_2,
          RefUniqueSet = DoubleUniqueSql<Ent>::UniqueSet1,
          NomUniqueSet = DoubleUniqueSql<Ent>::UniqueSet2,
          ParentUniqueSet = DoubleUniqueSql<Ent>::UniqueSet2};
    CONSTR_DEFAUT(RefNomParentUniqueSql)

    //! Destructeur.
    ~RefNomParentUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override
        {bindValue(RefUnique,entity.ref());}

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique_2(const Ent &entity) override {
        bindValue(NomUnique,entity.nom());
        bindValue(ParentUnique,entity.parent());
    }
};
}
#endif // UNIQUESQLBASE_H
