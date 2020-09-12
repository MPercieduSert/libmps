#include "AttributSimple.h"

using namespace attributMPS;
constexpr std::array<int, AttributDecimale::NbrValues> AttributDecimale::Decimale{{1,2,3,4,5,6,8,10,20,25,50,100,200,250,500,1000}};
const QRegularExpression AttributRef::reg {"^[a-z][a-z0-9_]*$"};
const std::map<int,QString> AttributAlpha::m_romain{ {1000,"M"}, {900,"CM"}, {500,"D"}, {400,"CD"}, {100,"C"}, {90,"XC"}, {50,"L"}, {40,"XL"}, {10,"X"}, {9,"IX"}, {5,"V"}, {4,"IV"}, {1,"I"}};
QString AttributEntity::affiche() const
        {return nameAtt().append(" : ").append(nameClasseAttribut())
                                            .append(" (").append(validToString())
                                            .append("): ").append(toStringAttribut());}

QString AttributAlpha::alphaTexte(int num) const {
    if(num >= 0) {
        switch (m_valeur) {
        case ArabeAlphaTexte:
            return QString::number(num + 1);
        case RomainAlphaTexte: {
            num += 1;
            QString str;
            for (auto iter = m_romain.cbegin(); num; ++iter) {
                while (num >= iter->first) {
                    str.append(iter->second);
                    num -= iter->first;
                }
            }
            return str;
        }
        case MinusculeAlphaTexte: {
            auto i = 0x61;
            i += num;
            return QChar(i);
        }
        case MajusculeAlphaTexte: {
            auto i = 0x41;
            i += num;
            return QChar(i);
        }
        case GrecMinusculeAlphaTexte: {
            auto i = 0x3B1;
            i += num;
            return QChar(i);
        }
        case GrecMajusculeAlphaTexte: {
            auto i = 0x391;
            i += num;
            return QChar(i);
        }
        default:
            return QString();
        }
    }
    return QString();
}

DESTR_VIDE_DEF(AttributEntity)
DESTR_VIDE_DEF(AttributAlpha)
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

DESTR_VIDE_DEF(AlphaAttribut)
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
