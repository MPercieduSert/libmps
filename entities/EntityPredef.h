/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef ENTITYPREDEF_H
#define ENTITYPREDEF_H

#include "EntityCible.h"
#include "EntityDivers.h"
#include "EntityRelation.h"

/*! \ingroup groupeEntity
 * \brief Espace de noms contenant les enumérations sur les données.
 */
namespace donnee {
    //! Différents types d'affichage.
    enum affiche_alpha {Sans = 0,
                        Numeric = 1,
                        AlphabeticMaj = 2,
                        AlphabeticMin = 3};

    //! Différents catégories de données.
    enum typeDn {Alpha = 0,
                Perso,
                ProgramationInterne,
                Contenu};

    //! Différents types des valeurs des données
    enum typeVal {NoDonnee = -1,
                  Int = 0,
                  String = 1,
                  Bool = 2,
                  Float = 3,
                  Double = 4,
                  Date = 5,
                  DateTime = 6};

    /*//! Identifiant de programation
    enum prog {NoId,
              ContactIdProg,
              AdresseIdProg,
              NumRueIdProg,
              RueIdProg,
              CodePostalIdProg,
              VilleIdProg,
              PaysIdProg,
              TelIdProg,
              MailIdProg,
              ProgramationInterneIdProg,
              GeneriqueIdProg,
              NcIdProg,
              NomIdProg,
              NumIdProg,
              BddIdProg,
              IdBddIdProg,
              MotCleIdProg,
              MatieresIdProg,
              ListeMatieresIdProg,
              CompetenceIdProg,
              ListeCompetenceIdProg,
              DifficulteIdProg,
              ListeDifficulteIdProg
              };*/
}

// Entités de type prédéfinies
ID1_ENTITY(CibleCommentaire,CibleDateTimeCurrentNum,InfoEntity::CibleCommentaireId,Commentaire)
ID1_ENTITY(CibleDonnee,CibleNegDateTimeCurrentNumValeurVariant,InfoEntity::CibleDonneeId,Donnee)
ID1_ENTITY(CibleMotCle,Cible,InfoEntity::CibleMotCleId,MotCle)
ID1_ENTITY(CibleTexte,CibleDateTimeCurrentNumType,InfoEntity::CibleTexteId,Texte)
ID1_ENTITY(MotClePermission,IdCibleCode,InfoEntity::MotClePermissionId,MotCle)
ID1_ENTITY(Type,IdArbreSimpleNcNom,InfoEntity::TypeId,Prog);
ID1_ENTITY(TypePermission,IdCibleCode,InfoEntity::TypePermissionId,Type)
ID1_ENTITY(Utilisation,Utilisation,InfoEntity::UtilisationId,Usage)
RELATION_ENTITY(TexteSource,,InfoEntity::TexteSourceId,Source,Texte)
using Commentaire = TexteEntity<InfoEntity::CommentaireId>;
using MotCle = NcNomEntity<InfoEntity::MotCleId>;
using Texte = TexteEntity<InfoEntity::TexteId>;
using Historique = HistoriqueEntity<InfoEntity::HistoriqueId>;
using Restriction = CibleSimpleCodeEntity<InfoEntity::RestrictionId>;
using Source = NcNomTypeEntity<InfoEntity::SourceId>;
using Usage = NcNomTypeEntity<InfoEntity::UsageId>;

/*! \ingroup groupeEntity
 * \brief Représentation de l'entité Donnee.
 */
class Donnee : public EntityAttributsID<Attributs<NomTypeAttribut,IdProgAttribut,TpValAttribut>,InfoEntity::DonneeId>
{
public:
    using EAID = EntityAttributsID<Attributs<NomTypeAttribut,IdProgAttribut,TpValAttribut>,InfoEntity::DonneeId>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,Donnee>::Position,
                   Nom = PositionEnum<NomAttribut,Donnee>::Position,
                   Type = PositionEnum<TypeAttribut,Donnee>::Position,
                   IdProg = PositionEnum<IdProgAttribut,Donnee>::Position,
                   TpVal = PositionEnum<TpValAttribut,Donnee>::Position,
                   NbrAtt = EAID::NbrAtt};

    //! Identifiant de programation
    enum prog {NoId};

    //! Identifiant des donnée hors de l'arbre.
    enum idNegDonneProg{
        IdProgIdND = -1,    // permet de retrouver l'identifiant d'une entité grâce à un identifiant de programmation.
        EntityIdND = -2,    // enregistrement d'entité en tant que donnée.
        EntityNatureIdND = -3,  // contient la nature (sous forme numéro de cilbe) de l'entité enregistrée.
        EntityCardIdND = -4 // contient le cardinal maximale d'entitée pouvant être enregistrée.
    };

    using EAID::EntityAttributsID;
    BASE_ENTITY(Donnee)

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    /*Donnee(int idProg, int id)
        : EAID(id)
        {setIdProg(idProg);}*/

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    Donnee(int idProg, int id);

    //! Constructeur à partir des valeurs attributs.
    Donnee(const QString & nom, int type, int tpVal, int idProg = prog::NoId, int id = 0)
        : Donnee(idProg,id)
    {
        setNom(nom);
        setType(type);
        setTpVal(tpVal);
    }
};

/*! \ingroup groupeEntity
 * \brief Représentation de l'entité DonneeCard.
 */
class DonneeCard :
        public EntityAttributsID<Attributs<Id1NegAttribut,CardAttribut,ExactAttribut,CibleAttribut>,InfoEntity::DonneeCardId>
{
public:
    using EAID = EntityAttributsID<Attributs<Id1NegAttribut,CardAttribut,ExactAttribut,CibleAttribut>,InfoEntity::DonneeCardId>;
    //! Positions des attributs.
    enum Position {Id = PositionEnum<IdAttribut,DonneeCard>::Position,
                   Id1 = PositionEnum<Id1NegAttribut,DonneeCard>::Position,
                   Card = PositionEnum<CardAttribut,DonneeCard>::Position,
                   Exact = PositionEnum<ExactAttribut,DonneeCard>::Position,
                   Cible = PositionEnum<CibleAttribut,DonneeCard>::Position,
                   NbrAtt = EAID::NbrAtt,
                   IdDonnee = Id1,};

    //! Cardinal.
    enum cardinal {Infini = -1};


    using EAID::EntityAttributsID;
    BASE_ENTITY(DonneeCard)
    ALIAS_CLE(Donnee,1)

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    DonneeCard(int idDn, int cible, int id = 0)
        : EAID(id)
    {
        setIdDonnee(idDn);
        setCible(cible);
    }

    //! Constructeur à partir des valeurs attributs.
    DonneeCard(int idDn, int card, bool exact,int cible, int id = 0)
        : DonneeCard(idDn, cible, id)
    {
        setCard(card);
        setExact(exact);
    }
};

#endif // ENTITYPREDEF_H
