/*Auteur: PERCIE DU SERT Maxime
 *Date: 19/02/2016
 */
#ifndef BDDPREDEF_H
#define BDDPREDEF_H

#include <QFont>
#include "Bdd.h"
#include "ManagersPredef.h"
#include "EntityPredef.h"

namespace bddMPS {
    namespace bddVersion {
        enum creationBddPredef {CreationPredef = NbrVersion,
                        NbrVersionPredef};}

    namespace code {
        enum invalide : flag::flag_type {Invalide = 0x1000000};
        enum permission : flag::flag_type {Interdit = 0,
                              Visible = 0x1,
                              Attribuable = 0x2};
    }

/*! \ingroup groupeFile
 * \brief Gestionnaire de la base de donnée pour les entités prédéfinie.
 *
 * Gestionnaire de la base de donnée pour les entités prédéfinie:
 *      - Commentaire,
 *      - CommentaireCible,
 *      - Donnee,
 *      - DonneeCard,
 *      - DonneeCible,
 *      - Evenement,
 *      - EvenementCible,
 *      - EvenementStyle,
 *      - Historique,
 *      - MotCle,
 *      - MotCleCible,
 *      - MotClePermission,
 *      - Restriction,
 *      - Source,
 *      - SourceTexte,
 *      - Texte,
 *      - TexteCible,
 *      - Type,
 *      - TypePermission,
 *      - Utilisation
 */
class BddPredef : public Bdd {
public:
    enum {InvalideEnum = 0x10000};
    //! Constructeur. Donner en argument le type ainsi que le chemin de la base de donnée.
    BddPredef(const QString & dbtype, const std::vector<int> & version, std::unique_ptr<managerMPS::ManagersPredef> && manager)
        : Bdd(dbtype,version,std::move(manager)) {}

    //! Constructeur. Donner en argument le type ainsi que le chemin de la base de donnée.
    BddPredef(const QString & dbtype, const QString & fileName, const std::vector<int> & version,
              std::unique_ptr<managerMPS::ManagersPredef> && manager)
        : Bdd(dbtype,fileName,version,std::move(manager)) {}

    //! Destructeur. Referme la base de donnée.
    ~BddPredef() override = default;

    //! Retourne le numéro de cible associé à l'identifiant d'une entité.
    int cible(entidt id) const
        {return managers().cible(id);}

    //! Retourne le numéro de cible associé à un type d'entité.
    template<class Ent> int cible() const
        {return managers().cible<Ent>();}

    //! Renvoie le drapeau associé à str pour une entitée de type idEntity.
    virtual flag code(idt idEntity, const QString & str) const;

    //! Suppresseur d'une entité enregistrée comme donnée.
    void delEntityInDonnee(idt idCible, int cible, int num = 0);

    //! Accesseur d'une entité enregistrée comme donnée.
    template<class Ent> Ent getEntityInDonnee(idt idCible, int cible, int num = 0);

    //! Acceseur du manageur.
    const managerMPS::ManagersPredef & managers() const
        {return static_cast<const managerMPS::ManagersPredef &>(*m_manager);}

    //! Acceseur du manageur.
    managerMPS::ManagersPredef & managers()
        {return static_cast<managerMPS::ManagersPredef &>(*m_manager);}

    //! Renvoie le nombre des entités liées à la cible.
    virtual int nbrEntityCible(int /*cible*/) const
        {return 0;}//à finir

    //! Renvoie le nom des entités liées à la cible.
    virtual std::map<int,QString> nomsEntityCible(int /*cible*/) const
        {return std::map<int,QString>();}//à finir

    //! Mutateur d'une entité enregistrée comme donnée.
    template<class Ent> void setEntityInDonnee(const Ent & entity, idt idCible, int cible, int num = 0);

    //! Renvoie l'enumeration associé à str pour une entitée de type idEntity.
    virtual int strToEnum(idt idEntity, const QString & str) const;
protected:
    //! Enregistre les données xml associées à une entité dans la base de donnée.
    void associatedXml(Entity & entity, xml_iterator iter, QString & controle) override;

    //! Supprime l'entité d'identifiant id de type d'identifiant idEntity de la base de données.
    bool delP(idt id, entidt idEntity) override;

    //! Hydrate un attribut de l'entité par la valeur contenue dans le XmlDox à l'endroit pointé par iter.
    void hydrateAttributXml(entityMPS::Entity & entity, post pos, xml_iterator iter, QString & controle) override;

    //! Hydrate un attribut de l'entité entity_ass associée à entity avec le couple pair<clé,valeur>.
    void hydrateAttributAssociatedXml(Entity & entity_ass, const std::pair<const QString,QString> & pair,
                                      const Entity & entity, QString & controle) override;

    //! Renvoie le numero de début d'enregistrement d'une entité.
    std::pair<int, int> intervalEntityInDonnee(idt idCible, int cible, int num);

    //! Teste si un attribut d'une entité associée est multiple.
    bool isMultipleAssociatedXml(const QString & att) const override;

    //! Mise à jour de la base de donnée.
    void listeMiseAJourBdd(int version, idt type) override;

    //! Enregistre un mot clé dans la base de donnée.
    void motCleXml(Entity & entity, xml_iterator iter, QString & controle);

    //! Enregistre une permission dans la base de donnée.
    void permissionXml(Entity & entity, xml_iterator iter, QString & controle);

    //! Renvoie l'autorisation de modification de l'entité donnée en argument.
    bool testAutorisationP(idt id, entidt idEntity, flag autoris) override;
};

template<class Ent> Ent BddPredef::getEntityInDonnee(idt idCible, int Cible, int num)
{
    using namespace entityMPS;
    DonneeCible nature(Donnee::EntityNatureIdND,idCible,Cible,num);
    if(!get(nature) || nature.valeur().toInt() == natureId::Vide || nature.valeur().toInt() != cible<Ent>())
        return Ent();

    std::pair<int,int> interval(intervalEntityInDonnee(idCible,Cible,num));
    auto liste = getList<DonneeCible>(std::vector<DonneeCible::Position>({DonneeCible::Num,
                                                     DonneeCible::Num,
                                                     DonneeCible::IdDonnee,
                                                     DonneeCible::IdCible,
                                                     DonneeCible::Cible}),
                                                     std::vector<QVariant>({interval.first,
                                                     interval.second,
                                                     Donnee::EntityIdND,
                                                     idCible,
                                                     Cible}),
                                                     std::vector<DonneeCible::Position>({DonneeCible::Num}),
                                                     std::vector<condition>({SupEgal, Inf}));
    Ent entity;
    auto j = liste.begin();
    for(post i = 0; i != Ent::NbrAtt; ++i, ++j)
        entity.setData(i,j->valeur());
    return entity;
}

template<class Ent> void BddPredef::setEntityInDonnee(const Ent & entity, idt idCible, int Cible, int num) {
    using namespace entityMPS;

    if(num < 0)
        throw std::invalid_argument("L'argument num doit être positif");

    DonneeCible nature(Donnee::EntityNatureIdND, idCible, Cible, num);
    if(get(nature)) {
        if(nature.valeur().toInt() != cible<Ent>()) {
            std::pair<int, int> intervalOld(intervalEntityInDonnee(idCible,Cible,num));
            auto  offset = Ent::NbrAtt - (intervalOld.second - intervalOld.first);
            if(offset != 0) {
                auto liste  = getList<DonneeCible>(std::vector<DonneeCible::Position>({DonneeCible::Num,
                                                                 DonneeCible::IdDonnee,
                                                                 DonneeCible::IdCible,
                                                                 DonneeCible::Cible}),
                                                                 std::vector<QVariant>({intervalOld.second,
                                                                 Donnee::EntityIdND,
                                                                 idCible,
                                                                 Cible}),
                                                                 std::vector<DonneeCible::Position>({DonneeCible::Num}),
                                                                 std::vector<condition>({SupEgal}),
                                                                 std::vector<bool>({offset < 0}));
                for(auto i = liste.begin(); i != liste.end(); ++i) {
                    i->setNum(i->num() + offset);
                    save(*i);
                }
            }
            nature.setValeur(cible<Ent>());
            save(nature);
        }
    }
    else {
        if(num != 0) {
            DonneeCible natureNumPrec(Donnee::EntityNatureIdND, idCible, Cible, num - 1);
            natureNumPrec.setValeur(natureId::Vide);
            while(natureNumPrec.num() >= 0 && !existsUnique(natureNumPrec)) {
                save(natureNumPrec);
                natureNumPrec.setNum(natureNumPrec.num() - 1);
            }
        }
        nature.setValeur(cible<Ent>());
        save(nature);
    }

    std::pair<int, int> interval(intervalEntityInDonnee(idCible,Cible,num));
    DonneeCible cd;
    cd.setId1(Donnee::EntityIdND);
    cd.setIdCible(idCible);
    cd.setCible(Cible);
    for(int i = 0; i != Ent::NbrAtt; ++i) {
        cd.setId(0);
        cd.setNum(interval.first + i);
        cd.setValeur(entity.data(i));
        saveUnique(cd);
    }
}
}
#endif // BDDPREDEF_H
