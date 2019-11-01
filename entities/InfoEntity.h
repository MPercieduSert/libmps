/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/08/2018
 */
#ifndef INFOENTITY_H
#define INFOENTITY_H

/*! \defgroup groupeInfoEntity Informations sur les entités
 * \ingroup groupeEntity
 * \brief Ensemble des informations sur les entités.
 */

/*! \ingroup groupeInfoEntity
 * \brief Espace de noms contenant les enumérations sur les entités.
 */
namespace InfoEntity
{
    //! Liste des identifiants des types d'entités.
    enum entityBaseId{entityId = -26,
                    ArbreId,
                    ArbreSimpleId,
                    CibleEntityId,
                    DateTimeCibleEntityId,
                    DateTimeNumEntityId,
                    DateTimeNumRelationEntityId,
                    DateTimeRelationEntityId,
                    IdNumEntityId,
                    NcNomEntityId,
                    NomEntityId,
                    NumDateTimeCibleEntityId,
                    NumEntityId,
                    NumRelationEntityId,
                    RelationEntityId,
                    RelationExactOneNotNullEntityId,
                    TexteEntityId,
                    TypeNcNomEntityId,
                    TypeNomEntityId,
                    ValeurDateTimeRelationEntityId,
                    ValeurIntDateTimeNumRelationEntityId,
                    ValeurDoubleIdNumEntityId,
                    ValeurIdNumEntityId,
                    ValeurIntIdNumEntityId,
                    ValeurNumDateTimeCibleEntityId,
                    VersionBddId};

    //! Liste des identifiants des entités prédéfinies.
    enum entityPredefinieID{CommentaireCibleId,
                    DonneeCibleId,
                    TexteCibleId,
                    CommentaireId,
                    DonneeId,
                    DonneeCardId,
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

    enum decimale{NbrDecimales = 16};
};

#endif // INFOENTITY_H
