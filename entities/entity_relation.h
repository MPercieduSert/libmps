/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/08/2016
 */
#ifndef ENTITY_RELATION
#define ENTITY_RELATION

#include "entity_divers.h"

//! \ingroup groupe_macro_entity
//! Macro implémentant le début d'une classe ayant 2 clés.
#define ENTITY_ALIAS_DEBUT(ENTITY,TYPE,IDT) \
    /*! \ingroup groupe_entity \brief Représentation de l'entité ENTITY.*/ \
    class ENTITY : public mps::entities_base::TYPE##_entity<IDT> \
    {public: \
    using mere = mps::entities_base::TYPE##_entity<IDT>; \
    using mps::entities_base::TYPE##_entity<IDT>::TYPE##_entity; \
    using mere::operator ==; \
    BASE_ENTITY(ENTITY)

//! \ingroup groupe_macro_entity
//! Macro implémentant une classe ayant 2 clés.
#define RELATION_ENTITY(ENTITY,TYPE,IDT,ID1,id1,ID2,id2) \
    ENTITY_ALIAS_DEBUT(ENTITY,TYPE,IDT)\
    ENUM_##TYPE(ID1,ID2) \
    ALIAS_CLE(id1,1)\
    ALIAS_CLE(id2,2)};

//! \ingroup groupe_macro_entity
//! Macro implémentant une classe ayant 2 clés dont la seconde négative.
#define RELATION_ENTITY_NEG(ENTITY,TYPE,IDT,ID1,id1,ID2,id2) \
    ENTITY_ALIAS_DEBUT(ENTITY,TYPE,IDT)\
    ENUM_##TYPE(ID1,ID2) \
    ALIAS_CLE(id1,1)\
    ALIAS_CLE_NEG(id2,2)};

//! \ingroup groupe_macro_entity
//! Macro implémentant une classe ayant 2 clés négatives.
#define RELATION_ENTITY_NEG_NEG(ENTITY,TYPE,IDT,ID1,id1,ID2,id2) \
    ENTITY_ALIAS_DEBUT(ENTITY,TYPE,IDT)\
    ENUM_##TYPE(ID1,ID2) \
    ALIAS_CLE_NEG(id1,1)\
    ALIAS_CLE_NEG(id2,2)};

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une relation simple.
#define ENUM_relation(ID1,ID2) /*! \brief positions des attributs */ \
    enum position:post {Id, Id_1 = mere::Id_1, Id_2 = mere::Id_2, Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1, Id_ ## ID2 = Id_2};

namespace mps {
namespace entities_base {
/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant pour attribut deux clés.
 */
template<entidt IDM > class relation_entity : public entity_id<IDM,relation_attribut> {
public:
    using eaid = entity_id<IDM,relation_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<id_1_attribut,eaid>::Position,
                   Id_2 = position_enum<id_2_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_id<IDM,relation_attribut>::entity_id;
    using eaid::set_id_1;
    using eaid::set_id_2;
    BASE_ENTITY(relation_entity)

    //! Constructeur à partir des valeurs attributs.
    relation_entity(idt id_1, idt id_2, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_2(id_2);
    }
};

template<entidt IDM > relation_entity<IDM>::~relation_entity() {}

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une relation simple.
#define ENUM_relation_exact_one_not_null(ID1,ID2) ENUM_(ID1,ID2)

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant pour attribut deux clés dont une seule est non nulle et une troisième.
 */
template<entidt IDM > class relation_exact_one_not_null_entity : public entity_id<IDM,relation_exact_one_not_null_attribut> {
public:
    using eaid = entity_id<IDM,relation_exact_one_not_null_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<id_1_attribut,eaid>::Position,
                   Id_2 = position_enum<id_2_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_id<IDM,relation_exact_one_not_null_attribut>::entity_id;
    using eaid::set_id_1;
    using eaid::set_id_2;
    BASE_ENTITY(relation_exact_one_not_null_entity)

    //! Constructeur à partir des valeurs attributs.
    relation_exact_one_not_null_entity(idt id_1, idt id_2, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_2(id_2);
    }
};

template<entidt IDM > relation_exact_one_not_null_entity<IDM>::~relation_exact_one_not_null_entity() {}

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une relation simple.
#define ENUM_relation_cible_temp(ID1,ID2) /*! \brief positions des attributs */ \
    enum position:post {Id, Id_1 = mere::Id_1, Id_2 = mere::Id_2, Id_Cible = mere::Id_Cible, Cible = mere::Cible,\
    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1, Id_ ## ID2 = Id_2};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant pour attribut deux clés.
 */
template<entidt IDM , class Id_1_Att, class Id_2_Att> class relation_cible_entity_temp
        : public entity_ids<IDM,Id_1_Att,Id_2_Att,id_cible_attribut,cible_attribut> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using Id_2_Trans = typename Id_2_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,Id_2_Att,id_cible_attribut,cible_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<Id_1_Att,eaid>::Position,
                   Id_2 = position_enum<Id_2_Att,eaid>::Position,
                   Id_Cible = position_enum<id_cible_attribut,eaid>::Position,
                   Cible = position_enum<cible_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,Id_2_Att,id_cible_attribut,cible_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_id_2;
    using eaid::set_id_cible;
    using eaid::set_cible;
    BASE_ENTITY(relation_cible_entity_temp)

    //! Constructeur à partir des valeurs attributs.
    relation_cible_entity_temp(Id_1_Trans id_1, Id_2_Trans id_2, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_2(id_2);
    }

    //! Constructeur à partir des valeurs attributs.
    relation_cible_entity_temp(Id_1_Trans id_1, Id_2_Trans id_2, idt id_cible, int cible, idt id = 0)
        : relation_cible_entity_temp(id_1,id_2,id) {
        set_id_cible(id_cible);
        set_cible(cible);
    }
};

template<entidt IDM , class Id_1_Att, class Id_2_Att> relation_cible_entity_temp<IDM,Id_1_Att,Id_2_Att>
        ::~relation_cible_entity_temp() {}

template<entidt IDM > using relation_cible_entity
        = relation_cible_entity_temp<IDM,id_1_attribut,id_2_attribut>;
#define ENUM_relation_cible(ID1,ID2) ENUM_relation_cible_temp(ID1,ID2)

template<entidt IDM > using relation_cible_null_entity
        = relation_cible_entity_temp<IDM,id_1_null_attribut,id_2_attribut>;
#define ENUM_relation_cible_null(ID1,ID2) ENUM_relation_cible_temp(ID1,ID2)

template<entidt IDM > using relation_cible_neg_entity
        = relation_cible_entity_temp<IDM,id_1_neg_attribut,id_2_attribut>;
#define ENUM_relation_cible_neg(ID1,ID2) ENUM_relation_cible_temp(ID1,ID2)

template<entidt IDM > using relation_cible_null_null_entity
        = relation_cible_entity_temp<IDM,id_1_null_attribut,id_2_null_attribut>;
#define ENUM_relation_cible_null(ID1,ID2) ENUM_relation_cible_temp(ID1,ID2)

template<entidt IDM > using relation_cible_neg_null_entity
        = relation_cible_entity_temp<IDM,id_1_neg_attribut,id_2_null_attribut>;
#define ENUM_relation_cible_neg_null(ID1,ID2) ENUM_relation_cible_temp(ID1,ID2)

template<entidt IDM > using relation_cible_neg_neg_entity
        = relation_cible_entity_temp<IDM,id_1_neg_attribut,id_2_neg_attribut>;
#define ENUM_relation_cible_neg_neg(ID1,ID2) ENUM_relation_cible_temp(ID1,ID2)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une relation simple.
#define ENUM_relation_cible_att_code_temp(ID1,ID2) /*! \brief positions des attributs */ \
    enum position:post {Id, Id_1 = mere::Id_1, Id_2 = mere::Id_2, Cible = mere::Cible, Code = mere::Code,\
    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1, Id_ ## ID2 = Id_2};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant pour attribut deux clés.
 */
template<entidt IDM , class Id_1_Att, class Id_2_Att> class relation_cible_att_code_entity_temp
        : public entity_ids<IDM,Id_1_Att,Id_2_Att,cible_attribut,code_attribut> {
public:
    using Id_1_Trans = typename Id_1_Att::att_trans;
    using Id_2_Trans = typename Id_2_Att::att_trans;
    using eaid = entity_ids<IDM,Id_1_Att,Id_2_Att,cible_attribut,code_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<Id_1_Att,eaid>::Position,
                   Id_2 = position_enum<Id_2_Att,eaid>::Position,
                   Cible = position_enum<cible_attribut,eaid>::Position,
                   Code = position_enum<code_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,Id_1_Att,Id_2_Att,cible_attribut,code_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_id_2;
    using eaid::set_cible;
    using eaid::set_code;
    BASE_ENTITY(relation_cible_att_code_entity_temp)

    //! Constructeur à partir des valeurs attributs.
    relation_cible_att_code_entity_temp(Id_1_Trans id_1, Id_2_Trans id_2, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_2(id_2);
    }

    //! Constructeur à partir des valeurs attributs.
    relation_cible_att_code_entity_temp(Id_1_Trans id_1, Id_2_Trans id_2, int cible, flag code, idt id = 0)
        : relation_cible_att_code_entity_temp(id_1,id_2,id) {
        set_cible(cible);
        set_code(code);
    }
};

template<entidt IDM , class Id_1_Att, class Id_2_Att> relation_cible_att_code_entity_temp<IDM,Id_1_Att,Id_2_Att>
        ::~relation_cible_att_code_entity_temp() {}

template<entidt IDM > using relation_cible_att_code_entity
        = relation_cible_att_code_entity_temp<IDM,id_1_attribut,id_2_attribut>;
#define ENUM_relation_cible_att_code(ID1,ID2) ENUM_relation_cible_att_code_temp(ID1,ID2)

template<entidt IDM > using relation_cible_att_code_null_entity
        = relation_cible_att_code_entity_temp<IDM,id_1_null_attribut,id_2_attribut>;
#define ENUM_relation_cible_att_code_null(ID1,ID2) ENUM_relation_cible_att_code_temp(ID1,ID2)

template<entidt IDM > using relation_cible_att_code_negentity
        = relation_cible_att_code_entity_temp<IDM,id_1_neg_attribut,id_2_attribut>;
#define ENUM_relation_cible_att_code_neg(ID1,ID2) ENUM_relation_cible_att_code_temp(ID1,ID2)

template<entidt IDM > using relation_cible_att_code_null_null_entity
        = relation_cible_att_code_entity_temp<IDM,id_1_null_attribut,id_2_null_attribut>;
#define ENUM_relation_cible_att_code_null(ID1,ID2) ENUM_relation_cible_att_code_temp(ID1,ID2)

template<entidt IDM > using relation_cible_att_code_null_negentity
        = relation_cible_att_code_entity_temp<IDM,id_1_neg_attribut,id_2_null_attribut>;
#define ENUM_relation_cible_att_code_null_neg(ID1,ID2) ENUM_relation_cible_att_code_temp(ID1,ID2)

template<entidt IDM > using relation_cible_att_code_neg_negentity
        = relation_cible_att_code_entity_temp<IDM,id_1_neg_attribut,id_2_neg_attribut>;
#define ENUM_relation_cible_att_code_neg_neg(ID1,ID2) ENUM_relation_cible_att_code_temp(ID1,ID2)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une relation avec un attribut date_time.
#define ENUM_relation_date_time_temp(ID1,ID2) /*! \brief positions des attributs */ \
    enum position:post {Id, Id_1 = mere::Id_1, Id_2 = mere::Id_2, Date_Time = mere::Date_Time, \
    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1, Id_ ## ID2 = Id_2};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant pour attribut deux clés et une date_time.
 */
template<entidt IDM , class Date_Time_Att> class relation_date_time_entity_temp
        : public entity_ids<IDM,relation_attribut,Date_Time_Att> {
public:
    using eaid = entity_ids<IDM,relation_attribut,Date_Time_Att>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<id_1_attribut,eaid>::Position,
                   Id_2 = position_enum<id_2_attribut,eaid>::Position,
                   Date_Time = position_enum<Date_Time_Att,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,relation_attribut,Date_Time_Att>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_id_2;
    using eaid::set_date_time;
    BASE_ENTITY(relation_date_time_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    relation_date_time_entity_temp(idt id_1, idt id_2, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_2(id_2);
    }

    //! Constructeur à partir des valeurs attributs.
    relation_date_time_entity_temp(idt id_1, idt id_2, const QDateTime &date_time, idt id = 0)
        : relation_date_time_entity_temp(id_1, id_2, id)
        {set_date_time(date_time);}
};

template<entidt IDM , class Date_Time_Att> relation_date_time_entity_temp<IDM,Date_Time_Att>::~relation_date_time_entity_temp() {}

template<entidt IDM > using relation_date_time_current_entity = relation_date_time_entity_temp<IDM,date_time_current_attribut>;
#define ENUM_relation_date_time_current(ID1,ID2) ENUM_relation_date_time_temp(ID1,ID2)

template<entidt IDM > using relation_date_time_valide_entity = relation_date_time_entity_temp<IDM,date_time_valide_attribut>;
#define ENUM_relation_date_time_valide(ID1,ID2) ENUM_relation_date_time_temp(ID1,ID2)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une relation avec un attribut num.
#define ENUM_relation_num(ID1,ID2) /*! \brief positions des attributs */ \
    enum position:post {Id, Id_1 = mere::Id_1, Id_2 = mere::Id_2, Num = mere::Num, \
    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1, Id_ ## ID2 = Id_2};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant pour attribut deux clés et un numero.
 */
template<entidt IDM > class relation_num_entity : public entity_ids<IDM,relation_attribut,num_attribut> {
public:
    using eaid = entity_ids<IDM,relation_attribut,num_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<id_1_attribut,eaid>::Position,
                   Id_2 = position_enum<id_2_attribut,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,relation_attribut,num_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_id_2;
    using eaid::set_num;
    BASE_ENTITY(relation_num_entity)

    //! Constructeur à partir des valeurs attributs.
    relation_num_entity(idt id_1, idt id_2, int num = 0, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_2(id_2);
        set_num(num);
    }
};

template<entidt IDM > relation_num_entity<IDM>::~relation_num_entity() {}

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une relation avec un attribut num et code.
#define ENUM_relation_num_code(ID1,ID2) /*! \brief positions des attributs */ \
    enum position:post {Id, Id_1 = mere::Id_1, Id_2 = mere::Id_2, Num = mere::Num, Code = mere::Code \
    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1, Id_ ## ID2 = Id_2};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant pour attribut deux clés, un numero et un code.
 */
template<entidt IDM > class relation_num_code_entity : public entity_ids<IDM,relation_attribut,num_attribut,code_attribut> {
public:
    using eaid = entity_ids<IDM,relation_attribut,num_attribut,code_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<id_1_attribut,eaid>::Position,
                   Id_2 = position_enum<id_2_attribut,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Code = position_enum<code_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,relation_attribut,num_attribut,code_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_id_2;
    using eaid::set_num;
    using eaid::set_code;
    BASE_ENTITY(relation_num_code_entity)

    //! Constructeur à partir des valeurs attributs.
    relation_num_code_entity(idt id_1, idt id_2, int num = 0, flag code = 0, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_2(id_2);
        set_num(num);
        set_code(code);
    }
};

template<entidt IDM > relation_num_code_entity<IDM>::~relation_num_code_entity() {}

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une relation avec un attribut type.
#define ENUM_relation_type(ID1,ID2) /*! \brief positions des attributs */ \
    enum position:post {Id, Id_1 = mere::Id_1, Id_2 = mere::Id_2, Type = mere::Type, Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1, Id_ ## ID2 = Id_2};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant pour attribut deux clés et un numero.
 */
template<entidt IDM > class relation_type_entity : public entity_ids<IDM,relation_attribut,type_attribut> {
public:
    using eaid = entity_ids<IDM,relation_attribut,type_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<id_1_attribut,eaid>::Position,
                   Id_2 = position_enum<id_2_attribut,eaid>::Position,
                   Type = position_enum<type_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,relation_attribut,type_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_id_2;
    using eaid::set_type;
    BASE_ENTITY(relation_type_entity)

    //! Constructeur à partir des valeurs attributs.
    relation_type_entity(idt id_1, idt id_2, idt type = 0, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_2(id_2);
        set_type(type);
    }
};

template<entidt IDM > relation_type_entity<IDM>::~relation_type_entity() {}

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une relation avec un attribut date_time et num.
#define ENUM_relation_date_time_num_temp(ID1,ID2) /*! \brief positions des attributs */ \
    enum position:post {Id, Id_1 = mere::Id_1, Id_2 = mere::Id_2, Date_Time = mere::Date_Time, Num = mere::Num,\
                    Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1, Id_ ## ID2 = Id_2};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant pour attribut deux clés, une date_time et un numero.
 */
template<entidt IDM , class Date_Time_Att> class relation_date_time_num_entity_temp :
        public entity_ids<IDM,relation_attribut,Date_Time_Att,num_attribut> {
public:
    using eaid = entity_ids<IDM,relation_attribut,Date_Time_Att,num_attribut>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<id_1_attribut,eaid>::Position,
                   Id_2 = position_enum<id_2_attribut,eaid>::Position,
                   Date_Time = position_enum<Date_Time_Att,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,relation_attribut,Date_Time_Att,num_attribut>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_id_2;
    using eaid::set_date_time;
    using eaid::set_num;
    BASE_ENTITY(relation_date_time_num_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    relation_date_time_num_entity_temp(idt id_1, idt id_2, int num = 0, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_2(id_2);
        set_num(num);
    }

    //! Constructeur à partir des valeurs attributs.
    relation_date_time_num_entity_temp(idt id_1, idt id_2, const QDateTime &date_time, int num, idt id = 0)
        : relation_date_time_num_entity_temp(id_1, id_2, num, id)
        {set_date_time(date_time);}
};

template<entidt IDM , class Date_Time_Att> relation_date_time_num_entity_temp<IDM,Date_Time_Att>
        ::~relation_date_time_num_entity_temp() {}

template<entidt IDM > using relation_date_time_current_num_entity
        = relation_date_time_num_entity_temp<IDM,date_time_current_attribut>;
#define ENUM_relation_date_time_current_num(ID1,ID2) ENUM_relation_date_time_num_temp(ID1,ID2)

template<entidt IDM > using relation_date_time_valide_num_entity
        = relation_date_time_num_entity_temp<IDM,date_time_valide_attribut>;
#define ENUM_relation_date_time_valide_num(ID1,ID2) ENUM_relation_date_time_num_temp(ID1,ID2)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une relation avec un attribut date_time, num et Valeur.
#define ENUM_relation_date_time_valeur_temp(ID1,ID2) /*! \brief positions des attributs */ \
    enum position:post {Id, Id_1 = mere::Id_1, Id_2 = mere::Id_2, Date_Time = mere::Date_Time, Valeur = mere::Valeur, \
                Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1, Id_ ## ID2 = Id_2};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant pour attribut deux clés, une date_time et un numero.
 */
template<entidt IDM , class Date_Time_Att,class Valeur_Att> class relation_date_time_valeur_entity_temp :
        public entity_ids<IDM,relation_attribut,Date_Time_Att,Valeur_Att> {
public:
    using val_trans = typename Valeur_Att::att_trans;
    using eaid = entity_ids<IDM,relation_attribut,Date_Time_Att,Valeur_Att>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<id_1_attribut,eaid>::Position,
                   Id_2 = position_enum<id_2_attribut,eaid>::Position,
                   Date_Time = position_enum<Date_Time_Att,eaid>::Position,
                   Valeur = position_enum<Valeur_Att,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,relation_attribut,Date_Time_Att,Valeur_Att>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_id_2;
    using eaid::set_date_time;
    using eaid::set_valeur;
    BASE_ENTITY(relation_date_time_valeur_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    relation_date_time_valeur_entity_temp(idt id_1, idt id_2, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_2(id_2);
    }

    //! Constructeur à partir des valeurs attributs.
    relation_date_time_valeur_entity_temp(idt id_1, idt id_2, val_trans valeur, idt id)
        : relation_date_time_valeur_entity_temp(id_1, id_2, id)
        {set_valeur(valeur);}

    //! Constructeur à partir des valeurs attributs.
    relation_date_time_valeur_entity_temp(idt id_1, idt id_2, const QDateTime &date_time, val_trans valeur, idt id = 0)
        : relation_date_time_valeur_entity_temp(id_1, id_2, valeur, id)
        {set_date_time(date_time);}
};

template<entidt IDM , class Date_Time_Att,class Valeur_Att>
        relation_date_time_valeur_entity_temp<IDM, Date_Time_Att, Valeur_Att>::~relation_date_time_valeur_entity_temp() {}

template<entidt IDM > using relation_date_time_current_valeur_double_entity
        = relation_date_time_valeur_entity_temp<IDM,date_time_current_attribut,valeur_double_attribut>;
#define ENUM_relation_date_time_current_valeur_double(ID1,ID2) ENUM_relation_date_time_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_date_time_current_valeur_int_entity
        = relation_date_time_valeur_entity_temp<IDM,date_time_current_attribut,valeur_int_attribut>;
#define ENUM_relation_date_time_current_valeur_int(ID1,ID2) ENUM_relation_date_time_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_date_time_current_valeur_variant_entity
        = relation_date_time_valeur_entity_temp<IDM,date_time_current_attribut,valeur_variant_attribut>;
#define ENUM_relation_date_time_current_valeur_variant(ID1,ID2) ENUM_relation_date_time_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_date_time_valide_valeur_double_entity
        = relation_date_time_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_double_attribut>;
#define ENUM_relation_date_time_valide_valeur_double(ID1,ID2) ENUM_relation_date_time_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_date_time_valide_valeur_int_entity
        = relation_date_time_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_int_attribut>;
#define ENUM_relation_date_time_valide_valeur_int(ID1,ID2) ENUM_relation_date_time_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_date_time_valide_valeur_variant_entity
        = relation_date_time_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_variant_attribut>;
#define ENUM_relation_date_time_valide_valeur_variant(ID1,ID2) ENUM_relation_date_time_valeur_temp(ID1,ID2)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une relation avec un attribut date_time, num et Valeur.
#define ENUM_relation_date_time_num_valeur_temp(ID1,ID2) /*! \brief positions des attributs */ \
    enum position:post {Id, Id_1 = mere::Id_1, Id_2 = mere::Id_2, Date_Time = mere::Date_Time, Num = mere::Num, \
    Valeur = mere::Valeur, Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1, Id_ ## ID2 = Id_2};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant pour attribut deux clés, une date_time, un numero et une valeur.
 */
template<entidt IDM , class Date_Time_Att,class Valeur_Att> class relation_date_time_num_valeur_entity_temp :
        public entity_ids<IDM,relation_attribut,Date_Time_Att,num_attribut,Valeur_Att> {
public:
    using val_trans = typename Valeur_Att::att_trans;
    using eaid = entity_ids<IDM,relation_attribut,Date_Time_Att,num_attribut,Valeur_Att>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<id_1_attribut,eaid>::Position,
                   Id_2 = position_enum<id_2_attribut,eaid>::Position,
                   Date_Time = position_enum<Date_Time_Att,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Valeur = position_enum<Valeur_Att,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,relation_attribut,Date_Time_Att,num_attribut,Valeur_Att>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_id_2;
    using eaid::set_date_time;
    using eaid::set_num;
    using eaid::set_valeur;
    BASE_ENTITY(relation_date_time_num_valeur_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    relation_date_time_num_valeur_entity_temp(idt id_1, idt id_2, int num = 0, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_2(id_2);
        set_num(num);
    }

    //! Constructeur à partir des valeurs attributs.
    relation_date_time_num_valeur_entity_temp(idt id_1, idt id_2, int num, val_trans valeur, idt id = 0)
        : relation_date_time_num_valeur_entity_temp(id_1, id_2, num, id)
        {set_valeur(valeur);}

    //! Constructeur à partir des valeurs attributs.
    relation_date_time_num_valeur_entity_temp(idt id_1, idt id_2, const QDateTime &date_time, int num, val_trans valeur, idt id = 0)
        : relation_date_time_num_valeur_entity_temp(id_1, id_2, num, valeur, id)
        {set_date_time(date_time);}
};

template<entidt IDM , class Date_Time_Att,class Valeur_Att>
        relation_date_time_num_valeur_entity_temp<IDM,Date_Time_Att, Valeur_Att>::~relation_date_time_num_valeur_entity_temp() {}

template<entidt IDM > using relation_date_time_current_num_valeur_double_entity
        = relation_date_time_num_valeur_entity_temp<IDM,date_time_current_attribut,valeur_double_attribut>;
#define ENUM_relation_date_time_current_num_valeur_double(ID1,ID2) ENUM_relation_date_time_num_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_date_time_current_num_valeur_int_entity
        = relation_date_time_num_valeur_entity_temp<IDM,date_time_current_attribut,valeur_int_attribut>;
#define ENUM_relation_date_time_current_num_valeur_int(ID1,ID2) ENUM_relation_date_time_num_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_date_time_current_num_valeur_variant_entity
        = relation_date_time_num_valeur_entity_temp<IDM,date_time_current_attribut,valeur_variant_attribut>;
#define ENUM_relation_date_time_current_num_valeur_variant(ID1,ID2) ENUM_relation_date_time_num_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_date_time_valide_num_valeur_double_entity
        = relation_date_time_num_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_double_attribut>;
#define ENUM_relation_date_time_valide_num_valeur_double(ID1,ID2) ENUM_relation_date_time_num_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_date_time_valide_num_valeur_int_entity
        = relation_date_time_num_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_int_attribut>;
#define ENUM_relation_date_time_valide_num_valeur_int(ID1,ID2) ENUM_relation_date_time_num_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_date_time_valide_num_valeur_variant_entity
        = relation_date_time_num_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_variant_attribut>;
#define ENUM_relation_date_time_valide_num_valeur_variant(ID1,ID2) ENUM_relation_date_time_num_valeur_temp(ID1,ID2)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une relation avec un attribut code, date_time et Valeur.
#define ENUM_relation_code_date_time_valeur_temp(ID1,ID2) /*! \brief positions des attributs */ \
    enum position:post {Id, Id_1 = mere::Id_1, Id_2 = mere::Id_2, Code = mere::Code, Date_Time = mere::Date_Time,\
                        Valeur = mere::Valeur, Nbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1, Id_ ## ID2 = Id_2};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant pour attribut deux clés, un code, une date_time et une valeur.
 */
template<entidt IDM , class Date_Time_Att,class Valeur_Att> class relation_code_date_time_valeur_entity_temp :
                public entity_ids<IDM,relation_attribut,code_attribut,Date_Time_Att,Valeur_Att> {
public:
    using val_trans = typename Valeur_Att::att_trans;
    using eaid = entity_ids<IDM,relation_attribut,code_attribut,Date_Time_Att,Valeur_Att>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<id_1_attribut,eaid>::Position,
                   Id_2 = position_enum<id_2_attribut,eaid>::Position,
                   Code = position_enum<code_attribut,eaid>::Position,
                   Date_Time = position_enum<Date_Time_Att,eaid>::Position,
                   Valeur = position_enum<Valeur_Att,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,relation_attribut,code_attribut,Date_Time_Att,Valeur_Att>
                            ::entity_ids;
    using eaid::set_id_1;
    using eaid::set_id_2;
    using eaid::set_code;
    using eaid::set_date_time;
    using eaid::set_valeur;
    BASE_ENTITY(relation_code_date_time_valeur_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    relation_code_date_time_valeur_entity_temp(idt id_1, idt id_2, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_2(id_2);
    }

    //! Constructeur à partir des valeurs attributs.
    relation_code_date_time_valeur_entity_temp(idt id_1, idt id_2, val_trans valeur, idt id = 0)
        : relation_code_date_time_valeur_entity_temp(id_1, id_2, id)
    {set_valeur(valeur);}

    //! Constructeur à partir des valeurs attributs.
    relation_code_date_time_valeur_entity_temp(idt id_1, idt id_2, flag code, const QDateTime &date_time, val_trans valeur, idt id = 0)
        : relation_code_date_time_valeur_entity_temp(id_1, id_2, id) {
        set_code(code);
        set_date_time(date_time);
        set_valeur(valeur);
    }
};

template<entidt IDM , class Date_Time_Att,class Valeur_Att>
        relation_code_date_time_valeur_entity_temp<IDM,Date_Time_Att, Valeur_Att>
        ::~relation_code_date_time_valeur_entity_temp() {}

template<entidt IDM > using relation_code_date_time_current_valeur_double_entity
        = relation_code_date_time_valeur_entity_temp<IDM,date_time_current_attribut,valeur_double_attribut>;
#define ENUM_relation_code_date_time_current_valeur_double(ID1,ID2) ENUM_relation_code_date_time_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_code_date_time_current_valeur_int_entity
        = relation_code_date_time_valeur_entity_temp<IDM,date_time_current_attribut,valeur_int_attribut>;
#define ENUM_relation_code_date_time_current_valeur_int(ID1,ID2) ENUM_relation_code_date_time_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_code_date_time_current_valeur_variant_entity
        = relation_code_date_time_valeur_entity_temp<IDM,date_time_current_attribut,valeur_variant_attribut>;
#define ENUM_relation_code_date_time_current_valeur_variant(ID1,ID2) ENUM_relation_code_date_time_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_code_date_time_valide_valeur_double_entity
        = relation_code_date_time_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_double_attribut>;
#define ENUM_relation_code_date_time_valide_valeur_double(ID1,ID2) ENUM_relation_code_date_time_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_code_date_time_valide_valeur_int_entity
        = relation_code_date_time_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_int_attribut>;
#define ENUM_relation_code_date_time_valide_valeur_int(ID1,ID2) ENUM_relation_code_date_time_valeur_temp(ID1,ID2)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une relation avec un attribut code, date_time et Valeur.
#define ENUM_relation_date_time_saisie_valeur_temp(ID1,ID2) /*! \brief positions des attributs */ \
    enum position:post {Id, Id_1 = mere::Id_1, Id_2 = mere::Id_2, Date_Time = mere::Date_Time, Saisie = mere::Saisie,\
                        Valeur = mere::Valeur,nNbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1, Id_ ## ID2 = Id_2};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant pour attribut deux clés, un code, une date_time et une valeur.
 */
template<entidt IDM , class Date_Time_Att,class Valeur_Att> class relation_date_time_saisie_valeur_entity_temp :
                public entity_ids<IDM,relation_attribut,Date_Time_Att,saisie_attribut,Valeur_Att> {
public:
    using val_trans = typename Valeur_Att::att_trans;
    using eaid = entity_ids<IDM,relation_attribut,Date_Time_Att,saisie_attribut,Valeur_Att>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<id_1_attribut,eaid>::Position,
                   Id_2 = position_enum<id_2_attribut,eaid>::Position,
                   Date_Time = position_enum<Date_Time_Att,eaid>::Position,
                   Saisie = position_enum<saisie_attribut,eaid>::Position,
                   Valeur = position_enum<Valeur_Att,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,relation_attribut,Date_Time_Att,saisie_attribut,Valeur_Att>
                            ::entity_ids;
    using eaid::set_id_1;
    using eaid::set_id_2;
    using eaid::set_date_time;
    using eaid::set_saisie;
    using eaid::set_valeur;
    BASE_ENTITY(relation_date_time_saisie_valeur_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    relation_date_time_saisie_valeur_entity_temp(idt id_1, idt id_2, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_2(id_2);
    }

    //! Constructeur à partir des valeurs attributs.
    relation_date_time_saisie_valeur_entity_temp(idt id_1, idt id_2, val_trans valeur, idt id = 0)
        : relation_date_time_saisie_valeur_entity_temp(id_1, id_2, id)
    {set_valeur(valeur);}

    //! Constructeur à partir des valeurs attributs.
    relation_date_time_saisie_valeur_entity_temp(idt id_1, idt id_2, const QDateTime &date_time, flag saisie, val_trans valeur, idt id = 0)
        : relation_date_time_saisie_valeur_entity_temp(id_1, id_2, id) {
        set_date_time(date_time);
        set_saisie(saisie);
        set_valeur(valeur);
    }
};

template<entidt IDM , class Date_Time_Att,class Valeur_Att>
        relation_date_time_saisie_valeur_entity_temp<IDM,Date_Time_Att, Valeur_Att>
        ::~relation_date_time_saisie_valeur_entity_temp() {}

template<entidt IDM > using relation_code_date_time_valide_valeur_variant_entity
        = relation_date_time_saisie_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_variant_attribut>;
#define ENUM_relation_code_date_time_valide_valeur_variant(ID1,ID2) ENUM_relation_date_time_saisie_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_date_time_current_saisie_valeur_double_entity
        = relation_date_time_saisie_valeur_entity_temp<IDM,date_time_current_attribut,valeur_double_attribut>;
#define ENUM_relation_date_time_current_saisie_valeur_double(ID1,ID2) ENUM_relation_date_time_saisie_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_date_time_current_saisie_valeur_int_entity
        = relation_date_time_saisie_valeur_entity_temp<IDM,date_time_current_attribut,valeur_int_attribut>;
#define ENUM_relation_date_time_current_saisie_valeur_int(ID1,ID2) ENUM_relation_date_time_saisie_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_code_date_time_current_saisie_valeur_variant_entity
        = relation_date_time_saisie_valeur_entity_temp<IDM,date_time_current_attribut,valeur_variant_attribut>;
#define ENUM_relation_date_time_current_saisie_valeur_variant(ID1,ID2) ENUM_relation_date_time_saisie_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_code_date_time_valide_saisie_valeur_double_entity
        = relation_date_time_saisie_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_double_attribut>;
#define ENUM_relation_date_time_valide_saisie_valeur_double(ID1,ID2) ENUM_relation_date_time_saisie_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_code_date_time_valide_saisie_valeur_int_entity
        = relation_date_time_saisie_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_int_attribut>;
#define ENUM_relation_date_time_valide_saisie_valeur_int(ID1,ID2) ENUM_relation_date_time_saisie_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_code_date_time_valide_saisie_valeur_variant_entity
        = relation_date_time_saisie_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_variant_attribut>;
#define ENUM_relation_date_time_valide_saisie_valeur_variant(ID1,ID2) ENUM_relation_date_time_saisie_valeur_temp(ID1,ID2)

//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une relation avec un attribut code, date_time, num et valeur.
#define ENUM_relation_code_date_time_num_valeur_temp(ID1,ID2) /*! \brief positions des attributs */ \
    enum position:post {Id, Id_1 = mere::Id_1, Id_2 = mere::Id_2, Code = mere::Code, Date_Time = mere::Date_Time, Num = mere::Num,\
                    Valeur = mere::Valeur,nNbr_Att = mere::Nbr_Att, Id_ ## ID1 = Id_1, Id_ ## ID2 = Id_2};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant pour attribut deux clés, un code, une date_time, un numero et une valeur.
 */
template<entidt IDM , class Date_Time_Att,class Valeur_Att, class CodeAtt> class relation_code_date_time_num_valeur_entity_temp :
            public entity_ids<IDM,relation_attribut,CodeAtt,Date_Time_Att,num_attribut,Valeur_Att> {
public:
    using val_trans = typename Valeur_Att::att_trans;
    using eaid = entity_ids<IDM,relation_attribut,CodeAtt,Date_Time_Att,num_attribut,Valeur_Att>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                    Id_1 = position_enum<id_1_attribut,eaid>::Position,
                    Id_2 = position_enum<id_2_attribut,eaid>::Position,
                    Code = position_enum<code_attribut,eaid>::Position,
                    Date_Time = position_enum<Date_Time_Att,eaid>::Position,
                    Num = position_enum<num_attribut,eaid>::Position,
                    Valeur = position_enum<Valeur_Att,eaid>::Position,
                    Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,relation_attribut,CodeAtt,Date_Time_Att,num_attribut,Valeur_Att>
                        ::entity_ids;
    using eaid::set_code;
    using eaid::set_id_1;
    using eaid::set_id_2;
    using eaid::set_date_time;
    using eaid::set_num;
    using eaid::set_valeur;
    BASE_ENTITY(relation_code_date_time_num_valeur_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    relation_code_date_time_num_valeur_entity_temp(idt id_1, idt id_2, int num = 0, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_2(id_2);
        set_num(num);
    }

    //! Constructeur à partir des valeurs attributs.
    relation_code_date_time_num_valeur_entity_temp(idt id_1, idt id_2, int num, val_trans valeur, idt id = 0)
        : relation_code_date_time_num_valeur_entity_temp(id_1, id_2, num, id)
        {set_valeur(valeur);}

    //! Constructeur à partir des valeurs attributs.
    relation_code_date_time_num_valeur_entity_temp(idt id_1, idt id_2, flag code, const QDateTime &date_time, int num, val_trans valeur, idt id = 0)
        : relation_code_date_time_num_valeur_entity_temp(id_1, id_2, num, id) {
        set_code(code);
        set_date_time(date_time);
        set_valeur(valeur);
    }
};

template<entidt IDM , class Date_Time_Att,class Valeur_Att, class CodeAtt>
        relation_code_date_time_num_valeur_entity_temp<IDM,Date_Time_Att, Valeur_Att, CodeAtt>::~relation_code_date_time_num_valeur_entity_temp() {}

template<entidt IDM > using relation_code_date_time_current_num_valeur_double_entity
        = relation_code_date_time_num_valeur_entity_temp<IDM,date_time_current_attribut,valeur_double_attribut,code_attribut>;
#define ENUM_relation_code_date_time_current_num_valeur_double(ID1,ID2) ENUM_relation_code_date_time_num_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_code_date_time_current_num_valeur_int_entity
        = relation_code_date_time_num_valeur_entity_temp<IDM,date_time_current_attribut,valeur_int_attribut,code_attribut>;
#define ENUM_relation_code_date_time_current_num_valeur_int(ID1,ID2) ENUM_relation_code_date_time_num_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_code_date_time_current_num_valeur_variant_entity
        = relation_code_date_time_num_valeur_entity_temp<IDM,date_time_current_attribut,valeur_variant_attribut,code_attribut>;
#define ENUM_relation_code_date_time_current_num_valeur_variant(ID1,ID2) ENUM_relation_code_date_time_num_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_code_date_time_valide_num_valeur_double_entity
        = relation_code_date_time_num_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_double_attribut,code_attribut>;
#define ENUM_relation_code_date_time_valide_num_valeur_double(ID1,ID2) ENUM_relation_code_date_time_num_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_code_date_time_valide_num_valeur_int_entity
        = relation_code_date_time_num_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_int_attribut,code_attribut>;
#define ENUM_relation_code_date_time_valide_num_valeur_int(ID1,ID2) ENUM_relation_code_date_time_num_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_code_date_time_valide_num_valeur_variant_entity
        = relation_code_date_time_num_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_variant_attribut,saisie_attribut>;
#define ENUM_relation_code_date_time_valide_num_valeur_variant(ID1,ID2) ENUM_relation_code_date_time_num_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_date_time_current_num_saisie_valeur_double_entity
        = relation_code_date_time_num_valeur_entity_temp<IDM,date_time_current_attribut,valeur_double_attribut,saisie_attribut>;
#define ENUM_relation_date_time_current_num_saisie_valeur_double(ID1,ID2) ENUM_relation_code_date_time_num_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_date_time_current_num_saisie_valeur_int_entity
        = relation_code_date_time_num_valeur_entity_temp<IDM,date_time_current_attribut,valeur_int_attribut,saisie_attribut>;
#define ENUM_relation_date_time_current_num_saisie_valeur_int(ID1,ID2) ENUM_relation_code_date_time_num_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_code_date_time_current_num_saisie_valeur_variant_entity
        = relation_code_date_time_num_valeur_entity_temp<IDM,date_time_current_attribut,valeur_variant_attribut,saisie_attribut>;
#define ENUM_relation_date_time_current_num_saisie_valeur_variant(ID1,ID2) ENUM_relation_code_date_time_num_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_code_date_time_valide_num_saisie_valeur_double_entity
        = relation_code_date_time_num_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_double_attribut,saisie_attribut>;
#define ENUM_relation_date_time_valide_num_saisie_valeur_double(ID1,ID2) ENUM_relation_code_date_time_num_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_code_date_time_valide_num_saisie_valeur_int_entity
        = relation_code_date_time_num_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_int_attribut,saisie_attribut>;
#define ENUM_relation_date_time_valide_num_saisie_valeur_int(ID1,ID2) ENUM_relation_code_date_time_num_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_code_date_time_valide_num_saisie_valeur_variant_entity
        = relation_code_date_time_num_valeur_entity_temp<IDM,date_time_valide_attribut,valeur_variant_attribut,saisie_attribut>;
#define ENUM_relation_date_time_valide_num_saisie_valeur_variant(ID1,ID2) ENUM_relation_code_date_time_num_valeur_temp(ID1,ID2)


//! \ingroup groupe_macro_entity
//! Macro définissant les positions des attributs pour une relation avec un attribut date_time, num et Valeur.
#define ENUM_relation_num_type_valeur_temp(ID1,ID2) /*! \brief positions des attributs */ \
    enum position:post {Id, Id_1 = mere::Id_1, Id_2 = mere::Id_2, Num = mere::Num, Type = mere::Type, Valeur = mere::Valeur, Nbr_Att = mere::Nbr_Att,\
            Id_ ## ID1 = Id_1, Id_ ## ID2 = Id_2};

/*! \ingroup groupe_base_entity
 *\brief Classe de base des entités ayant pour attribut deux clés, un numero, un type et une valeur.
 */
template<entidt IDM , class Valeur_Att> class relation_num_type_valeur_entity_temp :
            public entity_ids<IDM,relation_attribut,num_attribut,type_attribut,Valeur_Att> {
public:
    using val_trans = typename Valeur_Att::att_trans;
    using eaid = entity_ids<IDM,relation_attribut,num_attribut,type_attribut,Valeur_Att>;
    //! positions des attributs.
    enum position:post {Id = position_enum<id_attribut,eaid>::Position,
                   Id_1 = position_enum<id_1_attribut,eaid>::Position,
                   Id_2 = position_enum<id_2_attribut,eaid>::Position,
                   Num = position_enum<num_attribut,eaid>::Position,
                   Type = position_enum<type_attribut,eaid>::Position,
                   Valeur = position_enum<Valeur_Att,eaid>::Position,
                   Nbr_Att = eaid::Nbr_Att};

    using entity_ids<IDM,relation_attribut,num_attribut,type_attribut,Valeur_Att>::entity_ids;
    using eaid::set_id_1;
    using eaid::set_id_2;
    using eaid::set_num;
    using eaid::set_type;
    using eaid::set_valeur;
    BASE_ENTITY(relation_num_type_valeur_entity_temp)

    //! Constructeur à partir d'un jeux de valeurs attributs unique.
    relation_num_type_valeur_entity_temp(idt id_1, idt id_2, int num = 0, idt type =0, idt id = 0)
        : eaid(id) {
        set_id_1(id_1);
        set_id_2(id_2);
        set_num(num);
        set_type(type);
    }

    //! Constructeur à partir des valeurs attributs.
    relation_num_type_valeur_entity_temp(idt id_1, idt id_2, int num, idt type, val_trans valeur, idt id = 0)
        : relation_num_type_valeur_entity_temp(id_1, id_2, num, type, id)
    {set_valeur(valeur);}
};

template<entidt IDM , class Valeur_Att> relation_num_type_valeur_entity_temp<IDM,Valeur_Att>
        ::~relation_num_type_valeur_entity_temp() {}

template<entidt IDM > using relation_num_type_valeur_double_entity
        = relation_num_type_valeur_entity_temp<IDM,valeur_double_attribut>;
#define ENUM_relation_num_type_valeur_double(ID1,ID2) ENUM_relation_num_type_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_num_type_valeur_int_entity
        = relation_num_type_valeur_entity_temp<IDM,valeur_int_attribut>;
#define ENUM_relation_num_type_valeur_int(ID1,ID2) ENUM_relation_num_type_valeur_temp(ID1,ID2)

template<entidt IDM > using relation_num_type_valeur_variant_entity
        = relation_num_type_valeur_entity_temp<IDM,valeur_variant_attribut>;
#define ENUM_relation_num_type_valeur_variant(ID1,ID2) ENUM_relation_num_type_valeur_temp(ID1,ID2)
}}
#endif // ENTITY_RELATION
