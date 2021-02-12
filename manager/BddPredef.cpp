#include "BddPredef.h"

using namespace bddMPS;
using namespace entityMPS;

void BddPredef::associatedXml(Entity & entity, xml_iterator iter, QString & controle) {
    if(iter->name() == "Permission")
        permissionXml(entity,iter,controle);
    if(iter->name() == "MotCle")
        motCleXml(entity,iter,controle);
    else
        Bdd::associatedXml(entity,iter,controle);
}

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

<<<<<<< HEAD
void BddPredef::hydrateAttributXml(entityMPS::Entity & entity, post pos, xml_iterator iter, QString &controle){
    if(iter->name() == "Cible") {
        auto i = m_manager->find(iter->text());
        if(i == nbrEntity())
            controle.append("Cible invalide : ").append(iter->text());
        else
            entity.setData(pos, cible(i));
    }
    else if(iter->name() == "Code") {
        auto list = iter->text().split(",");
        flag fl;
        for(auto iter_list = list.cbegin(); controle.isEmpty() && iter_list != list.cend(); ++iter_list) {
            auto cd = code(entity.idEntity(),*iter_list);
            if (cd != code::Invalide)
                fl |= cd;
            else
                controle.append("Code Invalide : ").append(*iter_list);
        }
        if(controle.isEmpty())
            entity.setData(pos, fl.value());
    }
    else if(iter->name() == "Card" || iter->name() == "Exact" || iter->name() == "TpVal") {
        auto i = strToEnum(entity.idEntity(),iter->text());
        if(i == InvalideEnum)
            controle.append(iter->name()).append("->Enum invalide : ").append(iter->text());
        else
            entity.setData(pos, i);
    }
    else
        Bdd::hydrateAttributXml(entity, pos,iter,controle);
}

void BddPredef::hydrateAttributAssociatedXml(Entity &entity_ass, const std::pair<const QString,QString> &pair,
                                             const Entity &entity, QString &controle) {
    if(pair.first == "code") {
        auto code_list = pair.second.split("|");
        flag code_flag;
        for (auto code_iter = code_list.cbegin();
             code_iter != code_list.cend() && controle.isEmpty(); ++code_iter) {
            auto code_value = code(entity_ass.idEntity(),*code_iter);
            if (code_value == code::Invalide)
                controle.append("Valeur de l'attribut code invalide : ").append(*code_iter);
            else
                code_flag |= code_value;
        }
        if(controle.isEmpty()) {
            auto pos = positionXml(entity_ass,"Code",controle);
            if(controle.isEmpty())
                entity_ass.setData(pos,code_flag.value());
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

void BddPredef::motCleXml(Entity & entity, xml_iterator iter, QString & controle){
    auto att = attributXml(iter,"ref_mot_cle",controle);
    if(controle.isEmpty()){
        entityMPS::MotCle mc;
        mc.setRef(att);
        if(!existsUnique(mc))
            controle.append("La référence de mot clé est inconnu : ").append(att);
        else {
            entityMPS::MotCleCible mcCible;
            mcCible.setIdMotCle(mc.id());
            mcCible.setCible(cible(entity.idEntity()));
            mcCible.setIdCible(entity.id());
            existsUnique(mcCible);
            save(mcCible);
        }
    }
    if(!controle.isEmpty())
        controle.append("\nMot clé associé à l'entité :\n").append(entity.affiche());
=======
bool BddPredef::isMultipleAssociatedXml(const QString & att) const {
    if(att == "cible")
        return true;
    return Bdd::isMultipleAssociatedXml(att);
>>>>>>> Début multipleAssociatedXml
}

void BddPredef::permissionXml(Entity &entity, xml_iterator iter, QString & controle) {
    // In -> entité de permission
    auto in_att = attributXml(iter,"in",controle);
    if(controle.isEmpty()) {
        auto perm = makeEntity(in_att);
        if(!perm)
            controle.append("L'entité ").append(in_att)
                    .append(" de l'atribut 'in' dans le noeud Permission est inconnue.");
        else {
//            // Id1 -> identifiant de l'entité
//            auto att = attributXml(iter,"id_ent",controle);
//            if(controle.isEmpty()) {
//                auto pos = positionXml(*perm,att,controle);
//                if(controle.isEmpty()) {
//                    perm->setData(pos,entity.id());
                    // code -> permission
//                    att = attributXml(iter,"code",controle);
//                    if(controle.isEmpty()) {
//                        auto code_list = att.split("|");
//                        flag code_flag;
//                        for (auto code_iter = code_list.cbegin();
//                             code_iter != code_list.cend() && controle.isEmpty(); ++code_iter) {
//                            auto code_value = code(perm->idEntity(),*code_iter);
//                            if (code_value == code::Invalide)
//                                controle.append("Valeur de l'attribut code invalide : ").append(*code_iter);
//                            else
//                                code_flag |= code_value;
//                        }
//                        if(controle.isEmpty()) {
//                            pos = positionXml(*perm,"Code",controle);
//                            if(controle.isEmpty()) {
//                                perm->setData(pos,code_flag.value());
                                // cible -> Cible
                                att = attributXml(iter,"cible",controle);
                                if(controle.isEmpty()) {
                                    pos = positionXml(*perm,"Cible",controle);
                                    if(controle.isEmpty()) {
                                        auto cible_list = att.split("|");
                                        for(auto cible_iter = cible_list.cbegin();
                                            cible_iter != cible_list.cend() && controle.isEmpty(); ++cible_iter) {
                                            auto i = m_manager->find(*cible_iter);
                                            if(i == nbrEntity())
                                                controle.append("La valeur de l'attribut cible est inconnue : ")
                                                        .append(*cible_iter);
                                            else
                                                entity.setData(pos, cible(i));
                                            if(controle.isEmpty()) {
                                                entity.setId(0);
                                                existsUnique(*perm);
                                                save(*perm);
                                                auto iter_restrict = iter->attributes().find("restriction");
                                                if(iter_restrict != iter->attributes().cend())
                                                    setRestriction(*perm,restrictionFromQString(iter_restrict->second,controle));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if(!controle.isEmpty())
        controle.append("\nPermission associé à l'entité :\n").append(entity.affiche());
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

int BddPredef::strToEnum(idt idEntity, const QString & str) const {
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
    default:
        return InvalideEnum;
    }
    return InvalideEnum;
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
