#include "bdd.h"

using namespace mps;
using namespace b2d;

entities_base::arbre bdd::arbre_xml(xml_iterator iter, QString &controle){
    entities_base::arbre arbre;
    for (auto att_iter = iter->attributes().cbegin(); att_iter != iter->attributes().cend() &&controle.isEmpty(); ++att_iter) {
        if(att_iter->first == "ref_parent") {
            auto parent = make_entity(iter.parent()->name());
            parent->set_data(parent->position("ref"),att_iter->second);
            if (exists_unique(*parent))
                arbre.set_parent(parent->id());
            else
                controle.append("-> Parent unique introuvable :\n")
                        .append(parent->affiche());
        }
        else if (att_iter->first == "num") {
            if(att_iter->second == "premier")
                arbre.set_num(0);
            else if(att_iter->second == "dernier"){
                arbre.set_num(-1);
            }
            else {
                bool ok;
                auto num = att_iter->second.toInt(&ok);
                if (ok)
                    arbre.set_num(num);
                else
                    controle.append("-> Numéro dans arbre invalide :\n")
                            .append(att_iter->second);
            }
        }
        else
            controle.append("-> Attribut d'arbre invalide : ").append(att_iter->first)
                    .append(".\nNoeud d'arbre de :\n").append(iter.parent()->name());
    }
    return arbre;
}

void bdd::associated_xml(entity &ent, xml_iterator iter, QString &controle) {
    if(iter->name() == "restriction") {
        auto rest = attribut_xml(iter,"code",controle);
        if(controle.isEmpty()){
            auto code = code_from_string(rest,Restriction_Code,false,controle);
            if(controle.isEmpty())
                set_restriction(ent,code.value());
        }
        if(!controle.isEmpty())
            controle.prepend("-> Dans la restriction\n");
    }
    else if (iter->name() != "arbre"){
        auto ent_ass = make_entity(iter->name());
        if(!ent_ass)
            controle.append("-> Donnée associée invalide : ").append(iter->name());
        else {
            std::list<decltype(iter->attributes().cbegin())> multi_atts;
            std::list<decltype(iter->attributes().cbegin())> after_atts;
            for (auto att_it = iter->attributes().cbegin();
                 att_it != iter->attributes().cend() &&controle.isEmpty(); ++att_it) {
                if(is_multiple_associated_xml(*att_it))
                    multi_atts.push_back(att_it);
                else if(is_after_associated_xml(*att_it))
                    after_atts.push_back(att_it);
                else
                    hydrate_attribut_associated_controled_xml(*ent_ass,*att_it,ent,controle);
            }
            if(controle.isEmpty()) {
                if(multi_atts.empty()){
                    auto exist = exists_unique_ensemble(*ent_ass);
                    if(exist == existe_uni::Conflit)
                        controle.append("-> Erreur d'unicité :\n").append(ent_ass->affiche());
                    else
                        save(*ent_ass);
                    for (auto iter = after_atts.cbegin(); iter != after_atts.cend() &&controle.isEmpty(); ++iter)
                        hydrate_attribut_associated_controled_xml(*ent_ass,**iter,ent,controle);
                }
                else {
                    std::list<xml_list_atts> multi;
                    for (auto iter = multi_atts.cbegin(); iter != multi_atts.cend() &&controle.isEmpty(); ++iter)
                        multi.push_back(list_multiple_associated_xml(**iter,controle));
                    std::list<xml_list_atts::const_iterator> multi_iter;
                    for (auto iter = multi.cbegin(); iter != multi.cend() &&controle.isEmpty(); ++iter) {
                        multi_iter.push_back(iter->cbegin());
                        hydrate_attribut_associated_controled_xml(*ent_ass,*(iter->cbegin()),ent,controle);
                    }
                    if(controle.isEmpty()) {
                        auto controle_while = true;
                        while(controle_while &&controle.isEmpty()){
                            ent_ass->set_id(0);
                            auto exist = exists_unique_ensemble(*ent_ass);
                            if(exist == existe_uni::Conflit)
                                controle.append("Erreur d'unicité :\n").append(ent_ass->affiche());
                            else
                                save(*ent_ass);
                            for (auto iter = after_atts.cbegin(); iter != after_atts.cend() &&controle.isEmpty(); ++iter)
                                hydrate_attribut_associated_controled_xml(*ent_ass,**iter,ent,controle);
                            if(controle.isEmpty()) {
                                auto iter_iter = multi_iter.begin();
                                auto iter_multi = multi.cbegin();
                                auto controle_do = false;
                                do {
                                    ++(*iter_iter);
                                    if(*iter_iter == iter_multi->cend()) {
                                        *iter_iter = iter_multi->cbegin();
                                        controle_do = true;
                                    }
                                    hydrate_attribut_associated_controled_xml(*ent_ass,**iter_iter,ent,controle);
                                    if(controle_do) {
                                        ++iter_iter;
                                        ++iter_multi;
                                    }
                                } while(controle_do &&iter_iter != multi_iter.end());
                                controle_while = iter_iter != multi_iter.end();
                            }
                        }
                    }
                }
            }
        }
    }
}

flag bdd::code_from_string(const QString &str, uint id_cat, bool for_entity, QString &controle) const {
    flag code;
    auto list = str.split("|");
    if(for_entity)
        for (auto iter = list.cbegin(); iter != list.cend() &&controle.isEmpty(); ++iter)
            code |= str_id_to_enum(*iter,id_cat,controle);
    else
        for (auto iter = list.cbegin(); iter != list.cend() &&controle.isEmpty(); ++iter)
            code |= str_categorie_to_enum(*iter,id_cat,controle);
    return code;
}

bool bdd::copy(const QString &name) {
    bdd bdd(m_bdd.driverName(), name,{});
    if(bdd.exists() &&bdd.is_valid()) {
        QFile file(name);
        return file.copy(m_name);
    }
    return false;
}

bool bdd::creer() {
    if(open_bdd()) {
        m_manager->creer_version();
        mise_a_jour_bdd();
        return true;
    }
    return false;
}

bool bdd::exists(const entity &ent)
    {return m_manager->get(ent.id_entity()).exists(ent);}

bool bdd::exists_unique(entity &ent)
    {return m_manager->get(ent.id_entity()).exists_unique(ent) != Aucun;}

bool bdd::exists_unique(const entity &ent)
    {return m_manager->get(ent.id_entity()).exists_unique(ent) != Aucun;}

existe_uni bdd::exists_unique_ensemble(entity &ent)
    {return m_manager->get(ent.id_entity()).exists_unique(ent);}

existe_uni bdd::exists_unique_ensemble(const entity &ent)
    {return m_manager->get(ent.id_entity()).exists_unique(ent);}

std::pair<existe_uni,idt> bdd::exists_unique_id(const entity &ent)
    {return m_manager->get(ent.id_entity()).exists_unique_id(ent);}

fichier::doc_xml bdd::export_xml(vector_ptr<entity> &&vec, flag option) {
    fichier::doc_xml doc;
    doc.begin()->set_name("entities");
    for (auto ent_it = vec.begin(); ent_it != vec.end(); ++ent_it) {
        auto ent_xml_iter = doc.push_back(doc.begin(),info(*ent_it).name());
        for (post pos = entities::entity::Nbr_Att; pos != ent_it->nbr_att(); ++pos) {
            auto att_xml_iter = doc.push_back(ent_xml_iter,ent_it->attribut_name(pos));
            att_xml_iter->set_text(ent_it->data(pos).toString());
        }
        if(option &Restriction_Exportation_Xml) {
            auto restriction_xml_iter = doc.push_back(ent_xml_iter,"restriction");
            auto restriction = get_restriction(*ent_it);
            restriction_xml_iter->set_attribut("code",QString::number(restriction.value()));
        }
        if(m_manager->get(ent_it->id_entity()).type_manager() &b2d::Arbre_Type_Manager){
            auto arb_xml_iter = doc.push_back(ent_xml_iter,"arbre");
            auto pos = ent_it->position("ref");
            if(pos != ent_it->nbr_att()) {
                ent_it->set_id(get_id_parent(*ent_it));
                if(ent_it->id() &&get(*ent_it))
                    arb_xml_iter->set_attribut("ref_parent",ent_it->data(pos).toString());
            }
        }
    }
    return doc;
}

bool bdd::get(entity &ent)
    {return m_manager->get(ent.id_entity()).get(ent);}

idt bdd::get_id_parent(const entity &ent)
    {return m_manager->get(ent.id_entity()).get_id_parent(ent.id());}

vector_ptr<entity> bdd::get_list(entidt id_ent)
    {return m_manager->get(id_ent).get_vector_entity();}

flag bdd::get_restriction(const entity &ent)
    {return m_manager->get(ent.id_entity()).get_restriction(ent.id());}

bool bdd::get_unique(entity &ent)
    {return m_manager->get(ent.id_entity()).get_unique(ent);}

void bdd::hydrate_attribut_xml(entities::entity &ent, post pos,
                             xml_iterator iter, QString &controle) {
    if(!iter->has_attributes() || iter->attributes().cbegin()->first != "type") {
        auto fkey_iter = info(ent).foreign_key_name().find(pos);
        if(fkey_iter == info(ent).foreign_key_name().end()){
            if(iter->has_attributes())
                controle.append("-> Seul les noeuds hydratant une clé étrangère\n")
                        .append("peuvent avoir un attribut différent de 'type'.")
                        .append("Attribut xml : ").append(iter->attributes().cbegin()->first).append(" = ")
                        .append(iter->attributes().cbegin()->second);
            else
                ent.set_data(pos, iter->text());
        }
        else {// Clé étrangère
            auto fkey_entity = make_entity(fkey_iter->second);
            if(iter.leaf()) {// Définie par un attribut
                if(iter->attributes().size() != 1)
                    controle.append("-> Une clé étrangère doit être hydratée par un attribut unique\nou par un ensemble de noeuds.");
                else {
                    auto pair_iter = iter->attributes().cbegin();
                    auto pos_fkey = position_xml(*fkey_entity,pair_iter->first,controle);
                    if(controle.isEmpty())
                        fkey_entity->set_data(pos_fkey,pair_iter->second);
                }
            }
            else // Définie par un ensemble de noeuds
                hydrate_entity_xml(*fkey_entity,iter,controle);
            if(controle.isEmpty()) {
                if(exists_unique(*fkey_entity))
                    ent.set_data(pos, fkey_entity->id());
                else
                    controle.append("-> Entité unique introuvable:\n").append(fkey_entity->affiche());
            }
            if(!controle.isEmpty())
                controle.prepend(QString("-> Dans la clé étrangère sur : ").append(fkey_iter->second).append("\n"));
        }
    }
    else {
        auto type = iter->attributes().cbegin()->second;
        if(type == "code") {
            auto code = code_from_string(iter->text(),ent.id_entity(),true,controle);
            if(controle.isEmpty())
                ent.set_data(pos, code.value());
        }
        else if(type == "couleur") {
            QColor color(iter->text());
            if(color.isValid())
                ent.set_data(pos,color.name());
            else
                controle.append("-> Couleur invalide ").append(iter->name()).append(" = ").append(iter->text());
        }
        else if (type == "enum") {
            auto i = str_id_to_enum(iter->text(),ent.id_entity(),controle);
            if(controle.isEmpty())
                ent.set_data(pos,i);
        }
        else if (type == "police") {
            QFont police;
            for (auto iter_police = iter.cbegin_child(); iter_police &&controle.isEmpty(); ++iter_police) {
                if(iter_police->name() == "famille")
                    police.setFamily(iter_police->text());
                else if(iter_police->name() == "poids") {
                    auto p = str_categorie_to_enum(iter_police->text(),Font_Weight,controle);
                    if(controle.isEmpty())
                        police.setWeight(static_cast<int>(p));
                    else
                        controle.prepend(QString("-> Dans le poids de la police de : ")
                                         .append(iter->name()).append(" = ").append(iter_police->text()).append("\n"));
                }
                else if (iter_police->name() == "taille") {
                    bool ok;
                    police.setPointSize(iter_police->text().toInt(&ok));
                    if(!ok)
                        controle.append("-> Taille de police invalide : ").append(iter_police->text());
                }
                else
                    controle.append("-> Noeud de police inconnue : ").append(iter_police->name());
            }
            if(controle.isEmpty())
                ent.set_data(pos,police.toString());
            else
                controle.prepend(QString("-> Dans la police de : ").append(iter->name()).append("\n"));
        }
        else
            controle.append("-> Type de noeud xml inconnue : ").append(type);
    }
}

void bdd::hydrate_attribut_associated_xml(entity &entity_ass, const std::pair<const QString,QString> &pair,
                                       const entity &ent, QString &controle) {
    if(pair.first == "id_ent"){
        auto pos = position_xml(entity_ass,pair.second,controle);
        if(controle.isEmpty())
            entity_ass.set_data(pos,ent.id());
    }
    else if(pair.first == "code") {
        auto code = code_from_string(pair.second,entity_ass.id_entity(),true,controle);
        if(controle.isEmpty()) {
            auto pos = position_xml(entity_ass,"Code",controle);
            if(controle.isEmpty())
                entity_ass.set_data(pos,code.value());
        }
    }
    else if(pair.first == "restriction") {
        auto code = code_from_string(pair.second,Restriction_Code,false,controle);
        if(controle.isEmpty())
            set_restriction(entity_ass,code.value());
        else
            controle.prepend("-> Dans la restriction de l'entité associée\n");
    }
    else
        controle.append("-> Attribut inconnu : ").append(pair.first);
}

std::list<bdd::xml_iterator> bdd::hydrate_entity_xml(entity &entity, xml_iterator iter, QString &controle) {
    std::list<xml_iterator> assotiated;
    for (auto sub_iter = iter.cbegin_child(); sub_iter &&controle.isEmpty(); ++sub_iter) {
        if(is_associated_xml(sub_iter,entity))
                assotiated.push_back(sub_iter);
        else {
            auto pos = position_xml(entity,sub_iter->name(),controle);
            if(controle.isEmpty())
                hydrate_attribut_controled_xml(entity, pos, sub_iter,controle);
        }
    }
    return assotiated;
}

QString bdd::import_xml(const fichier::doc_xml &doc){
    auto iter = doc.cbegin().cbegin_child();
    QString controle;
    while(controle.isEmpty() &&iter) {
        auto ent = make_entity(iter->name());
        if(!ent)
            controle.append("-> Nom d'entité inconnu : ").append(iter->name());
        else {
            auto associated = hydrate_entity_xml(*ent,iter,controle);
            if(controle.isEmpty()) {
                if(!ent->is_valid())
                    controle.append("-> Entité invalide.");
                else {
                    auto exist = exists_unique_ensemble(*ent);
                    if(exist == existe_uni::Conflit)
                        controle.append("-> Erreur d'unicité.");
                    else if (exist != Aucun || !(m_manager->get(ent->id_entity()).type_manager() &b2d::Arbre_Type_Manager))
                        save(*ent);
                    else {
                        auto iter_arbre = std::find_if(associated.cbegin(),associated.cend(),
                                                    [](const xml_iterator &i){return i->name() == "arbre";});
                        entities_base::arbre arb;
                        if(iter_arbre != associated.cend()){
                            arb = arbre_xml(*iter_arbre, controle);
                            associated.erase(iter_arbre);
                        }
                        if(controle.isEmpty())
                           insert(*ent,arb.parent(),arb.num());
                    }
                }
                if(controle.isEmpty())
                    for (auto iter_list = associated.cbegin(); iter_list != associated.cend() &&controle.isEmpty(); ++iter_list)
                        associated_controled_xml(*ent,*iter_list,controle);
            }
            if(!controle.isEmpty())
                controle.prepend(QString("-> Dans l'entité : ").append(info(*ent).name())
                                 .append("\n").append(ent->affiche()).append("\n"));
        }
        ++iter;
    }
    return controle;
}

const manager::info_bdd &bdd::info(const entity &ent) const
    {return m_manager->get(ent.id_entity()).info();}

void bdd::insert(entity &ent, idt id_parent, int num)
    {m_manager->get(ent.id_entity()).insert(ent,id_parent,num);}

void bdd::insert(const entity &ent, idt id_parent, int num)
    {m_manager->get(ent.id_entity()).insert(ent,id_parent,num);}

bool bdd::is_associated_xml(xml_iterator iter, entity &ent) const {
        if(!iter->has_attributes() || iter->has_text())
            return false;
        if(iter->attributes().size() > 1)
            return true;
        auto att = iter->attributes().cbegin()->first;
        if(att == "type")
            return false;
        if(att == "restriction")
            return true;
        auto pos = ent.position(iter->name());
        if(pos == ent.nbr_att())
            return true;
        return info(ent).foreign_key_name().find(pos) == info(ent).foreign_key_name().cend();
    }

bdd::xml_list_atts bdd::list_multiple_associated_xml(const std::pair<const QString,QString> &pair, QString &controle) {
    controle.append("L'attribut est inconnu : ").append(pair.first);
    return xml_list_atts();
}

void bdd::mise_a_jour_bdd() {
    for (idt type = 0; type != m_version.size(); ++type) {
        if(!m_manager->exists_version(type))
            liste_mise_a_jour_bdd(bdd_version::Initiale,type);
        auto version_bdd = m_manager->num_version(type);
        if(m_version[type] != version_bdd) {
            if(m_version[type] > version_bdd) {
                while(m_version[type] != version_bdd) {
                    liste_mise_a_jour_bdd(version_bdd,type);
                    version_bdd = m_manager->num_version(type);
                }
            }
            else {
                QString str("La base de données est d'une version plus récente que le programme pour le type: ");
                str.append(QString::number(type)).append(" \n");
                str.append("Version de la base de données requis par le programmen :").
                        append(QString::number(m_version[type])).append(".\n");
                str.append("Version de la base de données :").append(QString::number(m_manager->num_version(type))).append(".\n");
                throw std::runtime_error(str.toStdString());
            }
        }
    }
}

bool bdd::open() {
    if(open_bdd()) {
        mise_a_jour_bdd();
        return true;
    }
    return false;
}

bool bdd::open_bdd() {
    if(m_bdd.open()) {
        set_bdd();
        return true;
    }
    return false;
}

bool bdd::same_in_bdd(const entity &ent)
    {return m_manager->get(ent.id_entity()).same_in_bdd(ent);}

void bdd::save(entity &ent)
    {m_manager->get(ent.id_entity()).save(ent);}

void bdd::save(const entity &ent)
    {m_manager->get(ent.id_entity()).save(ent);}

void bdd::save(entity &ent, flag restrict)
    {m_manager->get(ent.id_entity()).save(ent, restrict);}

void bdd::save(vector_ptr<entity> &vector) {
    for (auto iter = vector.begin(); iter != vector.end(); ++iter)
        save(*iter);
}

void bdd::save(const vector_ptr<entity> &vector) {
    for (auto iter = vector.cbegin(); iter != vector.cend(); ++iter)
        save(*iter);
}

void bdd::save_unique(entity &ent)
    {m_manager->get(ent.id_entity()).save_unique(ent);}

void bdd::save_unique(const entity &ent)
    {m_manager->get(ent.id_entity()).save_unique(ent);}

fichier::doc_xml bdd::schema_xml_for_import() const{
    using namespace fichier;
    //const auto restToStr = RestrictionToStr();
    doc_xml schema;
    auto it = schema.begin();
    it->set_name("xs:schema");
    it->set_attribut("xmlns:xs","http://www.w3.org/2001/XMLSchema");
    it->set_attribut("elementFormDefault","qualified");
    it = schema.emplace_back(it,"xs:element");
    it->set_attribut("name","entities");
    it = schema.emplace_back(it,"xs:complexType");
    it = schema.emplace_back(it,"xs:choice");
    it->set_attribut("maxOccurs","unbounded");
    for (entidt i = 0; i <nbr_entity(); ++i) {
        if(managers().valide(i)) {
            auto ent_it = schema.emplace_back(it,"xs:element");
            ent_it->set_attribut("ref",managers().info(i).name());
        }
    }
    it.to_root();
    for (entidt i = 0; i < nbr_entity(); ++i) {
        if(managers().valide(i)) {
            auto ent_it = schema.emplace_back(it,"xs:element");
            ent_it->set_attribut("name",managers().info(i).name());
            ent_it = schema.emplace_back(ent_it,"xs:complexType");
            ent_it = schema.emplace_back(ent_it,"xs:all");
            auto namesAtt = managers().get(i).names_attribut();
            for (szt j = 1; j != namesAtt.size(); ++j) {
                auto att_it = schema.emplace_back(ent_it,"xs:element");
                att_it->set_attribut("name",namesAtt.at(j));
                auto key_it = managers().info(i).foreign_key_name().find(j);
                if(key_it != managers().info(i).foreign_key_name().cend()){
                    if(key_it->second.isEmpty()) {
                        att_it = schema.emplace_back(att_it,"xs:complexType");
                        att_it = schema.emplace_back(att_it,"xs:element");
                        att_it->set_attribut("ref","one_Unique");
                    }
                    else {
                        auto k = managers().find(key_it->second);
                        auto str = key_it->second;
                        if(managers().info(k).nbr_set_unique() != 0) {
                            att_it = schema.emplace_back(att_it,"xs:complexType");
                            if(managers().info(k).nbr_set_unique() == 1) {
                                att_it = schema.emplace_back(att_it,"xs:element");
                                att_it->set_attribut("ref",key_it->second + "_Unique");
                            }
                            else {
                                att_it = schema.emplace_back(att_it,"xs:choice");
                                for(numt l = 0; l != managers().info(k).nbr_set_unique(); ++l) {
                                    auto unique_it = schema.emplace_back(att_it,"xs:element");
                                    unique_it->set_attribut("ref",key_it->second + "_Unique"+QString::number(l));
                                }
                            }
                        }
                    }
                }
            }
            ent_it.to_parent();
//            auto restriction = managers().get(i).enable_restriction();
//            for (auto iter_restriction = restToStr.cbegin(); iter_restriction != restToStr.cend(); ++iter_restriction) {
//                if(restriction &iter_restriction->first) {
//                    auto iter_rest = schema.emplace_back(ent_it,"xs:attribute");
//                    iter_rest->set_attribut("name",iter_restriction->second);
//                }
//            }
        }

    }
    for (entidt i = 0; i < nbr_entity(); ++i){
        if(managers().valide(i)) {
            for(numt j = 0; j != managers().info(i).nbr_set_unique(); ++j) {
                auto unique_it = schema.emplace_back(it,"xs:element");
                unique_it->set_attribut("name",managers().info(i).name()  + "_Unique"+QString::number(j));
                unique_it = schema.emplace_back(unique_it,"xs:complexType");
                auto att_unique = managers().info(i).attribut_unique(j);
                if(att_unique.size() == 1){
                    unique_it = schema.emplace_back(unique_it,"xs:element");
                    unique_it->set_attribut("name",managers().get(i).names_attribut().at(att_unique.at(0)));
                }
                else {
                    unique_it = schema.emplace_back(unique_it,"xs:all");
                    for (auto iter_num = att_unique.cbegin(); iter_num != att_unique.cend(); ++iter_num) {
                        auto att_it = schema.emplace_back(unique_it,"xs:element");
                        att_it->set_attribut("name",managers().get(i).names_attribut().at(*iter_num));
                    }
                }
            }
        }
    }
    return schema;
}

void bdd::set_restriction(const entity &ent, flag restrict)
    {m_manager->get(ent.id_entity()).set_restriction(ent.id(), restrict);}

void bdd::set_bdd() {
    m_bdd.exec("PRAGMA foreign_keys = ON;");
    QSqlQuery req = QSqlQuery(m_bdd);
    req.setForwardOnly(true);
    m_manager->set_requete(req);
}

void bdd::set_name(const QString &name) {
    file_interface::set_name(name);
    if(m_bdd.isOpen())
        m_bdd.close();
    m_bdd.setDatabaseName(name);
}

enumt bdd::str_categorie_to_enum(const QString &str, flag categorie, QString &controle) const noexcept {
    if(categorie &Restriction_Code) {
        if(str == "Aucune")
            return b2d::Aucune;
        if(str == QString("Modification"))
            return b2d::Modif;
        if(str == QString("Suppression"))
            return b2d::Suppr;
    }
    if(categorie &Line_Style) {
        if(str == "Solid_Line")
            return Qt::SolidLine;
        if(str == "Dash_Line")
            return Qt::DashLine;
        if(str == "Dot_Line")
            return Qt::DotLine;
        if(str == "Dash_Dot_Line")
            return Qt::DashDotLine;
        if(str == "Dash_Dot_Dot_Line")
            return Qt::DashDotDotLine;
    }
    if(categorie &Brush_Style) {
        if(str == "No_Brush")
            return Qt::NoBrush;
        if(str == "Solid_Pattern")
            return Qt::SolidPattern;
        if(str == "Dense_1_Pattern")
            return Qt::Dense1Pattern;
        if(str == "Dense_2_Pattern")
            return Qt::Dense2Pattern;
        if(str == "Dense_3_Pattern")
            return Qt::Dense3Pattern;
        if(str == "Dense_4_Pattern")
            return Qt::Dense4Pattern;
        if(str == "Dense_5_Pattern")
            return Qt::Dense5Pattern;
        if(str == "Dense_6_Pattern")
            return Qt::Dense6Pattern;
        if(str == "Dense_7_Pattern")
            return Qt::Dense7Pattern;
        if(str == "Hor_Pattern")
            return Qt::HorPattern;
        if(str == "Ver_Pattern")
            return Qt::VerPattern;
        if(str == "Cross_Pattern")
            return Qt::CrossPattern;
        if(str == "B_Diag_Pattern")
            return Qt::BDiagPattern;
        if(str == "F_Diag_Pattern")
            return Qt::FDiagPattern;
        if(str == "Diag_Cross_Pattern")
            return Qt::DiagCrossPattern;
        if(str == "Linear_Gradient_Pattern")
            return Qt::LinearGradientPattern;
        if(str == "Radial_Gradient_Pattern")
            return Qt::RadialGradientPattern;
        if(str == "Conical_Gradient_Pattern")
            return Qt::ConicalGradientPattern;
        if(str == "Texture_Pattern")
            return Qt::TexturePattern;
    }
    if(categorie &Font_Weight) {
        if(str == "Thin")
            return QFont::Thin;
        if(str == "Extra_Light")
            return QFont::ExtraLight;
        if(str == "Light")
            return QFont::Light;
        if(str == "Normal")
            return QFont::Normal;
        if(str == "Medium")
            return QFont::Medium;
        if(str == "Demi_Bold")
            return QFont::DemiBold;
        if(str == "Bold")
            return QFont::Bold;
        if(str == "Extra_Bold")
            return QFont::ExtraBold;
        if(str == "Black")
            return QFont::Black;

    }
    controle.append("-> Enum invalide de categorie ").append(QString::number(static_cast<uint>(categorie))).append(": ").append(str);
    return Invalide_Enum;
}

bdd::vector_id_name bdd::table_entity_names(bool arbre) const {
    std::vector<std::pair<int,QString>> vec;
    vec.reserve(nbr_entity());
    for(entidt i = 0; i < nbr_entity(); ++i) {
        if(managers().valide(i)) {
            vec.emplace_back(i,managers().info(i).name());
            if(arbre &&!managers().get(i).info_arbre().name().isEmpty())
                vec.emplace_back(-i,managers().get(i).info_arbre().name());
        }
    }
    std::sort(vec.begin(),vec.end(),
              [](const std::pair<int,QString> &p1, const std::pair<int,QString> &p2)->bool {return p1.second < p2.second;});
    return vec;
}

bool bdd::test_autorisation_v(idt id, entidt id_entity, flag autoris)
    {return !(m_manager->get(id_entity).get_restriction(id) &autoris);}
