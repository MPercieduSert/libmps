#include "BddPredef.h"

using namespace bddMPS;
using namespace entityMPS;

flag BddPredef::code(idt idEntity, const QString & str) const {
    switch (idEntity) {
    case TypePermission::ID:
        if(str == "Interdit")
            return code::Interdit;
        if(str == "Visible")
            return code::Visible;
        if(str== "Attribuable")
            return code::Attribuable;
        break;
    default:
        return code::Invalide;
    }
    return code::Invalide;
}

bool BddPredef::delP(idt id, entidt idEntity) {
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
                            && delP(id,MotCle::ID); }
                        ,false);
        break;
    case Source::ID:
        controle = delList<SourceTexte>(SourceTexte::IdSource,id);
        break;
    case Evenement::ID:
        controle = delList<EvenementCible>(EvenementCible::IdEvenement,id);
        break;
    case EvenementStyle::ID:
        if(id != EvenementStyle::IdStyleDefault) {
            auto vec = getList<Evenement>(Evenement::IdStyle,id);
            for (auto iter = vec.begin(); iter != vec.end(); ++iter)
                iter->setIdStyle(EvenementStyle::IdStyleDefault);
            controle = true;
        }
        else
            controle = false;
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
    case Usage::ID:
        controle = foreachBeginChild<Usage>(id,
                        [this](idt id)->bool {
                         return delList<Utilisation>(Utilisation::IdUsage,id)
                                && delP(id,Usage::ID); }
                      ,false);
        break;
    }

    // Cible
    controle = controle && (!managers().ensembleEnable(CommentaireEnable)
                    || del<CommentaireCible>(getListId<CommentaireCible>(CommentaireCible::Cible,cible(idEntity),
                                                                         CommentaireCible::IdCible,id)))
                && (!managers().ensembleEnable(DonneeEnable)
                    || delList<DonneeCible>(DonneeCible::Cible,cible(idEntity),
                                            DonneeCible::IdCible,id))
                && (!managers().ensembleEnable(EvenementEnable)
                    || delList<EvenementCible>(EvenementCible::Cible,cible(idEntity),
                                            EvenementCible::IdCible,id))
                && (!managers().ensembleEnable(MotCleEnable)
                    || (delList<MotCleCible>(MotCleCible::Cible,cible(idEntity),
                                             MotCleCible::IdCible,id)
                        && delList<MotProgCible>(MotProgCible::Cible,cible(idEntity),
                                                 MotProgCible::IdCible,id)))
                && (!managers().ensembleEnable(TexteEnable)
                    || delList<TexteCible>(TexteCible::Cible,cible(idEntity),
                                           TexteCible::IdCible,id))
                && (!managers().ensembleEnable(RestrictionEnable)
                    || delList<Restriction>(Restriction::Cible,cible(idEntity),
                                            Restriction::IdCible,id))
                && (!managers().ensembleEnable(HistoriqueEnable)
                    || (delList<Historique>(Historique::Cible,cible(idEntity),
                                            Historique::IdCible,id)
                        && delList<Historique>(Historique::Etat,cible(idEntity),
                                               Historique::IdEtat,id)))
                && (!managers().ensembleEnable(UtilisationEnable)
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

bool BddPredef::testAutorisationP(idt id, entidt idEntity, flag autoris) {
    auto controle = Bdd::testAutorisationP(id,idEntity,autoris);
    if(autoris & Suppr) {
        // Cible
        controle = controle && (!managers().ensembleEnable(CommentaireEnable)
            || testAutorisationList<CommentaireCible>(autoris,
                                                     CommentaireCible::Cible,cible(idEntity),
                                                     CommentaireCible::IdCible,id))
        && (!managers().ensembleEnable(DonneeEnable)
            || testAutorisationList<DonneeCible>(autoris, DonneeCible::Cible,cible(idEntity),
                                                DonneeCible::IdCible,id))
        && (!managers().ensembleEnable(EvenementEnable)
            || testAutorisationList<EvenementCible>(autoris, EvenementCible::Cible,cible(idEntity),
                                                EvenementCible::IdCible,id))
        && (!managers().ensembleEnable(MotCleEnable)
            || testAutorisationList<MotCleCible>(autoris,
                                                MotCleCible::Cible,cible(idEntity),
                                                MotCleCible::IdCible,id))
        && (!managers().ensembleEnable(TexteEnable)
            || testAutorisationList<TexteCible>(autoris,
                                               TexteCible::Cible,cible(idEntity),
                                               TexteCible::IdCible,id))
        && (!managers().ensembleEnable(RestrictionEnable)
            || testAutorisationList<Restriction>(autoris,
                                                Restriction::Cible,cible(idEntity),
                                                Restriction::IdCible,id))
        && (!managers().ensembleEnable(HistoriqueEnable)
            || (testAutorisationList<Historique>(autoris,
                                                Historique::Cible,cible(idEntity),
                                                Historique::IdCible,id)
                && testAutorisationList<Historique>(autoris,
                                                   Historique::Etat,cible(idEntity),
                                                   Historique::IdEtat,id)))
        && (!managers().ensembleEnable(UtilisationEnable)
            ||  (testAutorisationList<Utilisation>(autoris,
                                                  Utilisation::Cible,cible(idEntity),
                                                  Utilisation::IdCible,id)
                && testAutorisationList<Utilisation>(autoris,
                                                    Utilisation::Etat,cible(idEntity),
                                                    Utilisation::IdEtat,id)));
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
            case Evenement::ID:
                controle = controle && testAutorisationList<EvenementCible>(autoris,
                                                                            EvenementCible::IdEvenement,id);
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
            case Usage::ID:
                controle = controle && foreachBeginChild<Usage>(id,
                        [this,autoris](idt id)->bool {
                        return testAutorisationP(id,Usage::ID,autoris)
                                && testAutorisationList<Utilisation>(autoris,
                                                                    Utilisation::IdUsage,id);}
                      ,false);
                break;
            }
        }
    }
    return controle;
}

QString BddPredef::hydrateAttributXml(entityMPS::Entity & entity, post pos, fichierMPS::XmlDoc::const_brother_iterator iter){
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
        flag fl;
        QString controle;
        for(auto iter_list = list.cbegin(); controle.isEmpty() && iter_list != list.cend(); ++iter_list) {
            auto cd = code(entity.idEntity(),*iter_list);
            if (cd != code::Invalide)
                fl |= cd;
            else
                controle = QString("Code Invalide : ").append(*iter_list);
        }
        if(controle.isEmpty())
            entity.setData(pos, fl.value());
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

void BddPredef::listeMiseAJourBdd(int version, idt type) {
    if(type == bddVersion::LibraryType) {
        switch (version) {
        case bddVersion::Creation:
            //Restriction Modification
            if(managers().ensembleEnable(RestrictionEnable))
                creerTable<Restriction>();
            //Type
            if(managers().ensembleEnable(TypeEnable)) {
                creerTable<Type>();
                creerTable<TypePermission>();
            }
            //Commentaire
            if(managers().ensembleEnable(CommentaireEnable)) {
                creerTable<Commentaire>();
                creerTable<CommentaireCible>();
            }
            //Donnee
            if(managers().ensembleEnable(DonneeEnable)) {
                creerTable<Donnee>();
                creerTable<DonneeCible>();
                creerTable<DonneeCard>();
                if(managers().ensembleEnable(TypeEnable)) {
                    Type configType;
                    configType.setNom("Configuration");
                    configType.setNc("config");
                    configType.setRef("config_root_tp");
                    save(configType,Suppr);
                    TypePermission configPerm;
                    configPerm.setIdType(configType.id());
                    configPerm.setCible(cible<Donnee>());
                    configPerm.setCode(code::Visible);
                    save(configPerm);
                    Type defValType;
                    defValType.setNom("Valeur par defaut");
                    defValType.setNc("defaut");
                    defValType.setRef("valeur_defaut_tp");
                    defValType.setParent(configType.id());
                    save(defValType,Suppr);
                    TypePermission defValPerm;
                    defValPerm.setIdType(defValType.id());
                    defValPerm.setCible(cible<Donnee>());
                    defValPerm.setCode(code::Visible | code::Attribuable);
                    save(defValPerm);
                    conteneurMPS::tree<Donnee> tree;
                    auto iter = tree.begin();
                    iter->setNom("Configuration");
                    iter->setType(configType.id());
                    iter->setTpVal(donnee::NoDonnee);
                    iter->setRef("config_dn");
                    iter = tree.push_back(iter);
                    iter->setNom("Valeurs par defaut");
                    iter->setType(configType.id());
                    iter->setTpVal(donnee::NoDonnee);
                    iter->setRef("valeur_defaut_dn");
                    iter = tree.push_back(iter);
                    iter->setNom("Date par defaut");
                    iter->setType(defValType.id());
                    iter->setTpVal(donnee::Date);
                    iter->setRef("date_defaut_dn");
                    save(tree,WithoutDelete);
                }
            }
            //Evenement
            if(managers().ensembleEnable(EvenementEnable)) {
                creerTable<EvenementStyle>();
                EvenementStyle style;
                style.setBordure(Qt::SolidLine);
                style.setCouleurBordure(QColor(Qt::black).name());
                style.setCouleurFond(QColor(Qt::gray).name());
                style.setCouleurTexte(QColor(Qt::black).name());
                style.setCouleurTitre(QColor(Qt::black).name());
                style.setForme(EvenementStyle::Rectangle);
                style.setNom("Style par défaut");
                style.setPoliceTexte(QFont("Times", 10).toString());
                style.setPoliceTitre(QFont("Times", 14, QFont::Bold).toString());
                style.setTexture(Qt::SolidPattern);
                save(style);
                creerTable<Evenement>();
                if(managers().ensembleEnable(TypeEnable)){
                    Type tpEve;
                    tpEve.setRef("evenement_root_tp");
                    tpEve.setNc("Événement");
                    tpEve.setNom("Événement");
                    save(tpEve);
                    TypePermission permEve;
                    permEve.setIdType(tpEve.id());
                    permEve.setCible(cible<Evenement>());
                    permEve.setCode(code::Visible);
                    save(permEve);
                }
                creerTable<EvenementCible>();
            }
            //Historique
            if(managers().ensembleEnable(HistoriqueEnable))
                creerTable<Historique>();
            //Mot Cle
            if(managers().ensembleEnable(MotCleEnable)) {
                creerTable<MotCle>();
                creerTable<MotCleCible>();
                creerTable<MotClePermission>();
                creerTable<MotProgCible>();
                creerTable<MotProgPermission>();
            }
            //Texte
            if(managers().ensembleEnable(TexteEnable)) {
                creerTable<Texte>();
                creerTable<TexteCible>();
                creerTable<Source>();
                creerTable<SourceTexte>();
            }
            //Utilisation
            if(managers().ensembleEnable(UtilisationEnable)) {
                creerTable<Utilisation>();
                creerTable<Usage>();
            }
            //Mise à jour de la version de la base de donnée.
            managers().saveVersion(bddVersion::CreationPredef,bddVersion::LibraryType);
        }
    }
}
