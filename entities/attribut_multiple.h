/*Auteur: PERCIE DU SERT Maxime
 *Date: 23/07/2018
 */
#ifndef ATTRIBUT_MULTIPLE_H
#define ATTRIBUT_MULTIPLE_H

#include "attribut_simple.h"

/*! \defgroup groupe_entity Entités
 *\brief Ensemble de classes représentant les entités de la base de donnée.
 */
namespace mps {
/*! \ingroup groupe_entity
 *\brief Espace de nom des entités.
 */
namespace entities {
using no_attribut = attribut::no_attribut;

/*! \ingroup groupe_entity
 *\brief Classe template des attributs multiple.
 */
template<class... Attribut> class attributs;

template<class Attribut> class attributs<Attribut> : public Attribut {
public:
    CONSTR_DEFAUT(attributs)
    CONSTR_AFFECT_DEFAUT(attributs)
};

template <class Attribut_Premier, class... Attribut_Suivant> class attributs<Attribut_Premier, Attribut_Suivant...>
        : public Attribut_Premier, public attributs<Attribut_Suivant...> {
public:
    //! Nombre d'attribut de l'entité.
    enum {Nbr_Att = Attribut_Premier::Nbr_Att + attributs<Attribut_Suivant...>::Nbr_Att};
    //!Filliation des attributs
    using att_pre = Attribut_Premier;
    using att_suiv = attributs<Attribut_Suivant...>;
    CONSTR_DEFAUT(attributs)
    CONSTR_AFFECT_DEFAUT(attributs)

    //! Renvoie une chaine de caractère contenant les noms, les validités et les valeurs des attributs.
    QString affiche() const override
        {return Attribut_Premier::affiche().append("\n").append(attributs<Attribut_Suivant...>::affiche());}

    //! Retourne un QVariant contenant la donnée souhaitée ou un QVariant nulle si num est invalide.
    QVariant data(numt num) const
        {return num < Nbr_Att ? data_v(num) : QVariant();}

    //! Teste si l'entité est valide.
    bool is_valid() const override
        {return Attribut_Premier::is_valid() &&attributs<Attribut_Suivant...>::is_valid();}

    //! Retourne le nom de l'attribut donnée souhaitée, pos doit être valide.
    static QString Name(numt pos)
        {return pos < Attribut_Premier::Nbr_Att ? Attribut_Premier::Name(pos)
                                                : attributs<Attribut_Suivant...>::Name(pos - Attribut_Premier::Nbr_Att);}

    //! Renvoie une chaine de caractère contenant les valeurs de l'attribut.
    QString to_string() const override
        {return Attribut_Premier::to_string().append(",").append(attributs<Attribut_Suivant...>::to_string());}

    //! Test d'égalité entre cette entité et celle transmise en argument.
    bool operator ==(const attributs<Attribut_Premier, Attribut_Suivant...> &ent) const
        {return Attribut_Premier::operator ==(ent) && attributs<Attribut_Suivant...>::operator ==(ent);}

protected:
    //! Retourne le nom de l'attribut donnée souhaitée, pos doit être valide.
    QString multiple_nom(numt pos) const
        {return pos < Attribut_Premier::Nbr_Att ? Attribut_Premier::multiple_nom(pos)
                                                : attributs<Attribut_Suivant...>::multiple_nom(pos - Attribut_Premier::Nbr_Att);}

    //! Retourne un QVariant contenant la donnée souhaitée, pos doit être valide.
    QVariant data_v(numt pos) const override
        {return pos < Attribut_Premier::Nbr_Att ? Attribut_Premier::data_v(pos)
                                                : attributs<Attribut_Suivant...>::data_v(pos - Attribut_Premier::Nbr_Att);}

    //! Remplace les attributs de l'entité par celle de l'entité transmise.
    void set(const  attributs<Attribut_Premier, Attribut_Suivant...> &ent) {
        Attribut_Premier::set(ent);
        attributs<Attribut_Suivant...>::set(ent);
    }

    //! Modifie la donnée à partir d'un QVariant, pos doit être valide.
   void set_data_v(numt pos, const QVariant &value) override
        {return pos < Attribut_Premier::Nbr_Att ? Attribut_Premier::set_data_v(pos, value)
                                                : attributs<Attribut_Suivant...>
                                                  ::set_data_v(pos - Attribut_Premier::Nbr_Att, value);}
};

/*! \ingroup groupe_entity
 *\brief Structure template de calcul des positions des attributs multiple.
 */
template<class Att_Found, class Att, class Att_Pre, class Att_Suiv, numt Pos> struct position_enum_temp {
    enum {Position = position_enum_temp<Att_Found,Att_Pre,typename Att_Pre::att_pre,typename Att_Pre::att_suiv,Pos>::Position
          + position_enum_temp<Att_Found,Att_Suiv,typename Att_Suiv::att_pre,typename Att_Suiv::att_suiv,Att_Pre::Nbr_Att + Pos>
          ::Position};
};

template<class Att_Found, class Att, numt Pos> struct position_enum_temp<Att_Found,Att,no_attribut,no_attribut,Pos>
    {enum {Position = 0};};

template<class Att_Found, numt Pos> struct position_enum_temp<Att_Found,Att_Found,no_attribut,no_attribut,Pos>
    {enum {Position = Pos};};

template<class Att_Found, numt Pos> struct position_enum_temp<Att_Found,attributs<Att_Found>,no_attribut,no_attribut,Pos>
    {enum {Position = Pos};};

template<class Att_Found, class Ent> using position_enum
    = position_enum_temp<Att_Found,Ent,typename Ent::att_pre,typename Ent::att_suiv,0>;
}

namespace attribut {
// attributs Multiples Prédéfinies
using cibles_attribut = entities::attributs<id_cible_attribut, cible_attribut>;
using cible_date_time_attribut = entities::attributs<cibles_attribut, date_time_valide_attribut>;
using cible_date_time_num_attribut = entities::attributs<cible_date_time_attribut, num_attribut>;
using creation_modification_attribut = entities::attributs<creation_attribut,modification_attribut>;
using debut_fin_attribut = entities::attributs<debut_attribut,fin_attribut>;
using etats_attribut = entities::attributs<id_etat_attribut, etat_attribut>;
template<class Valeur_Attribut> using cible_date_time_num_valeur_attribut
    = entities::attributs<cible_date_time_num_attribut,Valeur_Attribut>;
using id_num_attribut = entities::attributs<id_1_attribut, num_attribut>;
using id_prog_nom_attribut = entities::attributs<id_prog_attribut,nom_attribut>;
template<class Valeur_Attribut> using id_num_valeur_attribut = entities::attributs<id_num_attribut, Valeur_Attribut>;
using nom_type_attribut = entities::attributs<nom_attribut, type_attribut>;
using nom_ref_type_attribut = entities::attributs<nom_type_attribut, ref_attribut>;
using nc_nom_attribut = entities::attributs<nc_attribut, nom_attribut>;
using nc_nom_ref_attribut = entities::attributs<nc_attribut, nom_attribut, ref_attribut>;
using nc_nom_type_attribut = entities::attributs<nc_nom_attribut, type_attribut>;
using nc_nom_ref_type_attribut = entities::attributs<nc_nom_ref_attribut, type_attribut>;
using relation_attribut = entities::attributs<id_1_attribut, id_2_attribut>;
using relation_date_time_attribut = entities::attributs<relation_attribut, date_time_valide_attribut>;
using relation_num_attribut = entities::attributs<relation_attribut, num_attribut>;
using textes_attribut = entities::attributs<creation_modification_attribut,texte_attribut>;

/*! \ingroup groupe_entity
 *\brief attributs des entités de type arbre.
 */
class arbre_attribut : public entities::attributs<parent_attribut,feuille_attribut,num_attribut> {
public:
    enum {Ordre = entities::position_enum<num_attribut,arbre_attribut>::Position};
    CONSTR_DEFAUT(arbre_attribut)
    CONSTR_AFFECT_DEFAUT(arbre_attribut)
};

/*! \ingroup groupe_attribut_entity
 *\brief Attributs contenant deux identifiants dont exactement un est non nul pour être valide.
 */
class relation_exact_one_not_null_attribut : public entities::attributs<id_1_null_attribut,id_2_null_attribut> {
public:
    CONSTR_DEFAUT(relation_exact_one_not_null_attribut)
    CONSTR_AFFECT_DEFAUT(relation_exact_one_not_null_attribut)

    //! Teste si l'entité est valide.
    bool is_valid() const override
        {return (id_2() == 0 &&id_1_null_attribut::is_valid_attribut())
                || (id_1() == 0 &&id_2_null_attribut::is_valid_attribut());}
};

/*! \ingroup groupe_attribut_entity
 *\brief Attributs contenant trois identifiants dont exactement un est non nul pour être valide.
 */
class relation_trois_exact_one_not_null_attribut
        : public entities::attributs<id_1_null_attribut,id_2_null_attribut,id_3_null_attribut> {
public:
    CONSTR_DEFAUT(relation_trois_exact_one_not_null_attribut)
    CONSTR_AFFECT_DEFAUT(relation_trois_exact_one_not_null_attribut)

    //! Teste si l'entité est valide.
    bool is_valid() const override
        {return (id_2() == 0 && id_3() == 0 && id_1_null_attribut::is_valid_attribut())
                || (id_1() == 0 && id_3() == 0 && id_2_null_attribut::is_valid_attribut())
                || (id_1() == 0 && id_2() == 0 && id_3_null_attribut::is_valid_attribut());}
};
}}
#endif // ATTRIBUT_MULTIPLE_H
