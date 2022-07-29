#include "attribut_simple.h"

using namespace mps;
using namespace mps::attribut;
constexpr std::array<int, attribut_decimale::Nbr_Values> attribut_decimale::Decimale{{1,2,3,4,5,6,7,8,9,10,20,40,50,100,200,400,500,1000}};
constexpr std::array<int, attribut_decimale::Nbr_Values> attribut_decimale::Precision{{0,1,2,2,1,2,3,3,3,1,2,3,2,2,3,4,3,3}};
const QRegularExpression attribut_ref::Reg {"^[a-z][a-z0-9_]*$"};
QString attribut_entity::affiche() const
        {return attribut_nom().append(" : ").append(classe_nom())
                                            .append(" (").append(valid_to_string())
                                            .append("): ").append(to_string_attribut());}

DESTR_VIDE_DEF(attribut_entity)
DESTR_VIDE_DEF(attribut_brush)
DESTR_VIDE_DEF(attribut_code)
DESTR_VIDE_DEF(attribut_couleur)
DESTR_VIDE_DEF(attribut_date_current)
DESTR_VIDE_DEF(attribut_date_null)
DESTR_VIDE_DEF(attribut_date_valide)
DESTR_VIDE_DEF(attribut_date_time_current)
DESTR_VIDE_DEF(attribut_date_time_null)
DESTR_VIDE_DEF(attribut_date_time_valide)
DESTR_VIDE_DEF(attribut_date_time_valide_or_current)
DESTR_VIDE_DEF(attribut_decimale)
DESTR_VIDE_DEF(attribut_ref)
DESTR_VIDE_DEF(attribut_string_not_empty)

DESTR_VIDE_DEF(bordure_attribut)
DESTR_VIDE_DEF(card_attribut)
DESTR_VIDE_DEF(cible_attribut)
DESTR_VIDE_DEF(creation_attribut)
DESTR_VIDE_DEF(code_attribut)
DESTR_VIDE_DEF(couleur_attribut)
DESTR_VIDE_DEF(couleur_bordure_attribut)
DESTR_VIDE_DEF(couleur_fond_attribut)
DESTR_VIDE_DEF(couleur_texte_attribut)
DESTR_VIDE_DEF(couleur_titre_attribut)
DESTR_VIDE_DEF(date_valide_attribut)
DESTR_VIDE_DEF(date_time_current_attribut)
DESTR_VIDE_DEF(date_time_valide_attribut)
DESTR_VIDE_DEF(debut_attribut)
DESTR_VIDE_DEF(decimale_attribut)
DESTR_VIDE_DEF(etat_attribut)
DESTR_VIDE_DEF(exact_attribut)
DESTR_VIDE_DEF(feuille_attribut)
DESTR_VIDE_DEF(fin_attribut)
DESTR_VIDE_DEF(forme_attribut)
DESTR_VIDE_DEF(id_attribut)
DESTR_VIDE_DEF(id_cible_attribut)
DESTR_VIDE_DEF(id_etat_attribut)
DESTR_VIDE_DEF(id_prog_attribut)
DESTR_VIDE_DEF(id_1_attribut)
DESTR_VIDE_DEF(id_1_neg_attribut)
DESTR_VIDE_DEF(id_1_null_attribut)
DESTR_VIDE_DEF(id_1_zero_attribut)
DESTR_VIDE_DEF(id_2_attribut)
DESTR_VIDE_DEF(id_2_neg_attribut)
DESTR_VIDE_DEF(id_2_null_attribut)
DESTR_VIDE_DEF(id_3_attribut)
DESTR_VIDE_DEF(id_3_null_attribut)
DESTR_VIDE_DEF(id_4_attribut)
DESTR_VIDE_DEF(id_5_attribut)
DESTR_VIDE_DEF(modif_attribut)
DESTR_VIDE_DEF(modification_attribut)
DESTR_VIDE_DEF(nc_attribut)
DESTR_VIDE_DEF(nom_attribut)
DESTR_VIDE_DEF(num_attribut)
DESTR_VIDE_DEF(parent_attribut)
DESTR_VIDE_DEF(police_attribut)
DESTR_VIDE_DEF(police_texte_attribut)
DESTR_VIDE_DEF(police_titre_attribut)
DESTR_VIDE_DEF(ref_attribut)
DESTR_VIDE_DEF(saisie_attribut)
DESTR_VIDE_DEF(style_num_attribut)
DESTR_VIDE_DEF(texte_attribut)
DESTR_VIDE_DEF(texture_attribut)
DESTR_VIDE_DEF(titre_attribut)
DESTR_VIDE_DEF(total_attribut)
DESTR_VIDE_DEF(tp_val_attribut)
DESTR_VIDE_DEF(type_attribut)
DESTR_VIDE_DEF(type_id_attribut)
DESTR_VIDE_DEF(valeur_double_attribut)
DESTR_VIDE_DEF(valeur_int_attribut)
DESTR_VIDE_DEF(valeur_variant_attribut)
DESTR_VIDE_DEF(version_attribut)

szt attribut_decimale::Indice(int valeur) {
    szt k = 16;
    szt i = 0;
    if(valeur >= Decimale[k]){
        i = k;
        while(i != Nbr_Values &&Decimale[i] != valeur)
            ++i;
        return i;
    }
    else {
        while(k != 1){
            k /= 2;
            if(valeur >= Decimale[i + k])
                i += k;
        }
        if(valeur == Decimale[i])
            return i;
        else
            return  Nbr_Values;
    }
}

bool attribut_decimale::is_valid_attribut() const {
    szt k = 16;
    szt i = 0;
    if(m_valeur >= Decimale[k]){
        i = k;
        while(i != Nbr_Values &&m_valeur != Decimale[i])
            ++i;
        return i != Nbr_Values;
    }
    else {
        while(k != 1){
            k /= 2;
            if(m_valeur >= Decimale[i + k])
                i += k;
        }
        return m_valeur == Decimale[i];
    }
}

int attribut_decimale::Precision_decimale(int valeur) {
    auto i = Indice(valeur);
    return i != Nbr_Values ? Precision.at(i) : 0;
}
