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

    //! Identifiant de programmation
    enum prog {NoId};

    //! Exact
    enum cardinalCode {Exact,
                       Auplus,
                       AuMoins,
    };

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
    DonneeCard(int idDn, int card, int cible, flag exact = 0, idt id = 0)
        : DonneeCard(idDn, cible, id) {
        setCard(card);
        setExact(exact);
    }
};
ID1_ENTITY_NEG(DonneeCible,CibleNegDateTimeCurrentNumValeurVariant,infoEntity::DonneeCibleId,Donnee)
}
using namespace ensembleDonnee;

/*! \ingroup groupeEntity
 * \brief Ensemble des classes de la fonctionalité événement.
 */
namespace ensembleEvenement {
ID1_ENTITY(EvenementCible,CibleDateTimeCurrentNumTpValValeurVariant,infoEntity::EvenementCibleId,Evenement)
/*! \ingroup groupeEntity
 * \brief Représentation de l'entité Evenement.
 */
class Evenement : public EntityIDs<infoEntity::EvenementId, amps::Id1Attribut,
                                                            amps::NcNomTypeAttribut,
                                                            amps::CodeAttribut,
                                                            amps::DebutFinAttribut,
                                                            amps::CreaModifAttribut,
                                                            amps::TitreAttribut> {
protected:
    template<class T> using PositionEnum = PositionEnum<T,Evenement>;
public:
    //! Code.
    enum codeFlag : flag::flag_type {
        Aucun = 0x0,
        UnJour = 0x1,
        JourEntier = 0x2
    };
    using EAID = EntityIDs<infoEntity::EvenementId, amps::Id1Attribut,
                                                    amps::NcNomTypeAttribut,
                                                    amps::CodeAttribut,
                                                    amps::DebutFinAttribut,
                                                    amps::CreaModifAttribut,
                                                    amps::TitreAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut>::Position,
                       Id1 = PositionEnum<Id1Attribut>::Position,
                       Code = PositionEnum<CodeAttribut>::Position,
                       Creation = PositionEnum<CreationAttribut>::Position,
                       Debut = PositionEnum<DebutAttribut>::Position,
                       Fin = PositionEnum<FinAttribut>::Position,
                       Modification = PositionEnum<ModificationAttribut>::Position,
                       Nc = PositionEnum<NcAttribut>::Position,
                       Nom = PositionEnum<NomAttribut>::Position,
                       Titre = PositionEnum<TitreAttribut>::Position,
                       Type = PositionEnum<TypeAttribut>::Position,
                       NbrAtt = EAID::NbrAtt,
                       IdStyle = Id1};
    using EAID::EntityID;
    BASE_ENTITY(Evenement)
    ALIAS_CLE(Style,1)

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    Evenement(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    Evenement(idt idStyle, flag code, const QDateTime & creation, const QDateTime & debut, const QDateTime & fin,
              const QDateTime & modification, const QString & nc, const QString & nom, const QString & titre, idt type, idt id = 0)
        : Evenement(nom,id) {
        setIdStyle(idStyle);
        setCode(code);
        setCreation(creation);
        setDebut(debut);
        setFin(fin);
        setModification(modification);
        setNc(nc);
        setNom(nom);
        setTitre(titre);
        setType(type);
    }

    //! Teste si l'entité est valide.
    bool isValid() const override
        {return EAID::isValid() && debut() <= fin();}

};
/*! \ingroup groupeEntity
 * \brief Représentation de l'entité EvenementStyle.
 */
class EvenementStyle : public EntityIDs<infoEntity::EvenementStyleId, amps::BordureAttribut,
                                                                      amps::CouleurBordureAttribut,
                                                                      amps::CouleurFondAttribut,
                                                                      amps::CouleurTexteAttribut,
                                                                      amps::CouleurTitreAttribut,
                                                                      amps::FormeAttribut,
                                                                      amps::NomAttribut,
                                                                      amps::PoliceTexteAttribut,
                                                                      amps::PoliceTitreAttribut,
                                                                      amps::TextureAttribut>{
protected:
    template<class T> using PositionEnum = PositionEnum<T,EvenementStyle>;
public:
    enum {IdStyleDefault = 1};
    //! Forme.
    enum formeStyle {Rectangle,
                    NbrFormeStyle};
    using EAID = EntityIDs<infoEntity::EvenementStyleId,amps::BordureAttribut,
                                                        amps::CouleurBordureAttribut,
                                                        amps::CouleurFondAttribut,
                                                        amps::CouleurTexteAttribut,
                                                        amps::CouleurTitreAttribut,
                                                        amps::FormeAttribut,
                                                        amps::NomAttribut,
                                                        amps::PoliceTexteAttribut,
                                                        amps::PoliceTitreAttribut,
                                                        amps::TextureAttribut>;
    //! Positions des attributs.
    enum Position:szt {Id = PositionEnum<IdAttribut>::Position,
                       Bordure = PositionEnum<BordureAttribut>::Position,
                       CouleurBordure = PositionEnum<CouleurBordureAttribut>::Position,
                       CouleurFond = PositionEnum<CouleurFondAttribut>::Position,
                       CouleurTexte = PositionEnum<CouleurTexteAttribut>::Position,
                       CouleurTitre = PositionEnum<CouleurTitreAttribut>::Position,
                       Forme = PositionEnum<FormeAttribut>::Position,
                       Nom = PositionEnum<NomAttribut>::Position,
                       PoliceTexte = PositionEnum<PoliceTexteAttribut>::Position,
                       PolicceTitre = PositionEnum<PoliceTitreAttribut>::Position,
                       Texture = PositionEnum<TextureAttribut>::Position,
                       NbrAtt = EAID::NbrAtt};
    using EAID::EntityID;
    BASE_ENTITY(EvenementStyle)

    //! Constructeur à partir des valeurs d'un ensemble d'attributs unique.
    EvenementStyle(const QString & nom, idt id = 0)
        : EAID(id)
        {setNom(nom);}

    //! Constructeur à partir des valeurs attributs.
    EvenementStyle(int bordure, const QString & CouleurBordure, const QString & CouleurFond,
                   const QString & couleurTexte, const QString & couleurTitre,
                   int forme, const QString & nom, const QString & policeTexte, const QString & policeTitre, szt texture, idt id = 0)
        : EvenementStyle(nom,id) {
        setBordure(bordure);
        setCouleurBordure(CouleurBordure);
        setCouleurFond(CouleurFond);
        setCouleurTexte(couleurTexte);
        setCouleurTitre(couleurTitre);
        setForme(forme);
        setPoliceTexte(policeTexte);
        setPoliceTitre(policeTitre);
        setTexture(texture);
    }

    static QString formeString(int forme);
};
}
using namespace ensembleEvenement;

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
