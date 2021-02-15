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
                controle.append("-> Parent unique introuvable :\n")
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
                    controle.append("-> Numéro dans arbre invalide :\n")
                            .append(att_iter->second);
            }
        }
        else
            controle.append("-> Attribut d'Arbre invalide : ").append(att_iter->first)
                    .append(".\nNoeud d'arbre de :\n").append(iter.parent()->name());
    }
    return arbre;
}

void Bdd::associatedXml(Entity & entity, xml_iterator iter, QString & controle) {
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
    else if (iter->name() != "Arbre"){
        auto ent_ass = makeEntity(iter->name());
        if(!ent_ass)
            controle.append("-> Donnée associée invalide : ").append(iter->name());
        else {
            std::list<decltype(iter->attributes().cbegin())> multi_atts;
            std::list<decltype(iter->attributes().cbegin())> after_atts;
            for (auto iter_att = iter->attributes().cbegin();
                 iter_att != iter->attributes().cend() && controle.isEmpty(); ++iter_att) {
                if(isMultipleAssociatedXml(*iter_att))
                    multi_atts.push_back(iter_att);
                else if(isAfterAssociatedXml(*iter_att))
                    after_atts.push_back(iter_att);
                else
                    hydrateAttributAssociatedXml(*ent_ass,*iter_att,entity,controle);
            }
            if(controle.isEmpty()) {
                if(multi_atts.empty()){
                    auto exist = existsUniqueEnsemble(*ent_ass);
                    if(exist == existeUni::Conflit)
                        controle.append("-> Erreur d'unicité :\n").append(ent_ass->affiche());
                    else
                        save(*ent_ass);
                    for (auto iter = after_atts.cbegin(); iter != after_atts.cend() && controle.isEmpty(); ++iter)
                        hydrateAttributAssociatedXml(*ent_ass,**iter,entity,controle);
                }
                else {
                    std::list<xml_list_atts> multi;
                    for (auto iter = multi_atts.cbegin(); iter != multi_atts.cend() && controle.isEmpty(); ++iter)
                        multi.push_back(listMultipleAssociatedXml(**iter,controle));
                    std::list<xml_list_atts::const_iterator> multi_iter;
                    for (auto iter = multi.cbegin(); iter != multi.cend() && controle.isEmpty(); ++iter) {
                        multi_iter.push_back(iter->cbegin());
                        hydrateAttributAssociatedXml(*ent_ass,*(iter->cbegin()),entity,controle);
                    }
                    if(!controle.isEmpty()) {
                        auto controle_while = true;
                        while(controle_while && controle.isEmpty()){
                            ent_ass->setId(0);
                            auto exist = existsUniqueEnsemble(*ent_ass);
                            if(exist == existeUni::Conflit)
                                controle.append("Erreur d'unicité :\n").append(ent_ass->affiche());
                            else
                                save(*ent_ass);
                            for (auto iter = after_atts.cbegin(); iter != after_atts.cend() && controle.isEmpty(); ++iter)
                                hydrateAttributAssociatedXml(*ent_ass,**iter,entity,controle);
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
                                    hydrateAttributAssociatedXml(*ent_ass,**iter_iter,entity,controle);
                                    if(controle_do) {
                                        ++iter_iter;
                                        ++iter_multi;
                                    }
                                } while(controle_do && iter_iter != multi_iter.end());
                                controle_while = iter_iter != multi_iter.end();
                            }
                        }
                    }
                }
            }
            if(!controle.isEmpty())
                controle.prepend(QString("-> Dans l'entité associée : \n").append(ent_ass->affiche()).append("\n"));
        }
    }
}

flag Bdd::codeFromQString(const QString & str, uint idCat, bool forEntity, QString &controle) const {
    flag code;
    auto list = str.split("|");
    if(forEntity)
        for (auto iter = list.cbegin(); iter != list.cend() && controle.isEmpty(); ++iter)
            code |= strIdToEnum(*iter,idCat,controle);
    else
        for (auto iter = list.cbegin(); iter != list.cend() && controle.isEmpty(); ++iter)
            code |= strCategorieToEnum(*iter,idCat,controle);
    return code;
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
                             xml_iterator iter, const QString &type, QString & controle) {
    if(type.isEmpty()) {
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
                    controle.append("-> Entité unique introuvable:\n").append(fkey_entity->affiche());
            }
            if(!controle.isEmpty())
                controle.prepend("-> Dans la clé étrangère : ").prepend(fkey_iter->second).append("\n");
        }
    }
    else if(type == "code") {
        auto code = codeFromQString(iter->text(),entity.idEntity(),true,controle);
        if(controle.isEmpty())
            entity.setData(pos, code.value());
    }
    else if(type == "couleur") {
        QColor color(iter->text());
        if(color.isValid())
            entity.setData(pos,color.name());
        else
            controle.append("-> Couleur invalide ").append(iter->name()).append(" = ").append(iter->text());
    }
    else if (type == "enum") {
        auto i = strIdToEnum(iter->text(),entity.idEntity(),controle);
        if(controle.isEmpty())
            entity.setData(pos,i);
    }
    else if (type == "police") {
        QFont police;
        for (auto iter_police = iter.cbeginChild(); iter_police && controle.isEmpty(); ++iter_police) {
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
            entity.setData(pos,police.toString());
        else
            controle.prepend(QString("-> Dans la police de : ").append(iter->name()).append("\n"));
    }
    else
        controle.append("-> Type de noeud xml inconnue : ").append(type);
}

void Bdd::hydrateAttributAssociatedXml(Entity &entity_ass, const std::pair<const QString,QString> &pair,
                                       const Entity &entity, QString &controle) {
    if(pair.first == "id_ent"){
        auto pos = positionXml(entity_ass,pair.second,controle);
        if(controle.isEmpty())
            entity_ass.setData(pos,entity.id());
    }
    else if(pair.first == "code") {
        auto code = codeFromQString(pair.second,entity_ass.idEntity(),true,controle);
        if(controle.isEmpty()) {
            auto pos = positionXml(entity_ass,"Code",controle);
            if(controle.isEmpty())
                entity_ass.setData(pos,code.value());
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

std::list<Bdd::xml_iterator> Bdd::hydrateEntityXml(entityMPS::Entity & entity, xml_iterator iter, QString &controle) {
    std::list<xml_iterator> assotiated;
    for (auto sub_iter = iter.cbeginChild(); sub_iter && controle.isEmpty(); ++sub_iter) {
        if(sub_iter->hasAttributes() && (sub_iter->attributes().size() > 1 || sub_iter->attributes().cbegin()->first != "type"))
                assotiated.push_back(sub_iter);
        else {
            auto pos = entity.position(sub_iter->name());
            if(pos == entity.nbrAtt())
                controle.append("-> Attribut inconnue dans l'entité : ").append(info(entity).name());
            else
                hydrateAttributXml(entity, pos, sub_iter,
                                   sub_iter->hasAttributes() ? sub_iter->attributes().cbegin()->second : QString(),
                                   controle);

        }
        if(!controle.isEmpty())
            controle.prepend(QString("-> Dans l'attribut : ").append(sub_iter->name())
                             .append(" de valeur : ").append(sub_iter->text()).append("\n"));
    }
    return assotiated;
}

Bdd::xml_list_atts Bdd::listMultipleAssociatedXml(const std::pair<const QString,QString> &pair, QString &controle) {
    controle.append("L'attribut est inconnu : ").append(pair.first);
    return xml_list_atts();
}

QString Bdd::importXml(const fichierMPS::XmlDoc & doc){
    auto iter = doc.cbegin().cbeginChild();
    QString controle;
    while(controle.isEmpty() && iter) {
        auto entity = makeEntity(iter->name());
        if(!entity)
            controle.append("-> Nom d'entité inconnu : ").append(iter->name());
        else {
            auto associated = hydrateEntityXml(*entity,iter,controle);
            if(controle.isEmpty()) {
                if(!entity->isValid())
                    controle.append("-> Entité invalide.");
                else {
                    auto exist = existsUniqueEnsemble(*entity);
                    if(exist == existeUni::Conflit)
                        controle.append("-> Erreur d'unicité.");
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
                if(controle.isEmpty())
                    for (auto iter_list = associated.cbegin(); iter_list != associated.cend() && controle.isEmpty(); ++iter_list)
                        associatedXml(*entity,*iter_list,controle);
            }
            if(!controle.isEmpty())
                controle.prepend(QString("-> Dans l'entité :\n").append(entity->affiche()).append("\n"));
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
    //const auto restToStr = RestrictionToStr();
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
//            auto restriction = managers().get(i).enableRestriction();
//            for (auto iter_restriction = restToStr.cbegin(); iter_restriction != restToStr.cend(); ++iter_restriction) {
//                if(restriction & iter_restriction->first) {
//                    auto iter_rest = schema.emplace_back(iter_entity,"xs:attribute");
//                    iter_rest->setAttribut("name",iter_restriction->second);
//                }
//            }
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

enumt Bdd::strCategorieToEnum(const QString & str, flag categorie, QString &controle) const noexcept {
    if(categorie & RestrictionCode) {
        if(str == "Aucune")
            return bddMPS::Aucune;
        if(str == QString("modification"))
            return bddMPS::Modif;
        if(str == QString("suppression"))
            return bddMPS::Suppr;
    }
    if(categorie & LineStyle) {
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
    if(categorie & BrushStyle) {
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
    if(categorie & FontWeight) {
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
