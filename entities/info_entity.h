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

//        entityId = -26,
//                    arbre_simple_id,
//                    cible_entityId,
//                    Date_Timecible_entityId,
//                    date_time_num_entityId,
//                    Date_time_numrelation_entityId,
//                    Date_Timerelation_entityId,
//                    id_num_entityId,
//                    nc_nom_entityId,
//                    nom_entityId,
//                    NumDate_Timecible_entityId,
//                    num_entityId,
//                    Numrelation_entityId,
//                    relation_entityId,
//                    relation_exact_one_not_null_entityId,
//                    texte_entityId,
//                    Typenc_nom_entityId,
//                    Typenom_entityId,
//                    ValeurDate_Timerelation_entityId,
//                    valeur_intDate_time_numrelation_entityId,
//                    valeur_doubleid_num_entityId,
//                    Valeurid_num_entityId,
//                    valeur_intid_num_entityId,
//                    ValeurNumDate_Timecible_entityId,
//                    };

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
