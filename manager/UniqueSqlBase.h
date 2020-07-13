/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef UNIQUESQLBASE_H
#define UNIQUESQLBASE_H

#include "AbstractUniqueSql.h"

namespace managerMPS {
// Unique Arbre
/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (num,parent).
 */
template<class Ent> class ArbreUniqueSql : public SimpleUniqueSql<Ent> {
protected:
    using SimpleUniqueSql<Ent>::bindValue;

public:
    enum {NumUnique,ParentUnique,NbrUnique};
    CONSTR_DEFAUT(ArbreUniqueSql)

    //!Destructeur.
    ~ArbreUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent & entity) override {
        bindValue(NumUnique,entity.num());
        bindValue(ParentUnique,entity.parent());
    }
};

// Unique CibleSimple
/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (IdCible,Cible).
 */
template<class Ent> class CibleSimpleUniqueSql : public SimpleUniqueSql<Ent> {
protected:
    using SimpleUniqueSql<Ent>::bindValue;

public:
    enum {IdCibleUnique,CibleUnique,NbrUnique};
    CONSTR_DEFAUT(CibleSimpleUniqueSql)

    //!Destructeur.
    ~CibleSimpleUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent & entity) override {
        bindValue(IdCibleUnique,entity.idCible());
        bindValue(CibleUnique,entity.cible());
    }
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triple (IdCible,Cible,Num).
 */
template<class Ent> class CibleSimpleNumUniqueSql : public CibleSimpleUniqueSql<Ent> {
protected:
    using CibleSimpleUniqueSql<Ent>::bindValue;

public:
    enum {NumUnique = CibleSimpleUniqueSql<Ent>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(CibleSimpleNumUniqueSql)

    //!Destructeur.
    ~CibleSimpleNumUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent & entity) override {
        CibleSimpleUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(NumUnique,entity.num());
    }
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triple (IdCible,Cible,Num,Type).
 */
template<class Ent> class CibleSimpleNumTypeUniqueSql : public CibleSimpleNumUniqueSql<Ent> {
protected:
    using CibleSimpleNumUniqueSql<Ent>::bindValue;

public:
    enum {TypeUnique = CibleSimpleNumUniqueSql<Ent>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(CibleSimpleNumTypeUniqueSql)

    //!Destructeur.
    ~CibleSimpleNumTypeUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent & entity) override {
        CibleSimpleNumUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(TypeUnique,entity.type());
    }
};

// Unique Historique
/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le quadruplet (IdCible,Cible,IdEtat,Etat).
 */
template<class Ent> class HistoriqueUniqueSql : public CibleSimpleUniqueSql<Ent> {
protected:
    using CibleSimpleUniqueSql<Ent>::bindValue;

public:
    enum {IdEtatUnique = CibleSimpleUniqueSql<Ent>::NbrUnique, EtatUnique, NumUnique, NbrUnique};
    CONSTR_DEFAUT(HistoriqueUniqueSql)

    //!Destructeur.
    ~HistoriqueUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent & entity) override {
        CibleSimpleUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(IdEtatUnique,entity.idEtat());
        bindValue(EtatUnique,entity.etat());
        bindValue(NumUnique,entity.num());
    }
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triple (IdCible,Cible,ref).
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
    void bindValuesUnique(const Ent & entity) override {
        CibleSimpleUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(RefUnique,entity.ref());
    }
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur id1.
 */
template<class Ent> class IdUniqueSql : public SimpleUniqueSql<Ent> {
protected:
    using SimpleUniqueSql<Ent>::bindValue;

public:
    enum {Id1Unique,NbrUnique};
    CONSTR_DEFAUT(IdUniqueSql)

    //! Destructeur.
    ~IdUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override
        {bindValue(Id1Unique,entity.id1());}
};

// Unique Cible
/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (Id1,Cible).
 */
template<class Ent> class IdCibleUniqueSql : public IdUniqueSql<Ent> {
protected:
    using IdUniqueSql<Ent>::bindValue;

public:
    enum {CibleUnique = IdUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(IdCibleUniqueSql)

    //!Destructeur.
    ~IdCibleUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent & entity) override {
        IdUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(CibleUnique,entity.cible());
    }
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triple (Id1,Cible,Num).
 */
template<class Ent> class IdCibleNumUniqueSql : public IdCibleUniqueSql<Ent> {
protected:
    using IdCibleUniqueSql<Ent>::bindValue;

public:
    enum {NumUnique = IdCibleUniqueSql<Ent>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(IdCibleNumUniqueSql)

    //!Destructeur.
    ~IdCibleNumUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent & entity) override {
        IdCibleUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(NumUnique,entity.num());
    }
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triple (Id1,Cible,ref).
 */
template<class Ent> class IdCibleRefUniqueSql : public IdCibleUniqueSql<Ent> {
protected:
    using IdCibleUniqueSql<Ent>::bindValue;

public:
    enum {RefUnique = IdCibleUniqueSql<Ent>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(IdCibleRefUniqueSql)

    //!Destructeur.
    ~IdCibleRefUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent & entity) override {
        IdCibleUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(RefUnique,entity.ref());
    }
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triple (Id1,IdCible,Cible).
 */
template<class Ent> class CibleUniqueSql : public IdCibleUniqueSql<Ent> {
protected:
    using IdCibleUniqueSql<Ent>::bindValue;

public:
    enum {IdCibleUnique = IdCibleUniqueSql<Ent>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(CibleUniqueSql)

    //!Destructeur.
    ~CibleUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent & entity) override {
        IdCibleUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(IdCibleUnique,entity.idCible());
    }
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le 4-upplet (Id1,IdCible,Cible,DateTime).
 */
template<class Ent> class CibleDateTimeUniqueSql : public CibleUniqueSql<Ent> {
protected:
    using CibleUniqueSql<Ent>::bindValue;

public:
    enum {DateTimeUnique = CibleUniqueSql<Ent>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(CibleDateTimeUniqueSql)

    //!Destructeur.
    ~CibleDateTimeUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent & entity) override {
        CibleUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(DateTimeUnique,entity.dateTime());
    }
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le 4-upplet (Id1,IdCible,Cible,Num).
 */
template<class Ent> class CibleNumUniqueSql : public CibleUniqueSql<Ent> {
protected:
    using CibleUniqueSql<Ent>::bindValue;

public:
    enum {NumUnique = CibleUniqueSql<Ent>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(CibleNumUniqueSql)

    //!Destructeur.
    ~CibleNumUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent & entity) override {
        CibleUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(NumUnique,entity.num());
    }
};

// Cible Null

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (IdCible,Cible).
 */
template<class Ent> class CibleNullUniqueSql : public SimpleUniqueSql<Ent> {
protected:
    using SimpleUniqueSql<Ent>::bindValue;

public:
    enum {IdCibleUnique, CibleUnique, NbrUnique};
    CONSTR_DEFAUT(CibleNullUniqueSql)

    //!Destructeur.
    ~CibleNullUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent & entity) override {
        bindValue(IdCibleUnique,entity.idCible());
        bindValue(CibleUnique,entity.cible());
    }
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le tripplet (IdCible,Cible,Num).
 */
template<class Ent> class CibleNullNumUniqueSql : public CibleNullUniqueSql<Ent> {
protected:
    using CibleNullUniqueSql<Ent>::bindValue;

public:
    enum {NumUnique = CibleNullUniqueSql<Ent>::NbrUnique, NbrUnique};
    CONSTR_DEFAUT(CibleNullNumUniqueSql)

    //!Destructeur.
    ~CibleNullNumUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent & entity) override {
        CibleNullUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(NumUnique,entity.num());
    }
};

// Unique Nom

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le nom.
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
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (nc, nom).
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
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (nom, type).
 */
template<class Ent> class NomTypeUniqueSql : public NomUniqueSql<Ent> {
protected:
    using NomUniqueSql<Ent>::bindValue;

public:
    enum {TypeUnique = NomUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(NomTypeUniqueSql)

    //! Destructeur.
    ~NomTypeUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override {
        NomUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(TypeUnique,entity.type());
    }
};

// Unique Relation

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple de clé (id1,id2).
 */
template<class Ent> class RelationUniqueSql : public IdUniqueSql<Ent> {
protected:
    using IdUniqueSql<Ent>::bindValue;

public:
    enum {Id2Unique = IdUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(RelationUniqueSql)

    //! Destructeur.
    ~RelationUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override {
        IdUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(Id2Unique,entity.id2());
    }
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triplet (id1,id2,cible).
 */
template<class Ent> class RelationCibleUniqueSql : public RelationUniqueSql<Ent> {
protected:
    using RelationUniqueSql<Ent>::bindValue;

public:
    enum {CibleUnique = RelationUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(RelationCibleUniqueSql)

    //! Destructeur.
    ~RelationCibleUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override {
        RelationUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(CibleUnique,entity.cible());
    }
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triplet (id1,id2,date).
 */
template<class Ent> class RelationDateTimeUniqueSql : public RelationUniqueSql<Ent> {
protected:
    using RelationUniqueSql<Ent>::bindValue;

public:
    enum {DateTimeUnique = RelationUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(RelationDateTimeUniqueSql)

    //! Destructeur.
    ~RelationDateTimeUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override {
        RelationUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(DateTimeUnique,entity.dateTime());
    }
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le triplet (id1,id2,num).
 */
template<class Ent> class RelationNumUniqueSql : public RelationUniqueSql<Ent> {
protected:
    using RelationUniqueSql<Ent>::bindValue;

public:
    enum {NumUnique = RelationUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(RelationNumUniqueSql)

    //! Destructeur.
    ~RelationNumUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override {
        RelationUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(NumUnique,entity.num());
    }
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le quadruplet (id1,id2,id3,num).
 */
template<class Ent> class RelationTroisNumUniqueSql : public RelationNumUniqueSql<Ent> {
protected:
    using RelationNumUniqueSql<Ent>::bindValue;

public:
    enum {Id3Unique = RelationNumUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(RelationTroisNumUniqueSql)

    //! Destructeur.
    ~RelationTroisNumUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override {
        RelationNumUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(Id3Unique,entity.id3());
    }
};

// Unique Divers

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le num.
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
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (num, type).
 */
template<class Ent> class NumTypeUniqueSql : public NumUniqueSql<Ent> {
protected:
    using NumUniqueSql<Ent>::bindValue;

public:
    enum {TypeUnique = NumUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(NumTypeUniqueSql)

    //! Destructeur.
    ~NumTypeUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override {
        NumUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(TypeUnique,entity.type());
    }
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (id1,num).
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
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le couple (id1,type).
 */
template<class Ent> class IdTypeUniqueSql : public IdUniqueSql<Ent> {
protected:
    using IdUniqueSql<Ent>::bindValue;

public:
    enum {TypeUnique = IdUniqueSql<Ent>::NbrUnique,NbrUnique};
    CONSTR_DEFAUT(IdTypeUniqueSql)

    //! Destructeur.
    ~IdTypeUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override {
        IdUniqueSql<Ent>::bindValuesUnique(entity);
        bindValue(TypeUnique,entity.type());
    }
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur l'attribut idProg.
 */
template<class Ent> class IdProgUniqueSql : public SimpleUniqueSql<Ent> {
protected:
    using SimpleUniqueSql<Ent>::bindValue;

public:
    enum {IdProgUnique ,NbrUnique};
    CONSTR_DEFAUT(IdProgUniqueSql)

    //! Destructeur.
    ~IdProgUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override
        {bindValue(IdProgUnique,entity.idProg());}
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant deux conditions d'unicité sur l'attribut idProg et le couple (Parent, nom).
 */
template<class Ent> class IdProgNomUniqueSql : public DoubleUniqueSql<Ent> {
protected:
    using DoubleUniqueSql<Ent>::bindValue;
    using DoubleUniqueSql<Ent>::bindValuesUnique;
    using DoubleUniqueSql<Ent>::bindValuesUnique_2;

public:
    enum {IdProgUnique, NbrUnique_1, NomUnique = IdProgUnique, NbrUnique_2,
          IdProgUniqueSet = DoubleUniqueSql<Ent>::UniqueSet1, NomUniqueSet = DoubleUniqueSql<Ent>::UniqueSet2};
    CONSTR_DEFAUT(IdProgNomUniqueSql)

    //! Destructeur.
    ~IdProgNomUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique(const Ent &entity) override
        {bindValue(IdProgUnique,entity.idProg());}

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique_2(const Ent &entity) override
        {bindValue(NomUnique,entity.nom());}
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant deux conditions d'unicité sur l'attribut idProg et le nom.
 */
template<class Ent> class IdProgNomParentUniqueSql : public IdProgNomUniqueSql<Ent> {
protected:
    using IdProgNomUniqueSql<Ent>::bindValue;
    using IdProgNomUniqueSql<Ent>::bindValuesUnique_2;

public:
    enum {ParentUnique = IdProgNomUniqueSql<Ent>::NbrUnique_2, NbrUnique_2, ParentUniqueSet = IdProgNomUniqueSql<Ent>::NomUniqueSet};
    CONSTR_DEFAUT(IdProgNomParentUniqueSql)

    //! Destructeur.
    ~IdProgNomParentUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUnique_2(const Ent &entity) override {
        IdProgNomUniqueSql<Ent>::bindValuesUnique_2(entity);
        bindValue(ParentUnique,entity.parent());
    }
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur le texte.
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
 * \brief Classe condition d'unicité pour les entités possédant deux conditions d'unicité (id1,id3) et (id2,id3) avec id1 et id2
 * n'étant pas simultanément nuls ou simultanément non nuls.
 */
template<class Ent> class IdRelationExactOneNotNullUniqueSql : public AttsRelationExactOneNotNullUniqueSql<Ent> {
protected:
    using AREONNU = AttsRelationExactOneNotNullUniqueSql<Ent>;
    using AREONNU::bindValue;

public:
    enum {Id3Unique = AREONNU::NbrUnique_1 ,NbrUnique_1, NbrUnique_2 = NbrUnique_1};
    CONSTR_DEFAUT(IdRelationExactOneNotNullUniqueSql)

    //! Destructeur.
    ~IdRelationExactOneNotNullUniqueSql() override = default;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    void bindValuesUniqueAtts(const Ent &entity) override
        {bindValue(Id3Unique,entity.id3());}
};

/*! \ingroup groupeUniqueSql
 * \brief Classe condition d'unicité pour les entités possédant une seule condition d'unicité sur l'attribut ref.
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
 * \brief Classe condition d'unicité pour les entités possédant deux conditions d'unicité sur l'attribut ref et le couple (nom, type).
 */
template<class Ent> class RefNomTypeUniqueSql : public DoubleUniqueSql<Ent> {
protected:
    using DoubleUniqueSql<Ent>::bindValue;
    using DoubleUniqueSql<Ent>::bindValuesUnique;
    using DoubleUniqueSql<Ent>::bindValuesUnique_2;

public:
    enum {RefUnique, NbrUnique_1, NomUnique = RefUnique, TypeUnique, NbrUnique_2,
          RefUniqueSet = DoubleUniqueSql<Ent>::UniqueSet1,
          NomUniqueSet = DoubleUniqueSql<Ent>::UniqueSet2,
          TypeUniqueSet = DoubleUniqueSql<Ent>::UniqueSet2};
    CONSTR_DEFAUT(RefNomTypeUniqueSql)

    //! Destructeur.
    ~RefNomTypeUniqueSql() override = default;

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
 * \brief Classe condition d'unicité pour les entités possédant deux conditions d'unicité sur l'attribut ref et le couple (nom, parent).
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
