#include "Bdd.h"

using namespace bddMPS;

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

QString Bdd::hydrateAttributXml(entityMPS::Entity & entity, post pos, fichierMPS::XmlDoc::const_brother_iterator iter) {
    auto fkey_iter = info(entity).foreignKeyName().find(pos);
    if(fkey_iter == info(entity).foreignKeyName().end()) {
        entity.setData(pos, iter->text());
        return QString();
    }
    else {
        auto fkey_entity = makeEntity(fkey_iter->second);
        auto controle = hydrateEntityXml(*fkey_entity, iter);
        if(controle.isEmpty()) {
            if(getUnique(*fkey_entity))
                entity.setData(pos, fkey_entity->id());
            else
                controle = QString("Entité unique introuvable:\n").append(fkey_entity->affiche());
        }
        if(!controle.isEmpty())
            controle.prepend("->").prepend(fkey_iter->second);
        return controle;
    }
}

QString Bdd::hydrateEntityXml(entityMPS::Entity & entity, fichierMPS::XmlDoc::const_brother_iterator iter) {
    QString controle;
    for (auto att_iter = iter.cbeginChild(); controle.isEmpty() && att_iter; ++att_iter) {
        auto pos = entity.position(att_iter->name());
        if(pos == entity.nbrAtt())
            return QString(att_iter->name()).append("\nAttribut invalide de Valeur : ").append(att_iter->text())
                    .append(".\nPour l'entité :\n").append(entity.affiche());
        else
            controle = hydrateAttributXml(entity, pos, att_iter);
    }
    return controle;
}

QString Bdd::importXml(const fichierMPS::XmlDoc & doc){
    auto iter = doc.cbegin().cbeginChild();
    QString controle;
    const auto restriction = RestrictionToStr();
    while(controle.isEmpty() && iter) {
        auto entity = makeEntity(iter->name());
        if(!entity)
            controle.append(iter->name());
        else {
            auto typeManager = m_manager->get(entity->idEntity()).typeManager();
            if(typeManager & bddMPS::ArbreTypeManager) {
                //auto iterHA = iter.cbeginChild();
                //entityBaseMPS::Arbre arb;
            }
            else {
                controle = hydrateEntityXml(*entity, iter);
                if(controle.isEmpty()) {
                    if(entity->isValid()) {
                        existsUnique(*entity);
                        save(*entity);
                        if(typeManager & bddMPS::ModifControleTypeManager) {
                            flag restrict = bddMPS::Aucune;
                            for (auto iter_restrict = restriction.cbegin(); iter_restrict != restriction.cend(); ++iter_restrict) {
                                auto iter_att = iter->attributes().find(iter_restrict->second);
                                if(iter_att != iter->attributes().end())
                                    restrict |=iter_restrict->first;
                            }
                            setRestriction(*entity,restrict);
                        }
                    }
                    else
                        controle = QString("Entité invalide :\n").append(entity->affiche());
                }
                if(!controle.isEmpty())
                    controle.prepend("->").prepend(iter->name());
            }
        }
        ++iter;
    }
    return controle;
}

const managerMPS::InfoBdd & Bdd::info(const Entity & entity) const
    {return m_manager->get(entity.idEntity()).info();}

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

void Bdd::save(Entity & entity, const Entity & parent, int num)
    {m_manager->get(entity.idEntity()).save(entity,parent,num);}

void Bdd::save(const Entity & entity, const Entity & parent, int num)
    {m_manager->get(entity.idEntity()).save(entity,parent,num);}

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
