#include "BddPredef.h"

using namespace bddMPS;
using namespace entityMPS;

template<> bool BddPredef::del<Commentaire>(idt id)
    {return delList(getList<CommentaireCible>(CommentaireCible::IdCommentaire,id)) && delSimple<Commentaire>(id);}

template<> bool BddPredef::del<Donnee>(idt id) {
    return foreachBeginChild<Donnee>(id,
                                     [this](idt id)->bool {
            return delList(getList<DonneeCible>(DonneeCible::IdDonnee,id))
                    && delList(getList<DonneeCard>(DonneeCard::IdDonnee,id))
                    && delSimple<Donnee>(id);
    }
    ,false);
}

template<> bool BddPredef::del<MotCle>(idt id) {
    return foreachBeginChild<MotCle>(id,
                                     [this](idt id)->bool {
            return delList(getList<MotCleCible>(MotCleCible::IdMotCle,id))
                    && delList(getList<MotClePermission>(MotClePermission::IdMotCle,id))
                    && delSimple<MotCle>(id);
    }
    ,false);
}

template<> bool BddPredef::del<Source>(idt id)
    {return delList(getList<SourceTexte>(SourceTexte::IdSource,id)) && delSimple<Source>(id);}

template<> bool BddPredef::del<Texte>(idt id)
    {return delList(getList<SourceTexte>(SourceTexte::IdTexte,id))
            && delList(getList<TexteCible>(TexteCible::IdTexte,id))
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

bool BddPredef::delCible(idt idCible, int cible) {
    return (!managers().commentaireIsEnabled() || delList(getList<CommentaireCible>(CommentaireCible::Cible,cible,CommentaireCible::IdCible,idCible)))
            && (!managers().donneeIsEnabled() || delList(getList<DonneeCible>(DonneeCible::Cible,cible,DonneeCible::IdCible,idCible)))
            && (!managers().motCleIsEnabled() || delList(getList<MotCleCible>(MotCleCible::Cible,cible,MotCleCible::IdCible,idCible)))
            && (!managers().texteIsEnabled()|| delList(getList<TexteCible>(TexteCible::Cible,cible,TexteCible::IdCible,idCible)))
            && (!managers().restrictionModificationIsEnabled() || delList(getList<Restriction>(Restriction::Cible,
                                                               cible,Restriction::IdCible)))
            && (!managers().historiqueIsEnabled() || (delList(getList<Historique>(Historique::Cible,cible,Historique::IdCible))
                                       && delList(getList<Historique>(Historique::Etat,cible,Historique::IdEtat))))
            && (!managers().utilisationIsEnabled() || (delList(getList<Utilisation>(Utilisation::Cible,cible,Utilisation::IdCible))
                                       && delList(getList<Utilisation>(Utilisation::Etat,cible,Utilisation::IdEtat))));
}

void BddPredef::delEntityInDonnee(idt idCible, int cible, int num) {
    if(num < 0)
        throw std::invalid_argument("L'argument num doit être positif");

    DonneeCible nature(Donnee::EntityNatureIdND, idCible, cible, num);
    if(get(nature)) {
        if(nature.valeur().toInt() != cibleId::Vide)
        {
            std::pair<int, int> interval(intervalEntityInDonnee(idCible,cible,num));
            delList(getList<DonneeCible>(std::vector<DonneeCible::Position>({DonneeCible::Num,
                                                                 DonneeCible::Num,
                                                                 DonneeCible::IdDonnee,
                                                                 DonneeCible::IdCible,
                                                                 DonneeCible::Cible}),
                                                                 std::vector<QVariant>({interval.first,
                                                                 interval.second,
                                                                 Donnee::EntityIdND,
                                                                 idCible,
                                                                 cible}),
                                                                 std::vector<DonneeCible::Position>({DonneeCible::Num}),
                                                                 std::vector<condition>({SupEgal, Inf})));
            auto liste = getList<DonneeCible>(std::vector<DonneeCible::Position>({DonneeCible::Num,
                                                             DonneeCible::IdDonnee,
                                                             DonneeCible::IdCible,
                                                             DonneeCible::Cible}),
                                                             std::vector<QVariant>({interval.second,
                                                             Donnee::EntityIdND,
                                                             idCible,
                                                             cible}),
                                                             std::vector<DonneeCible::Position>({DonneeCible::Num}),
                                                             std::vector<condition>({SupEgal}));
            for(auto i = liste.begin(); i != liste.end(); ++i) {
                i->setNum(i->num() - (interval.second - interval.first));
                save(*i);
            }
            nature.setValeur(cibleId::Vide);
            save(nature);
        }
    }
}

idt BddPredef::idDonnee(idt idP) {
     Donnee dn(idP,0);
     return existsUnique(dn) ? dn.id() : 0;
}

std::pair<int, int> BddPredef::intervalEntityInDonnee(idt idCible, int cible, int num) {
    if(num < 0)
        return std::pair<int, int>(-1,-1);
    auto listeNum = getList<DonneeCible>(std::vector<DonneeCible::Position>({DonneeCible::Num,
                                                         DonneeCible::IdDonnee,
                                                         DonneeCible::IdCible,
                                                         DonneeCible::Cible}),
                                                         std::vector<QVariant>({num,
                                                         Donnee::EntityNatureIdND,
                                                         idCible,
                                                         cible}),
                                                         std::vector<DonneeCible::Position>({DonneeCible::Num}),
                                                         std::vector<condition>({InfEgal}));
    std::pair<int, int> interval(0,0);
    for(auto i = listeNum.begin(); i != listeNum.end(); ++i) {
        interval.first = interval.second;
        interval.second += managers().nombreAttributCible(i->valeur().toInt());
    }
    return interval;
}

void BddPredef::listeMiseAJourBdd(int version) {
    if(version == bddVersion::initiale) {
        //Type
        if(managers().typeIsEnabled()) {
            creerTable<Type>();
            creerTable<TypePermission>();
        }
        //Commentaire
        if(managers().commentaireIsEnabled()) {
            creerTable<Commentaire>();
            creerTable<CommentaireCible>();
        }
        //Donnee
        if(managers().donneeIsEnabled()) {
            creerTable<Donnee>();
            creerTable<DonneeCible>();
            creerTable<DonneeCard>();
        }
        //Historique
        if(managers().historiqueIsEnabled())
            creerTable<Historique>();
        //Mot Cle
        if(managers().motCleIsEnabled()) {
            creerTable<MotCle>();
            creerTable<MotCleCible>();
            creerTable<MotClePermission>();
            creerTable<MotProgCible>();
            creerTable<MotProgPermission>();
        }
        //Restriction Modification
        if(managers().restrictionModificationIsEnabled())
            creerTable<Restriction>();
        //Texte
        if(managers().texteIsEnabled()) {
            creerTable<Texte>();
            creerTable<TexteCible>();
            creerTable<Source>();
            creerTable<SourceTexte>();
        }
        //Utilisation
        if(managers().utilisationIsEnabled()) {
            creerTable<Utilisation>();
            creerTable<Usage>();
        }
        //Mise à jour de la version de la base de donnée.
        managers().saveVersion(bddVersion::initialePredef);
    }
}
