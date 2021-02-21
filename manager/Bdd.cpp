#include "Bdd.h"

using namespace bddMPS;

entities_base::arbre Bdd::arbreXml(xml_iterator iter, QString &controle){
    entities_base::arbre arbre;
    for (auto att_iter = iter->attributes().cbegin(); att_iter != iter->attributes().cend() &&controle.isEmpty(); ++att_iter) {
        if(att_iter->first == "ref_parent") {
            auto parent = makeentity(iter.parent()->name());
            parent->set_data(parent->position("Ref"),att_iter->second);
            if (existsUnique(*parent))
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

void Bdd::associatedXml(entity &ent, xml_iterator iter, QString &controle) {
    if(iter->name() == "Restriction") {
        auto rest = attributXml(iter,"code",controle);
        if(controle.isEmpty()){
            auto code = codeFromQString(rest,RestrictionCode,false,controle);
            if(controle.isEmpty())
                setRestriction(entity,code.value());
        }
        if(!controle.isEmpty())
            controle.prepend("-> Dans la restriction\n");
    }
    else if (iter->name() != "arbre"){
        auto ent_ass = makeentity(iter->name());
        if(!ent_ass)
            controle.append("-> Donnée associée invalide : ").append(iter->name());
        else {
            std::list<decltype(iter->attributes().cbegin())> multi_atts;
            std::list<decltype(iter->attributes().cbegin())> after_atts;
            for (auto iter_att = iter->attributes().cbegin();
                 iter_att != iter->attributes().cend() &&controle.isEmpty(); ++iter_att) {
                if(isMultipleAssociatedXml(*iter_att))
                    multi_atts.push_back(iter_att);
                else if(isAfterAssociatedXml(*iter_att))
                    after_atts.push_back(iter_att);
                else
                    hydrateAttributAssociatedControledXml(*ent_ass,*iter_att,entity,controle);
            }
            if(controle.isEmpty()) {
                if(multi_atts.empty()){
                    auto exist = existsUniqueEnsemble(*ent_ass);
                    if(exist == existeUni::Conflit)
                        controle.append("-> Erreur d'unicité :\n").append(ent_ass->affiche());
                    else
                        save(*ent_ass);
                    for (auto iter = after_atts.cbegin(); iter != after_atts.cend() &&controle.isEmpty(); ++iter)
                        hydrateAttributAssociatedControledXml(*ent_ass,**iter,entity,controle);
                }
                else {
                    std::list<xml_list_atts> multi;
                    for (auto iter = multi_atts.cbegin(); iter != multi_atts.cend() &&controle.isEmpty(); ++iter)
                        multi.push_back(listMultipleAssociatedXml(**iter,controle));
                    std::list<xml_list_atts::const_iterator> multi_iter;
                    for (auto iter = multi.cbegin(); iter != multi.cend() &&controle.isEmpty(); ++iter) {
                        multi_iter.push_back(iter->cbegin());
                        hydrateAttributAssociatedControledXml(*ent_ass,*(iter->cbegin()),entity,controle);
                    }
                    if(controle.isEmpty()) {
                        auto controle_while = true;
                        while(controle_while &&controle.isEmpty()){
                            ent_ass->set_id(0);
                            auto exist = existsUniqueEnsemble(*ent_ass);
                            if(exist == existeUni::Conflit)
                                controle.append("Erreur d'unicité :\n").append(ent_ass->affiche());
                            else
                                save(*ent_ass);
                            for (auto iter = after_atts.cbegin(); iter != after_atts.cend() &&controle.isEmpty(); ++iter)
                                hydrateAttributAssociatedControledXml(*ent_ass,**iter,entity,controle);
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
                                    hydrateAttributAssociatedControledXml(*ent_ass,**iter_iter,entity,controle);
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

flag Bdd::codeFromQString(const QString &str, uint idCat, bool forentity, QString &controle) const {
    flag code;
    auto list = str.split("|");
    if(forentity)
        for (auto iter = list.cbegin(); iter != list.cend() &&controle.isEmpty(); ++iter)
            code |= strIdToEnum(*iter,idCat,controle);
    else
        for (auto iter = list.cbegin(); iter != list.cend() &&controle.isEmpty(); ++iter)
            code |= strCategorieToEnum(*iter,idCat,controle);
    return code;
}

bool Bdd::copy(const QString &name) {
    Bdd bdd(m_bdd.driverName(), name,{});
    if(bdd.exists() &&bdd.is_valid()) {
        QFile file(name);
        return file.copy(m_name);
    }
    return false;
}

bool Bdd::creer() {
    if(openBdd()) {
        m_manager->creerVersion();
        miseAJourBdd();
        return true;
    }
    return false;
}

bool Bdd::exists(const entity &ent)
    {return m_manager->get(entity.id_entity()).exists(entity);}

bool Bdd::existsUnique(entity &ent)
    {return m_manager->get(entity.id_entity()).existsUnique(entity) != Aucun;}

bool Bdd::existsUnique(const entity &ent)
    {return m_manager->get(entity.id_entity()).existsUnique(entity) != Aucun;}

existeUni Bdd::existsUniqueEnsemble(entity &ent)
    {return m_manager->get(entity.id_entity()).existsUnique(entity);}

existeUni Bdd::existsUniqueEnsemble(const entity &ent)
    {return m_manager->get(entity.id_entity()).existsUnique(entity);}

std::pair<existeUni,idt> Bdd::existsUniqueId(const entity &ent)
    {return m_manager->get(entity.id_entity()).existsUniqueId(entity);}

fichierMPS::doc_xml Bdd::exportXml(vector_ptr<entity> &&vec, flag option) {
    fichierMPS::doc_xml doc;
    doc.begin()->set_name("entities");
    for (auto entity_iter = vec.begin(); entity_iter != vec.end(); ++entity_iter) {
        auto ent_xml_iter = doc.push_back(doc.begin(),info(*ent_iter).name());
        for (post pos = entities::entity::Nbr_Att; pos != entity_iter->nbr_att(); ++pos) {
            auto att_xml_iter = doc.push_back(ent_xml_iter,entity_iter->attribut_name(pos));
            att_xml_iter->set_text(entity_iter->data(pos).to_string());
        }
        if(option &RestrictionExportationXml) {
            auto restriction_xml_iter = doc.push_back(ent_xml_iter,"Restriction");
            auto restriction = getRestriction(*ent_iter);
            restriction_xml_iter->set_attribut("code",QString::number(restriction.value()));
        }
        if(m_manager->get(entity_iter->id_entity()).typeManager() &bddMPS::arbreTypeManager){
            auto arb_xml_iter = doc.push_back(ent_xml_iter,"arbre");
            auto pos = entity_iter->position("Ref");
            if(pos != entity_iter->nbr_att()) {
                entity_iter->set_id(getIdParent(*ent_iter));
                if(entity_iter->id() &&get(*ent_iter))
                    arb_xml_iter->set_attribut("ref_parent",entity_iter->data(pos).to_string());
            }
        }
    }
    return doc;
}

bool Bdd::get(entity &ent)
    {return m_manager->get(entity.id_entity()).get(entity);}

idt Bdd::getIdParent(const entity &ent)
    {return m_manager->get(entity.id_entity()).getIdParent(entity.id());}

vector_ptr<entity> Bdd::getList(entidt id_ent)
    {return m_manager->get(id_ent).getVectorentity();}

flag Bdd::getRestriction(const entity &ent)
    {return m_manager->get(entity.id_entity()).getRestriction(entity.id());}

bool Bdd::getUnique(entity &ent)
    {return m_manager->get(entity.id_entity()).getUnique(entity);}

void Bdd::hydrateAttributXml(entities::entity &ent, post pos,
                             xml_iterator iter, QString &controle) {
    if(!iter->has_attributes() || iter->attributes().cbegin()->first != "type") {
        auto fkey_iter = info(entity).foreignKeyName().find(pos);
        if(fkey_iter == info(entity).foreignKeyName().end()){
            if(iter->has_attributes())
                controle.append("-> Seul les noeuds hydratant une clé étrangère\n")
                        .append("peuvent avoir un attribut différent de 'type'.")
                        .append("Attribut xml : ").append(iter->attributes().cbegin()->first).append(" = ")
                        .append(iter->attributes().cbegin()->second);
            else
                entity.set_data(pos, iter->text());
        }
        else {// Clé étrangère
            auto fkey_entity = makeentity(fkey_iter->second);
            if(iter.leaf()) {// Définie par un attribut
                if(iter->attributes().size() != 1)
                    controle.append("-> Une clé étrangère doit être hydratée par un attribut unique\nou par un ensemble de noeuds.");
                else {
                    auto pair_iter = iter->attributes().cbegin();
                    auto pos_fkey = positionXml(*fkey_entity,pair_iter->first,controle);
                    if(controle.isEmpty())
                        fkey_entity->set_data(pos_fkey,pair_iter->second);
                }
            }
            else // Définie par un ensemble de noeuds
                hydrateentityXml(*fkey_entity,iter,controle);
            if(controle.isEmpty()) {
                if(existsUnique(*fkey_entity))
                    entity.set_data(pos, fkey_entity->id());
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
            auto code = codeFromQString(iter->text(),entity.id_entity(),true,controle);
            if(controle.isEmpty())
                entity.set_data(pos, code.value());
        }
        else if(type == "couleur") {
            QColor color(iter->text());
            if(color.is_valid())
                entity.set_data(pos,color.name());
            else
                controle.append("-> Couleur invalide ").append(iter->name()).append(" = ").append(iter->text());
        }
        else if (type == "enum") {
            auto i = strIdToEnum(iter->text(),entity.id_entity(),controle);
            if(controle.isEmpty())
                entity.set_data(pos,i);
        }
        else if (type == "police") {
            QFont police;
            for (auto iter_police = iter.cbegin_child(); iter_police &&controle.isEmpty(); ++iter_police) {
                if(iter_police->name() == "Famille")
                    police.setFamily(iter_police->text());
                else if(iter_police->name() == "Poids") {
                    auto p = strCategorieToEnum(iter_police->text(),FontWeight,controle);
                    if(controle.isEmpty())
                        police.setWeight(static_cast<int>(p));
                    else
                        controle.prepend(QString("-> Dans le poids de la police de : ")
                                         .append(iter->name()).append(" = ").append(iter_police->text()).append("\n"));
                }
                else if (iter_police->name() == "Taille") {
                    bool ok;
                    police.setPointSize(iter_police->text().toInt(&ok));
                    if(!ok)
                        controle.append("-> Taille de police invalide : ").append(iter_police->text());
                }
                else
                    controle.append("-> Noeud de police inconnue : ").append(iter_police->name());
            }
            if(controle.isEmpty())
                entity.set_data(pos,police.to_string());
            else
                controle.prepend(QString("-> Dans la police de : ").append(iter->name()).append("\n"));
        }
        else
            controle.append("-> Type de noeud xml inconnue : ").append(type);
    }
}

void Bdd::hydrateAttributAssociatedXml(entity &entity_ass, const std::pair<const QString,QString> &pair,
                                       const entity &entity, QString &controle) {
    if(pair.first == "id_ent"){
        auto pos = positionXml(entity_ass,pair.second,controle);
        if(controle.isEmpty())
            entity_ass.set_data(pos,entity.id());
    }
    else if(pair.first == "code") {
        auto code = codeFromQString(pair.second,entity_ass.id_entity(),true,controle);
        if(controle.isEmpty()) {
            auto pos = positionXml(entity_ass,"Code",controle);
            if(controle.isEmpty())
                entity_ass.set_data(pos,code.value());
        }
    }
    else if(pair.first == "restriction") {
        auto code = codeFromQString(pair.second,RestrictionCode,false,controle);
        if(controle.isEmpty())
            setRestriction(entity_ass,code.value());
        else
            controle.prepend("-> Dans la restriction de l'entité associée\n");
    }
    else
        controle.append("-> Attribut inconnu : ").append(pair.first);
}

std::list<Bdd::xml_iterator> Bdd::hydrateentityXml(entity &entity, xml_iterator iter, QString &controle) {
    std::list<xml_iterator> assotiated;
    for (auto sub_iter = iter.cbegin_child(); sub_iter &&controle.isEmpty(); ++sub_iter) {
        if(isAssociatedXml(sub_iter,entity))
                assotiated.push_back(sub_iter);
        else {
            auto pos = positionXml(entity,sub_iter->name(),controle);
            if(controle.isEmpty())
                hydrateAttributControledXml(entity, pos, sub_iter,controle);
        }
    }
    return assotiated;
}

QString Bdd::importXml(const fichierMPS::doc_xml &doc){
    auto iter = doc.cbegin().cbegin_child();
    QString controle;
    while(controle.isEmpty() &&iter) {
        auto entity = makeentity(iter->name());
        if(!entity)
            controle.append("-> Nom d'entité inconnu : ").append(iter->name());
        else {
            auto associated = hydrateentityXml(*ent,iter,controle);
            if(controle.isEmpty()) {
                if(!entity->is_valid())
                    controle.append("-> Entité invalide.");
                else {
                    auto exist = existsUniqueEnsemble(*ent);
                    if(exist == existeUni::Conflit)
                        controle.append("-> Erreur d'unicité.");
                    else if (exist != Aucun || !(m_manager->get(entity->id_entity()).typeManager() &bddMPS::arbreTypeManager))
                        save(*ent);
                    else {
                        auto iter_arbre = std::find_if(associated.cbegin(),associated.cend(),
                                                    [](const xml_iterator &i){return i->name() == "arbre";});
                        entities_base::arbre arb;
                        if(iter_arbre != associated.cend()){
                            arb = arbreXml(*iter_arbre, controle);
                            associated.erase(iter_arbre);
                        }
                        if(controle.isEmpty())
                           insert(*ent,arb.parent(),arb.num());
                    }
                }
                if(controle.isEmpty())
                    for (auto iter_list = associated.cbegin(); iter_list != associated.cend() &&controle.isEmpty(); ++iter_list)
                        associatedControledXml(*ent,*iter_list,controle);
            }
            if(!controle.isEmpty())
                controle.prepend(QString("-> Dans l'entité : ").append(info(*ent).name())
                                 .append("\n").append(entity->affiche()).append("\n"));
        }
        ++iter;
    }
    return controle;
}

const managerMPS::InfoBdd &Bdd::info(const entity &ent) const
    {return m_manager->get(entity.id_entity()).info();}

void Bdd::insert(entity &ent, idt idParent, int num)
    {m_manager->get(entity.id_entity()).insert(entity,idParent,num);}

void Bdd::insert(const entity &ent, idt idParent, int num)
    {m_manager->get(entity.id_entity()).insert(entity,idParent,num);}

bool Bdd::isAssociatedXml(xml_iterator iter, entity &entity) const {
        if(!iter->has_attributes() || iter->has_text())
            return false;
        if(iter->attributes().size() > 1)
            return true;
        auto att = iter->attributes().cbegin()->first;
        if(att == "type")
            return false;
        if(att == "restriction")
            return true;
        auto pos = entity.position(iter->name());
        if(pos == entity.nbr_att())
            return true;
        return info(entity).foreignKeyName().find(pos) == info(entity).foreignKeyName().cend();
    }

Bdd::xml_list_atts Bdd::listMultipleAssociatedXml(const std::pair<const QString,QString> &pair, QString &controle) {
    controle.append("L'attribut est inconnu : ").append(pair.first);
    return xml_list_atts();
}

void Bdd::miseAJourBdd() {
    for (idt type = 0; type != m_version.size(); ++type) {
        if(!m_manager->existsVersion(type))
            listeMiseAJourBdd(bddVersion::Initiale,type);
        auto version_bdd = m_manager->numVersion(type);
        if(m_version[type] != version_bdd) {
            if(m_version[type] > version_bdd) {
                while(m_version[type] != version_bdd) {
                    listeMiseAJourBdd(version_bdd,type);
                    version_bdd = m_manager->numVersion(type);
                }
            }
            else {
                QString str("La base de données est d'une version plus récente que le programme pour le type: ");
                str.append(QString::number(type)).append(" \n");
                str.append("Version de la base de données requis par le programmen :").
                        append(QString::number(m_version[type])).append(".\n");
                str.append("Version de la base de données :").append(QString::number(m_manager->numVersion(type))).append(".\n");
                throw std::runtime_error(str.toStdString());
            }
        }
    }
}

bool Bdd::open() {
    if(openBdd()) {
        miseAJourBdd();
        return true;
    }
    return false;
}

bool Bdd::openBdd() {
    if(m_bdd.open()) {
        setBdd();
        return true;
    }
    return false;
}

bool Bdd::sameInBdd(const entity &ent)
    {return m_manager->get(entity.id_entity()).sameInBdd(entity);}

void Bdd::save(entity &ent)
    {m_manager->get(entity.id_entity()).save(entity);}

void Bdd::save(const entity &ent)
    {m_manager->get(entity.id_entity()).save(entity);}

void Bdd::save(entity &ent, flag restrict)
    {m_manager->get(entity.id_entity()).save(entity, restrict);}

void Bdd::save(vector_ptr<entity> &vector) {
    for (auto iter = vector.begin(); iter != vector.end(); ++iter)
        save(*iter);
}

void Bdd::save(const vector_ptr<entity> &vector) {
    for (auto iter = vector.cbegin(); iter != vector.cend(); ++iter)
        save(*iter);
}

void Bdd::saveUnique(entity &ent)
    {m_manager->get(entity.id_entity()).saveUnique(entity);}

void Bdd::saveUnique(const entity &ent)
    {m_manager->get(entity.id_entity()).saveUnique(entity);}

fichierMPS::doc_xml Bdd::schemaXmlForImport() const{
    using namespace fichierMPS;
    //const auto restToStr = RestrictionToStr();
    doc_xml schema;
    auto iter = schema.begin();
    iter->set_name("xs:schema");
    iter->set_attribut("xmlns:xs","http://www.w3.org/2001/XMLSchema");
    iter->set_attribut("elementFormDefault","qualified");
    iter = schema.emplace_back(iter,"xs:element");
    iter->set_attribut("name","entities");
    iter = schema.emplace_back(iter,"xs:complexType");
    iter = schema.emplace_back(iter,"xs:choice");
    iter->set_attribut("maxOccurs","unbounded");
    for (entidt i = 0; i <nbrentity(); ++i) {
        if(managers().valide(i)) {
            auto iter_entity = schema.emplace_back(iter,"xs:element");
            iter_entity->set_attribut("ref",managers().info(i).name());
        }
    }
    iter.to_root();
    for (entidt i = 0; i < nbrentity(); ++i) {
        if(managers().valide(i)) {
            auto iter_entity = schema.emplace_back(iter,"xs:element");
            iter_entity->set_attribut("name",managers().info(i).name());
            iter_entity = schema.emplace_back(iter_entity,"xs:complexType");
            iter_entity = schema.emplace_back(iter_entity,"xs:all");
            auto namesAtt = managers().get(i).names_attributs();
            for (szt j = 1; j != namesAtt.size(); ++j) {
                auto iter_att = schema.emplace_back(iter_entity,"xs:element");
                iter_att->set_attribut("name",namesAtt.at(j));
                auto iter_key = managers().info(i).foreignKeyName().find(j);
                if(iter_key != managers().info(i).foreignKeyName().cend()){
                    if(iter_key->second.isEmpty()) {
                        iter_att = schema.emplace_back(iter_att,"xs:complexType");
                        iter_att = schema.emplace_back(iter_att,"xs:element");
                        iter_att->set_attribut("ref","one_Unique");
                    }
                    else {
                        auto k = managers().find(iter_key->second);
                        auto str = iter_key->second;
                        if(managers().info(k).nbrSetUnique() != 0) {
                            iter_att = schema.emplace_back(iter_att,"xs:complexType");
                            if(managers().info(k).nbrSetUnique() == 1) {
                                iter_att = schema.emplace_back(iter_att,"xs:element");
                                iter_att->set_attribut("ref",iter_key->second + "_Unique");
                            }
                            else {
                                iter_att = schema.emplace_back(iter_att,"xs:choice");
                                for(numt l = 0; l != managers().info(k).nbrSetUnique(); ++l) {
                                    auto iter_unique = schema.emplace_back(iter_att,"xs:element");
                                    iter_unique->set_attribut("ref",iter_key->second + "_Unique"+QString::number(l));
                                }
                            }
                        }
                    }
                }
            }
            iter_entity.to_parent();
//            auto restriction = managers().get(i).enableRestriction();
//            for (auto iter_restriction = restToStr.cbegin(); iter_restriction != restToStr.cend(); ++iter_restriction) {
//                if(restriction &iter_restriction->first) {
//                    auto iter_rest = schema.emplace_back(iter_entity,"xs:attribute");
//                    iter_rest->set_attribut("name",iter_restriction->second);
//                }
//            }
        }

    }
    for (entidt i = 0; i < nbrentity(); ++i){
        if(managers().valide(i)) {
            for(numt j = 0; j != managers().info(i).nbrSetUnique(); ++j) {
                auto iter_unique = schema.emplace_back(iter,"xs:element");
                iter_unique->set_attribut("name",managers().info(i).name()  + "_Unique"+QString::number(j));
                iter_unique = schema.emplace_back(iter_unique,"xs:complexType");
                auto attUnique = managers().info(i).attributUnique(j);
                if(attUnique.size() == 1){
                    iter_unique = schema.emplace_back(iter_unique,"xs:element");
                    iter_unique->set_attribut("name",managers().get(i).names_attributs().at(attUnique.at(0)));
                }
                else {
                    iter_unique = schema.emplace_back(iter_unique,"xs:all");
                    for (auto iter_num = attUnique.cbegin(); iter_num != attUnique.cend(); ++iter_num) {
                        auto iter_att = schema.emplace_back(iter_unique,"xs:element");
                        iter_att->set_attribut("name",managers().get(i).names_attributs().at(*iter_num));
                    }
                }
            }
        }
    }
    return schema;
}

void Bdd::setRestriction(const entity &ent, flag restrict)
    {m_manager->get(entity.id_entity()).setRestriction(entity.id(), restrict);}

void Bdd::setBdd() {
    m_bdd.exec("PRAGMA foreign_keys = ON;");
    QSqlQuery req = QSqlQuery(m_bdd);
    req.setForwardOnly(true);
    m_manager->setRequete(req);
}

void Bdd::set_name(const QString &name) {
    file_interface::set_name(name);
    if(m_bdd.isOpen())
        m_bdd.close();
    m_bdd.set_databaseName(name);
}

enumt Bdd::strCategorieToEnum(const QString &str, flag categorie, QString &controle) const noexcept {
    if(categorie &RestrictionCode) {
        if(str == "Aucune")
            return bddMPS::Aucune;
        if(str == QString("modification"))
            return bddMPS::Modif;
        if(str == QString("suppression"))
            return bddMPS::Suppr;
    }
    if(categorie &LineStyle) {
        if(str == "SolidLine")
            return Qt::SolidLine;
        if(str == "DashLine")
            return Qt::DashLine;
        if(str == "DotLine")
            return Qt::DotLine;
        if(str == "DashDotLine")
            return Qt::DashDotLine;
        if(str == "DashDotDotLine")
            return Qt::DashDotDotLine;
    }
    if(categorie &BrushStyle) {
        if(str == "NoBrush")
            return Qt::NoBrush;
        if(str == "SolidPattern")
            return Qt::SolidPattern;
        if(str == "Dense1Pattern")
            return Qt::Dense1Pattern;
        if(str == "Dense2Pattern")
            return Qt::Dense2Pattern;
        if(str == "Dense3Pattern")
            return Qt::Dense3Pattern;
        if(str == "Dense4Pattern")
            return Qt::Dense4Pattern;
        if(str == "Dense5Pattern")
            return Qt::Dense5Pattern;
        if(str == "Dense6Pattern")
            return Qt::Dense6Pattern;
        if(str == "Dense7Pattern")
            return Qt::Dense7Pattern;
        if(str == "HorPattern")
            return Qt::HorPattern;
        if(str == "VerPattern")
            return Qt::VerPattern;
        if(str == "CrossPattern")
            return Qt::CrossPattern;
        if(str == "BDiagPattern")
            return Qt::BDiagPattern;
        if(str == "FDiagPattern")
            return Qt::FDiagPattern;
        if(str == "DiagCrossPattern")
            return Qt::DiagCrossPattern;
        if(str == "LinearGradientPattern")
            return Qt::LinearGradientPattern;
        if(str == "RadialGradientPattern")
            return Qt::RadialGradientPattern;
        if(str == "ConicalGradientPattern")
            return Qt::ConicalGradientPattern;
        if(str == "TexturePattern")
            return Qt::TexturePattern;
    }
    if(categorie &FontWeight) {
        if(str == "Thin")
            return QFont::Thin;
        if(str == "ExtraLight")
            return QFont::ExtraLight;
        if(str == "Light")
            return QFont::Light;
        if(str == "Normal")
            return QFont::Normal;
        if(str == "Medium")
            return QFont::Medium;
        if(str == "DemiBold")
            return QFont::DemiBold;
        if(str == "Bold")
            return QFont::Bold;
        if(str == "ExtraBold")
            return QFont::ExtraBold;
        if(str == "Black")
            return QFont::Black;

    }
    controle.append("-> Enum invalide de categorie ").append(QString::number(static_cast<uint>(categorie))).append(": ").append(str);
    return InvalideEnum;
}

Bdd::vector_id_name Bdd::table_entity_names(bool arbre) const {
    std::vector<std::pair<int,QString>> vec;
    vec.reserve(nbrentity());
    for(entidt i = 0; i < nbrentity(); ++i) {
        if(managers().valide(i)) {
            vec.emplace_back(i,managers().info(i).name());
            if(arbre &&!managers().get(i).infoarbre().name().isEmpty())
                vec.emplace_back(-i,managers().get(i).infoarbre().name());
        }
    }
    std::sort(vec.begin(),vec.end(),
              [](const std::pair<int,QString> &p1, const std::pair<int,QString> &p2)->bool {return p1.second < p2.second;});
    return vec;
}

bool Bdd::testAutorisationP(idt id, entidt id_entity, flag autoris)
    {return !(m_manager->get(id_entity).getRestriction(id) &autoris);}
