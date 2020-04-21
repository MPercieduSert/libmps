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

    enum prog {NoId,
               defaultValueId};
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

namespace entityMPS {
using namespace typeMPS;
namespace amps = attributMPS;
namespace ebmps = entityBaseMPS;

// Entités de type prédéfinies
/*! \ingroup groupeEntity
 * \brief Ensemble des classes de la fonctionalité commentaire.
 */
namespace ensembleCommentaire {
using Commentaire = ebmps::TexteEntity<infoEntity::CommentaireId>;
ID1_ENTITY(CommentaireCible,CibleDateTimeCurrentNum,infoEntity::CommentaireCibleId,Commentaire)
}
using namespace ensembleCommentaire;

/*! \ingroup groupeEntity
 * \brief Ensemble des classes de la fonctionalité donnee.
 */
namespace ensembleDonnee {
/*! \ingroup groupeEntity
 * \brief Représentation de l'entité Donnee.
 */
class Donnee : public EntityIDs<infoEntity::DonneeId,amps::NomTypeAttribut,
                                                             amps::IdProgAttribut,
                                                             amps::TpValAttribut> {
protected:
    template<class T> using PositionEnum = PositionEnum<T,Donnee>;

public:
    using EAID = EntityIDs<infoEntity::DonneeId,amps::NomTypeAttribut,
                                                        amps::IdProgAttribut,
                                                        amps::TpValAttribut>;

    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut>::Position,
                   Nom = PositionEnum<NomAttribut>::Position,
                   Type = PositionEnum<TypeAttribut>::Position,
                   IdProg = PositionEnum<IdProgAttribut>::Position,
                   TpVal = PositionEnum<TpValAttribut>::Position,
                   NbrAtt = EAID::NbrAtt};

//    //! Identifiant de programation
//    enum prog {NoId};

    //! Identifiant des donnée hors de l'arbre.
    enum idNegDonneProg{
        IdProgIdND = -1,    // permet de retrouver l'identifiant d'une entité grâce à un identifiant de programmation.
        EntityIdND = -2,    // enregistrement d'entité en tant que donnée.
        EntityNatureIdND = -3,  // contient la nature (sous forme numéro de cilbe) de l'entité enregistrée.
        EntityCardIdND = -4 // contient le cardinal maximale d'entitée pouvant être enregistrée.
    };

    using EAID::EntityID;
    BASE_ENTITY(Donnee)

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    Donnee(idt idProg, idt id)
        : EAID(id)
        {setIdProg(idProg);}

    //! Constructeur à partir des valeurs attributs.
    Donnee(const QString & nom, idt type, int tpVal, idt idProg = donnee::NoId, idt id = 0)
        : Donnee(idProg,id) {
        setNom(nom);
        setType(type);
        setTpVal(tpVal);
    }
};

/*! \ingroup groupeEntity
 * \brief Représentation de l'entité DonneeCard.
 */
class DonneeCard :
        public EntityIDs<infoEntity::DonneeCardId,amps::Id1NegAttribut,
                                                          amps::CardAttribut,
                                                          amps::ExactAttribut,
                                                          amps::CibleAttribut> {
protected:
    template<class T> using PositionEnum = PositionEnum<T,DonneeCard>;
public:
    using EAID = EntityIDs<infoEntity::DonneeCardId,amps::Id1NegAttribut,
                                                            amps::CardAttribut,
                                                            amps::ExactAttribut,
                                                            amps::CibleAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut>::Position,
                   Id1 = PositionEnum<Id1NegAttribut>::Position,
                   Card = PositionEnum<CardAttribut>::Position,
                   Cible = PositionEnum<CibleAttribut>::Position,
                   Exact = PositionEnum<ExactAttribut>::Position,
                   NbrAtt = EAID::NbrAtt,
                   IdDonnee = Id1,};

    //! Cardinal.
    enum cardinal {Infini = 0};


    using EAID::EntityID;
    BASE_ENTITY(DonneeCard)
    ALIAS_CLE_NEG(Donnee,1)

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    DonneeCard(int idDn, int cible, idt id = 0)
        : EAID(id) {
        setIdDonnee(idDn);
        setCible(cible);
    }

    //! Constructeur à partir des valeurs attributs.
    DonneeCard(int idDn, int card, int cible, codeType exact = 0, idt id = 0)
        : DonneeCard(idDn, cible, id) {
        setCard(card);
        setExact(exact);
    }
};
ID1_ENTITY_NEG(DonneeCible,CibleNegDateTimeCurrentNumValeurVariant,infoEntity::DonneeCibleId,Donnee)
}
using namespace ensembleDonnee;

/*! \ingroup groupeEntity
 * \brief Ensemble des classes de la fonctionalité historique.
 */
namespace ensembleHistorique {
using Historique = ebmps::HistoriqueEntity<infoEntity::HistoriqueId>;
}
using namespace ensembleHistorique;

/*! \ingroup groupeEntity
 * \brief Ensemble des classes de la fonctionalité mot clé.
 */
namespace ensembleMotCle {
using MotCle = ebmps::NcNomEntity<infoEntity::MotCleId>;
ID1_ENTITY(MotCleCible,Cible,infoEntity::MotCleCibleId,MotCle)
ID1_ENTITY(MotClePermission,IdCibleCode,infoEntity::MotClePermissionId,MotCle)
RELATION_ENTITY_NEG(MotProgCible,RelationCibleNeg,infoEntity::MotProgCibleId,MotCle,Prog)
RELATION_ENTITY_NEG(MotProgPermission,RelationCibleAttCodeNeg,infoEntity::MotProgPermissionId,MotCle,Prog)
}
using namespace ensembleMotCle;

/*! \ingroup groupeEntity
 * \brief Ensemble des classes de la fonctionalité restriction.
 */
namespace ensembleRestriction {
using Restriction = ebmps::CibleSimpleCodeEntity<infoEntity::RestrictionId>;
}
using namespace ensembleRestriction;

/*! \ingroup groupeEntity
 * \brief Ensemble des classes de la fonctionalité texte.
 */
namespace ensembleTexte {
using Source = ebmps::NcNomTypeEntity<infoEntity::SourceId>;
RELATION_ENTITY(SourceTexte,Relation,infoEntity::SourceTexteId,Source,Texte)
using Texte = ebmps::TexteEntity<infoEntity::TexteId>;
ID1_ENTITY(TexteCible,CibleDateTimeCurrentNumType,infoEntity::TexteCibleId,Texte)
}
using namespace ensembleTexte;

/*! \ingroup groupeEntity
 * \brief Ensemble des classes de la fonctionalité type.
 */
namespace ensembleType {
ID1_ENTITY(Type,IdNullArbreSimpleNcNom,infoEntity::TypeId,Prog);
ID1_ENTITY(TypePermission,IdCibleCode,infoEntity::TypePermissionId,Type)
}
using namespace ensembleType;

/*! \ingroup groupeEntity
 * \brief Ensemble des classes de la fonctionalité utilisation.
 */
namespace ensembleUtilisation {
using Usage = ebmps::NcNomTypeEntity<infoEntity::UsageId>;
ID1_ENTITY(Utilisation,Utilisation,infoEntity::UtilisationId,Usage)
}
using namespace ensembleUtilisation;
}
#endif // ENTITYPREDEF_H
