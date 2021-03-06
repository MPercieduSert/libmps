/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef ENTITYDIVERS
#define ENTITYDIVERS

#include "Entity.h"

/*! \defgroup groupeBaseEntity Entités de base
 * \ingroup groupeEntity
 * \brief Ensemble de classes représentant les entités de base.
 */

/*! \ingroup groupeBaseEntity
 * \brief Espace de nom de entités de base.
 */
namespace entityBaseMPS {
using namespace entityMPS;
using namespace attributMPS;

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut dateTime, Nom et type.
 */
class Arbre : public EntityID<infoEntity::ArbreId,ArbreAttribut> {
public:
    using EAID = EntityID<infoEntity::ArbreId,ArbreAttribut>;
    //! Positions des attributs.
    enum Position:post {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Feuille = PositionEnum<FeuilleAttribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   Parent = PositionEnum<ParentAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EAID::EntityID;
    BASE_ENTITY(Arbre)

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    Arbre(int num, idt parent, idt id = 0)
        : EAID(id) {
        setNum(num);
        setParent(parent);
    }

    //! Constructeur à partir des valeurs attributs.
    Arbre(bool feuille, int num, idt parent, idt id = 0)
        : Arbre(num,parent,id)
        {setFeuille(feuille);}
};

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut dateTime, Nom et type.
 */
template<entidt IDM , class DateTimeAtt> class DateTimeNomTypeEntityTemp
        : public EntityIDs<IDM,DateTimeAtt,NomAttribut,TypeAttribut> {
public:
    using EAID = EntityIDs<IDM,DateTimeAtt,NomAttribut,TypeAttribut>;
    //! Positions des attributs.
    enum Position:post {Id = PositionEnum<IdAttribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Nom = PositionEnum<NomAttribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,DateTimeAtt,NomAttribut,TypeAttribut>::EntityIDs;
    using EAID::setDateTime;
    using EAID::setNom;
    using EAID::setType;
    BASE_ENTITY(DateTimeNomTypeEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    DateTimeNomTypeEntityTemp(const QString & nom, idt type = 0, idt id = 0)
        : EAID(id) {
        setNom(nom);
        setType(type);
    }

    //! Constructeur à partir des valeurs attributs.
    DateTimeNomTypeEntityTemp(const QDateTime & dateTime, const QString & nom, idt type, idt id = 0)
        : DateTimeNomTypeEntityTemp(nom, type, id)
        {setDateTime(dateTime);}
};

template<entidt IDM , class DateTimeAtt> DateTimeNomTypeEntityTemp<IDM,DateTimeAtt>::~DateTimeNomTypeEntityTemp() {}

template<entidt IDM > using DateTimeCurrentNomTypeEntity = DateTimeNomTypeEntityTemp<IDM,DateTimeCurrentAttribut>;
template<entidt IDM > using DateTimeValideNomTypeEntity = DateTimeNomTypeEntityTemp<IDM,DateTimeValideAttribut>;

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut dateTime, Nc, Nom et type.
 */
template<entidt IDM , class DateTimeAtt> class DateTimeNcNomTypeEntityTemp
        : public EntityIDs<IDM,DateTimeAtt,NcNomAttribut,TypeAttribut> {
public:
    using EAID = EntityIDs<IDM,DateTimeAtt,NcNomAttribut,TypeAttribut>;
    //! Positions des attributs.
    enum Position:post {Id = PositionEnum<IdAttribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Nc = PositionEnum<NcAttribut,EAID>::Position,
                   Nom = PositionEnum<NomAttribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,DateTimeAtt,NcNomAttribut,TypeAttribut>::EntityIDs;
    using EAID::setDateTime;
    using EAID::setNom;
    using EAID::setNc;
    using EAID::setType;
    BASE_ENTITY(DateTimeNcNomTypeEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    DateTimeNcNomTypeEntityTemp(const QString & nom, idt type = 0, idt id = 0)
        : EAID(id) {
        setNom(nom);
        setType(type);
    }

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    DateTimeNcNomTypeEntityTemp(const QString & nc, const QString & nom, idt type = 0, idt id = 0)
        : DateTimeNcNomTypeEntityTemp(nom, type, id)
        {setNc(nc);}

    //! Constructeur à partir des valeurs attributs.
    DateTimeNcNomTypeEntityTemp(const QDateTime & dateTime, const QString & nc, const QString & nom, idt type, idt id = 0)
        : DateTimeNcNomTypeEntityTemp(nc, nom, type, id)
        {setDateTime(dateTime);}
};

template<entidt IDM , class DateTimeAtt> DateTimeNcNomTypeEntityTemp<IDM,DateTimeAtt>::~DateTimeNcNomTypeEntityTemp() {}

template<entidt IDM > using DateTimeCurrentNcNomTypeEntity = DateTimeNcNomTypeEntityTemp<IDM,DateTimeCurrentAttribut>;
template<entidt IDM > using DateTimeValideNcNomTypeEntity = DateTimeNcNomTypeEntityTemp<IDM,DateTimeValideAttribut>;

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut dateTime et Num.
 */
template<entidt IDM , class DateTimeAtt> class DateTimeNumEntityTemp : public EntityIDs<IDM,DateTimeAtt,NumAttribut> {
public:
    using EAID = EntityIDs<IDM,DateTimeAtt,NumAttribut>;
    //! Positions des attributs.
    enum Position:post {Id = PositionEnum<IdAttribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};
    using EntityIDs<IDM,DateTimeAtt,NumAttribut>::EntityIDs;
    using EAID::setDateTime;
    using EAID::setNum;
    BASE_ENTITY(DateTimeNumEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    DateTimeNumEntityTemp(int num, idt id)
        : EAID(id)
        {setNum(num);}

    //! Constructeur à partir des valeurs attributs.
    DateTimeNumEntityTemp(const QDateTime & dateTime, int num, idt id = 0)
        : DateTimeNumEntityTemp(num, id)
        {setDateTime(dateTime);}
};

template<entidt IDM , class DateTimeAtt> DateTimeNumEntityTemp<IDM,DateTimeAtt>::~DateTimeNumEntityTemp() {}

template<entidt IDM > using DateTimeCurrentNumEntity = DateTimeNumEntityTemp<IDM,DateTimeCurrentAttribut>;
template<entidt IDM > using DateTimeValideNumEntity = DateTimeNumEntityTemp<IDM,DateTimeValideAttribut>;

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut dateTime, type et valeur.
 */
template<entidt IDM , class DateTimeAtt, class ValeurAtt> class DateTimeTypeValeurEntityTemp
        : public EntityIDs<IDM,DateTimeAtt,TypeAttribut,ValeurAtt> {
public:
    using ValTrans = typename ValeurAtt::AttTrans;
    using EAID = EntityIDs<IDM,DateTimeAtt,TypeAttribut,ValeurAtt>;
    //! Positions des attributs.
    enum Position:post {Id = PositionEnum<IdAttribut,EAID>::Position,
                   DateTime = PositionEnum<DateTimeAtt,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   Valeur = PositionEnum<ValeurAtt,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,DateTimeAtt,TypeAttribut,ValeurAtt>::EntityIDs;
    using EAID::setDateTime;
    using EAID::setType;
    using EAID::setValeur;
    BASE_ENTITY(DateTimeTypeValeurEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    DateTimeTypeValeurEntityTemp(idt type, ValTrans valeur, idt id = 0)
        : EAID(id) {
        setType(type);
        setValeur(valeur);
    }

    //! Constructeur à partir des valeurs attributs.
    DateTimeTypeValeurEntityTemp(const QDateTime & dateTime, idt type, ValTrans valeur, idt id = 0)
        : DateTimeTypeValeurEntityTemp(type, valeur, id)
        {setDateTime(dateTime);}
};

template<entidt IDM , class DateTimeAtt, class ValeurAtt> DateTimeTypeValeurEntityTemp<IDM,DateTimeAtt, ValeurAtt>
                                                                    ::~DateTimeTypeValeurEntityTemp() {}

template<entidt IDM > using DateTimeCurrentTypeValeurDoubleEntity = DateTimeTypeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurDoubleAttribut>;
template<entidt IDM > using DateTimeCurrentTypeValeurIntEntity = DateTimeTypeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurIntAttribut>;
template<entidt IDM > using DateTimeCurrentTypeValeurVariantEntity
    = DateTimeTypeValeurEntityTemp<IDM,DateTimeCurrentAttribut,ValeurVariantAttribut>;
template<entidt IDM > using DateTimeValideTypeValeurDoubleEntity = DateTimeTypeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurDoubleAttribut>;
template<entidt IDM > using DateTimeValideTypeValeurIntEntity = DateTimeTypeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurIntAttribut>;
template<entidt IDM > using DateTimeValideTypeValeurVariantEntity = DateTimeTypeValeurEntityTemp<IDM,DateTimeValideAttribut,ValeurVariantAttribut>;

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant des attributs parent, nc et nom.
 */
template<entidt IDM > class ArbreSimpleNcNomEntity : public EntityIDs<IDM,ParentAttribut,NcAttribut,NomAttribut> {
public:
    using EAID = EntityIDs<IDM,ParentAttribut,NcAttribut,NomAttribut>;
    //! Positions des attributs.
    enum Position:post {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Nc = PositionEnum<NcAttribut,EAID>::Position,
                   Nom = PositionEnum<NomAttribut,EAID>::Position,
                   Parent = PositionEnum<ParentAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt,
                   Ordre = Nom};

    using EntityIDs<IDM,ParentAttribut,NcAttribut,NomAttribut>::EntityIDs;
    using EAID::setNc;
    using EAID::setNom;
    using EAID::setParent;
    BASE_ENTITY(ArbreSimpleNcNomEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    ArbreSimpleNcNomEntity(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    ArbreSimpleNcNomEntity(const QString & nc, const QString & nom, idt parent = 0, idt id = 0)
        : ArbreSimpleNcNomEntity(nom, id) {
        setNc(nc);
        setParent(parent);
    }
};

template<entidt IDM > ArbreSimpleNcNomEntity<IDM>::~ArbreSimpleNcNomEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant des attributs parent, nc, nom et ref.
 */
template<entidt IDM > class ArbreSimpleNcNomRefEntity : public EntityIDs<IDM,ParentAttribut,NcNomRefAttribut> {
public:
    using EAID = EntityIDs<IDM,ParentAttribut,NcNomRefAttribut>;
    //! Positions des attributs.
    enum Position:post {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Nc = PositionEnum<NcAttribut,EAID>::Position,
                   Nom = PositionEnum<NomAttribut,EAID>::Position,
                   Parent = PositionEnum<ParentAttribut,EAID>::Position,
                   Ref = PositionEnum<RefAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt,
                   Ordre = Nom};

    using EntityIDs<IDM,ParentAttribut,NcNomRefAttribut>::EntityIDs;
    using EAID::setNc;
    using EAID::setNom;
    using EAID::setParent;
    using EAID::setRef;
    BASE_ENTITY(ArbreSimpleNcNomRefEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    ArbreSimpleNcNomRefEntity(const QString & ref, idt id = 0)
        : EAID(id)
    {setRef(ref);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    ArbreSimpleNcNomRefEntity(const QString & nom, idt parent, idt id = 0)
        : EAID(id) {
        setNom(nom);
        setParent(parent);
    }

    //! Constructeur à partir des valeurs attributs.
    ArbreSimpleNcNomRefEntity(const QString & nc, const QString & nom, idt parent = 0, const QString & ref = QString(), idt id = 0)
        : ArbreSimpleNcNomRefEntity(nom, parent, id) {
        setNc(nc);
        setParent(parent);
        setRef(ref);
    }
};

template<entidt IDM > ArbreSimpleNcNomRefEntity<IDM>::~ArbreSimpleNcNomRefEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut nom et nc.
 */
template<entidt IDM > class NcNomEntity : public EntityID<IDM,NcNomAttribut> {
public:
    using EAID = EntityID<IDM,NcNomAttribut>;
    //! Positions des attributs.
    enum Position:post {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Nc = PositionEnum<NcAttribut,EAID>::Position,
                   Nom = PositionEnum<NomAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityID<IDM,NcNomAttribut>::EntityID;
    using EAID::setNc;
    using EAID::setNom;
    BASE_ENTITY(NcNomEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    NcNomEntity(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    NcNomEntity(const QString & nc, const QString & nom, idt id = 0)
        : NcNomEntity(nom, id)
        {setNc(nc);}
};

template<entidt IDM > NcNomEntity<IDM>::~NcNomEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant des attributs nc, nom et type.
 */
template<entidt IDM > class NcNomTypeEntity : public EntityID<IDM,NcNomTypeAttribut> {
public:
    using EAID = EntityID<IDM,NcNomTypeAttribut>;
    //! Positions des attributs.
    enum Position:post {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Nc = PositionEnum<NcAttribut,EAID>::Position,
                   Nom = PositionEnum<NomAttribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityID<IDM,NcNomTypeAttribut>::EntityID;
    using EAID::setNc;
    using EAID::setNom;
    using EAID::setType;
    BASE_ENTITY(NcNomTypeEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    NcNomTypeEntity(const QString & nom)
        {setNom(nom);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    NcNomTypeEntity(const QString & nom, idt type)
        : NcNomTypeEntity(nom)
        {setType(type);}

    //! Constructeur à partir des valeurs attributs.
    NcNomTypeEntity(const QString & nc, const QString & nom, idt type, idt id = 0)
        : EAID(id) {
        setNc(nc);
        setNom(nom);
        setType(type);
    }
};

template<entidt IDM > NcNomTypeEntity<IDM>::~NcNomTypeEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant des attributs nc, nom, ref et type.
 */
template<entidt IDM > class NcNomRefTypeEntity : public EntityID<IDM,NcNomRefTypeAttribut> {
public:
    using EAID = EntityID<IDM,NcNomRefTypeAttribut>;
    //! Positions des attributs.
    enum Position:post {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Nc = PositionEnum<NcAttribut,EAID>::Position,
                   Nom = PositionEnum<NomAttribut,EAID>::Position,
                   Ref = PositionEnum<RefAttribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityID<IDM,NcNomRefTypeAttribut>::EntityID;
    using EAID::setNc;
    using EAID::setNom;
    using EAID::setRef;
    using EAID::setType;
    BASE_ENTITY(NcNomRefTypeEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    NcNomRefTypeEntity(const QString & ref)
        {setRef(ref);}

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    NcNomRefTypeEntity(const QString & nom, idt type, idt id = 0)
        : EAID (id) {
        setNom(nom);
        setType(type);
    }

    //! Constructeur à partir des valeurs attributs.
    NcNomRefTypeEntity(const QString & nc, const QString & nom, const QString & ref, idt type, idt id = 0)
        : NcNomRefTypeEntity(nom,type,id) {
        setNc(nc);
        setRef(ref);
    }
};

template<entidt IDM > NcNomRefTypeEntity<IDM>::~NcNomRefTypeEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut nom.
 */
template<entidt IDM > class NomEntity : public EntityID<IDM,NomAttribut> {
public:
    using EAID = EntityID<IDM,NomAttribut>;
    //! Positions des attributs.
    enum Position:post {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Nom = PositionEnum<NomAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityID<IDM,NomAttribut>::EntityID;
    using EAID::setNom;
    BASE_ENTITY(NomEntity)

    //! Constructeur à partir des valeurs attributs.
    NomEntity(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}
};

template<entidt IDM > NomEntity<IDM>::~NomEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut nom et type.
 */
template<entidt IDM > class NomTypeEntity : public EntityID<IDM,NomTypeAttribut> {
public:
    using EAID = EntityID<IDM,NomTypeAttribut>;
    //! Positions des attributs.
    enum Position:post {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Nom = PositionEnum<NomAttribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityID<IDM,NomTypeAttribut>::EntityID;
    using EAID::setNom;
    using EAID::setType;
    BASE_ENTITY(NomTypeEntity)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    NomTypeEntity(const QString & nom)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    NomTypeEntity(const QString & nom, idt type, idt id = 0)
        : EAID(id) {
        setNom(nom);
        setType(type);
    }
};

template<entidt IDM > NomTypeEntity<IDM>::~NomTypeEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut num.
 */
template<entidt IDM > class NumEntity : public EntityID<IDM,NumAttribut> {
public:
    using EAID = EntityID<IDM,NumAttribut>;
    //! Positions des attributs.
    enum Position:post {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Num = PositionEnum<NumAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityID<IDM,NumAttribut>::EntityID;
    using EAID::setNum;
    BASE_ENTITY(NumEntity)

    //! Constructeur à partir des valeurs attributs.
    NumEntity(int num, idt id = 0)
        : EAID(id)
        {setNum(num);}
};

template<entidt IDM > NumEntity<IDM>::~NumEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut Texte et deux DateTime.
 */
template<entidt IDM > class TexteEntity : public EntityID<IDM,TexteAttributs> {
public:
    using EAID = EntityID<IDM,TexteAttributs>;
    //! Positions des attributs.
    enum Position:post {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Creation = PositionEnum<CreationAttribut,EAID>::Position,
                   Modification = PositionEnum<ModificationAttribut,EAID>::Position,
                   Texte = PositionEnum<TexteAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityID<IDM,TexteAttributs>::EntityID;
    using EAID::setCreation;
    using EAID::setModification;
    using EAID::setTexte;
    BASE_ENTITY(TexteEntity)

    //! Constructeur à partir des valeurs attributs.
    TexteEntity(const QString & texte, idt id = 0)
        : EAID(id)
        {setTexte(texte);}

    //! Constructeur à partir des valeurs attributs.
    TexteEntity(const QDateTime & creation, const QDateTime & modification, const QString & texte, idt id = 0)
        : TexteEntity(texte,id) {
        setCreation(creation);
        setModification(modification);
    }
};

template<entidt IDM > TexteEntity<IDM>::~TexteEntity() {}

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant un attribut type, valeur et version.
 */
template<entidt IDM , class ValeurAtt> class TypeValeurVersionEntityTemp
        : public EntityIDs<IDM,TypeAttribut,ValeurAtt,VersionAttribut> {
public:
    using ValTrans = typename ValeurAtt::AttTrans;
    using EAID = EntityIDs<IDM,TypeAttribut,ValeurAtt,VersionAttribut>;
    //! Positions des attributs.
    enum Position:post {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   Valeur = PositionEnum<ValeurAtt,EAID>::Position,
                   Version = PositionEnum<VersionAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,TypeAttribut,ValeurAtt,VersionAttribut>::EntityIDs;
    using EAID::setType;
    using EAID::setValeur;
    using EAID::setVersion;
    BASE_ENTITY(TypeValeurVersionEntityTemp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    TypeValeurVersionEntityTemp(idt type, ValTrans valeur, int version, idt id = 0)
        : EAID(id) {
        setType(type);
        setValeur(valeur);
        setVersion(version);
    }
};

template<entidt IDM , class ValeurAtt> TypeValeurVersionEntityTemp<IDM, ValeurAtt>
                                                                    ::~TypeValeurVersionEntityTemp() {}

template<entidt IDM > using TypeValeurDoubleVersionEntity = TypeValeurVersionEntityTemp<IDM,ValeurDoubleAttribut>;
template<entidt IDM > using TypeValeurIntVersionEntity = TypeValeurVersionEntityTemp<IDM,ValeurIntAttribut>;
template<entidt IDM > using TypeValeurVariantVersionEntity = TypeValeurVersionEntityTemp<IDM,ValeurVariantAttribut>;

/*! \ingroup groupeBaseEntity
 * \brief Classe de base des entités ayant les attributs type et version.
 */
template<entidt IDM > class TypeVersionEntity : public EntityIDs<IDM,TypeAttribut,VersionAttribut> {
public:
    using EAID = EntityIDs<IDM,TypeAttribut,VersionAttribut>;
    //! Positions des attributs.
    enum Position:post {Id = PositionEnum<IdAttribut,EAID>::Position,
                   Type = PositionEnum<TypeAttribut,EAID>::Position,
                   Version = PositionEnum<VersionAttribut,EAID>::Position,
                   NbrAtt = EAID::NbrAtt};

    using EntityIDs<IDM,TypeAttribut,VersionAttribut>::EntityIDs;
    using EAID::setType;
    using EAID::setVersion;
    BASE_ENTITY(TypeVersionEntity)

    //! Constructeur à partir des valeurs attributs.
    TypeVersionEntity(idt type, int version, idt id = 0)
        : EAID(id) {
        setType(type);
        setVersion(version);
    }
};

template<entidt IDM > TypeVersionEntity<IDM>::~TypeVersionEntity() {}
}

namespace entityMPS {
/*! \ingroup groupeEntity
 * \brief Représentation un changment de version de la base de donnée.
 */
class VersionBdd : public EntityIDs<infoEntity::VersionBddId,amps::DateTimeCurrentAttribut,
                                                             amps::NumAttribut,
                                                             amps::TypeIdtAttribut> {
protected:
    template<class T> using PositionEnum = PositionEnum<T,VersionBdd>;
public:
    using EAID = EntityIDs<infoEntity::VersionBddId,amps::DateTimeCurrentAttribut,
                                                    amps::NumAttribut,
                                                    amps::TypeIdtAttribut>;
    //! Positions des attributs.
    enum Position:post {Id = PositionEnum<IdAttribut>::Position,
                       DateTime = PositionEnum<DateTimeCurrentAttribut>::Position,
                       Num = PositionEnum<NumAttribut>::Position,
                       Type = PositionEnum<TypeIdtAttribut>::Position,
                       NbrAtt = EAID::NbrAtt};

    using EAID::EntityID;
    BASE_ENTITY(VersionBdd)

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    VersionBdd(int num, idt type, idt id = 0)
        : EAID(id) {
        setNum(num);
        setType(type);
    }

    //! Constructeur à partir des valeurs attributs.
    VersionBdd(const QDateTime & dt, int num, idt type, idt id = 0)
        : VersionBdd(num, type, id)
        {setDateTime(dt);}
};
}
#endif // ENTITYDIVERS
