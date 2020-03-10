/*Auteur: PERCIE DU SERT Maxime
 *Date: 19/02/2016
 */
#ifndef BDDPREDEF_H
#define BDDPREDEF_H

#include "Bdd.h"
#include "ManagersPredef.h"
#include "EntityPredef.h"

namespace bddMPS {
    namespace bddVersion {
        enum creationBddPredef {InitialePredef = NbrCreationBdd,
                        NbrCreationBddPredef};}

    namespace code {
        enum invalide : codeType {Invalide = sizeof(codeType) + 1};
        enum permission : codeType {Interdit = 0,
                              Visible = 1,
                              Attribuable = 2,
                            NbrPermission};
    }

/*! \ingroup groupeFile
 * \brief Gestionnaire de la base de donnée pour les entités prédéfinie.
 *
 * Gestionnaire de la base de donnée pour les entités prédéfinie:
 *      - CommentaireCible,
 *      - DonneeCible,
 *      - MotCleCible,
 *      - TexteCible,
 *      - Commentaire,
 *      - Donnee,
 *      - DonneeCard,
 *      - Historique,
 *      - MotCle,
 *      - MotClePermission,
 *      - Restriction,
 *      - Source,
 *      - Texte,
 *      - SourceTexte,
 *      - TypeEnt,
 *      - TypePermission,
 *      - Utilisation
 */
class BddPredef : public Bdd {
public:
    //! Constructeur. Donner en argument le type ainsi que le chemin de la base de donnée.
    BddPredef(const QString & dbtype, int version, std::unique_ptr<managerMPS::ManagersPredef> && manager)
        : Bdd(dbtype,version,std::move(manager)) {}

    //! Constructeur. Donner en argument le type ainsi que le chemin de la base de donnée.
    BddPredef(const QString & dbtype, const QString & fileName, int version, std::unique_ptr<managerMPS::ManagersPredef> && manager)
        : Bdd(dbtype,fileName,version,std::move(manager)) {}

    //! Destructeur. Referme la base de donnée.
    ~BddPredef() override = default;

    //! Retourne le numéro de cible associé à l'identifiant d'une entité.
    int cible(szt id) const
        {return managers().cible(id);}

    //! Retourne le numéro de cible associé à un type d'entité.
    template<class Ent> int cible() const
        {return managers().cible<Ent>();}

    //! Suppresseur d'une entité enregistrée comme donnée.
    void delEntityInDonnee(idt idCible, int cible, int num = 0);

    //! Accesseur d'une entité enregistrée comme donnée.
    template<class Ent> Ent getEntityInDonnee(idt idCible, int cible, int num = 0);

    //! Renvoie le nombre des entités liées à la cible.
    virtual int nbrEntityCible(int /*cible*/) const
        {return 0;}//à finir

    //! Renvoie le nom des entités liées à la cible.
    virtual std::map<int,QString> nomsEntityCible(int /*cible*/) const
        {return std::map<int,QString>();}//à finir

    //! renvoie le numéro permission associée à la chaine de caractères, ou NbrPermission la chaine ne correspond à aucun numéro.
    virtual codeType code(const QString & str) const;

    //! Mutateur d'une entité enregistrée comme donnée.
    template<class Ent> void setEntityInDonnee(const Ent & entity, idt idCible, int cible, int num = 0);

protected:
    //! Supprime l'entité d'identifiant id de type d'identifiant idEntity de la base de données.
    bool delP(idt id, szt idEntity) override;

    //! Renvoie l'autorisation de modification de l'entité donnée en argument.
    bool getAutorisationP(idt id, szt idEntity, autorisation autoris) override;

    //! Hydrate un attribut de l'entité par la valeur contenue dans le XmlDox à l'endroit pointé par iter.
    QString hydrateAttributXml(entityMPS::Entity & entity, szt pos, fichierMPS::XmlDoc::const_brother_iterator iter) override;

    //! Renvoie le numero de début d'enregistrement d'une entité.
    std::pair<int, int> intervalEntityInDonnee(idt idCible, int cible, int num);

    //! Mise à jour de la base de donnée.
    void listeMiseAJourBdd(int version) override;

    //! Acceseur du manageur.
    const managerMPS::ManagersPredef & managers() const
        {return static_cast<const managerMPS::ManagersPredef &>(*m_manager);}

    //! Acceseur du manageur.
    managerMPS::ManagersPredef & managers()
        {return static_cast<managerMPS::ManagersPredef &>(*m_manager);}
};

//DEF_DEL_METHODE(BddPredef)

//template<class Ent> bool BddPredef::del(idt id)
//    {return delSimple<Ent>(id);}

//template<class Ent> bool BddPredef::delSimple(idt id)
//    {return getAutorisation(Ent(id),Suppr) && delCible(id, cible<Ent>()) && Bdd::del<Ent>(id);}

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
    for(szt i = 0; i != Ent::NbrAtt; ++i, ++j)
        entity.setData(j->valeur(),i);
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
