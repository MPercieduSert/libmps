#include "BddPredef.h"

template<> bool BddPredef::del<Commentaire>(idt id)
    {return delList(getList<CibleCommentaire>(CibleCommentaire::IdCommentaire,id)) && delSimple<Commentaire>(id);}

template<> bool BddPredef::del<Donnee>(idt id)
{
    return foreachBeginChild<Donnee>(id,
                                     [this](idt id)->bool
    {
            return delList(getList<CibleDonnee>(CibleDonnee::IdDonnee,id))
                    && delList(getList<DonneeCard>(DonneeCard::IdDonnee,id))
                    && delSimple<Donnee>(id);
    }
    ,false);
}

template<> bool BddPredef::del<MotCle>(idt id)
{
    return foreachBeginChild<MotCle>(id,
                                     [this](idt id)->bool
    {
            return delList(getList<CibleMotCle>(CibleMotCle::IdMotCle,id))
                    && delList(getList<MotClePermission>(MotClePermission::IdMotCle,id))
                    && delSimple<MotCle>(id);
    }
    ,false);
}

template<> bool BddPredef::del<Source>(idt id)
    {return delList(getList<TexteSource>(TexteSource::IdSource,id)) && delSimple<Source>(id);}

template<> bool BddPredef::del<Texte>(idt id)
    {return delList(getList<TexteSource>(TexteSource::IdTexte,id))
            && delList(getList<CibleTexte>(CibleTexte::IdTexte,id))
            && delSimple<Texte>(id);}

/*template<> bool BddPredef::del<TypeEnt>(idt id)
{
    return foreachBeginChild<TypeEnt>(id,
                                     [this](idt id)->bool
    {
            return delList(getList<Utilisation>(Utilisation::IdTp,id))
                    && delSimple<TypeEnt>(id);
    }
    ,false);
}*/

bool BddPredef::delCible(idt idCible, int cible)
{
    return (!managers()->commentaireIsEnabled() || delList(getList<CibleCommentaire>(CibleCommentaire::Cible,cible,CibleCommentaire::IdCible,idCible)))
            && (!managers()->donneeIsEnabled() || delList(getList<CibleDonnee>(CibleDonnee::Cible,cible,CibleDonnee::IdCible,idCible)))
            && (!managers()->motCleIsEnabled() || delList(getList<CibleMotCle>(CibleMotCle::Cible,cible,CibleMotCle::IdCible,idCible)))
            && (!managers()->texteIsEnabled()|| delList(getList<CibleTexte>(CibleTexte::Cible,cible,CibleTexte::IdCible,idCible)))
            && (!managers()->restrictionModificationIsEnabled() || delList(getList<Restriction>(Restriction::Cible,
                                                               cible,Restriction::IdCible)))
            && (!managers()->historiqueIsEnabled() || (delList(getList<Historique>(Historique::Cible,cible,Historique::IdCible))
                                       && delList(getList<Historique>(Historique::Etat,cible,Historique::IdEtat))))
            && (!managers()->utilisationIsEnabled() || (delList(getList<Utilisation>(Utilisation::Cible,cible,Utilisation::IdCible))
                                       && delList(getList<Utilisation>(Utilisation::Etat,cible,Utilisation::IdEtat))));
}

void BddPredef::delEntityInDonnee(idt idCible, int cible, int num)
{
    if(num < 0)
        throw std::invalid_argument("L'argument num doit être positif");

    CibleDonnee nature(Donnee::EntityNatureIdND, idCible, cible, num);
    if(get(nature))
    {
        if(nature.valeur().toInt() != bdd::cibleId::Vide)
        {
            QPair<int, int> interval(intervalEntityInDonnee(idCible,cible,num));
            delList(getList<CibleDonnee>(QList<CibleDonnee::Position>()<<CibleDonnee::Num
                                                                 <<CibleDonnee::Num
                                                                 <<CibleDonnee::IdDonnee
                                                                 <<CibleDonnee::IdCible
                                                                 <<CibleDonnee::Cible,
                                                                 QList<QVariant>()<<interval.first
                                                                 <<interval.second
                                                                 <<Donnee::EntityIdND
                                                                 <<idCible
                                                                 <<cible,
                                                                 QList<CibleDonnee::Position>()<<CibleDonnee::Num,
                                                                 QList<bdd::Condition>()<<bdd::SupEgal<<bdd::Inf));
            ListPtr<CibleDonnee> liste (getList<CibleDonnee>(QList<CibleDonnee::Position>()<<CibleDonnee::Num
                                                             <<CibleDonnee::IdDonnee
                                                             <<CibleDonnee::IdCible
                                                             <<CibleDonnee::Cible,
                                                             QList<QVariant>()<<interval.second
                                                             <<Donnee::EntityIdND
                                                             <<idCible
                                                             <<cible,
                                                             QList<CibleDonnee::Position>()<<CibleDonnee::Num,
                                                             QList<bdd::Condition>()<<bdd::SupEgal));
            for(ListPtr<CibleDonnee>::iterator i = liste.begin(); i != liste.end(); ++i)
            {
                (*i).setNum((*i).num() - (interval.second - interval.first));
                save(*i);
            }
            nature.setValeur(bdd::cibleId::Vide);
            save(nature);
        }
    }
}

idt BddPredef::idDonnee(idt idP)
{
     Donnee dn(idP,0);
     return existsUnique(dn) ? dn.id() : 0;
}

QPair<int, int> BddPredef::intervalEntityInDonnee(idt idCible, int cible, int num)
{
    if(num < 0)
        return QPair<int, int>(-1,-1);
    ListPtr<CibleDonnee> listeNum (getList<CibleDonnee>(QList<CibleDonnee::Position>()<<CibleDonnee::Num
                                                         <<CibleDonnee::IdDonnee
                                                         <<CibleDonnee::IdCible
                                                         <<CibleDonnee::Cible,
                                                         QList<QVariant>()<<num
                                                         <<Donnee::EntityNatureIdND
                                                         <<idCible
                                                         <<cible,
                                                         QList<CibleDonnee::Position>()<<CibleDonnee::Num,
                                                         QList<bdd::Condition>()<<bdd::InfEgal));
    QPair<int, int> interval(0,0);
    for(auto i = listeNum.begin(); i != listeNum.end(); ++i)
    {
        interval.first = interval.second;
        interval.second += managers()->nombreAttributCible((*i).valeur().toInt());
    }
    return interval;
}

void BddPredef::listeMiseAJourBdd(int version)
{
    if(version == bdd::bddVersion::initiale)
    {
        //Commentaire
        if(managers()->commentaireIsEnabled())
        {
            creerTable<Commentaire>();
            creerTable<CibleCommentaire>();
        }
        //Donnee
        if(managers()->donneeIsEnabled())
        {
            creerTable<Donnee>();
            creerTable<CibleDonnee>();
            creerTable<DonneeCard>();
        }
        //Historique
        if(managers()->historiqueIsEnabled())
            creerTable<Historique>();
        //Mot Cle
        if(managers()->motCleIsEnabled())
        {
            creerTable<MotCle>();
            creerTable<CibleMotCle>();
            creerTable<MotClePermission>();
        }
        //Restriction Modification
        if(managers()->restrictionModificationIsEnabled())
            creerTable<Restriction>();
        //Texte
        if(managers()->texteIsEnabled())
        {
            creerTable<Texte>();
            creerTable<CibleTexte>();
            creerTable<Source>();
            creerTable<TexteSource>();
        }
        //Type
        if(managers()->typeIsEnabled())
        {
            creerTable<Type>();
            creerTable<TypePermission>();
        }
        //Utilisation
        if(managers()->utilisationIsEnabled())
        {
            creerTable<Utilisation>();
            creerTable<Usage>();
        }
        //Mise à jour de la version de la base de donnée.
        managers()->saveVersion(bdd::bddVersion::initialePredef);
    }
}
