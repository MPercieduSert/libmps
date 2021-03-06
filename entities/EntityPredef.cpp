#include "EntityPredef.h"

using namespace entityMPS;

DESTR_VIDE_DEF(CommentaireCible)
DESTR_VIDE_DEF(DonneeCible)
DESTR_VIDE_DEF(Donnee)
DESTR_VIDE_DEF(DonneeCard)
DESTR_VIDE_DEF(Evenement)
DESTR_VIDE_DEF(EvenementCible)
DESTR_VIDE_DEF(EvenementStyle)
DESTR_VIDE_DEF(MotCleCible)
DESTR_VIDE_DEF(MotClePermission)
DESTR_VIDE_DEF(MotProgCible)
DESTR_VIDE_DEF(MotProgPermission)
DESTR_VIDE_DEF(Restriction)
DESTR_VIDE_DEF(SourceTexte)
DESTR_VIDE_DEF(TexteCible)
DESTR_VIDE_DEF(TypePermission)
DESTR_VIDE_DEF(Utilisation)

QString EvenementStyle::formeString(int forme) {
    if(forme == EvenementStyle::Rectangle)
        return QObject::tr("Rectangle");
    return QObject::tr("Forme inconnue");
}



