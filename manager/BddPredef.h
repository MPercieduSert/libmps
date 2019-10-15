/*Auteur: PERCIE DU SERT Maxime
 *Date: 19/02/2016
 */
#ifndef BDDPREDEF_H
#define BDDPREDEF_H

#include "Bdd.h"
#include "ManagersPredef.h"
#include "EntityPredef.h"

namespace bdd {
    namespace bddVersion {
        enum creationBddPredef {initialePredef = NbrCreationBdd,
                               NbrCreationBddPredef};}

    enum motClePermissionNum {InterditMCNum = 0,
                              PermisMCNum = 1};

}

/*! \ingroup groupeFile
 * \brief Gestionnaire de la base de donnée pour les entités prédéfinie.
 *
 * Gestionnaire de la base de donnée pour les entités prédéfinie:
 *      - CibleCommentaire,
 *      - CibleDonnee,
 *      - CibleMotCle,
 *      - CibleTexte,
 *      - Commentaire,
 *      - Donnee,
 *      - DonneeCard,
 *      - Historique,
 *      - MotCle,
 *      - MotClePermission,
 *      - Restriction,
 *      - Source,
 *      - Texte,
 *      - TexteSource,
 *      - TypeEnt,
 *      - TypePermission,
 *      - Utilisation
 */
class BddPredef : public Bdd
{
public:
    //! Constructeur. Donner en argument le type ainsi que le chemin de la base de donnée.
    BddPredef(const QString & dbtype, int version, ManagersPredef * manager)
        : Bdd(dbtype,version,manager)
    {}

    //! Constructeur. Donner en argument le type ainsi que le chemin de la base de donnée.
    BddPredef(const QString & dbtype, const QString & fileName, int version, ManagersPredef * manager)
        : Bdd(dbtype,fileName,version,manager)
    {}

    //! Destructeur. Referme la base de donnée.
    virtual ~BddPredef() override = default;

    //! Retourne le numéro de cible associé à l'identifiant d'une entité.
    int cible(int id) const
        {return managers()->cible(id);}

    //! Retourne le numéro de cible associé à un type d'entité.
    template<class Ent> int cible() const
        {return managers()->cible<Ent>();}

    DECL_DEL_METHODE

    //! Suppresseur d'une entité enregistrée comme donnée.
    void delEntityInDonnee(int idCible, int cible, int num = 0);

    //! Accesseur d'une entité enregistrée comme donnée.
    template<class Ent> Ent getEntityInDonnee(int idCible, int cible, int num = 0);

    //! Renvoie le nombre des entités liées à la cible.
    virtual int nbrEntityCible(int /*cible*/) const
        {return 0;}//à finir

    //! Renvoie le nom des entités liées à la cible.
    virtual QMap<int,QString> nomsEntityCible(int /*cible*/) const
        {return QMap<int,QString>();}//à finir

    //! Renvoie l'identifiant de la donnée d'idProg idP fourni ou -1 si elle n'existe pas.
    int idDonnee(int idP);

    //! Mutateur d'une entité enregistrée comme donnée.
    template<class Ent> void setEntityInDonnee(const Ent & entity, int idCible, int cible, int num = 0);

protected:
    //! Création de la table de l'entité en base de donnée.
    template<class Ent> void creerTable()
        {m_manager->get<Ent>()->creer();}

    //! Suppresseur d'une entité à partir de son identifiant.
    template<class Ent> bool del(int id);

    //! Suppresseur d'un entité à partir de son identitiant et de ses dépendance de type cible.
    template<class Ent> bool delSimple(int id);

    //! Suppresseur des entités de dont idCible est l'identifiant de type cible.
    bool delCible(int idCible, int Cible);

    //! Mise à jour de la base de donnée.
    void listeMiseAJourBdd(int version) override;

    //! Renvoie le numero de début d'enregistrement d'une entité.
    QPair<int, int> intervalEntityInDonnee(int idCible, int cible, int num);

    //! Acceseur du manageur.
    ManagersPredef * managers() const
        {return static_cast<ManagersPredef *>(m_manager);}
};

DEF_DEL_METHODE(BddPredef)

template<class Ent> bool BddPredef::del(int id)
    {return delSimple<Ent>(id);}

template<class Ent> bool BddPredef::delSimple(int id)
    {return getAutorisation(Ent(id),bdd::Suppr) && delCible(id, cible<Ent>()) && Bdd::del<Ent>(id);}

template<class Ent> Ent BddPredef::getEntityInDonnee(int idCible, int Cible, int num)
{
    CibleDonnee nature(Donnee::EntityNatureIdND,idCible,Cible,num);
    if(!get(nature) || nature.valeur().toInt() == bdd::cibleId::Vide || nature.valeur().toInt() != cible<Ent>())
        return Ent();

    QPair<int,int> interval(intervalEntityInDonnee(idCible,Cible,num));
    ListPtr<CibleDonnee> liste (getList<CibleDonnee>(QList<CibleDonnee::Position>()<<CibleDonnee::Num
                                                     <<CibleDonnee::Num
                                                     <<CibleDonnee::IdDonnee
                                                     <<CibleDonnee::IdCible
                                                     <<CibleDonnee::Cible,
                                                     QList<QVariant>()<<interval.first
                                                     <<interval.second
                                                     <<Donnee::EntityIdND
                                                     <<idCible
                                                     <<Cible,
                                                     QList<CibleDonnee::Position>()<<CibleDonnee::Num,
                                                     QList<bdd::Condition>()<<bdd::SupEgal<<bdd::Inf));
    Ent entity;
    ListPtr<CibleDonnee>::iterator j = liste.begin();
    for(int i = 0; i != Ent::NbrAtt; ++i, ++j)
        entity.setData((*j).valeur(),i);
    return entity;
}

template<class Ent> void BddPredef::setEntityInDonnee(const Ent & entity, int idCible, int Cible, int num)
{
    if(num < 0)
        throw std::invalid_argument("L'argument num doit être positif");

    CibleDonnee nature(Donnee::EntityNatureIdND, idCible, Cible, num);
    if(get(nature))
    {
        if(nature.valeur().toInt() != cible<Ent>())
        {
            QPair<int, int> intervalOld(intervalEntityInDonnee(idCible,Cible,num));
            int offset(Ent::NbrAtt - (intervalOld.second - intervalOld.first));
            if(offset != 0)
            {
                ListPtr<CibleDonnee> liste (getList<CibleDonnee>(QList<CibleDonnee::Position>()<<CibleDonnee::Num
                                                                 <<CibleDonnee::IdDonnee
                                                                 <<CibleDonnee::IdCible
                                                                 <<CibleDonnee::Cible,
                                                                 QList<QVariant>()<<intervalOld.second
                                                                 <<Donnee::EntityIdND
                                                                 <<idCible
                                                                 <<Cible,
                                                                 QList<CibleDonnee::Position>()<<CibleDonnee::Num,
                                                                 QList<bdd::Condition>()<<bdd::SupEgal,
                                                                 QList<bool>()<< (offset < 0)));
                for(ListPtr<CibleDonnee>::iterator i = liste.begin(); i != liste.end(); ++i)
                {
                    (*i).setNum((*i).num() + offset);
                    save(*i);
                }
            }
            nature.setValeur(cible<Ent>());
            save(nature);
        }
    }
    else
    {
        if(num != 0)
        {
            CibleDonnee natureNumPrec(Donnee::EntityNatureIdND, idCible, Cible, num - 1);
            natureNumPrec.setValeur(bdd::cibleId::Vide);
            while(natureNumPrec.num() >= 0 && !existsUnique(natureNumPrec))
            {
                save(natureNumPrec);
                natureNumPrec.setNum(natureNumPrec.num() - 1);
            }
        }
        nature.setValeur(cible<Ent>());
        save(nature);
    }

    QPair<int, int> interval(intervalEntityInDonnee(idCible,Cible,num));
    CibleDonnee cd;
    cd.setId1(Donnee::EntityIdND);
    cd.setIdCible(idCible);
    cd.setCible(Cible);
    for(int i = 0; i != Ent::NbrAtt; ++i)
    {
        cd.setId(0);
        cd.setNum(interval.first + i);
        cd.setValeur(entity.data(i));
        saveUnique(cd);
    }
}

#endif // BDDPREDEF_H
