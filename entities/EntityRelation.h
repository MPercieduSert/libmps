/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/08/2016
 */
#ifndef ENTITYRELATION
#define ENTITYRELATION

#include "EntityDivers.h"


//! \ingroup groupeMacroEntity
//! Macro implémentant le début d'une classe ayant 2 clés.
#define ENTITY_ALIAS_DEBUT(ENTITY,TYPE,IDT,ID1,ID2) \
    /*! \ingroup groupeEntity \brief Représentation de l'entité ENTITY.*/ \
    class ENTITY : public entityBaseMPS::TYPE##Entity<IDT> \
    {public: \
    using mere = entityBaseMPS::TYPE##Entity<IDT>; \
    using entityBaseMPS::TYPE##Entity<IDT>::TYPE##Entity; \
    using mere::operator ==; \
    BASE_ENTITY(ENTITY)

//! \ingroup groupeMacroEntity
//! Macro implémentant une classe ayant 2 clés.
#define RELATION_ENTITY(ENTITY,TYPE,IDT,ID1,ID2) \
    ENTITY_ALIAS_DEBUT(ENTITY,TYPE,IDT,ID1,ID2)\
    ENUM_##TYPE(ID1,ID2) \
    ALIAS_CLE(ID1,1)\
    ALIAS_CLE(ID2,2)};

//! \ingroup groupeMacroEntity
//! Macro implémentant une classe ayant 2 clés dont la seconde négative.
#define RELATION_ENTITY_NEG(ENTITY,TYPE,IDT,ID1,ID2) \
    ENTITY_ALIAS_DEBUT(ENTITY,TYPE,IDT,ID1,ID2)\
    ENUM_##TYPE(ID1,ID2) \
    ALIAS_CLE_NEG(ID1,1)\
    ALIAS_CLE(ID2,2)};

//! \ingroup groupeMacroEntity
//! Macro implémentant une classe ayant 2 clés négatives.
#define RELATION_ENTITY_NEGNEG(ENTITY,TYPE,IDT,ID1,ID2) \
    ENTITY_ALIAS_DEBUT(ENTITY,TYPE,IDT,ID1,ID2)\
    ENUM_##TYPE(ID1,ID2) \
    ALIAS_CLE_NEG(ID1,1)\
    ALIAS_CLE_NEG(ID2,2)};

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation simple.
#define ENUM_Relation(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position:szt {Id, Id1 = mere::Id1, Id2 = mere::Id2, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

namespace entityBaseMPS {

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés.
 */
template<szt IDM > class RelationEntity : public EntityID<IDM,RelationAttribut> {
public:
    using EAID = EntityID<IDM,RelationAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityID<IDM,RelationAttribut>::EntityID;
    using EAID::setId1;
    using EAID::setId2;
    BASE_ENTITY(RelationEntity)

    //! Constructeur à partir des valeurs attributs.
    RelationEntity(idt id1, idt id2, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setId2(id2);
    }
};

template<szt IDM > RelationEntity<IDM>::~RelationEntity() {}

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation simple.
#define ENUM_RelationExactOneNotNull(ID1,ID2) ENUM_(ID1,ID2)

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés dont une seule est non nulle et une troisième.
 */
template<szt IDM > class RelationExactOneNotNullEntity : public EntityID<IDM,RelationExactOneNotNullAttribut> {
public:
    using EAID = EntityID<IDM,RelationExactOneNotNullAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityID<IDM,RelationExactOneNotNullAttribut>::EntityID;
    using EAID::setId1;
    using EAID::setId2;
    BASE_ENTITY(RelationExactOneNotNullEntity)

    //! Constructeur à partir des valeurs attributs.
    RelationExactOneNotNullEntity(idt id1, idt id2, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setId2(id2);
    }
};

template<szt IDM > RelationExactOneNotNullEntity<IDM>::~RelationExactOneNotNullEntity() {}

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation simple.
#define ENUM_RelationCibleTemp(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position:szt {Id, Id1 = mere::Id1, Id2 = mere::Id2, IdCible = mere::IdCible, Cible = mere::Cible,\
    NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés.
 */
template<szt IDM , class Id1Att, class Id2Att> class RelationCibleEntityTemp
        : public EntityIDs<IDM,Id1Att,Id2Att,IdCibleAttribut,CibleAttribut> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using Id2Trans = typename Id2Att::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,Id2Att,IdCibleAttribut,CibleAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   Id2 = PositionEnum<Id2Att,EAID>::Position,
                   IdCible = PositionEnum<IdCibleAttribut,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att,Id2Att,IdCibleAttribut,CibleAttribut>::EntityIDs;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setIdCible;
    using EAID::setCible;
    BASE_ENTITY(RelationCibleEntityTemp)

    //! Constructeur à partir des valeurs attributs.
    RelationCibleEntityTemp(Id1Trans id1, Id2Trans id2, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setId2(id2);
    }

    //! Constructeur à partir des valeurs attributs.
    RelationCibleEntityTemp(Id1Trans id1, Id2Trans id2, idt idCible, int cible, idt id = 0)
        : RelationCibleEntityTemp(id1,id2,id) {
        setIdCible(idCible);
        setCible(cible);
    }
};

template<szt IDM , class Id1Att, class Id2Att> RelationCibleEntityTemp<IDM,Id1Att,Id2Att>::~RelationCibleEntityTemp() {}

template<szt IDM > using RelationCibleEntity = RelationCibleEntityTemp<IDM,Id1Attribut,Id2Attribut>;
#define ENUM_RelationCible(ID1,ID2) ENUM_RelationCibleTemp(ID1,ID2)

template<szt IDM > using RelationCibleNullEntity = RelationCibleEntityTemp<IDM,Id1NullAttribut,Id2Attribut>;
#define ENUM_RelationCibleNull(ID1,ID2) ENUM_RelationCibleTemp(ID1,ID2)

template<szt IDM > using RelationCibleNegEntity = RelationCibleEntityTemp<IDM,Id1NegAttribut,Id2Attribut>;
#define ENUM_RelationCibleNeg(ID1,ID2) ENUM_RelationCibleTemp(ID1,ID2)

template<szt IDM > using RelationCibleNullNullEntity = RelationCibleEntityTemp<IDM,Id1NullAttribut,Id2NullAttribut>;
#define ENUM_RelationCibleNull(ID1,ID2) ENUM_RelationCibleTemp(ID1,ID2)

template<szt IDM > using RelationCibleNegNullEntity = RelationCibleEntityTemp<IDM,Id1NegAttribut,Id2NullAttribut>;
#define ENUM_RelationCibleNegNull(ID1,ID2) ENUM_RelationCibleTemp(ID1,ID2)

template<szt IDM > using RelationCibleNegNegEntity = RelationCibleEntityTemp<IDM,Id1NegAttribut,Id2NegAttribut>;
#define ENUM_RelationCibleNegNeg(ID1,ID2) ENUM_RelationCibleTemp(ID1,ID2)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation simple.
#define ENUM_RelationCibleAttCodeTemp(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position:szt {Id, Id1 = mere::Id1, Id2 = mere::Id2, Cible = mere::Cible, Code = mere::Code,\
    NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés.
 */
template<szt IDM , class Id1Att, class Id2Att> class RelationCibleAttCodeEntityTemp
        : public EntityIDs<IDM,Id1Att,Id2Att,CibleAttribut,CodeAttribut> {
public:
    using Id1Trans = typename Id1Att::AttTrans;
    using Id2Trans = typename Id2Att::AttTrans;
    using EAID = EntityIDs<IDM,Id1Att,Id2Att,CibleAttribut,CodeAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Att,EAID>::Position,
                   Id2 = PositionEnum<Id2Att,EAID>::Position,
                   Cible = PositionEnum<CibleAttribut,EAID>::Position,
                   Code = PositionEnum<CodeAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,Id1Att,Id2Att,CibleAttribut,CodeAttribut>::EntityIDs;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setCible;
    using EAID::setCode;
    BASE_ENTITY(RelationCibleAttCodeEntityTemp)

    //! Constructeur à partir des valeurs attributs.
    RelationCibleAttCodeEntityTemp(Id1Trans id1, Id2Trans id2, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setId2(id2);
    }

    //! Constructeur à partir des valeurs attributs.
    RelationCibleAttCodeEntityTemp(Id1Trans id1, Id2Trans id2, int cible, flag code, idt id = 0)
        : RelationCibleAttCodeEntityTemp(id1,id2,id) {
        setCible(cible);
        setCode(code);
    }
};

template<szt IDM , class Id1Att, class Id2Att> RelationCibleAttCodeEntityTemp<IDM,Id1Att,Id2Att>::~RelationCibleAttCodeEntityTemp() {}

template<szt IDM > using RelationCibleAttCodeEntity = RelationCibleAttCodeEntityTemp<IDM,Id1Attribut,Id2Attribut>;
#define ENUM_RelationCibleAttCode(ID1,ID2) ENUM_RelationCibleAttCodeTemp(ID1,ID2)

template<szt IDM > using RelationCibleAttCodeNullEntity = RelationCibleAttCodeEntityTemp<IDM,Id1NullAttribut,Id2Attribut>;
#define ENUM_RelationCibleAttCodeNull(ID1,ID2) ENUM_RelationCibleAttCodeTemp(ID1,ID2)

template<szt IDM > using RelationCibleAttCodeNegEntity = RelationCibleAttCodeEntityTemp<IDM,Id1NegAttribut,Id2Attribut>;
#define ENUM_RelationCibleAttCodeNeg(ID1,ID2) ENUM_RelationCibleAttCodeTemp(ID1,ID2)

template<szt IDM > using RelationCibleAttCodeNullNullEntity = RelationCibleAttCodeEntityTemp<IDM,Id1NullAttribut,Id2NullAttribut>;
#define ENUM_RelationCibleAttCodeNull(ID1,ID2) ENUM_RelationCibleAttCodeTemp(ID1,ID2)

template<szt IDM > using RelationCibleAttCodeNullNegEntity = RelationCibleAttCodeEntityTemp<IDM,Id1NegAttribut,Id2NullAttribut>;
#define ENUM_RelationCibleAttCodeNullNeg(ID1,ID2) ENUM_RelationCibleAttCodeTemp(ID1,ID2)

template<szt IDM > using RelationCibleAttCodeNegNegEntity = RelationCibleAttCodeEntityTemp<IDM,Id1NegAttribut,Id2NegAttribut>;
#define ENUM_RelationCibleAttCodeNegNeg(ID1,ID2) ENUM_RelationCibleAttCodeTemp(ID1,ID2)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation avec un attribut dateTime.
#define ENUM_RelationDateTimeTemp(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position:szt {Id, Id1 = mere::Id1, Id2 = mere::Id2, DateTime = mere::DateTime, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés et une dateTime.
 */
template<szt IDM , class DateTimeAtt> class RelationDateTimeEntityTemp : public EntityIDs<IDM,RelationAttribut,DateTimeAtt> {
public:
    using EAID = EntityIDs<IDM,RelationAttribut,DateTimeAtt>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,RelationAttribut,DateTimeAtt>::EntityIDs;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setDateTime;
    BASE_ENTITY(RelationDateTimeEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    RelationDateTimeEntityTemp(idt id1, idt id2, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setId2(id2);
    }

    //! Constructeur à partir des valeurs attributs.
    RelationDateTimeEntityTemp(idt id1, idt id2, const QDateTime & dateTime, idt id = 0)
        : RelationDateTimeEntityTemp(id1, id2, id)
        {setDateTime(dateTime);}
};

template<szt IDM , class DateTimeAtt> RelationDateTimeEntityTemp<IDM,DateTimeAtt>::~RelationDateTimeEntityTemp() {}

template<szt IDM > using RelationDateTimeCurrentEntity = RelationDateTimeEntityTemp<IDM,DateTimeCurrentAttribut>;
#define ENUM_RelationDateTimeCurrent(ID1,ID2) ENUM_RelationDateTimeTemp(ID1,ID2)

template<szt IDM > using RelationDateTimeValideEntity = RelationDateTimeEntityTemp<IDM,DateTimeValideAttribut>;
#define ENUM_RelationDateTimeValide(ID1,ID2) ENUM_RelationDateTimeTemp(ID1,ID2)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation avec un attribut num.
#define ENUM_RelationNum(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position:szt {Id, Id1 = mere::Id1, Id2 = mere::Id2, Num = mere::Num, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés et un numero.
 */
template<szt IDM > class RelationNumEntity : public EntityIDs<IDM,RelationAttribut,NumAttribut> {
public:
    using EAID = EntityIDs<IDM,RelationAttribut,NumAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,RelationAttribut,NumAttribut>::EntityIDs;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setNum;
    BASE_ENTITY(RelationNumEntity)

    //! Constructeur à partir des valeurs attributs.
    RelationNumEntity(idt id1, idt id2, int num = 0, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setId2(id2);
        setNum(num);
    }
};

template<szt IDM > RelationNumEntity<IDM>::~RelationNumEntity() {}

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation avec un attribut type.
#define ENUM_RelationType(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position:szt {Id, Id1 = mere::Id1, Id2 = mere::Id2, Type = mere::Type, NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés et un numero.
 */
template<szt IDM > class RelationTypeEntity : public EntityIDs<IDM,RelationAttribut,TypeAttribut> {
public:
    using EAID = EntityIDs<IDM,RelationAttribut,TypeAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,RelationAttribut,TypeAttribut>::EntityIDs;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setType;
    BASE_ENTITY(RelationTypeEntity)

    //! Constructeur à partir des valeurs attributs.
    RelationTypeEntity(idt id1, idt id2, idt type = 0, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setId2(id2);
        setType(type);
    }
};

template<szt IDM > RelationTypeEntity<IDM>::~RelationTypeEntity() {}

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation avec un attribut dateTime et num.
#define ENUM_RelationDateTimeNumTemp(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position:szt {Id, Id1 = mere::Id1, Id2 = mere::Id2, DateTime = mere::DateTime, Num = mere::Num,\
                    NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés, une dateTime et un numero.
 */
template<szt IDM , class DateTimeAtt> class RelationDateTimeNumEntityTemp :
        public EntityIDs<IDM,RelationAttribut,DateTimeAtt,NumAttribut> {
public:
    using EAID = EntityIDs<IDM,RelationAttribut,DateTimeAtt,NumAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,RelationAttribut,DateTimeAtt,NumAttribut>::EntityIDs;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setDateTime;
    using EAID::setNum;
    BASE_ENTITY(RelationDateTimeNumEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    RelationDateTimeNumEntityTemp(idt id1, idt id2, int num = 0, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setId2(id2);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    RelationDateTimeNumEntityTemp(idt id1, idt id2, const QDateTime & dateTime, int num, idt id = 0)
        : RelationDateTimeNumEntityTemp(id1, id2, num, id)
        {setDateTime(dateTime);}
};

template<szt IDM , class DateTimeAtt> RelationDateTimeNumEntityTemp<IDM,DateTimeAtt>::~RelationDateTimeNumEntityTemp() {}

template<szt IDM > using RelationDateTimeCurrentNumEntity = RelationDateTimeNumEntityTemp<IDM,DateTimeCurrentAttribut>;
#define ENUM_RelationDateTimeCurrentNum(ID1,ID2) ENUM_RelationDateTimeNumTemp(ID1,ID2)

template<szt IDM > using RelationDateTimeValideNumEntity = RelationDateTimeNumEntityTemp<IDM,DateTimeValideAttribut>;
#define ENUM_RelationDateTimeValideNum(ID1,ID2) ENUM_RelationDateTimeNumTemp(ID1,ID2)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation avec un attribut dateTime, num et Valeur.
#define ENUM_RelationDateTimeValeurTemp(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position:szt {Id, Id1 = mere::Id1, Id2 = mere::Id2, DateTime = mere::DateTime, Valeur = mere::Valeur,\
                NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés, une dateTime et un numero.
 */
template<szt IDM , class DateTimeAtt,class ValeurAtt> class RelationDateTimeValeurEntityTemp :
        public EntityIDs<IDM,RelationAttribut,DateTimeAtt,ValeurAtt> {
public:
    using ValTrans = typename ValeurAtt::AttTrans;
    using EAID = EntityIDs<IDM,RelationAttribut,DateTimeAtt,ValeurAtt>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Valeur = PositionEnum<ValeurAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,RelationAttribut,DateTimeAtt,ValeurAtt>::EntityIDs;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setDateTime;
    using EAID::setValeur;
    BASE_ENTITY(RelationDateTimeValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    RelationDateTimeValeurEntityTemp(idt id1, idt id2, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setId2(id2);
    }

    //! Constructeur à partir des valeurs attributs.
    RelationDateTimeValeurEntityTemp(idt id1, idt id2, ValTrans valeur, idt id)
        : RelationDateTimeValeurEntityTemp(id1, id2, id)
        {setValeur(valeur);}

    //! Constructeur à partir des valeurs attributs.
    RelationDateTimeValeurEntityTemp(idt id1, idt id2, const QDateTime & dateTime, ValTrans valeur, idt id = 0)
        : RelationDateTimeValeurEntityTemp(id1, id2, valeur, id)
        {setDateTime(dateTime);}
};

template<szt IDM , class DateTimeAtt,class ValeurAtt>
    RelationDateTimeValeurEntityTemp<IDM, DateTimeAtt, ValeurAtt>::~RelationDateTimeValeurEntityTemp() {}

template<szt IDM > using RelationDateTimeCurrentValeurDoubleEntity
    = RelationDateTimeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurDoubleAttribut>;
#define ENUM_RelationDateTimeCurrentValeurDouble(ID1,ID2) ENUM_RelationDateTimeValeurTemp(ID1,ID2)

template<szt IDM > using RelationDateTimeCurrentValeurIntEntity
    = RelationDateTimeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurIntAttribut>;
#define ENUM_RelationDateTimeCurrentValeurInt(ID1,ID2) ENUM_RelationDateTimeValeurTemp(ID1,ID2)

template<szt IDM > using RelationDateTimeCurrentValeurVariantEntity
    = RelationDateTimeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurVariantAttribut>;
#define ENUM_RelationDateTimeCurrentValeurVariant(ID1,ID2) ENUM_RelationDateTimeValeurTemp(ID1,ID2)

template<szt IDM > using RelationDateTimeValideValeurDoubleEntity
    = RelationDateTimeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurDoubleAttribut>;
#define ENUM_RelationDateTimeValideValeurDouble(ID1,ID2) ENUM_RelationDateTimeValeurTemp(ID1,ID2)

template<szt IDM > using RelationDateTimeValideValeurIntEntity
    = RelationDateTimeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurIntAttribut>;
#define ENUM_RelationDateTimeValideValeurInt(ID1,ID2) ENUM_RelationDateTimeValeurTemp(ID1,ID2)

template<szt IDM > using RelationDateTimeValideValeurVariantEntity
    = RelationDateTimeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurVariantAttribut>;
#define ENUM_RelationDateTimeValideValeurVariant(ID1,ID2) ENUM_RelationDateTimeValeurTemp(ID1,ID2)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation avec un attribut dateTime, num et Valeur.
#define ENUM_RelationDateTimeNumValeurTemp(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position:szt {Id, Id1 = mere::Id1, Id2 = mere::Id2, DateTime = mere::DateTime, Num = mere::Num, Valeur = mere::Valeur,\
                NbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés, une dateTime, un numero et une valeur.
 */
template<szt IDM , class DateTimeAtt,class ValeurAtt> class RelationDateTimeNumValeurEntityTemp :
        public EntityIDs<IDM,RelationAttribut,DateTimeAtt,NumAttribut,ValeurAtt> {
public:
    using ValTrans = typename ValeurAtt::AttTrans;
    using EAID = EntityIDs<IDM,RelationAttribut,DateTimeAtt,NumAttribut,ValeurAtt>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   Valeur = PositionEnum<ValeurAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,RelationAttribut,DateTimeAtt,NumAttribut,ValeurAtt>::EntityIDs;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setDateTime;
    using EAID::setNum;
    using EAID::setValeur;
    BASE_ENTITY(RelationDateTimeNumValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    RelationDateTimeNumValeurEntityTemp(idt id1, idt id2, int num = 0, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setId2(id2);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    RelationDateTimeNumValeurEntityTemp(idt id1, idt id2, int num, ValTrans valeur, idt id = 0)
        : RelationDateTimeNumValeurEntityTemp(id1, id2, num, id)
        {setValeur(valeur);}

    //! Constructeur à partir des valeurs attributs.
    RelationDateTimeNumValeurEntityTemp(idt id1, idt id2, const QDateTime & dateTime, int num, ValTrans valeur, idt id = 0)
        : RelationDateTimeNumValeurEntityTemp(id1, id2, num, valeur, id)
        {setDateTime(dateTime);}
};

template<szt IDM , class DateTimeAtt,class ValeurAtt>
    RelationDateTimeNumValeurEntityTemp<IDM,DateTimeAtt, ValeurAtt>::~RelationDateTimeNumValeurEntityTemp() {}

template<szt IDM > using RelationDateTimeCurrentNumValeurDoubleEntity
    = RelationDateTimeNumValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurDoubleAttribut>;
#define ENUM_RelationDateTimeCurrentNumValeurDouble(ID1,ID2) ENUM_RelationDateTimeNumValeurTemp(ID1,ID2)

template<szt IDM > using RelationDateTimeCurrentNumValeurIntEntity
    = RelationDateTimeNumValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurIntAttribut>;
#define ENUM_RelationDateTimeCurrentNumValeurInt(ID1,ID2) ENUM_RelationDateTimeNumValeurTemp(ID1,ID2)

template<szt IDM > using RelationDateTimeCurrentNumValeurVariantEntity
    = RelationDateTimeNumValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurVariantAttribut>;
#define ENUM_RelationDateTimeCurrentNumValeurVariant(ID1,ID2) ENUM_RelationDateTimeNumValeurTemp(ID1,ID2)

template<szt IDM > using RelationDateTimeValideNumValeurDoubleEntity
    = RelationDateTimeNumValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurDoubleAttribut>;
#define ENUM_RelationDateTimeValideNumValeurDouble(ID1,ID2) ENUM_RelationDateTimeNumValeurTemp(ID1,ID2)

template<szt IDM > using RelationDateTimeValideNumValeurIntEntity
    = RelationDateTimeNumValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurIntAttribut>;
#define ENUM_RelationDateTimeValideNumValeurInt(ID1,ID2) ENUM_RelationDateTimeNumValeurTemp(ID1,ID2)

template<szt IDM > using RelationDateTimeValideNumValeurVariantEntity
    = RelationDateTimeNumValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurVariantAttribut>;
#define ENUM_RelationDateTimeValideNumValeurVariant(ID1,ID2) ENUM_RelationDateTimeNumValeurTemp(ID1,ID2)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation avec un attribut code, dateTime et Valeur.
#define ENUM_RelationCodeDateTimeValeurTemp(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position:szt {Id, Id1 = mere::Id1, Id2 = mere::Id2, Code = mere::Code, DateTime = mere::DateTime,\
                        Valeur = mere::Valeur,nNbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés, un code, une dateTime et une valeur.
 */
template<szt IDM , class DateTimeAtt,class ValeurAtt> class RelationCodeDateTimeValeurEntityTemp :
                public EntityIDs<IDM,RelationAttribut,CodeAttribut,DateTimeAtt,ValeurAtt> {
public:
    using ValTrans = typename ValeurAtt::AttTrans;
    using EAID = EntityIDs<IDM,RelationAttribut,CodeAttribut,DateTimeAtt,ValeurAtt>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   Code = PositionEnum<CodeAttribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Valeur = PositionEnum<ValeurAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,RelationAttribut,CodeAttribut,DateTimeAtt,ValeurAtt>
                            ::EntityIDs;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setCode;
    using EAID::setDateTime;
    using EAID::setValeur;
    BASE_ENTITY(RelationCodeDateTimeValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    RelationCodeDateTimeValeurEntityTemp(idt id1, idt id2, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setId2(id2);
    }

    //! Constructeur à partir des valeurs attributs.
    RelationCodeDateTimeValeurEntityTemp(idt id1, idt id2, ValTrans valeur, idt id = 0)
        : RelationCodeDateTimeValeurEntityTemp(id1, id2, id)
    {setValeur(valeur);}

    //! Constructeur à partir des valeurs attributs.
    RelationCodeDateTimeValeurEntityTemp(idt id1, idt id2, flag code, const QDateTime & dateTime, ValTrans valeur, idt id = 0)
        : RelationCodeDateTimeValeurEntityTemp(id1, id2, id) {
        setCode(code);
        setDateTime(dateTime);
        setValeur(valeur);
    }
};

template<szt IDM , class DateTimeAtt,class ValeurAtt>
            RelationCodeDateTimeValeurEntityTemp<IDM,DateTimeAtt, ValeurAtt>::~RelationCodeDateTimeValeurEntityTemp() {}

template<szt IDM > using RelationCodeDateTimeCurrentValeurDoubleEntity
    = RelationCodeDateTimeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurDoubleAttribut>;
#define ENUM_RelationCodeDateTimeCurrentValeurDouble(ID1,ID2) ENUM_RelationCodeDateTimeValeurTemp(ID1,ID2)

template<szt IDM > using RelationCodeDateTimeCurrentValeurIntEntity
    = RelationCodeDateTimeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurIntAttribut>;
#define ENUM_RelationCodeDateTimeCurrentValeurInt(ID1,ID2) ENUM_RelationCodeDateTimeValeurTemp(ID1,ID2)

template<szt IDM > using RelationCodeDateTimeCurrentValeurVariantEntity
    = RelationCodeDateTimeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurVariantAttribut>;
#define ENUM_RelationCodeDateTimeCurrentValeurVariant(ID1,ID2) ENUM_RelationCodeDateTimeValeurTemp(ID1,ID2)

template<szt IDM > using RelationCodeDateTimeValideValeurDoubleEntity
    = RelationCodeDateTimeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurDoubleAttribut>;
#define ENUM_RelationCodeDateTimeValideValeurDouble(ID1,ID2) ENUM_RelationCodeDateTimeValeurTemp(ID1,ID2)

template<szt IDM > using RelationCodeDateTimeValideValeurIntEntity
    = RelationCodeDateTimeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurIntAttribut>;
#define ENUM_RelationCodeDateTimeValideValeurInt(ID1,ID2) ENUM_RelationCodeDateTimeValeurTemp(ID1,ID2)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation avec un attribut code, dateTime et Valeur.
#define ENUM_RelationDateTimeSaisieValeurTemp(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position:szt {Id, Id1 = mere::Id1, Id2 = mere::Id2, DateTime = mere::DateTime, Saisie = mere::Saisie,\
                        Valeur = mere::Valeur,nNbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés, un code, une dateTime et une valeur.
 */
template<szt IDM , class DateTimeAtt,class ValeurAtt> class RelationDateTimeSaisieValeurEntityTemp :
                public EntityIDs<IDM,RelationAttribut,DateTimeAtt,SaisieAttribut,ValeurAtt> {
public:
    using ValTrans = typename ValeurAtt::AttTrans;
    using EAID = EntityIDs<IDM,RelationAttribut,DateTimeAtt,SaisieAttribut,ValeurAtt>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Saisie = PositionEnum<SaisieAttribut,EAID>::Position,
                   Valeur = PositionEnum<ValeurAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,RelationAttribut,DateTimeAtt,SaisieAttribut,ValeurAtt>
                            ::EntityIDs;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setDateTime;
    using EAID::setSaisie;
    using EAID::setValeur;
    BASE_ENTITY(RelationDateTimeSaisieValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    RelationDateTimeSaisieValeurEntityTemp(idt id1, idt id2, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setId2(id2);
    }

    //! Constructeur à partir des valeurs attributs.
    RelationDateTimeSaisieValeurEntityTemp(idt id1, idt id2, ValTrans valeur, idt id = 0)
        : RelationDateTimeSaisieValeurEntityTemp(id1, id2, id)
    {setValeur(valeur);}

    //! Constructeur à partir des valeurs attributs.
    RelationDateTimeSaisieValeurEntityTemp(idt id1, idt id2, const QDateTime & dateTime, flag saisie, ValTrans valeur, idt id = 0)
        : RelationDateTimeSaisieValeurEntityTemp(id1, id2, id) {
        setDateTime(dateTime);
        setSaisie(saisie);
        setValeur(valeur);
    }
};

template<szt IDM , class DateTimeAtt,class ValeurAtt>
            RelationDateTimeSaisieValeurEntityTemp<IDM,DateTimeAtt, ValeurAtt>::~RelationDateTimeSaisieValeurEntityTemp() {}

template<szt IDM > using RelationCodeDateTimeValideValeurVariantEntity
    = RelationDateTimeSaisieValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurVariantAttribut>;
#define ENUM_RelationCodeDateTimeValideValeurVariant(ID1,ID2) ENUM_RelationDateTimeSaisieValeurTemp(ID1,ID2)

template<szt IDM > using RelationDateTimeCurrentSaisieValeurDoubleEntity
    = RelationDateTimeSaisieValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurDoubleAttribut>;
#define ENUM_RelationDateTimeCurrentSaisieValeurDouble(ID1,ID2) ENUM_RelationDateTimeSaisieValeurTemp(ID1,ID2)

template<szt IDM > using RelationDateTimeCurrentSaisieValeurIntEntity
    = RelationDateTimeSaisieValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurIntAttribut>;
#define ENUM_RelationDateTimeCurrentSaisieValeurInt(ID1,ID2) ENUM_RelationDateTimeSaisieValeurTemp(ID1,ID2)

template<szt IDM > using RelationCodeDateTimeCurrentSaisieValeurVariantEntity
    = RelationDateTimeSaisieValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurVariantAttribut>;
#define ENUM_RelationDateTimeCurrentSaisieValeurVariant(ID1,ID2) ENUM_RelationDateTimeSaisieValeurTemp(ID1,ID2)

template<szt IDM > using RelationCodeDateTimeValideSaisieValeurDoubleEntity
    = RelationDateTimeSaisieValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurDoubleAttribut>;
#define ENUM_RelationDateTimeValideSaisieValeurDouble(ID1,ID2) ENUM_RelationDateTimeSaisieValeurTemp(ID1,ID2)

template<szt IDM > using RelationCodeDateTimeValideSaisieValeurIntEntity
    = RelationDateTimeSaisieValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurIntAttribut>;
#define ENUM_RelationDateTimeValideSaisieValeurInt(ID1,ID2) ENUM_RelationDateTimeSaisieValeurTemp(ID1,ID2)

template<szt IDM > using RelationCodeDateTimeValideSaisieValeurVariantEntity
    = RelationDateTimeSaisieValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurVariantAttribut>;
#define ENUM_RelationDateTimeValideSaisieValeurVariant(ID1,ID2) ENUM_RelationDateTimeSaisieValeurTemp(ID1,ID2)

//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation avec un attribut code, dateTime, num et valeur.
#define ENUM_RelationCodeDateTimeNumValeurTemp(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position:szt {Id, Id1 = mere::Id1, Id2 = mere::Id2, Code = mere::Code, DateTime = mere::DateTime, Num = mere::Num,\
                    Valeur = mere::Valeur,nNbrAtt = mere::NbrAtt, Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés, un code, une dateTime, un numero et une valeur.
 */
template<szt IDM , class DateTimeAtt,class ValeurAtt, class CodeAtt> class RelationCodeDateTimeNumValeurEntityTemp :
            public EntityIDs<IDM,RelationAttribut,CodeAtt,DateTimeAtt,NumAttribut,ValeurAtt> {
public:
    using ValTrans = typename ValeurAtt::AttTrans;
    using EAID = EntityIDs<IDM,RelationAttribut,CodeAtt,DateTimeAtt,NumAttribut,ValeurAtt>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                    Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                    Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                    Code = PositionEnum<CodeAttribut,EAID>::Position,
                    DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                    Num = PositionEnum<NumAttribut,EAID>::Position,
                    Valeur = PositionEnum<ValeurAtt,EAID>::Position,
                    NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,RelationAttribut,CodeAtt,DateTimeAtt,NumAttribut,ValeurAtt>
                        ::EntityIDs;
    using EAID::setCode;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setDateTime;
    using EAID::setNum;
    using EAID::setValeur;
    BASE_ENTITY(RelationCodeDateTimeNumValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    RelationCodeDateTimeNumValeurEntityTemp(idt id1, idt id2, int num = 0, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setId2(id2);
        setNum(num);
    }

    //! Constructeur à partir des valeurs attributs.
    RelationCodeDateTimeNumValeurEntityTemp(idt id1, idt id2, int num, ValTrans valeur, idt id = 0)
        : RelationCodeDateTimeNumValeurEntityTemp(id1, id2, num, id)
        {setValeur(valeur);}

    //! Constructeur à partir des valeurs attributs.
    RelationCodeDateTimeNumValeurEntityTemp(idt id1, idt id2, flag code, const QDateTime & dateTime, int num, ValTrans valeur, idt id = 0)
        : RelationCodeDateTimeNumValeurEntityTemp(id1, id2, num, id) {
        setCode(code);
        setDateTime(dateTime);
        setValeur(valeur);
    }
};

template<szt IDM , class DateTimeAtt,class ValeurAtt, class CodeAtt>
        RelationCodeDateTimeNumValeurEntityTemp<IDM,DateTimeAtt, ValeurAtt, CodeAtt>::~RelationCodeDateTimeNumValeurEntityTemp() {}

template<szt IDM > using RelationCodeDateTimeCurrentNumValeurDoubleEntity
        = RelationCodeDateTimeNumValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurDoubleAttribut,CodeAttribut>;
#define ENUM_RelationCodeDateTimeCurrentNumValeurDouble(ID1,ID2) ENUM_RelationCodeDateTimeNumValeurTemp(ID1,ID2)

template<szt IDM > using RelationCodeDateTimeCurrentNumValeurIntEntity
        = RelationCodeDateTimeNumValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurIntAttribut,CodeAttribut>;
#define ENUM_RelationCodeDateTimeCurrentNumValeurInt(ID1,ID2) ENUM_RelationCodeDateTimeNumValeurTemp(ID1,ID2)

template<szt IDM > using RelationCodeDateTimeCurrentNumValeurVariantEntity
        = RelationCodeDateTimeNumValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurVariantAttribut,CodeAttribut>;
#define ENUM_RelationCodeDateTimeCurrentNumValeurVariant(ID1,ID2) ENUM_RelationCodeDateTimeNumValeurTemp(ID1,ID2)

template<szt IDM > using RelationCodeDateTimeValideNumValeurDoubleEntity
        = RelationCodeDateTimeNumValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurDoubleAttribut,CodeAttribut>;
#define ENUM_RelationCodeDateTimeValideNumValeurDouble(ID1,ID2) ENUM_RelationCodeDateTimeNumValeurTemp(ID1,ID2)

template<szt IDM > using RelationCodeDateTimeValideNumValeurIntEntity
        = RelationCodeDateTimeNumValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurIntAttribut,CodeAttribut>;
#define ENUM_RelationCodeDateTimeValideNumValeurInt(ID1,ID2) ENUM_RelationCodeDateTimeNumValeurTemp(ID1,ID2)

template<szt IDM > using RelationCodeDateTimeValideNumValeurVariantEntity
        = RelationCodeDateTimeNumValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurVariantAttribut,SaisieAttribut>;
#define ENUM_RelationCodeDateTimeValideNumValeurVariant(ID1,ID2) ENUM_RelationCodeDateTimeNumValeurTemp(ID1,ID2)

template<szt IDM > using RelationDateTimeCurrentNumSaisieValeurDoubleEntity
        = RelationCodeDateTimeNumValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurDoubleAttribut,SaisieAttribut>;
#define ENUM_RelationDateTimeCurrentNumSaisieValeurDouble(ID1,ID2) ENUM_RelationCodeDateTimeNumValeurTemp(ID1,ID2)

template<szt IDM > using RelationDateTimeCurrentNumSaisieValeurIntEntity
        = RelationCodeDateTimeNumValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurIntAttribut,SaisieAttribut>;
#define ENUM_RelationDateTimeCurrentNumSaisieValeurInt(ID1,ID2) ENUM_RelationCodeDateTimeNumValeurTemp(ID1,ID2)

template<szt IDM > using RelationCodeDateTimeCurrentNumSaisieValeurVariantEntity
        = RelationCodeDateTimeNumValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurVariantAttribut,SaisieAttribut>;
#define ENUM_RelationDateTimeCurrentNumSaisieValeurVariant(ID1,ID2) ENUM_RelationCodeDateTimeNumValeurTemp(ID1,ID2)

template<szt IDM > using RelationCodeDateTimeValideNumSaisieValeurDoubleEntity
        = RelationCodeDateTimeNumValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurDoubleAttribut,SaisieAttribut>;
#define ENUM_RelationDateTimeValideNumSaisieValeurDouble(ID1,ID2) ENUM_RelationCodeDateTimeNumValeurTemp(ID1,ID2)

template<szt IDM > using RelationCodeDateTimeValideNumSaisieValeurIntEntity
        = RelationCodeDateTimeNumValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurIntAttribut,SaisieAttribut>;
#define ENUM_RelationDateTimeValideNumSaisieValeurInt(ID1,ID2) ENUM_RelationCodeDateTimeNumValeurTemp(ID1,ID2)

template<szt IDM > using RelationCodeDateTimeValideNumSaisieValeurVariantEntity
        = RelationCodeDateTimeNumValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurVariantAttribut,SaisieAttribut>;
#define ENUM_RelationDateTimeValideNumSaisieValeurVariant(ID1,ID2) ENUM_RelationCodeDateTimeNumValeurTemp(ID1,ID2)


//! \ingroup groupeMacroEntity
//! Macro définissant les positions des attributs pour une relation avec un attribut dateTime, num et Valeur.
#define ENUM_RelationNumTypeValeurTemp(ID1,ID2) /*! \brief Positions des attributs */ \
    enum Position:szt {Id, Id1 = mere::Id1, Id2 = mere::Id2, Num = mere::Num, Type = mere::Type, Valeur = mere::Valeur, NbrAtt = mere::NbrAtt,\
            Id ## ID1 = Id1, Id ## ID2 = Id2};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant pour attribut deux clés, un numero, un type et une valeur.
 */
template<szt IDM , class ValeurAtt> class RelationNumTypeValeurEntityTemp :
            public EntityIDs<IDM,RelationAttribut,NumAttribut,TypeAttribut,ValeurAtt> {
public:
    using ValTrans = typename ValeurAtt::AttTrans;
    using EAID = EntityIDs<IDM,RelationAttribut,NumAttribut,TypeAttribut,ValeurAtt>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Id1 = PositionEnum<Id1Attribut,EAID>::Position,
                   Id2 = PositionEnum<Id2Attribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   Valeur = PositionEnum<ValeurAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,RelationAttribut,NumAttribut,TypeAttribut,ValeurAtt>::EntityIDs;
    using EAID::setId1;
    using EAID::setId2;
    using EAID::setNum;
    using EAID::setType;
    using EAID::setValeur;
    BASE_ENTITY(RelationNumTypeValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    RelationNumTypeValeurEntityTemp(idt id1, idt id2, int num = 0, idt type =0, idt id = 0)
        : EAID(id) {
        setId1(id1);
        setId2(id2);
        setNum(num);
        setType(type);
    }

    //! Constructeur à partir des valeurs attributs.
    RelationNumTypeValeurEntityTemp(idt id1, idt id2, int num, idt type, ValTrans valeur, idt id = 0)
        : RelationNumTypeValeurEntityTemp(id1, id2, num, type, id)
    {setValeur(valeur);}
};

template<szt IDM , class ValeurAtt> RelationNumTypeValeurEntityTemp<IDM,ValeurAtt>::~RelationNumTypeValeurEntityTemp() {}

template<szt IDM > using RelationNumTypeValeurDoubleEntity = RelationNumTypeValeurEntityTemp<IDM,ValeurDoubleAttribut>;
#define ENUM_RelationNumTypeValeurDouble(ID1,ID2) ENUM_RelationNumTypeValeurTemp(ID1,ID2)

template<szt IDM > using RelationNumTypeValeurIntEntity = RelationNumTypeValeurEntityTemp<IDM,ValeurIntAttribut>;
#define ENUM_RelationNumTypeValeurInt(ID1,ID2) ENUM_RelationNumTypeValeurTemp(ID1,ID2)

template<szt IDM > using RelationNumTypeValeurVariantEntity = RelationNumTypeValeurEntityTemp<IDM,ValeurVariantAttribut>;
#define ENUM_RelationNumTypeValeurVariant(ID1,ID2) ENUM_RelationNumTypeValeurTemp(ID1,ID2)
}
#endif // ENTITYRELATION
