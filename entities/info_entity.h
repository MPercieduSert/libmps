/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/08/2018
 */
#ifndef INFO_ENTITY_H
#define INFO_ENTITY_H

#include "type_mps.h"

/*! \defgroup groupeinfo_entity Informations sur les entités
 *\ingroup groupe_entity
 *\brief Ensemble des informations sur les entités.
 */

namespace mps {
/*! \ingroup groupeinfo_entity
 *\brief Espace de noms contenant les enumérations sur les entités.
 */
namespace info_entity {
    //! Liste des identifiants des types d'entités de base, n'ayant pas de manageurs dans accessible par identifiant.
    enum entities_base_id : entidt{arbre,
                                 Version_Bdd,
                                 Nbr_entity_Base};

    //! Liste des identifiants des entités prédéfinies.
    enum entity_predef_id : entidt{Commentair_Cible = Nbr_entity_Base,
                    donnee_Cible,
                    Texte_Cible,
                    Commentaire,
                    donnee,
                    Donnee_Card,
                    Evenement,
                    Evenemen_Cible,
                    Evenement_Style,
                    Historique,
                    Mot_Cle,
                    Mot_Cle_Cible,
                    Mot_Cle_Permission,
                    Mot_Prog_Cible,
                    Mot_Prog_Permission,
                    Restriction,
                    Source,
                    Texte,
                    source_texte,
                    Type,
                    Type_Permission,
                    Utilisation,
                    Usage,
                    NbrentityPredefinie};

    enum decimale{Nbr_Decimales = 18};
}}
#endif // INFO_ENTITY_H
