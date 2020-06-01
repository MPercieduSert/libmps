#include "BddPredef.h"

using namespace bddMPS;
using namespace entityMPS;

flag BddPredef::code(const QString & str) const {
    if(str == "Interdit")
        return code::Interdit;
    if(str == "Visible")
        return code::Visible;
    if(str== "Attribuable")
        return code::Attribuable;
    return code::Invalide;
}

bool BddPredef::delP(idt id, szt idEntity) {
    auto controle = true;
    // Spécifique
    switch (idEntity) {
    case Commentaire::ID:
        controle = delList<CommentaireCible>(CommentaireCible::IdCommentaire,id);
        break;
    case Donnee::ID:
        controle = foreachBeginChild<Donnee>(id,
                        [this](idt id)->bool {
                         return delList<DonneeCible>(DonneeCible::IdDonnee,id)
                                && delList<DonneeCard>(DonneeCard::IdDonnee,id)
                                && delP(id,Donnee::ID); }
                      ,false);
        break;
    case MotCle::ID:
        controle = foreachBeginChild<MotCle>(id,
                        [this](idt id)->bool {
                        return delList<MotCleCible>(MotCleCible::IdMotCle,id)
                            && delList<MotClePermission>(MotClePermission::IdMotCle,id)
                            && delList<MotProgCible>(MotProgCible::IdMotCle,id)
                            && delList<MotProgPermission>(MotProgPermission::IdMotCle,id)
                            && delP(id,MotCle::ID);
            }
            ,false);
        break;
    case Source::ID:
        controle = delList<SourceTexte>(SourceTexte::IdSource,id);
        break;
    case Texte::ID:
        controle = delList<SourceTexte>(SourceTexte::IdTexte,id)
                    && delList<TexteCible>(TexteCible::IdTexte,id);
        break;
    case Type::ID:
        controle = foreachBeginChild<Type>(id,
                        [this](idt id)->bool{return delP(id,Type::ID);}
             ,false);
        break;
    }

    // Cible
    controle = controle && (!managers().commentaireIsEnabled()
                    || del<CommentaireCible>(getListId<CommentaireCible>(CommentaireCible::Cible,cible(idEntity),
                                                                         CommentaireCible::IdCible,id)))
                && (!managers().donneeIsEnabled()
                    || delList<DonneeCible>(DonneeCible::Cible,cible(idEntity),
                                            DonneeCible::IdCible,id))
                && (!managers().motCleIsEnabled()
                    || (delList<MotCleCible>(MotCleCible::Cible,cible(idEntity),
                                             MotCleCible::IdCible,id)
                        && delList<MotProgCible>(MotProgCible::Cible,cible(idEntity),
                                                 MotProgCible::IdCible,id)))
                && (!managers().texteIsEnabled()
                    || delList<TexteCible>(TexteCible::Cible,cible(idEntity),
                                           TexteCible::IdCible,id))
                && (!managers().restrictionModificationIsEnabled()
                    || delList<Restriction>(Restriction::Cible,cible(idEntity),
                                            Restriction::IdCible,id))
                && (!managers().historiqueIsEnabled()
                    || (delList<Historique>(Historique::Cible,cible(idEntity),
                                            Historique::IdCible,id)
                        && delList<Historique>(Historique::Etat,cible(idEntity),
                                               Historique::IdEtat,id)))
                && (!managers().utilisationIsEnabled()
                    ||(delList<Utilisation>(Utilisation::Cible,cible(idEntity),
                                            Utilisation::IdCible,id)
                        && delList<Utilisation>(Utilisation::Etat,cible(idEntity),
                                                Utilisation::IdEtat,id)));

    return controle && Bdd::delP(id,idEntity);
}

void BddPredef::delEntityInDonnee(idt idCible, int cible, int num) {
    if(num < 0)
        throw std::invalid_argument("L'argument num doit être positif");

    DonneeCible nature(Donnee::EntityNatureIdND, idCible, cible, num);
    if(get(nature)) {
        if(nature.valeur().toInt() != natureId::Vide)
        {
            std::pair<int, int> interval(intervalEntityInDonnee(idCible,cible,num));
            delList<DonneeCible>(std::vector<DonneeCible::Position>({DonneeCible::Num,
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
                                                                 std::vector<condition>({SupEgal, Inf}));
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
            nature.setValeur(natureId::Vide);
            save(nature);
        }
    }
}

bool BddPredef::testAutorisationP(idt id, szt idEntity, flag autoris) {
    auto controle = Bdd::testAutorisationP(id,idEntity,autoris);
    if(autoris & Suppr) {
        // Cible
        controle = controle && (!managers().commentaireIsEnabled()
            || testAutorisationList<CommentaireCible>(autoris,
                                                     CommentaireCible::Cible,cible(idEntity),
                                                     CommentaireCible::IdCible,id
                                                     ))
        && (!managers().donneeIsEnabled()
            || testAutorisationList<DonneeCible>(autoris, DonneeCible::Cible,cible(idEntity),
                                                DonneeCible::IdCible,id
                                                ))
        && (!managers().motCleIsEnabled()
            || testAutorisationList<MotCleCible>(autoris,
                                                MotCleCible::Cible,cible(idEntity),
                                                MotCleCible::IdCible,id
                                                ))
        && (!managers().texteIsEnabled()
            || testAutorisationList<TexteCible>(autoris,
                                               TexteCible::Cible,cible(idEntity),
                                               TexteCible::IdCible,id
                                               ))
        && (!managers().restrictionModificationIsEnabled()
            || testAutorisationList<Restriction>(autoris,
                                                Restriction::Cible,cible(idEntity),
                                                Restriction::IdCible,id
                                                ))
        && (!managers().historiqueIsEnabled()
            || (testAutorisationList<Historique>(autoris,
                                                Historique::Cible,cible(idEntity),
                                                Historique::IdCible,id
                                                )
                && testAutorisationList<Historique>(autoris,
                                                   Historique::Etat,cible(idEntity),
                                                   Historique::IdEtat,id
                                                   )))
        && (!managers().utilisationIsEnabled()
            ||  (testAutorisationList<Utilisation>(autoris,
                                                  Utilisation::Cible,cible(idEntity),
                                                  Utilisation::IdCible,id
                                                  )
                && testAutorisationList<Utilisation>(autoris,
                                                    Utilisation::Etat,cible(idEntity),
                                                    Utilisation::IdEtat,id
                                                    )));
        // Spécifique
        if(controle) {
            switch (idEntity) {
            case Commentaire::ID:
                controle = controle && testAutorisationList<CommentaireCible>(autoris,
                                                                             CommentaireCible::IdCommentaire,id);
                break;
            case Donnee::ID:
                controle = controle && foreachBeginChild<Donnee>(id,
                        [this,autoris](idt id)->bool {
                        return testAutorisationP(id,Donnee::ID,autoris)
                                && testAutorisationList<DonneeCible>(autoris,
                                                                    DonneeCible::IdDonnee,id)
                                && testAutorisationList<DonneeCard>(autoris,
                                                                   DonneeCard::IdDonnee,id);}
                      ,false);
                break;
            case MotCle::ID:
                controle = controle && foreachBeginChild<MotCle>(id,
                        [this,autoris](idt id)->bool {
                        return testAutorisationP(id,MotCle::ID,autoris)
                            && testAutorisationList<MotCleCible>(autoris,
                                                                MotCleCible::IdMotCle,id)
                            && testAutorisationList<MotClePermission>(autoris,
                                                                     MotClePermission::IdMotCle,id)
                            && testAutorisationList<MotProgCible>(autoris,
                                                                 MotProgCible::IdMotCle,id)
                            && testAutorisationList<MotProgPermission>(autoris,
                                                                      MotProgPermission::IdMotCle,id);}
                        ,false);
                break;
            case Source::ID:
                controle = controle && testAutorisationList<SourceTexte>(autoris,
                                                                        SourceTexte::IdSource,id);
                break;
            case Texte::ID:
                controle = controle && testAutorisationList<SourceTexte>(autoris,
                                                                        SourceTexte::IdTexte,id)
                            && testAutorisationList<TexteCible>(autoris,
                                                               TexteCible::IdTexte,id);
                break;
            case Type::ID:
                controle = foreachBeginChild<Type>(id,
                                [this,autoris](idt id)->bool{return testAutorisationP(id,Type::ID,autoris);}
                     ,false);
                break;
            }
        }
    }
    return controle;
}

QString BddPredef::hydrateAttributXml(entityMPS::Entity & entity, szt pos, fichierMPS::XmlDoc::const_brother_iterator iter){
    if(iter->name() == "Cible") {
        auto i = m_manager->find(iter->text());
        if(i == nbrEntity())
            return QString("Cible invalide : ").append(iter->text());
        else {
            entity.setData(pos, cible(i));
            return QString();
        }
    }
    if(iter->name() == "Code") {
        auto list = iter->text().split(",");
        attributMPS::CodeAttribut codeAtt;
        QString controle;
        for(auto iter_list = list.cbegin(); controle.isEmpty() && iter_list != list.cend(); ++iter_list) {
            auto cd = code(*iter_list);
            if (cd != code::Invalide)
                codeAtt.add(cd);
            else
                controle = QString("Code Invalide : ").append(*iter_list);
        }
        if(controle.isEmpty())
            entity.setData(pos, codeAtt.getToBdd());
        return controle;
    }

    return Bdd::hydrateAttributXml(entity, pos,iter);
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
    if(version == bddVersion::Initiale) {
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
//            Donnee dn;
//            dn.setIdProg(donnee::defaultValueId);
//            dn.setNom("Valeur par défaut");
//            dn.setType()
//            save(Donnee)
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
        managers().saveVersion(bddVersion::InitialePredef);
    }
}
