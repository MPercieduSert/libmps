#include "BddPredef.h"

using namespace bddMPS;
using namespace entityMPS;

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

void BddPredef::hydrateAttributXml(entityMPS::Entity & entity, post pos, xml_iterator iter, QString &controle){
    if(iter->hasAttributes() && iter->attributes().cbegin()->first == "type"
                             && iter->attributes().cbegin()->second == "cible") {
        auto i = m_manager->find(iter->text());
        if(i == nbrEntity())
            controle.append("Cible invalide : ").append(iter->text());
        else
            entity.setData(pos, cible(i));
    }
    else
        Bdd::hydrateAttributXml(entity, pos,iter,controle);
}

void BddPredef::hydrateAttributAssociatedXml(Entity &entity_ass, const std::pair<const QString,QString> &pair,
                                             const Entity &entity, QString &controle) {
    if(pair.first == "cible"){
        auto i = m_manager->find(pair.second);
        if(i == nbrEntity())
            controle.append("-> La valeur de l'attribut cible est inconnue : ").append(pair.second);
        else{
            auto pos = positionXml(entity_ass,"Cible",controle);
            if(controle.isEmpty())
                entity_ass.setData(pos, cible(i));
        }
    }
    else if (pair.first == "id1_cible") {
        if(info(entity_ass).foreignKeyName().size() != 1)
            controle.append("-> Pour utilisé l'attribut id1_cible, l'entité associé doit avoir exactement une clé étrangère.");
        else {
            entity_ass.setData(info(entity_ass).foreignKeyName().cbegin()->first,entity.id());
            hydrateAttributAssociatedXml(entity_ass,{"cible",pair.second},entity,controle);
        }
    }
    else if (pair.first == "idcible_id1") {
        auto pos = positionXml(entity_ass,"IdCible",controle);
        if(controle.isEmpty()) {
            entity_ass.setData(pos,entity.id());
            pos = positionXml(entity_ass,"Cible",controle);
            if(controle.isEmpty()){
                entity_ass.setData(pos,cible(entity.idEntity()));
                if(info(entity_ass).foreignKeyName().size() != 1)
                    controle.append("-> Pour utilisé l'attribut idcible_id1,")
                            .append("l'entité associé doit avoir exactement une clé étrangère : ")
                            .append(entity_ass.affiche());
                else {
                    auto fkey_iter = info(entity_ass).foreignKeyName().cbegin();
                    auto fkey_entity = makeEntity(fkey_iter->second);
                    pos = positionXml(*fkey_entity,"Ref",controle);
                    if(controle.isEmpty()) {
                        fkey_entity->setData(pos,pair.second);
                        if(existsUnique(*fkey_entity))
                            entity_ass.setData(fkey_iter->first,fkey_entity->id());
                        else
                            controle.append("-> Entité unique intouvable: \n").append(fkey_entity->affiche());
                    }
                }
            }
        }
    }
    else
        Bdd::hydrateAttributAssociatedXml(entity_ass,pair,entity,controle);
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

bool BddPredef::isMultipleAssociatedXml(const std::pair<const QString, QString> &pair) const {
    if(pair.first == "cible" || pair.first == "id1_cible")
        return pair.second.contains("|");
    return Bdd::isMultipleAssociatedXml(pair);
}

Bdd::xml_list_atts BddPredef::listMultipleAssociatedXml(const std::pair<const QString,QString> &pair, QString &controle) {
    if(pair.first == "cible" || pair.first == "id1_cible"){
        xml_list_atts list;
        auto cibles = pair.second.split("|");
        for (auto iter = cibles.cbegin(); iter != cibles.cend(); ++iter)
            list.push_back({pair.first,*iter});
        return list;
    }
    return Bdd::listMultipleAssociatedXml(pair,controle);
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

enumt BddPredef::strCategorieToEnum(const QString &str, flag categorie, QString & controle) const noexcept {
    if(categorie & PermissionCode){
        if(str == "Interdit")
            return code::Interdit;
        if(str == "Visible")
            return code::Visible;
        if(str== "Attribuable")
            return code::Attribuable;
    }
    return Bdd::strCategorieToEnum(str,categorie,controle);
}

enumt BddPredef::strIdToEnum(const QString & str, idt idEntity, QString &controle) const noexcept {
    switch (idEntity) {
    case Donnee::ID:
        if(str == "NoDonnee")
            return donnee::NoDonnee;
        if(str == "Int")
            return donnee::Int;
        if(str == "String")
            return donnee::String;
        if(str == "Bool")
            return donnee::Bool;
        if(str == "Float")
            return donnee::Float;
        if(str == "Double")
            return donnee::Double;
        if(str == "Date")
            return donnee::Date;
        if(str == "DateTime")
            return donnee::DateTime;
        break;
    case DonneeCard::ID:
        if(str == "Exact")
            return donnee::Exact;
        if(str == "Au plus")
            return donnee::AuPlus;
        if(str == "Au moins")
            return donnee::AuMoins;
        break;
    case EvenementStyle::ID:
        if(str == "Rectangle")
            return EvenementStyle::Rectangle;
        return strCategorieToEnum(str,LineStyle|BrushStyle,controle);
    case MotClePermission::ID:
    case TypePermission::ID:
        return strCategorieToEnum(str,PermissionCode,controle);
    }
    return Bdd::strIdToEnum(str,idEntity,controle);
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
            }
            //Evenement
            if(managers().ensembleEnable(EvenementEnable)) {
                creerTable<EvenementStyle>();
                creerTable<Evenement>();
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
