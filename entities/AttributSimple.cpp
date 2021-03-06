#include "AttributSimple.h"

using namespace attributMPS;
constexpr std::array<int, AttributDecimale::NbrValues> AttributDecimale::Decimale{{1,2,3,4,5,6,7,8,9,10,20,40,50,100,200,400,500,1000}};
constexpr std::array<int, AttributDecimale::NbrValues> AttributDecimale::Precision{{0,1,2,2,1,2,3,3,3,1,2,3,2,2,3,4,3,3}};
const QRegularExpression AttributRef::reg {"^[a-z][a-z0-9_]*$"};
QString AttributEntity::affiche() const
        {return nameAtt().append(" : ").append(nameClasseAttribut())
                                            .append(" (").append(validToString())
                                            .append("): ").append(toStringAttribut());}

DESTR_VIDE_DEF(AttributEntity)
DESTR_VIDE_DEF(AttributBrush)
DESTR_VIDE_DEF(AttributCode)
DESTR_VIDE_DEF(AttributCouleur)
DESTR_VIDE_DEF(AttributDateCurrent)
DESTR_VIDE_DEF(AttributDateNull)
DESTR_VIDE_DEF(AttributDateValide)
DESTR_VIDE_DEF(AttributDateTimeCurrent)
DESTR_VIDE_DEF(AttributDateTimeNull)
DESTR_VIDE_DEF(AttributDateTimeValide)
DESTR_VIDE_DEF(AttributDecimale)
DESTR_VIDE_DEF(AttributRef)
DESTR_VIDE_DEF(AttributStringNotEmpty)

DESTR_VIDE_DEF(BordureAttribut)
DESTR_VIDE_DEF(CardAttribut)
DESTR_VIDE_DEF(CibleAttribut)
DESTR_VIDE_DEF(CreationAttribut)
DESTR_VIDE_DEF(CodeAttribut)
DESTR_VIDE_DEF(CouleurAttribut)
DESTR_VIDE_DEF(CouleurBordureAttribut)
DESTR_VIDE_DEF(CouleurFondAttribut)
DESTR_VIDE_DEF(CouleurTexteAttribut)
DESTR_VIDE_DEF(CouleurTitreAttribut)
DESTR_VIDE_DEF(DateValideAttribut)
DESTR_VIDE_DEF(DateTimeCurrentAttribut)
DESTR_VIDE_DEF(DateTimeValideAttribut)
DESTR_VIDE_DEF(DebutAttribut)
DESTR_VIDE_DEF(DecimaleAttribut)
DESTR_VIDE_DEF(EtatAttribut)
DESTR_VIDE_DEF(ExactAttribut)
DESTR_VIDE_DEF(FeuilleAttribut)
DESTR_VIDE_DEF(FinAttribut)
DESTR_VIDE_DEF(FormeAttribut)
DESTR_VIDE_DEF(IdAttribut)
DESTR_VIDE_DEF(IdCibleAttribut)
DESTR_VIDE_DEF(IdEtatAttribut)
DESTR_VIDE_DEF(IdProgAttribut)
DESTR_VIDE_DEF(Id1Attribut)
DESTR_VIDE_DEF(Id1NegAttribut)
DESTR_VIDE_DEF(Id1NullAttribut)
DESTR_VIDE_DEF(Id2Attribut)
DESTR_VIDE_DEF(Id2NegAttribut)
DESTR_VIDE_DEF(Id2NullAttribut)
DESTR_VIDE_DEF(Id3Attribut)
DESTR_VIDE_DEF(Id3NullAttribut)
DESTR_VIDE_DEF(Id4Attribut)
DESTR_VIDE_DEF(Id5Attribut)
DESTR_VIDE_DEF(ModifAttribut)
DESTR_VIDE_DEF(ModificationAttribut)
DESTR_VIDE_DEF(NcAttribut)
DESTR_VIDE_DEF(NomAttribut)
DESTR_VIDE_DEF(NumAttribut)
DESTR_VIDE_DEF(ParentAttribut)
DESTR_VIDE_DEF(PoliceAttribut)
DESTR_VIDE_DEF(PoliceTexteAttribut)
DESTR_VIDE_DEF(PoliceTitreAttribut)
DESTR_VIDE_DEF(RefAttribut)
DESTR_VIDE_DEF(SaisieAttribut)
DESTR_VIDE_DEF(StyleNumAttribut)
DESTR_VIDE_DEF(TexteAttribut)
DESTR_VIDE_DEF(TextureAttribut)
DESTR_VIDE_DEF(TitreAttribut)
DESTR_VIDE_DEF(TotalAttribut)
DESTR_VIDE_DEF(TpValAttribut)
DESTR_VIDE_DEF(TypeAttribut)
DESTR_VIDE_DEF(TypeIdtAttribut)
DESTR_VIDE_DEF(ValeurDoubleAttribut)
DESTR_VIDE_DEF(ValeurIntAttribut)
DESTR_VIDE_DEF(ValeurVariantAttribut)
DESTR_VIDE_DEF(VersionAttribut)

szt AttributDecimale::indice(int valeur) {
    szt k = 16;
    szt i = 0;
    if(valeur >= Decimale[k]){
        i = k;
        while(i != NbrValues && Decimale[i] != valeur)
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
            return  NbrValues;
    }
}

bool AttributDecimale::isValidAttribut() const {
    szt k = 16;
    szt i = 0;
    if(m_valeur >= Decimale[k]){
        i = k;
        while(i != NbrValues && m_valeur != Decimale[i])
            ++i;
        return i != NbrValues;
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

int AttributDecimale::precisionDecimale(int valeur) {
    auto i = indice(valeur);
    if(i != NbrValues)
        return Precision.at(i);
    else
        return 0;
}
