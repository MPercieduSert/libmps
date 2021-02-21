#include "entity_predef.h"

using namespace mps::entities;

DESTR_VIDE_DEF(commentaire_cible)
DESTR_VIDE_DEF(donnee_cible)
DESTR_VIDE_DEF(donnee)
DESTR_VIDE_DEF(donnee_card)
DESTR_VIDE_DEF(evenement)
DESTR_VIDE_DEF(evenement_cible)
DESTR_VIDE_DEF(evenement_style)
DESTR_VIDE_DEF(mot_cle_cible)
DESTR_VIDE_DEF(mot_cle_permission)
DESTR_VIDE_DEF(mot_prog_cible)
DESTR_VIDE_DEF(mot_prog_permission)
DESTR_VIDE_DEF(restriction)
DESTR_VIDE_DEF(source_texte)
DESTR_VIDE_DEF(texte_cible)
DESTR_VIDE_DEF(type_permission)
DESTR_VIDE_DEF(utilisation)

QString evenement_style::forme_string(int forme) {
    if(forme == evenement_style::Rectangle)
        return QObject::tr("Rectangle");
    return QObject::tr("Forme inconnue");
}
