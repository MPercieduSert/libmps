#include "BddPredef.h"

using namespace bddMPS;
using namespace entities;

bool BddPredef::delP(idt id, entidt id_entity) {
    auto controle = true;
    // Spécifique
    switch (id_entity) {
    case Commentaire::ID:
        controle = delList<commentaire_cible>(commentaire_cible::IdCommentaire,id);
        break;
    case donnee::ID:
        controle = foreachBeginChild<donnee>(id,
                        [this](idt id)->bool {
                         return delList<donnee_cible>(donnee_cible::Iddonnee,id)
                                &&delList<donnee_card>(donnee_card::Iddonnee,id)
                                &&delP(id,donnee::ID); }
                      ,false);
        break;
    case mot_cle::ID:
        controle = foreachBeginChild<mot_cle>(id,
                        [this](idt id)->bool {
                        return delList<mot_cle_cible>(mot_cle_cible::Idmot_cle,id)
                            &&delList<mot_cle_permission>(mot_cle_permission::Idmot_cle,id)
                            &&delList<mot_prog_cible>(mot_prog_cible::Idmot_cle,id)
                            &&delList<mot_prog_permission>(mot_prog_permission::Idmot_cle,id)
                            &&delP(id,mot_cle::ID); }
                        ,false);
        break;
    case Source::ID:
        controle = delList<source_texte>(source_texte::IdSource,id);
        break;
    case Evenement::ID:
        controle = delList<evenement_cible>(evenement_cible::IdEvenement,id);
        break;
    case evenement_style::ID:
        if(id != evenement_style::Id_Style_Default) {
            auto vec = getList<Evenement>(Evenement::IdStyle,id);
            for (auto iter = vec.begin(); iter != vec.end(); ++iter)
                iter->set_id_style(evenement_style::Id_Style_Default);
            controle = true;
        }
        else
            controle = false;
        break;
    case Texte::ID:
        controle = delList<source_texte>(source_texte::IdTexte,id)
                    &&delList<texte_cible>(texte_cible::IdTexte,id);
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
                                &&delP(id,Usage::ID); }
                      ,false);
        break;
    }

    // Cible
    controle = controle &&(!managers().ensembleEnable(CommentaireEnable)
                    || del<commentaire_cible>(getListId<commentaire_cible>(commentaire_cible::Cible,cible(id_entity),
                                                                         commentaire_cible::Id_Cible,id)))
                &&(!managers().ensembleEnable(donneeEnable)
                    || delList<donnee_cible>(donnee_cible::Cible,cible(id_entity),
                                            donnee_cible::Id_Cible,id))
                &&(!managers().ensembleEnable(EvenementEnable)
                    || delList<evenement_cible>(evenement_cible::Cible,cible(id_entity),
                                            evenement_cible::Id_Cible,id))
                &&(!managers().ensembleEnable(mot_cleEnable)
                    || (delList<mot_cle_cible>(mot_cle_cible::Cible,cible(id_entity),
                                             mot_cle_cible::Id_Cible,id)
                        &&delList<mot_prog_cible>(mot_prog_cible::Cible,cible(id_entity),
                                                 mot_prog_cible::Id_Cible,id)))
                &&(!managers().ensembleEnable(TexteEnable)
                    || delList<texte_cible>(texte_cible::Cible,cible(id_entity),
                                           texte_cible::Id_Cible,id))
                &&(!managers().ensembleEnable(RestrictionEnable)
                    || delList<Restriction>(Restriction::Cible,cible(id_entity),
                                            Restriction::Id_Cible,id))
                &&(!managers().ensembleEnable(HistoriqueEnable)
                    || (delList<Historique>(Historique::Cible,cible(id_entity),
                                            Historique::Id_Cible,id)
                        &&delList<Historique>(Historique::Etat,cible(id_entity),
                                               Historique::Id_Etat,id)))
                &&(!managers().ensembleEnable(UtilisationEnable)
                    ||(delList<Utilisation>(Utilisation::Cible,cible(id_entity),
                                            Utilisation::Id_Cible,id)
                        &&delList<Utilisation>(Utilisation::Etat,cible(id_entity),
                                                Utilisation::Id_Etat,id)));

    return controle &&Bdd::delP(id,id_entity);
}

void BddPredef::delentityIndonnee(idt id_cible, int cible, int num) {
    if(num < 0)
        throw std::invalid_argument("L'argument num doit être positif");

    donnee_cible nature(donnee::Entity_Nature_IdND, id_cible, cible, num);
    if(get(nature)) {
        if(nature.valeur().toInt() != natureId::Vide)
        {
            std::pair<int, int> interval(intervalentityIndonnee(id_cible,cible,num));
            delList<donnee_cible>(std::vector<donnee_cible::Position>({donnee_cible::Num,
                                                                 donnee_cible::Num,
                                                                 donnee_cible::Iddonnee,
                                                                 donnee_cible::Id_Cible,
                                                                 donnee_cible::Cible}),
                                                                 std::vector<QVariant>({interval.first,
                                                                 interval.second,
                                                                 donnee::Entity_IdND,
                                                                 id_cible,
                                                                 cible}),
                                                                 std::vector<donnee_cible::Position>({donnee_cible::Num}),
                                                                 std::vector<condition>({SupEgal, Inf}));
            auto liste = getList<donnee_cible>(std::vector<donnee_cible::Position>({donnee_cible::Num,
                                                             donnee_cible::Iddonnee,
                                                             donnee_cible::Id_Cible,
                                                             donnee_cible::Cible}),
                                                             std::vector<QVariant>({interval.second,
                                                             donnee::Entity_IdND,
                                                             id_cible,
                                                             cible}),
                                                             std::vector<donnee_cible::Position>({donnee_cible::Num}),
                                                             std::vector<condition>({SupEgal}));
            for(auto i = liste.begin(); i != liste.end(); ++i) {
                i->set_num(i->num() - (interval.second - interval.first));
                save(*i);
            }
            nature.set_valeur(natureId::Vide);
            save(nature);
        }
    }
}

void BddPredef::hydrateAttributXml(entities::entity &ent, post pos, xml_iterator iter, QString &controle){
    if(iter->has_attributes() &&iter->attributes().cbegin()->first == "type"
                             &&iter->attributes().cbegin()->second == "cible") {
        auto i = m_manager->find(iter->text());
        if(i == nbrentity())
            controle.append("Cible invalide : ").append(iter->text());
        else
            entity.set_data(pos, cible(i));
    }
    else
        Bdd::hydrateAttributXml(entity, pos,iter,controle);
}

void BddPredef::hydrateAttributAssociatedXml(entity &entity_ass, const std::pair<const QString,QString> &pair,
                                             const entity &entity, QString &controle) {
    if(pair.first == "cible"){
        auto i = m_manager->find(pair.second);
        if(i == nbrentity())
            controle.append("-> La valeur de l'attribut cible est inconnue : ").append(pair.second);
        else{
            auto pos = positionXml(entity_ass,"Cible",controle);
            if(controle.isEmpty())
                entity_ass.set_data(pos, cible(i));
        }
    }
    else if (pair.first == "id_1_cible") {
        if(info(entity_ass).foreignKeyName().size() != 1)
            controle.append("-> Pour utilisé l'attribut id_1_cible, l'entité associé doit avoir exactement une clé étrangère.");
        else {
            entity_ass.set_data(info(entity_ass).foreignKeyName().cbegin()->first,entity.id());
            hydrateAttributAssociatedXml(entity_ass,{"cible",pair.second},entity,controle);
        }
    }
    else if (pair.first == "idcible_id_1") {
        auto pos = positionXml(entity_ass,"Id_Cible",controle);
        if(controle.isEmpty()) {
            entity_ass.set_data(pos,entity.id());
            pos = positionXml(entity_ass,"Cible",controle);
            if(controle.isEmpty()){
                entity_ass.set_data(pos,cible(entity.id_entity()));
                if(info(entity_ass).foreignKeyName().size() != 1)
                    controle.append("-> Pour utilisé l'attribut idcible_id_1,")
                            .append("l'entité associé doit avoir exactement une clé étrangère : ")
                            .append(entity_ass.affiche());
                else {
                    auto fkey_iter = info(entity_ass).foreignKeyName().cbegin();
                    auto fkey_entity = makeentity(fkey_iter->second);
                    pos = positionXml(*fkey_entity,"Ref",controle);
                    if(controle.isEmpty()) {
                        fkey_entity->set_data(pos,pair.second);
                        if(existsUnique(*fkey_entity))
                            entity_ass.set_data(fkey_iter->first,fkey_entity->id());
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

std::pair<int, int> BddPredef::intervalentityIndonnee(idt id_cible, int cible, int num) {
    if(num < 0)
        return std::pair<int, int>(-1,-1);
    auto listeNum = getList<donnee_cible>(std::vector<donnee_cible::Position>({donnee_cible::Num,
                                                         donnee_cible::Iddonnee,
                                                         donnee_cible::Id_Cible,
                                                         donnee_cible::Cible}),
                                                         std::vector<QVariant>({num,
                                                         donnee::Entity_Nature_IdND,
                                                         id_cible,
                                                         cible}),
                                                         std::vector<donnee_cible::Position>({donnee_cible::Num}),
                                                         std::vector<condition>({InfEgal}));
    std::pair<int, int> interval(0,0);
    for(auto i = listeNum.begin(); i != listeNum.end(); ++i) {
        interval.first = interval.second;
        interval.second += managers().nombreAttributCible(i->valeur().toInt());
    }
    return interval;
}

bool BddPredef::isMultipleAssociatedXml(const std::pair<const QString, QString> &pair) const {
    if(pair.first == "cible" || pair.first == "id_1_cible")
        return pair.second.contains("|");
    return Bdd::isMultipleAssociatedXml(pair);
}

Bdd::xml_list_atts BddPredef::listMultipleAssociatedXml(const std::pair<const QString,QString> &pair, QString &controle) {
    if(pair.first == "cible" || pair.first == "id_1_cible"){
        xml_list_atts list;
        auto cibles = pair.second.split("|");
        for (auto iter = cibles.cbegin(); iter != cibles.cend(); ++iter)
            list.push_back({pair.first,*iter});
        return list;
    }
    return Bdd::listMultipleAssociatedXml(pair,controle);
}

bool BddPredef::testAutorisationP(idt id, entidt id_entity, flag autoris) {
    auto controle = Bdd::testAutorisationP(id,id_entity,autoris);
    if(autoris &Suppr) {
        // Cible
        controle = controle &&(!managers().ensembleEnable(CommentaireEnable)
            || testAutorisationList<commentaire_cible>(autoris,
                                                     commentaire_cible::Cible,cible(id_entity),
                                                     commentaire_cible::Id_Cible,id))
        &&(!managers().ensembleEnable(donneeEnable)
            || testAutorisationList<donnee_cible>(autoris, donnee_cible::Cible,cible(id_entity),
                                                donnee_cible::Id_Cible,id))
        &&(!managers().ensembleEnable(EvenementEnable)
            || testAutorisationList<evenement_cible>(autoris, evenement_cible::Cible,cible(id_entity),
                                                evenement_cible::Id_Cible,id))
        &&(!managers().ensembleEnable(mot_cleEnable)
            || testAutorisationList<mot_cle_cible>(autoris,
                                                mot_cle_cible::Cible,cible(id_entity),
                                                mot_cle_cible::Id_Cible,id))
        &&(!managers().ensembleEnable(TexteEnable)
            || testAutorisationList<texte_cible>(autoris,
                                               texte_cible::Cible,cible(id_entity),
                                               texte_cible::Id_Cible,id))
        &&(!managers().ensembleEnable(RestrictionEnable)
            || testAutorisationList<Restriction>(autoris,
                                                Restriction::Cible,cible(id_entity),
                                                Restriction::Id_Cible,id))
        &&(!managers().ensembleEnable(HistoriqueEnable)
            || (testAutorisationList<Historique>(autoris,
                                                Historique::Cible,cible(id_entity),
                                                Historique::Id_Cible,id)
                &&testAutorisationList<Historique>(autoris,
                                                   Historique::Etat,cible(id_entity),
                                                   Historique::Id_Etat,id)))
        &&(!managers().ensembleEnable(UtilisationEnable)
            ||  (testAutorisationList<Utilisation>(autoris,
                                                  Utilisation::Cible,cible(id_entity),
                                                  Utilisation::Id_Cible,id)
                &&testAutorisationList<Utilisation>(autoris,
                                                    Utilisation::Etat,cible(id_entity),
                                                    Utilisation::Id_Etat,id)));
        // Spécifique
        if(controle) {
            switch (id_entity) {
            case Commentaire::ID:
                controle = controle &&testAutorisationList<commentaire_cible>(autoris,
                                                                             commentaire_cible::IdCommentaire,id);
                break;
            case donnee::ID:
                controle = controle &&foreachBeginChild<donnee>(id,
                        [this,autoris](idt id)->bool {
                        return testAutorisationP(id,donnee::ID,autoris)
                                &&testAutorisationList<donnee_cible>(autoris,
                                                                    donnee_cible::Iddonnee,id)
                                &&testAutorisationList<donnee_card>(autoris,
                                                                   donnee_card::Iddonnee,id);}
                      ,false);
                break;
            case Evenement::ID:
                controle = controle &&testAutorisationList<evenement_cible>(autoris,
                                                                            evenement_cible::IdEvenement,id);
                break;
            case mot_cle::ID:
                controle = controle &&foreachBeginChild<mot_cle>(id,
                        [this,autoris](idt id)->bool {
                        return testAutorisationP(id,mot_cle::ID,autoris)
                            &&testAutorisationList<mot_cle_cible>(autoris,
                                                                mot_cle_cible::Idmot_cle,id)
                            &&testAutorisationList<mot_cle_permission>(autoris,
                                                                     mot_cle_permission::Idmot_cle,id)
                            &&testAutorisationList<mot_prog_cible>(autoris,
                                                                 mot_prog_cible::Idmot_cle,id)
                            &&testAutorisationList<mot_prog_permission>(autoris,
                                                                      mot_prog_permission::Idmot_cle,id);}
                        ,false);
                break;
            case Source::ID:
                controle = controle &&testAutorisationList<source_texte>(autoris,
                                                                        source_texte::IdSource,id);
                break;
            case Texte::ID:
                controle = controle &&testAutorisationList<source_texte>(autoris,
                                                                        source_texte::IdTexte,id)
                            &&testAutorisationList<texte_cible>(autoris,
                                                               texte_cible::IdTexte,id);
                break;
            case Type::ID:
                controle = foreachBeginChild<Type>(id,
                                [this,autoris](idt id)->bool{return testAutorisationP(id,Type::ID,autoris);}
                     ,false);
                break;
            case Usage::ID:
                controle = controle &&foreachBeginChild<Usage>(id,
                        [this,autoris](idt id)->bool {
                        return testAutorisationP(id,Usage::ID,autoris)
                                &&testAutorisationList<Utilisation>(autoris,
                                                                    Utilisation::IdUsage,id);}
                      ,false);
                break;
            }
        }
    }
    return controle;
}

enumt BddPredef::strCategorieToEnum(const QString &str, flag categorie, QString &controle) const noexcept {
    if(categorie &PermissionCode){
        if(str == "Interdit")
            return code::Interdit;
        if(str == "Visible")
            return code::Visible;
        if(str== "Attribuable")
            return code::Attribuable;
    }
    return Bdd::strCategorieToEnum(str,categorie,controle);
}

enumt BddPredef::strIdToEnum(const QString &str, idt id_entity, QString &controle) const noexcept {
    switch (id_entity) {
    case donnee::ID:
        if(str == "Nodonnee")
            return donnee::Nodonnee;
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
        if(str == "Date_Time")
            return donnee::Date_Time;
        break;
    case donnee_card::ID:
        if(str == "Exact")
            return donnee::Exact;
        if(str == "Au plus")
            return donnee::AuPlus;
        if(str == "Au moins")
            return donnee::AuMoins;
        break;
    case evenement_style::ID:
        if(str == "Rectangle")
            return evenement_style::Rectangle;
        return strCategorieToEnum(str,LineStyle|BrushStyle,controle);
    case mot_cle_permission::ID:
    case type_permission::ID:
        return strCategorieToEnum(str,PermissionCode,controle);
    }
    return Bdd::strIdToEnum(str,id_entity,controle);
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
                creerTable<type_permission>();
            }
            //Commentaire
            if(managers().ensembleEnable(CommentaireEnable)) {
                creerTable<Commentaire>();
                creerTable<commentaire_cible>();
            }
            //donnee
            if(managers().ensembleEnable(donneeEnable)) {
                creerTable<donnee>();
                creerTable<donnee_cible>();
                creerTable<donnee_card>();
            }
            //Evenement
            if(managers().ensembleEnable(EvenementEnable)) {
                creerTable<evenement_style>();
                creerTable<Evenement>();
                creerTable<evenement_cible>();
            }
            //Historique
            if(managers().ensembleEnable(HistoriqueEnable))
                creerTable<Historique>();
            //Mot Cle
            if(managers().ensembleEnable(mot_cleEnable)) {
                creerTable<mot_cle>();
                creerTable<mot_cle_cible>();
                creerTable<mot_cle_permission>();
                creerTable<mot_prog_cible>();
                creerTable<mot_prog_permission>();
            }
            //Texte
            if(managers().ensembleEnable(TexteEnable)) {
                creerTable<Texte>();
                creerTable<texte_cible>();
                creerTable<Source>();
                creerTable<source_texte>();
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
