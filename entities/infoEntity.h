/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/08/2018
 */
#ifndef INFOENTITY_H
#define INFOENTITY_H

#include "typemps.h"

/*! \defgroup groupeinfoEntity Informations sur les entités
 * \ingroup groupeEntity
 * \brief Ensemble des informations sur les entités.
 */

/*! \ingroup groupeinfoEntity
 * \brief Espace de noms contenant les enumérations sur les entités.
 */
namespace infoEntity {
    //! Liste des identifiants des types d'entités de base, n'ayant pas de manageurs dans accessible par identifiant.
    enum entityBaseId:typeMPS::szt{ArbreId,
                      VersionBddId,
                      NbrEntBaseId};

//        entityId = -26,
//                    ArbreSimpleId,
//                    CibleEntityId,
//                    DateTimeCibleEntityId,
//                    DateTimeNumEntityId,
//                    DateTimeNumRelationEntityId,
//                    DateTimeRelationEntityId,
//                    IdNumEntityId,
//                    NcNomEntityId,
//                    NomEntityId,
//                    NumDateTimeCibleEntityId,
//                    NumEntityId,
//                    NumRelationEntityId,
//                    RelationEntityId,
//                    RelationExactOneNotNullEntityId,
//                    TexteEntityId,
//                    TypeNcNomEntityId,
//                    TypeNomEntityId,
//                    ValeurDateTimeRelationEntityId,
//                    ValeurIntDateTimeNumRelationEntityId,
//                    ValeurDoubleIdNumEntityId,
//                    ValeurIdNumEntityId,
//                    ValeurIntIdNumEntityId,
//                    ValeurNumDateTimeCibleEntityId,
//                    };

    //! Liste des identifiants des entités prédéfinies.
    enum entityPredefinieID:typeMPS::szt{CommentaireCibleId = NbrEntBaseId,
                    DonneeCibleId,
                    TexteCibleId,
                    CommentaireId,
                    DonneeId,
                    DonneeCardId,
                    EvenementId,
                    EvenementCibleId,
                    EvenementStyleId,
                    HistoriqueId,
                    MotCleId,
                    MotCleCibleId,
                    MotClePermissionId,
                    MotProgCibleId,
                    MotProgPermissionId,
                    RestrictionId,
                    SourceId,
                    TexteId,
                    SourceTexteId,
                    TypeId,
                    TypePermissionId,
                    UtilisationId,
                    UsageId,
                    NbrEntityPredefinie};

    enum decimale{NbrDecimales = 18};
};

#endif // INFOENTITY_H
