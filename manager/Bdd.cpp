#include "Bdd.h"

using namespace bddMPS;

entityBaseMPS::Arbre Bdd::arbreXml(xml_iterator iter, QString & controle){
    entityBaseMPS::Arbre arbre;
    for (auto att_iter = iter->attributes().cbegin(); att_iter != iter->attributes().cend() && controle.isEmpty(); ++att_iter) {
        if(att_iter->first == "ref_parent") {
            auto parent = makeEntity(iter.parent()->name());
            parent->setData(parent->position("Ref"),att_iter->second);
            if (existsUnique(*parent))
                arbre.setParent(parent->id());
            else
                controle.append(iter->name()).append("\nParent unique introuvable :\n")
                        .append(parent->affiche());
        }
        else if (att_iter->first == "num") {
            if(att_iter->second == "premier")
                arbre.setNum(0);
            else if(att_iter->second == "dernier"){
                arbre.setNum(-1);
            }
            else {
                bool ok;
                auto num = att_iter->second.toInt(&ok);
                if (ok)
                    arbre.setNum(num);
                else
                    controle.append(iter->name()).append("\nNum dans arbre invalide :\n")
                            .append(att_iter->second);
            }
        }
        else
            controle.append("Attribut d'Arbre invalide : ").append(att_iter->first)
                    .append(".\nNoeud d'arbre de :\n").append(iter.parent()->name());
    }
    return arbre;
}

void Bdd::associatedXml(Entity & entity, xml_iterator iter, QString & controle) {
    if(iter->name() == "Restriction") {
        auto iter_code = iter->attributes().find("code");
        setRestriction(entity,restrictionFromQString(iter_code->second,controle));
    }
    else if (iter->name() != "Arbre"){
        auto ent_ass = makeEntity(iter->name());
        if(!ent_ass)
            controle.append("Donnée associée invalide : ").append(iter->name())
                    .append("\n Pour l'entitée : \n").append(entity.affiche());
        else
            for (auto iter_att = iter->attributes().cbegin();
                 iter_att != iter->attributes().cend() && controle.isEmpty(); ++iter_att)
                hydrateAttributAssociatedXml(*ent_ass,*iter_att,entity,controle);
    }
}

bool Bdd::copy(const QString & name) {
    Bdd bdd(m_bdd.driverName(), name,{});
    if(bdd.exists() && bdd.isValid()) {
        QFile file(name);
        return file.copy(m_fileName);
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

bool Bdd::exists(const Entity & entity)
    {return m_manager->get(entity.idEntity()).exists(entity);}

bool Bdd::existsUnique(Entity & entity)
    {return m_manager->get(entity.idEntity()).existsUnique(entity) != Aucun;}

bool Bdd::existsUnique(const Entity & entity)
    {return m_manager->get(entity.idEntity()).existsUnique(entity) != Aucun;}

existeUni Bdd::existsUniqueEnsemble(Entity & entity)
    {return m_manager->get(entity.idEntity()).existsUnique(entity);}

existeUni Bdd::existsUniqueEnsemble(const Entity & entity)
    {return m_manager->get(entity.idEntity()).existsUnique(entity);}

std::pair<existeUni,idt> Bdd::existsUniqueId(const Entity & entity)
    {return m_manager->get(entity.idEntity()).existsUniqueId(entity);}

bool Bdd::get(Entity & entity)
    {return m_manager->get(entity.idEntity()).get(entity);}

bool Bdd::testAutorisationP(idt id, entidt idEntity, flag autoris)
    {return !(m_manager->get(idEntity).getRestriction(id) & autoris);}

flag Bdd::getRestriction(const Entity & entity)
    {return m_manager->get(entity.idEntity()).getRestriction(entity.id());}

bool Bdd::getUnique(Entity & entity)
    {return m_manager->get(entity.idEntity()).getUnique(entity);}

void Bdd::hydrateAttributXml(entityMPS::Entity & entity, post pos,
                             xml_iterator iter, QString & controle) {
    auto fkey_iter = info(entity).foreignKeyName().find(pos);
    if(fkey_iter == info(entity).foreignKeyName().end())
        entity.setData(pos, iter->text());
    else {
        auto fkey_entity = makeEntity(fkey_iter->second);
        hydrateEntityXml(*fkey_entity, iter, controle);
        if(controle.isEmpty()) {
            if(existsUnique(*fkey_entity))
                entity.setData(pos, fkey_entity->id());
            else
                controle.append("Entité unique introuvable:\n").append(fkey_entity->affiche());
        }
        if(!controle.isEmpty())
            controle.prepend("->").prepend(fkey_iter->second);
    }
}

void Bdd::hydrateAttributAssociatedXml(Entity &entity_ass, const std::pair<const QString,QString> &pair,
                                       const Entity &entity, QString &controle) {
    if(pair.first == "id_ent"){
        auto pos = positionXml(entity_ass,pair.second,controle);
        if(controle.isEmpty())
            entity_ass.setData(pos,entity.id());
    }
    else
        controle.append("L'attribut est inconnue : ").append(pair.first);
    if(!controle.isEmpty())
        controle.append("\nDans l'entité associée : \n").append(entity_ass.affiche())
                .append("\nDans l'entité : \n").append(entity.affiche());
}

std::list<Bdd::xml_iterator> Bdd::hydrateEntityXml(entityMPS::Entity & entity, xml_iterator iter, QString &controle) {
    std::list<xml_iterator> assotiated;
    for (auto sub_iter = iter.cbeginChild(); controle.isEmpty() && sub_iter; ++sub_iter) {
        if(sub_iter->hasAttributes())
                assotiated.push_back(sub_iter);
        else {
            auto pos = entity.position(sub_iter->name());
            if(pos == entity.nbrAtt())
                controle.append(sub_iter->name()).append("\nAttribut invalide de valeur : ").append(sub_iter->text())
                        .append(".\nPour l'entité :\n").append(entity.affiche());
            else
                hydrateAttributXml(entity, pos, sub_iter,controle);
        }
    }
    return assotiated;
}

flag Bdd::restrictionFromQString(const QString & str,QString & controle) {
    flag code;
    auto list = str.split("|");
    const auto restriction = RestrictionToStr();
    for (auto iter = list.cbegin(); iter != list.cend(); ++iter) {
        auto iter_restriction = restriction.cbegin();
        while( iter_restriction->second != *iter)
            ++iter_restriction;
        if(iter_restriction != restriction.cend())
            code |= iter_restriction->first;
        else
            controle.append("Restriction invalide : ").append(*iter);
    }
    return code;
}

QString Bdd::importXml(const fichierMPS::XmlDoc & doc){
    auto iter = doc.cbegin().cbeginChild();
    QString controle;
    while(controle.isEmpty() && iter) {
        auto entity = makeEntity(iter->name());
        if(!entity)
            controle.append(iter->name());
        else {
            auto associated = hydrateEntityXml(*entity,iter,controle);
            if(!controle.isEmpty())
                controle.prepend("->").prepend(iter->name());
            else {
                if(!entity->isValid())
                    controle.append("Entité invalide :\n").append(entity->affiche());
                else {
                    auto exist = existsUniqueEnsemble(*entity);
                    if(exist == existeUni::Conflit)
                        controle.append("Erreur d'unicité :\n").append(entity->affiche());
                    else if (exist != Aucun || !(m_manager->get(entity->idEntity()).typeManager() & bddMPS::ArbreTypeManager))
                        save(*entity);
                    else {
                        auto iter_arbre = std::find_if(associated.cbegin(),associated.cend(),
                                                    [](const xml_iterator & i){return i->name() == "Arbre";});
                        entityBaseMPS::Arbre arb;
                        if(iter_arbre != associated.cend()){
                            arb = arbreXml(*iter_arbre, controle);
                            associated.erase(iter_arbre);
                        }
                        if(controle.isEmpty())
                           insert(*entity,arb.parent(),arb.num());
                    }
                }
                if(!controle.isEmpty())
                    controle.prepend("->").prepend(iter->name());
                else
                    for (auto iter_list = associated.cbegin(); iter_list != associated.cend() && controle.isEmpty(); ++iter_list)
                        associatedXml(*entity,*iter_list,controle);
            }
        }
        ++iter;
    }
    return controle;
}

const managerMPS::InfoBdd & Bdd::info(const Entity & entity) const
    {return m_manager->get(entity.idEntity()).info();}

void Bdd::insert(Entity & entity, idt idParent, int num)
    {m_manager->get(entity.idEntity()).insert(entity,idParent,num);}

void Bdd::insert(const Entity & entity, idt idParent, int num)
    {m_manager->get(entity.idEntity()).insert(entity,idParent,num);}

void Bdd::miseAJourBdd() {
    for (idt type = 0; type != m_version.size(); ++type) {
        if(!m_manager->existsVersion(type))
            listeMiseAJourBdd(bddVersion::Initiale,type);
        auto versionBdd = m_manager->numVersion(type);
        if(m_version[type] != versionBdd) {
            if(m_version[type] > versionBdd) {
                while(m_version[type] != versionBdd) {
                    listeMiseAJourBdd(versionBdd,type);
                    versionBdd = m_manager->numVersion(type);
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

std::map<flag::flag_type, QString> Bdd::RestrictionToStr() {
    std::map<flag::flag_type,QString> map;
    map[Modif] = "modification";
    map[Suppr] = "suppression";
    return map;
}

bool Bdd::sameInBdd(const Entity & entity)
    {return m_manager->get(entity.idEntity()).sameInBdd(entity);}

void Bdd::save(Entity & entity)
    {m_manager->get(entity.idEntity()).save(entity);}

void Bdd::save(const Entity & entity)
    {m_manager->get(entity.idEntity()).save(entity);}

void Bdd::save(Entity & entity, flag restrict)
    {m_manager->get(entity.idEntity()).save(entity, restrict);}

void Bdd::save(VectorPtr<Entity> & vector) {
    for (auto iter = vector.begin(); iter != vector.end(); ++iter)
        save(*iter);
}

void Bdd::save(const VectorPtr<Entity> & vector) {
    for (auto iter = vector.cbegin(); iter != vector.cend(); ++iter)
        save(*iter);
}

void Bdd::saveUnique(Entity & entity)
    {m_manager->get(entity.idEntity()).saveUnique(entity);}

void Bdd::saveUnique(const Entity & entity)
    {m_manager->get(entity.idEntity()).saveUnique(entity);}

fichierMPS::XmlDoc Bdd::schemaXmlForImport() const{
    using namespace fichierMPS;
    const auto restToStr = RestrictionToStr();
    XmlDoc schema;
    auto iter = schema.begin();
    iter->setName("xs:schema");
    iter->setAttribut("xmlns:xs","http://www.w3.org/2001/XMLSchema");
    iter->setAttribut("elementFormDefault","qualified");
    iter = schema.emplace_back(iter,"xs:element");
    iter->setAttribut("name","entities");
    iter = schema.emplace_back(iter,"xs:complexType");
    iter = schema.emplace_back(iter,"xs:choice");
    iter->setAttribut("maxOccurs","unbounded");
    for (entidt i = 0; i <nbrEntity(); ++i) {
        if(managers().valide(i)) {
            auto iter_entity = schema.emplace_back(iter,"xs:element");
            iter_entity->setAttribut("ref",managers().info(i).name());
        }
    }
    iter.toRoot();
    for (entidt i = 0; i < nbrEntity(); ++i) {
        if(managers().valide(i)) {
            auto iter_entity = schema.emplace_back(iter,"xs:element");
            iter_entity->setAttribut("name",managers().info(i).name());
            iter_entity = schema.emplace_back(iter_entity,"xs:complexType");
            iter_entity = schema.emplace_back(iter_entity,"xs:all");
            auto namesAtt = managers().get(i).namesAttributs();
            for (szt j = 1; j != namesAtt.size(); ++j) {
                auto iter_att = schema.emplace_back(iter_entity,"xs:element");
                iter_att->setAttribut("name",namesAtt.at(j));
                auto iter_key = managers().info(i).foreignKeyName().find(j);
                if(iter_key != managers().info(i).foreignKeyName().cend()){
                    if(iter_key->second.isEmpty()) {
                        iter_att = schema.emplace_back(iter_att,"xs:complexType");
                        iter_att = schema.emplace_back(iter_att,"xs:element");
                        iter_att->setAttribut("ref","one_Unique");
                    }
                    else {
                        auto k = managers().find(iter_key->second);
                        auto str = iter_key->second;
                        if(managers().info(k).nbrSetUnique() != 0) {
                            iter_att = schema.emplace_back(iter_att,"xs:complexType");
                            if(managers().info(k).nbrSetUnique() == 1) {
                                iter_att = schema.emplace_back(iter_att,"xs:element");
                                iter_att->setAttribut("ref",iter_key->second + "_Unique");
                            }
                            else {
                                iter_att = schema.emplace_back(iter_att,"xs:choice");
                                for(numt l = 0; l != managers().info(k).nbrSetUnique(); ++l) {
                                    auto iter_unique = schema.emplace_back(iter_att,"xs:element");
                                    iter_unique->setAttribut("ref",iter_key->second + "_Unique"+QString::number(l));
                                }
                            }
                        }
                    }
                }
            }
            iter_entity.toParent();
            auto restriction = managers().get(i).enableRestriction();
            for (auto iter_restriction = restToStr.cbegin(); iter_restriction != restToStr.cend(); ++iter_restriction) {
                if(restriction & iter_restriction->first) {
                    auto iter_rest = schema.emplace_back(iter_entity,"xs:attribute");
                    iter_rest->setAttribut("name",iter_restriction->second);
                }
            }
        }

    }
    for (entidt i = 0; i < nbrEntity(); ++i){
        if(managers().valide(i)) {
            for(numt j = 0; j != managers().info(i).nbrSetUnique(); ++j) {
                auto iter_unique = schema.emplace_back(iter,"xs:element");
                iter_unique->setAttribut("name",managers().info(i).name()  + "_Unique"+QString::number(j));
                iter_unique = schema.emplace_back(iter_unique,"xs:complexType");
                auto attUnique = managers().info(i).attributUnique(j);
                if(attUnique.size() == 1){
                    iter_unique = schema.emplace_back(iter_unique,"xs:element");
                    iter_unique->setAttribut("name",managers().get(i).namesAttributs().at(attUnique.at(0)));
                }
                else {
                    iter_unique = schema.emplace_back(iter_unique,"xs:all");
                    for (auto iter_num = attUnique.cbegin(); iter_num != attUnique.cend(); ++iter_num) {
                        auto iter_att = schema.emplace_back(iter_unique,"xs:element");
                        iter_att->setAttribut("name",managers().get(i).namesAttributs().at(*iter_num));
                    }
                }
            }
        }
    }
    return schema;
}

void Bdd::setRestriction(const Entity & entity, flag restrict)
    {m_manager->get(entity.idEntity()).setRestriction(entity.id(), restrict);}

void Bdd::setBdd() {
    m_bdd.exec("PRAGMA foreign_keys = ON;");
    QSqlQuery req = QSqlQuery(m_bdd);
    req.setForwardOnly(true);
    m_manager->setRequete(req);
}

void Bdd::setFileName(const QString & fileName) {
    FileInterface::setFileName(fileName);
    if(m_bdd.isOpen())
        m_bdd.close();
    m_bdd.setDatabaseName(fileName);
}

void Bdd::addRestriction(const Entity & entity, flag restrict)
    {m_manager->get(entity.idEntity()).addRestriction(entity.id(), restrict);}

flag Bdd::strToRestriction(const QString &str) noexcept {
    if(str == QString("modification"))
        return bddMPS::Modif;
    if(str == QString("suppression"))
        return bddMPS::Suppr;
    return bddMPS::Aucune;
}
