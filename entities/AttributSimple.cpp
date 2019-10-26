#include "AttributSimple.h"
constexpr std::array<int, AttributDecimale::NbrValues> AttributDecimale::Decimale{{1,2,3,4,5,6,8,10,20,25,50,100,200,250,500,1000}};

QString AttributEntity::affiche() const
        {return nameAttribut().append(" (").append(validToString()).append("): ").append(toStringAttribut());}

DESTR_VIDE_DEF(AttributEntity)
//DESTR_VIDE_DEF(AttributBool)
DESTR_VIDE_DEF(AttributCode)
//DESTR_VIDE_DEF(AttributDate)
DESTR_VIDE_DEF(AttributDateCurrent)
DESTR_VIDE_DEF(AttributDateNull)
DESTR_VIDE_DEF(AttributDateValide)
//DESTR_VIDE_DEF(AttributDateTime)
DESTR_VIDE_DEF(AttributDateTimeCurrent)
DESTR_VIDE_DEF(AttributDateTimeNull)
DESTR_VIDE_DEF(AttributDateTimeValide)
//DESTR_VIDE_DEF(AttributInt)
DESTR_VIDE_DEF(AttributDecimale)
//DESTR_VIDE_DEF(AttributDouble)
//DESTR_VIDE_DEF(AttributIntNull)
//DESTR_VIDE_DEF(AttributIntNullPositif)
//DESTR_VIDE_DEF(AttributNumNull)
//DESTR_VIDE_DEF(AttributString)
DESTR_VIDE_DEF(AttributStringNotEmpty)
//DESTR_VIDE_DEF(AttributUnsigned)
//DESTR_VIDE_DEF(AttributUnsignedNull)
//DESTR_VIDE_DEF(AttributVariant)

DESTR_VIDE_DEF(AlphaAttribut)
DESTR_VIDE_DEF(CardAttribut)
DESTR_VIDE_DEF(CibleAttribut)
DESTR_VIDE_DEF(CreationAttribut)
DESTR_VIDE_DEF(CodeAttribut)
DESTR_VIDE_DEF(DateValideAttribut)
DESTR_VIDE_DEF(DateTimeCurrentAttribut)
DESTR_VIDE_DEF(DateTimeValideAttribut)
DESTR_VIDE_DEF(DecimaleAttribut)
DESTR_VIDE_DEF(EtatAttribut)
DESTR_VIDE_DEF(ExactAttribut)
DESTR_VIDE_DEF(IdAttribut)
DESTR_VIDE_DEF(IdCibleAttribut)
DESTR_VIDE_DEF(IdEtatAttribut)
DESTR_VIDE_DEF(IdProgAttribut)
DESTR_VIDE_DEF(Id1Attribut)
DESTR_VIDE_DEF(Id1NegAttribut)
DESTR_VIDE_DEF(Id1NullAttribut)
DESTR_VIDE_DEF(Id2Attribut)
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
//DESTR_VIDE_DEF(NumNullAttribut)
DESTR_VIDE_DEF(ParentAttribut)
DESTR_VIDE_DEF(SaisieAttribut)
DESTR_VIDE_DEF(TexteAttribut)
DESTR_VIDE_DEF(TotalAttribut)
DESTR_VIDE_DEF(TpValAttribut)
DESTR_VIDE_DEF(TypeAttribut)
DESTR_VIDE_DEF(ValeurDoubleAttribut)
DESTR_VIDE_DEF(ValeurIntAttribut)
DESTR_VIDE_DEF(ValeurVariantAttribut)
