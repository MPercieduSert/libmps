#include "abstract_colonnes_model.h"

using namespace mps;
using namespace model_base;

abstract_colonnes_model::abstract_colonne::~abstract_colonne() = default;

abstract_colonnes_model::abstract_colonnes_model(bool unique_ligne, bool valide_ligne, QObject *parent)
    : abstract_model(parent), m_unique_ligne(unique_ligne), m_valide_ligne(valide_ligne) {
    if(m_valide_ligne || m_unique_ligne) {
        connect(this,&abstract_colonnes_model::rowsRemoved,this,&abstract_colonnes_model::resize_etats);
        connect(this,&abstract_colonnes_model::rowsInserted,this,
                [this](const QModelIndex &parent,int first,int last){
                    resize_etats();
                    update_etats(first,last,parent);});
        connect(this,&abstract_colonnes_model::dataChanged,this,
                [this](const QModelIndex &top_left, const QModelIndex &bottom_right) {
                    if(top_left.isValid() &&bottom_right.isValid())
                        update_etats(top_left.row(), bottom_right.row(), top_left.parent());});
        connect(this,&abstract_colonnes_model::modelReset,this,
                [this](){resize_etats();
                update_all_etats();});
    m_brush.resize(NbrEtat);
    m_brush[Sauver] = QBrush();
    m_brush[Valide] = QBrush(QColor(Qt::green));
    m_brush[Invalide] = QBrush(QColor(Qt::red));
    m_brush[Double] = QBrush(QColor(255,125,0));
    m_brush[DoubleSauver] = QBrush(QColor(155,88,0));
    }
}

QVariant abstract_colonnes_model::data(const QModelIndex &index, int role) const {
    if(checkIndex(index))
        return colonne(index).data(ligne(index),role);
    return QVariant();
}

Qt::ItemFlags abstract_colonnes_model::flags(const QModelIndex &index) const {
    if(checkIndex(index))
        return colonne(index).flags(ligne(index));
    return Qt::ItemIsEnabled;
}

QVariant abstract_colonnes_model::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole &&orientation == Qt::Horizontal)
        return m_colonnes[static_cast<szt>(section)]->header();
    return QVariant();
}

void abstract_colonnes_model::hydrate(int row, const QModelIndex &parent){
    m_data->hydrate(row_to_ligne(row,parent));
    emit dataChanged(index(row,0,parent),index(row,columnCount() - 1,parent));
}

bool abstract_colonnes_model::insert_colonne(int pos, const new_colonne_info &info, bool allParent){
    if(pos < 0 || pos > columnCount())
        pos = columnCount();
    auto iter = std::next(m_colonnes.cbegin(),pos);
    if(allParent) {
        beginResetModel();
            m_colonnes.insert(iter,m_data->make_colonne(info));
        endResetModel();
    }
    else {
        beginInsertColumns(QModelIndex(),pos,pos);
            m_colonnes.insert(iter,m_data->make_colonne(info));
        endInsertColumns();
    }
    return true;
}

bool abstract_colonnes_model::push_back_colonne(const new_colonne_info &info, bool allParent){
    if(allParent) {
        beginResetModel();
            m_colonnes.push_back(m_data->make_colonne(info));
        endResetModel();
    }
    else {
        beginInsertColumns(QModelIndex(),columnCount(),columnCount());
            m_colonnes.push_back(m_data->make_colonne(info));
        endInsertColumns();
    }
    return true;
}

void abstract_colonnes_model::save() {
    if(!m_valide_ligne &&!m_unique_ligne)
        for (szt ligne = 0; ligne != nbr_lignes(); ++ligne)
            m_data->save(ligne);
    else {
        for (szt ligne = 0; ligne != nbr_lignes(); ++ligne) {
            if(m_etats[ligne] == Valide){
                m_data->save(ligne);
                m_etats[ligne] = Sauver;
                etat_ligne_changed(ligne);
            }
        }
    }
}

bool abstract_colonnes_model::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(checkIndex(index) && colonne(index).set_data(ligne(index),value,role)) {
        emit dataChanged(index,index);
        return true;
    }
    return false;
}

void abstract_colonnes_model::set_tableau(std::unique_ptr<abstract_tableau> &&tableau) {
    m_data = std::move(tableau);
    reset_row();
}

QVariant abstract_colonnes_model::statistique(int pos, enumt fonction, bool lignes_visibles) const {
    if(pos < 0 || pos >= columnCount())
        return QVariant();
    auto col = static_cast<szt>(pos);
    auto id_lignes = stat_on_lignes(lignes_visibles);
    switch (fonction) {
    case Max: {
        auto iter = std::max_element(id_lignes.cbegin(),id_lignes.cend(),
                                     [this,col](szt a, szt b) {return m_colonnes[col]->compare(a,b);});
        return m_colonnes[col]->data(*iter,Qt::DisplayRole);}
    case Min: {
        auto iter = std::min_element(id_lignes.cbegin(),id_lignes.cend(),
                                     [this,col](szt a, szt b) {return m_colonnes[col]->compare(a,b);});
        return m_colonnes[col]->data(*iter,Qt::DisplayRole);}
    default:
        return QVariant();
    }
}

void abstract_colonnes_model::update_all_etats() {
    if(m_valide_ligne && !m_unique_ligne){
        for(szt ligne = 0; ligne != nbr_lignes(); ++ligne)
            update_valide_ligne(ligne);
        emit etats_changed();
    }
    else if(m_unique_ligne) {
        for(szt ligne = 0; ligne != nbr_lignes(); ++ligne)
            update_unique_ligne(ligne, false);
        emit etats_changed();
    }
}

void abstract_colonnes_model::update_etats(int first, int last, const QModelIndex &parent) {
    if((m_valide_ligne || m_unique_ligne) &&first >= 0 &&first <= last &&last < rowCount(parent)) {
        if(!m_unique_ligne)
            for(auto row = first; row != last + 1; ++row)
                update_valide_ligne(ligne(index(row,0,parent)));
        else
            for(auto row = first; row != last + 1; ++row)
                update_unique_ligne(ligne(index(row,0,parent)));
        emit etat_rows_changed(parent,first,last);
    }
}

void abstract_colonnes_model::update_valide_ligne(szt ligne){
    if(!m_data->valide(ligne))
        m_etats[ligne] = Invalide;
    else if (m_data->new_or_modif(ligne))
        m_etats[ligne] = Valide;
    else
        m_etats[ligne] = Sauver;

}

void abstract_colonnes_model::update_unique_ligne(szt ligne, bool update){
    if(!m_data->valide(ligne))
        m_etats[ligne] = Invalide;
    else if (m_data->new_or_modif(ligne)) {
        auto iter = m_doubles.find(ligne);
        auto find = true;
        if(iter != m_doubles.end()) {
            if(!m_data->egal(ligne,iter->second.front())){
                if(iter->second.size() == 1) {
                    auto ligneD = iter->second.front();
                    m_doubles.erase(ligne);
                    m_doubles.erase(ligneD);
                    if(m_data->new_or_modif(ligneD)){
                        m_etats[ligneD] = Valide;
                        if(update)
                            emit etat_ligne_changed(ligneD);
                    }
                    else{
                        m_etats[ligneD] = Sauver;
                        if(update)
                            emit etat_ligne_changed(ligneD);
                    }
                }
                else {
                    for (auto list_it = iter->second.cbegin(); list_it != iter->second.cend(); ++list_it) {
                        auto &list_double = m_doubles[*list_it];
                        list_double.erase(std::find(list_double.cbegin(),list_double.cend(),ligne));
                    }
                    m_doubles.erase(ligne);
                }
            }
            else
                find = false;
        }
        if(find) {
            szt ligne_2 = 0;
            while(ligne_2 != nbr_lignes() && (ligne_2 == ligne || !m_data->egal(ligne_2,ligne)))
                ++ligne_2;
            if(ligne_2 == nbr_lignes())
                m_etats[ligne] = Valide;
            else {
                m_etats[ligne] = Double;
                if(m_doubles[ligne_2].empty()){
                    m_doubles[ligne].push_back(ligne_2);
                    m_doubles[ligne_2].push_back(ligne);
                    if(m_data->new_or_modif(ligne_2)) {
                        m_etats[ligne_2] = Double;
                        if(update)
                            emit etat_ligne_changed(ligne_2);
                    }
                    else {
                        m_etats[ligne_2] = DoubleSauver;
                        if(update)
                            emit etat_ligne_changed(ligne_2);
                    }
                }
                else {
                    m_doubles[ligne] = m_doubles[ligne_2];
                    m_doubles[ligne].push_back(ligne_2);
                    for (auto iter = m_doubles[ligne].cbegin(); iter != m_doubles[ligne].cend(); ++iter)
                        m_doubles[*iter].push_back(ligne);
                }
            }
        }
    }
    else
        m_etats[ligne] = Sauver;
}

///////////////////////////////////////// Tableau /////////////////////////
void abstract_colonnes_model::abstract_tableau::erase(szt first, szt last) {
    for (auto ligne = first; ligne != last; ++ligne)
        erase(ligne);
}
