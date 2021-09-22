#include "bdd_predef.h"

using namespace mps;
using namespace b2d;
using namespace entities;

bool bdd_predef::del_v(idt id, entidt id_entity) {
    auto controle = true;
    // Spécifique
    switch (id_entity) {
    case commentaire::ID:
        controle = del_list<commentaire_cible>(commentaire_cible::Id_Commentaire,id);
        break;
    case donnee::ID:
        controle = foreach_begin_child<donnee>(id,
                        [this](idt id)->bool {
                         return del_list<donnee_cible>(donnee_cible::Id_Donnee,id)
                                &&del_list<donnee_card>(donnee_card::Id_Donnee,id)
                                &&del_v(id,donnee::ID); }
                      ,false);
        break;
    case mot_cle::ID:
        controle = foreach_begin_child<mot_cle>(id,
                        [this](idt id)->bool {
                        return del_list<mot_cle_cible>(mot_cle_cible::Id_Mot_Cle,id)
                            &&del_list<mot_cle_permission>(mot_cle_permission::Id_Mot_Cle,id)
                            &&del_list<mot_prog_cible>(mot_prog_cible::Id_Mot_Cle,id)
                            &&del_list<mot_prog_permission>(mot_prog_permission::Id_Mot_Cle,id)
                            &&del_v(id,mot_cle::ID); }
                        ,false);
        break;
    case source::ID:
        controle = del_list<source_texte>(source_texte::Id_Source,id);
        break;
    case evenement::ID:
        controle = del_list<evenement_cible>(evenement_cible::Id_Evenement,id);
        break;
    case evenement_style::ID:
        if(id != evenement_style::Id_Style_Default) {
            auto vec = get_list<evenement>(evenement::Id_Style,id);
            for (auto iter = vec.begin(); iter != vec.end(); ++iter)
                iter->set_id_style(evenement_style::Id_Style_Default);
            controle = true;
        }
        else
            controle = false;
        break;
    case texte::ID:
        controle = del_list<source_texte>(source_texte::Id_Texte,id)
                    &&del_list<texte_cible>(texte_cible::Id_Texte,id);
        break;
    case type::ID:
        controle = foreach_begin_child<type>(id,
                        [this](idt id)->bool{return del_v(id,type::ID);}
             ,false);
        break;
    case usage::ID:
        controle = foreach_begin_child<usage>(id,
                        [this](idt id)->bool {
                         return del_list<utilisation>(utilisation::Id_Usage,id)
                                &&del_v(id,usage::ID); }
                      ,false);
        break;
    }

    // Cible
    controle = controle &&(!managers().ensemble_enable(Commentaire_Enable)
                    || del<commentaire_cible>(get_list_id<commentaire_cible>(commentaire_cible::Cible,cible(id_entity),
                                                                         commentaire_cible::Id_Cible,id)))
                &&(!managers().ensemble_enable(Donnee_Enable)
                    || del_list<donnee_cible>(donnee_cible::Cible,cible(id_entity),
                                            donnee_cible::Id_Cible,id))
                &&(!managers().ensemble_enable(Evenement_Enable)
                    || del_list<evenement_cible>(evenement_cible::Cible,cible(id_entity),
                                            evenement_cible::Id_Cible,id))
                &&(!managers().ensemble_enable(Mot_Cle_Enable)
                    || (del_list<mot_cle_cible>(mot_cle_cible::Cible,cible(id_entity),
                                             mot_cle_cible::Id_Cible,id)
                        &&del_list<mot_prog_cible>(mot_prog_cible::Cible,cible(id_entity),
                                                 mot_prog_cible::Id_Cible,id)))
                &&(!managers().ensemble_enable(Texte_Enable)
                    || del_list<texte_cible>(texte_cible::Cible,cible(id_entity),
                                           texte_cible::Id_Cible,id))
                &&(!managers().ensemble_enable(Restriction_Enable)
                    || del_list<restriction>(restriction::Cible,cible(id_entity),
                                            restriction::Id_Cible,id))
                &&(!managers().ensemble_enable(Historique_Enable)
                    || (del_list<historique>(historique::Cible,cible(id_entity),
                                            historique::Id_Cible,id)
                        &&del_list<historique>(historique::Etat,cible(id_entity),
                                               historique::Id_Etat,id)))
                &&(!managers().ensemble_enable(Utilisation_Enable)
                    ||(del_list<utilisation>(utilisation::Cible,cible(id_entity),
                                            utilisation::Id_Cible,id)
                        &&del_list<utilisation>(utilisation::Etat,cible(id_entity),
                                                utilisation::Id_Etat,id)));

    return controle &&bdd::del_v(id,id_entity);
}

void bdd_predef::del_entity_in_donnee(idt id_cible, int cible, int num) {
    if(num < 0)
        throw std::invalid_argument("L'argument num doit être positif");

    donnee_cible nature(donnee::Entity_Nature_IdND, id_cible, cible, num);
    if(get(nature)) {
        if(nature.valeur().toInt() != nature_id::Vide)
        {
            std::pair<int, int> interval(interval_entity_in_donnee(id_cible,cible,num));
            del_list<donnee_cible>(std::vector<donnee_cible::position>({donnee_cible::Num,
                                                                 donnee_cible::Num,
                                                                 donnee_cible::Id_Donnee,
                                                                 donnee_cible::Id_Cible,
                                                                 donnee_cible::Cible}),
                                                                 std::vector<QVariant>({interval.first,
                                                                 interval.second,
                                                                 donnee::Entity_IdND,
                                                                 id_cible,
                                                                 cible}),
                                                                 std::vector<donnee_cible::position>({donnee_cible::Num}),
                                                                 std::vector<condition>({Sup_Egal, Inf}));
            auto liste = get_list<donnee_cible>(std::vector<donnee_cible::position>({donnee_cible::Num,
                                                             donnee_cible::Id_Donnee,
                                                             donnee_cible::Id_Cible,
                                                             donnee_cible::Cible}),
                                                             std::vector<QVariant>({interval.second,
                                                             donnee::Entity_IdND,
                                                             id_cible,
                                                             cible}),
                                                             std::vector<donnee_cible::position>({donnee_cible::Num}),
                                                             std::vector<condition>({Sup_Egal}));
            for(auto i = liste.begin(); i != liste.end(); ++i) {
                i->set_num(i->num() - (interval.second - interval.first));
                save(*i);
            }
            nature.set_valeur(nature_id::Vide);
            save(nature);
        }
    }
}

QVariant bdd_predef::donnee_ref(const QString & ref, const entity & ent, int num) {
    donnee_cible cb;
    cb.set_cible(cible(ent.id_entity()));
    cb.set_id_cible(ent.id());
    cb.set_num(num);
    cb.set_id_donnee(ref_to_id<donnee>(ref));
    get_unique(cb);
    return cb.valeur();
}

void bdd_predef::hydrate_attribut_xml(entities::entity &ent, post pos, xml_iterator iter, QString &controle){
    if(iter->has_attributes() && iter->attributes().cbegin()->first == "type"
                             && iter->attributes().cbegin()->second == "cible") {
        auto i = m_manager->find(iter->text());
        if(i == nbr_entity())
            controle.append("Cible invalide : ").append(iter->text());
        else
            ent.set_data(pos, cible(i));
    }
    else if (iter->has_attributes() && iter->attributes().cbegin()->first == "cible") {
        auto ent_cible = make_entity(iter->attributes().cbegin()->second);
        if(!ent_cible)
            controle.append("-> Nom d'entité inconnu : ").append(iter->attributes().cbegin()->second);
        else {
            auto p = position_xml(*ent_cible,"ref",controle);
            if(controle.isEmpty()) {
                ent_cible->set_data(p,iter->text());
                if(!exists_unique(*ent_cible))
                    controle.append("-> Il n'existe pas d'entité de type ")
                        .append(iter->attributes().cbegin()->second)
                        .append("d'attribut ref = ")
                        .append(iter->text());
                else {
                    p = position_xml(ent,"cible",controle);
                    if (controle.isEmpty()) {
                        ent.set_data(p,cible(ent_cible->id_entity()));
                        ent.set_data(pos,ent_cible->id());
                    }
                }
            }
        }
    }
    else if (ent.id_entity() == donnee_cible::ID && iter->name() == "id_donnee") {
        if(iter->attributes().size() != 1 || iter->attributes().cbegin()->first != "ref")
            controle.append("-> L'attribut id_donnee de donnee_cible doit-être hydraté par une attribut xml ref");
        else {
            auto id_dn = ref_to_id<donnee>(iter->attributes().cbegin()->second);
            if(id_dn)
                ent.set_data(pos,id_dn);
            else
                controle.append("-> Il n'existe pas de donnee de ref = ")
                        .append(iter->attributes().cbegin()->second);
        }
    }
    else
        bdd::hydrate_attribut_xml(ent, pos,iter,controle);
}

void bdd_predef::hydrate_attribut_associated_xml(entity &ent_ass, const std::pair<const QString,QString> &pair,
                                             const entity &ent, QString &controle) {
    if(pair.first == "cible"){
        auto i = m_manager->find(pair.second);
        if(i == nbr_entity())
            controle.append("-> La valeur de l'attribut cible est inconnue : ").append(pair.second);
        else{
            auto pos = position_xml(ent_ass,"cible",controle);
            if(controle.isEmpty())
                ent_ass.set_data(pos, cible(i));
        }
    }
    else if (pair.first == "id_1_cible") {
        if(info(ent_ass).foreign_key_name().size() != 1)
            controle.append("-> Pour utilisé l'attribut id_1_cible, l'entité associé doit avoir exactement une clé étrangère.");
        else {
            ent_ass.set_data(info(ent_ass).foreign_key_name().cbegin()->first,ent.id());
            hydrate_attribut_associated_xml(ent_ass,{"cible",pair.second},ent,controle);
        }
    }
    else if (pair.first == "id_cible_id_1") {
        auto pos = position_xml(ent_ass,"id_cible",controle);
        if(controle.isEmpty()) {
            ent_ass.set_data(pos,ent.id());
            pos = position_xml(ent_ass,"cible",controle);
            if(controle.isEmpty()){
                ent_ass.set_data(pos,cible(ent.id_entity()));
                if(info(ent_ass).foreign_key_name().size() != 1)
                    controle.append("-> Pour utilisé l'attribut idcible_id_1,")
                            .append("l'entité associé doit avoir exactement une clé étrangère : ")
                            .append(ent_ass.affiche());
                else {
                    auto fkey_iter = info(ent_ass).foreign_key_name().cbegin();
                    auto fkey_entity = make_entity(fkey_iter->second);
                    pos = position_xml(*fkey_entity,"ref",controle);
                    if(controle.isEmpty()) {
                        fkey_entity->set_data(pos,pair.second);
                        if(exists_unique(*fkey_entity))
                            ent_ass.set_data(fkey_iter->first,fkey_entity->id());
                        else
                            controle.append("-> Entité unique intouvable: \n").append(fkey_entity->affiche());
                    }
                }
            }
        }
    }
    else if (pair.first == "card") {
        auto pos = position_xml(ent_ass,"card",controle);
        if(controle.isEmpty()){
            bool ok;
            pair.second.toInt(&ok);
            if(ok)
                ent_ass.set_data(pos,pair.second);
            else
                ent_ass.set_data(pos,str_categorie_to_enum(pair.second,Style_Num,controle));
        }

    }
    else if (pair.first == "exact") {
        auto pos = position_xml(ent_ass,"exact",controle);
        if(controle.isEmpty())
            ent_ass.set_data(pos,str_id_to_enum(pair.second,donnee_card::ID,controle));
    }
    else
        bdd::hydrate_attribut_associated_xml(ent_ass,pair,ent,controle);
}

std::pair<int, int> bdd_predef::interval_entity_in_donnee(idt id_cible, int cible, int num) {
    if(num < 0)
        return std::pair<int, int>(-1,-1);
    auto listeNum = get_list<donnee_cible>(std::vector<donnee_cible::position>({donnee_cible::Num,
                                                         donnee_cible::Id_Donnee,
                                                         donnee_cible::Id_Cible,
                                                         donnee_cible::Cible}),
                                                         std::vector<QVariant>({num,
                                                         donnee::Entity_Nature_IdND,
                                                         id_cible,
                                                         cible}),
                                                         std::vector<donnee_cible::position>({donnee_cible::Num}),
                                                         std::vector<condition>({Inf_Egal}));
    std::pair<int, int> interval(0,0);
    for(auto i = listeNum.begin(); i != listeNum.end(); ++i) {
        interval.first = interval.second;
        interval.second += managers().nbr_att_cible(i->valeur().toInt());
    }
    return interval;
}

bool bdd_predef::is_associated_xml(xml_iterator iter, entity &ent) const {
    if(ent.id_entity() == donnee_cible::ID && iter->name() == "id_donnee")
        return false;
    if(iter->attributes().size() == 1 && iter->attributes().cbegin()->first == "cible")
        return false;
    return bdd::is_associated_xml(iter,ent);
}

bool bdd_predef::is_multiple_associated_xml(const std::pair<const QString, QString> &pair) const {
    if(pair.first == "cible" || pair.first == "id_1_cible")
        return pair.second.contains("|");
    return bdd::is_multiple_associated_xml(pair);
}

bdd::xml_list_atts bdd_predef::list_multiple_associated_xml(const std::pair<const QString,QString> &pair, QString &controle) {
    if(pair.first == "cible" || pair.first == "id_1_cible"){
        xml_list_atts list;
        auto cibles = pair.second.split("|");
        for (auto iter = cibles.cbegin(); iter != cibles.cend(); ++iter)
            list.push_back({pair.first,*iter});
        return list;
    }
    return bdd::list_multiple_associated_xml(pair,controle);
}

bool bdd_predef::test_autorisation_v(idt id, entidt id_entity, flag autoris) {
    auto controle = bdd::test_autorisation_v(id,id_entity,autoris);
    if(autoris &Suppr) {
        // Cible
        controle = controle &&(!managers().ensemble_enable(Commentaire_Enable)
            || test_autorisation_list<commentaire_cible>(autoris,
                                                     commentaire_cible::Cible,cible(id_entity),
                                                     commentaire_cible::Id_Cible,id))
        &&(!managers().ensemble_enable(Donnee_Enable)
            || test_autorisation_list<donnee_cible>(autoris, donnee_cible::Cible,cible(id_entity),
                                                donnee_cible::Id_Cible,id))
        &&(!managers().ensemble_enable(Evenement_Enable)
            || test_autorisation_list<evenement_cible>(autoris, evenement_cible::Cible,cible(id_entity),
                                                evenement_cible::Id_Cible,id))
        &&(!managers().ensemble_enable(Mot_Cle_Enable)
            || test_autorisation_list<mot_cle_cible>(autoris,
                                                mot_cle_cible::Cible,cible(id_entity),
                                                mot_cle_cible::Id_Cible,id))
        &&(!managers().ensemble_enable(Texte_Enable)
            || test_autorisation_list<texte_cible>(autoris,
                                               texte_cible::Cible,cible(id_entity),
                                               texte_cible::Id_Cible,id))
        &&(!managers().ensemble_enable(Restriction_Enable)
            || test_autorisation_list<restriction>(autoris,
                                                restriction::Cible,cible(id_entity),
                                                restriction::Id_Cible,id))
        &&(!managers().ensemble_enable(Historique_Enable)
            || (test_autorisation_list<historique>(autoris,
                                                historique::Cible,cible(id_entity),
                                                historique::Id_Cible,id)
                &&test_autorisation_list<historique>(autoris,
                                                   historique::Etat,cible(id_entity),
                                                   historique::Id_Etat,id)))
        &&(!managers().ensemble_enable(Utilisation_Enable)
            ||  (test_autorisation_list<utilisation>(autoris,
                                                  utilisation::Cible,cible(id_entity),
                                                  utilisation::Id_Cible,id)
                &&test_autorisation_list<utilisation>(autoris,
                                                    utilisation::Etat,cible(id_entity),
                                                    utilisation::Id_Etat,id)));
        // Spécifique
        if(controle) {
            switch (id_entity) {
            case commentaire::ID:
                controle = controle &&test_autorisation_list<commentaire_cible>(autoris,
                                                                             commentaire_cible::Id_Commentaire,id);
                break;
            case donnee::ID:
                controle = controle &&foreach_begin_child<donnee>(id,
                        [this,autoris](idt id)->bool {
                        return test_autorisation_v(id,donnee::ID,autoris)
                                &&test_autorisation_list<donnee_cible>(autoris,
                                                                    donnee_cible::Id_Donnee,id)
                                &&test_autorisation_list<donnee_card>(autoris,
                                                                   donnee_card::Id_Donnee,id);}
                      ,false);
                break;
            case evenement::ID:
                controle = controle &&test_autorisation_list<evenement_cible>(autoris,
                                                                            evenement_cible::Id_Evenement,id);
                break;
            case mot_cle::ID:
                controle = controle &&foreach_begin_child<mot_cle>(id,
                        [this,autoris](idt id)->bool {
                        return test_autorisation_v(id,mot_cle::ID,autoris)
                            &&test_autorisation_list<mot_cle_cible>(autoris,
                                                                mot_cle_cible::Id_Mot_Cle,id)
                            &&test_autorisation_list<mot_cle_permission>(autoris,
                                                                     mot_cle_permission::Id_Mot_Cle,id)
                            &&test_autorisation_list<mot_prog_cible>(autoris,
                                                                 mot_prog_cible::Id_Mot_Cle,id)
                            &&test_autorisation_list<mot_prog_permission>(autoris,
                                                                      mot_prog_permission::Id_Mot_Cle,id);}
                        ,false);
                break;
            case source::ID:
                controle = controle &&test_autorisation_list<source_texte>(autoris,
                                                                        source_texte::Id_Source,id);
                break;
            case texte::ID:
                controle = controle &&test_autorisation_list<source_texte>(autoris,
                                                                        source_texte::Id_Texte,id)
                            &&test_autorisation_list<texte_cible>(autoris,
                                                               texte_cible::Id_Texte,id);
                break;
            case type::ID:
                controle = foreach_begin_child<type>(id,
                                [this,autoris](idt id)->bool{return test_autorisation_v(id,type::ID,autoris);}
                     ,false);
                break;
            case usage::ID:
                controle = controle &&foreach_begin_child<usage>(id,
                        [this,autoris](idt id)->bool {
                        return test_autorisation_v(id,usage::ID,autoris)
                                &&test_autorisation_list<utilisation>(autoris,
                                                                    utilisation::Id_Usage,id);}
                      ,false);
                break;
            }
        }
    }
    return controle;
}

enumt bdd_predef::str_categorie_to_enum(const QString &str, flag categorie, QString &controle) const noexcept {
    if(categorie &Permission_Code){
        if(str == "interdit")
            return code::Interdit;
        if(str == "visible")
            return code::Visible;
        if(str == "attribuable")
            return code::Attribuable;
    }
    return bdd::str_categorie_to_enum(str,categorie,controle);
}

enumt bdd_predef::str_id_to_enum(const QString &str, idt id_entity, QString &controle) const noexcept {
    switch (id_entity) {
    case donnee::ID:
        if(str == "no_donnee")
            return donnee_info::No_Donnee;
        if(str == "int")
            return donnee_info::Int;
        if(str == "unsigned")
            return donnee_info::Unsigned;
        if(str == "string")
            return donnee_info::String;
        if(str == "bool")
            return donnee_info::Bool;
        if(str == "float")
            return donnee_info::Float;
        if(str == "double")
            return donnee_info::Double;
        if(str == "date")
            return donnee_info::Date;
        if(str == "date_time")
            return donnee_info::Date_Time;
        break;
    case donnee_card::ID:
        if(str == "exact")
            return donnee_info::Exact;
        if(str == "au_plus")
            return donnee_info::Au_Plus;
        if(str == "au_moins")
            return donnee_info::Au_Moins;
        break;
    case donnee_cible::ID:
        return str_categorie_to_enum(str,Style_Num,controle);
    case evenement_style::ID:
        if(str == "rectangle")
            return evenement_style::Rectangle;
        return str_categorie_to_enum(str,Line_Style|Brush_Style,controle);
    case mot_cle_permission::ID:
    case type_permission::ID:
        return str_categorie_to_enum(str,Permission_Code,controle);
    }
    return bdd::str_id_to_enum(str,id_entity,controle);
}

void bdd_predef::liste_mise_a_jour_bdd(int version, idt tp) {
    if(tp == bdd_version::Library_Type) {
        switch (version) {
        case bdd_version::Creation:
            //Restriction Modification
            if(managers().ensemble_enable(Restriction_Enable))
                creer_table<restriction>();
            //Type
            if(managers().ensemble_enable(Type_Enable)) {
                creer_table<type>();
                creer_table<type_permission>();
            }
            //Commentaire
            if(managers().ensemble_enable(Commentaire_Enable)) {
                creer_table<commentaire>();
                creer_table<commentaire_cible>();
            }
            //donnee
            if(managers().ensemble_enable(Donnee_Enable)) {
                creer_table<donnee>();
                creer_table<donnee_cible>();
                creer_table<donnee_card>();
            }
            //Evenement
            if(managers().ensemble_enable(Evenement_Enable)) {
                creer_table<evenement_style>();
                creer_table<evenement>();
                creer_table<evenement_cible>();
            }
            //Historique
            if(managers().ensemble_enable(Historique_Enable))
                creer_table<historique>();
            //Mot Cle
            if(managers().ensemble_enable(Mot_Cle_Enable)) {
                creer_table<mot_cle>();
                creer_table<mot_cle_cible>();
                creer_table<mot_cle_permission>();
                creer_table<mot_prog_cible>();
                creer_table<mot_prog_permission>();
            }
            //Texte
            if(managers().ensemble_enable(Texte_Enable)) {
                creer_table<texte>();
                creer_table<texte_cible>();
                creer_table<source>();
                creer_table<source_texte>();
            }
            //Utilisation
            if(managers().ensemble_enable(Utilisation_Enable)) {
                creer_table<utilisation>();
                creer_table<usage>();
            }
            //Mise à jour de la version de la base de donnée.
            managers().save_version(bdd_version::Creation_Predef,bdd_version::Library_Type);
        }
    }
}
