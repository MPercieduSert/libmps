#include "table_model.h"

using namespace mps;
using namespace model_base;

///////////////////////////////////////// table_model //////////////////////////
table_model::table_model(bool unique_ligne, bool valide_ligne, QObject *parent)
    : abstract_colonnes_model (unique_ligne,valide_ligne,parent){
    connect(this,&table_model::etat_ligne_changed,this,[this](szt ligne){
        auto row = ligne_to_row(ligne);
        if(row != -1)
            emit headerDataChanged(Qt::Vertical,row,row);});
    connect(this,&table_model::etat_rows_changed,this,[this](const QModelIndex &parent, int first, int last){
        if(!parent.isValid())
            emit headerDataChanged(Qt::Vertical,first,last);});
    connect(this,&table_model::etats_changed,this,[this](){emit headerDataChanged(Qt::Vertical,0,rowCount());});
}

void table_model::coller(const QModelIndexList &selection) {
    selected_range range(selection);
    if(!range.is_valid())
        QMessageBox::critical(nullptr,tr("Erreur de séléction"),
                              tr("Ce type de séléction multiple n'est pas valide pour éffectuer un collage"));
    else {
        auto str = QApplication::clipboard()->text();
        auto rows = str.splitRef('\n');
        if(!rows.isEmpty() &&rows.last().isEmpty())
            {rows.removeLast();}
        auto num_rows = rows.count();
        auto num_columns = rows.first().count('\t') + 1;
        dialog_selection n = dialog_selection::No_Dialog;
        if(range.row_count() *range.column_count() != 1
            &&(range.row_count() < num_rows || range.column_count() < num_columns))
            n = dialog_coller();

        switch (n) {
        case dialog_selection::Insert_In:
            if(num_rows > range.row_count())
                num_rows = range.row_count();
            if(num_columns > range.column_count())
                num_columns = range.column_count();
        [[clang::fallthrough]];
        case dialog_selection::No_Dialog:
        case dialog_selection::Insert_Out:
            if(range.is_rect()) {
                for (int i = 0; i < num_rows; ++i) {
                    auto columns = rows.at(i).split('\t');
                    for (auto j = 0; j < num_columns; ++j) {
                        auto row = range.top() + i;
                        auto column = range.left() + j;
                        if (row < rowCount() &&column < columnCount())
                            setData(index(row, column), columns.at(j).toString());
                    }
                }
            }
            else {
                for (int i = 0; i < num_rows; ++i) {
                    auto columns = rows.at(i).split('\t');
                    for (int j = 0; j < num_columns; ++j) {
                        auto row = i < range.row_count() ?
                                    range.row(i) : range.rows().back() + i - range.row_count() + 1;
                        auto column =  j < range.column_count() ?
                                    range.column(j) : range.columns().back() + j - range.column_count() + 1;
                        if (row < rowCount() &&column < columnCount())
                            setData(index(row, column), columns.at(j).toString());
                    }
                }
            }
            break;
        case Annuler:
            break;
        }
    }
}

void table_model::copier(const QModelIndexList &selection) {
    selected_range range(selection);
    if(!range.is_valid())
        QMessageBox::critical(nullptr,tr("Erreur de séléction"),
                              tr("Ce type de séléction multiple n'est pas valide pour éffectuer un collage"));
    else {
        QString str;
        if(range.is_rect()) {
            for (int i = 0; i < range.row_count(); ++i) {
                if (i > 0)
                    str.append("\n");
                for (int j = 0; j < range.column_count(); ++j) {
                    if (j > 0)
                        str.append("\t");
                    str.append(data(index(range.top() + i, range.left() + j)).toString());
                }
            }
        }
        else {
            for (int i = 0; i < range.row_count(); ++i) {
                if (i > 0)
                    str += "\n";
                for (int j = 0; j < range.column_count(); ++j)  {
                    if (j > 0)
                        str += "\t";
                    str += data(index(range.row(i), range.column(j))).toString();
                }
            }
        }
        QApplication::clipboard()->setText(str);
    }
}

void table_model::couper(const QModelIndexList &selection) {
    copier(selection);
    effacer(selection);
}

table_model::dialog_selection table_model::dialog_coller() const {
    QMessageBox dial;
    dial.setText(tr("Erreur collage"));
    dial.setInformativeText(tr("La taille de la selection est plus petite que la taille du contenu du presse papier."));
    dial.addButton(QMessageBox::Cancel);
    QPushButton *collerInBouton = dial.addButton(tr("Coller dans la selection"),QMessageBox::YesRole);
    QPushButton *collerOutBouton = dial.addButton(tr("Coller tout"),QMessageBox::NoRole);
    dial.exec();

    if(dial.clickedButton() == collerInBouton)
        return dialog_selection::Insert_In;
    if(dial.clickedButton() == collerOutBouton)
        return dialog_selection::Insert_Out;
    return dialog_selection::Annuler;
}

void table_model::effacer(const QModelIndexList &selection) {
    for(QModelIndexList::const_iterator i = selection.cbegin(); i != selection.cend(); ++i)
        setData(*i,QVariant());
}

void table_model::find(Abstract_Find_Model *find_model) {
    if(find_model){
        beginResetModel();
            m_row_to_ligne.clear();
            if(find_model->root_leaf()){
                for (szt id = 0; id != nbr_lignes(); ++id)
                    if(find_model->test_root(id))
                        m_row_to_ligne.push_back(id);
            }
            else {
                for (szt id = 0; id != nbr_lignes(); ++id)
                    if(find_model->test_tree(id))
                        m_row_to_ligne.push_back(id);
            }
            if(m_colonne_sorted >=0 &&m_colonne_sorted < columnCount())
                sort(m_colonne_sorted,m_order_sort);
        endResetModel();
    }
}

Qt::ItemFlags table_model::flags(const QModelIndex &index) const {
    Qt::ItemFlags f = abstract_colonnes_model::flags(index);
    if (index.isValid())
        f |= Qt::ItemNeverHasChildren;
    return f;
}

bool table_model::hasChildren(const QModelIndex &parent) const {
    if (!parent.isValid())
        return rowCount(parent) > 0 &&columnCount(parent) > 0;
    return false;
}

QVariant table_model::headerData(int section, Qt::Orientation orientation, int role) const {
    if(orientation == Qt::Vertical){
        if(role == Qt::DisplayRole)
            return section + 1;
        if((m_valide_ligne || m_unique_ligne) &&role == Qt::BackgroundRole)
            return m_brush[m_etats[m_row_to_ligne[static_cast<szt>(section)]]];
    }
    return abstract_colonnes_model::headerData(section,orientation,role);
}

QModelIndex table_model::index(int row, int column, const QModelIndex &parent) const
    {return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();}


bool table_model::insertRows(int row, int count, const QModelIndex &parent) {
    if (count <= 0 || row < 0 || row > rowCount())
        return false;
    auto nbr_ligne_old = nbr_lignes();
    m_data->add(static_cast<szt>(count));
    // Insertion des nouvelles lignes dans le vecteur des indices.
    beginInsertRows(parent, row, row + count - 1);
        auto new_it = m_row_to_ligne.insert(std::next(m_row_to_ligne.cbegin(),row),static_cast<szt>(count),0);
        for(szt ligne = nbr_ligne_old; ligne != nbr_lignes(); ++ligne, ++new_it)
            *new_it = ligne;
    endInsertRows();
    return true;
}

int table_model::ligne_to_row(szt ligne) const {
    int row = 0;
    auto iter = m_row_to_ligne.cbegin();
    while (iter != m_row_to_ligne.end() &&*iter != ligne){
        ++row;
        ++iter;
    }
    return iter != m_row_to_ligne.end() ? row : -1;
}

bool table_model::removeRows(int row, int count, const QModelIndex &parent) {
    count = std::min(count, rowCount()-row);
    if(count <= 0 || row < 0)
        return false;
    beginRemoveRows(parent,row,row + count - 1);
        // Suppression d'une ligne.
        if (count == 1) {
            auto ligne = row_to_ligne(row);
            if(!m_data->exists_internal_data(ligne) || m_data->remove_internal_data(ligne)){
                m_row_to_ligne.erase(std::next(m_row_to_ligne.cbegin(),row));
                for (auto write = m_row_to_ligne.begin(); write != m_row_to_ligne.end(); ++write) {
                    if(*write > ligne)
                        --(*write);
                }
                m_data->erase(ligne);
            }
        }
        //Suppression de plusieur lignes.
        else {
            // Récupération des indices des lignes à supprimer et suppression des lignes du vecteur des lignes.
            auto read = std::next(m_row_to_ligne.cbegin(),row);
            auto vec_id = std::vector<szt>();
            vec_id.reserve(static_cast<szt>(count));
            auto first = read;
            auto last = read;
            auto new_plage = false;
            for(auto i = 0; i != count; ++i, ++read){
                if(!m_data->exists_internal_data(*read) ||m_data->remove_internal_data(*read)){
                    if(new_plage)
                        first = last = read;
                    vec_id.push_back(*read);
                    ++last;
                }
                else {
                    m_row_to_ligne.erase(first,last);
                    new_plage = true;
                }
            }
            if(!new_plage)
                m_row_to_ligne.erase(first,last);
            // Mise à jour du vecteur des lignes.
            std::sort(vec_id.begin(),vec_id.end());
            for (auto write = m_row_to_ligne.begin(); write != m_row_to_ligne.end(); ++write) {
                szt pos = 0;
                read = vec_id.begin();
                while (read != vec_id.end() &&*write > *read) {
                    ++pos;
                    ++read;
                }
                *write -= pos;
            }
            szt first_ligne = vec_id.back();
            szt last_ligne = first_ligne;
            new_plage = true;
            // Suppréssion des lignes de données par numéro décroissant.
            for(auto iter = std::next(vec_id.crbegin(),1); iter != vec_id.crend(); ++iter){
                if (*iter == first_ligne - 1)
                    --first_ligne;
                else {
                    if(first_ligne == last_ligne)
                        m_data->erase(first_ligne);
                    else
                        m_data->erase(first_ligne, last_ligne + 1);
                    first_ligne = last_ligne = *iter;
                }
            }
            if(first_ligne == last_ligne)
                m_data->erase(first_ligne);
            else
                m_data->erase(first_ligne, last_ligne + 1);
        }
    endRemoveRows();
    return true;
}

bool table_model::remove_rows_selected(const QModelIndexList &selection) {
    selected_range range(selection);
    if(range.is_valid() &&range.left() == 0 &&range.right() == columnCount() - 1){
        auto bloc = range.row_bloc();
        for (auto read = bloc.crbegin(); read != bloc.crend(); ++read)
            removeRows(read->first,read->second);
        return true;
    }
    else
        return false;
}

void table_model::reset_row_to_ligne() {
    m_row_to_ligne.resize(nbr_lignes());
    std::iota(m_row_to_ligne.begin(),m_row_to_ligne.end(),0);
    if(m_colonne_sorted >=0 &&m_colonne_sorted < columnCount())
        sort(m_colonne_sorted);
}

void table_model::save() {
    abstract_colonnes_model::save();
    if(m_valide_ligne || m_unique_ligne)
        headerDataChanged(Qt::Vertical,0,rowCount()-1);
}

void table_model::sort(int column, Qt::SortOrder order){
    set_colonne_sorted(column,order);
    beginResetModel();
        auto col = static_cast<szt>(column);
        if(order == Qt::AscendingOrder)
            std::stable_sort(m_row_to_ligne.begin(),m_row_to_ligne.end(),[this,col](szt a, szt b) {return m_colonnes[col]->compare(a,b);});
        else
            std::stable_sort(m_row_to_ligne.begin(),m_row_to_ligne.end(),[this,col](szt a, szt b) {return m_colonnes[col]->compare(b,a);});
    endResetModel();
}

std::vector<szt> table_model::stat_on_lignes(bool lignes_visibles) const {
    if(lignes_visibles)
        return m_row_to_ligne;
    auto vec = std::vector<szt>(nbr_lignes());
    std::iota(vec.begin(),vec.end(),0);
    return vec;

}

void table_model::update_all_etats() {
    abstract_colonnes_model::update_all_etats();
    headerDataChanged(Qt::Vertical,0,rowCount()-1);
}

void table_model::update_etats(int first, int last, const QModelIndex &parent) {
    if(!parent.isValid()) {
        abstract_colonnes_model::update_etats(first,last,parent);
        headerDataChanged(Qt::Vertical,first,last);
    }
}

/////////////////////////////////// selected_range///////////////////////////////////////
table_model::selected_range::selected_range(const QModelIndexList &range)
    : m_selection(range) {
    if(range.isEmpty()) {
        m_bottom = -1;
        m_left = -1;
        m_right = -1;
        m_top = -1;
        m_is_rect = false;
        m_is_valid = false;
        m_corners_found = true;
        m_validity_set = true;
    }
    else {
        m_corners_found = false;
        m_validity_set = false;
    }
}

const std::vector<int> &table_model::selected_range::columns() {
    if(is_rect() &&m_column_list.empty()){
        m_column_list.resize(static_cast<szt>(m_right - m_left + 1));
        std::iota(m_column_list.begin(),m_column_list.end(),m_left);
    }
    set_validity();
    return m_column_list;
}

std::list<std::pair<int,int>> table_model::selected_range::column_bloc() {
    std::list<std::pair<int,int>> list;
    if(is_rect())
        list.push_back({m_left, m_right - m_left + 1});
    else {
        set_validity();
        list.push_back({m_column_list.front(),1});
        for (auto read = std::next(m_column_list.cbegin(),1); read != m_column_list.cend(); ++read) {
            if(*read == list.back().first + list.back().second)
                ++list.back().second;
            else
                list.push_back({*read,1});
        }
    }
    return list;
}

void table_model::selected_range::find_corners() {
    if(!m_corners_found){
        m_bottom = m_selection.first().row();
        m_left = m_selection.first().column();
        m_right = m_selection.first().column();
        m_top = m_selection.first().row();
        for(QModelIndexList::const_iterator i = m_selection.cbegin(); i != m_selection.cend(); ++i) {
            if(i->row() < m_top)
                m_top = i->row();
            else if(i->row() > m_bottom)
                m_bottom = i->row();

            if(i->column() < m_left)
                m_left = i->column();
            else if(i->column() > m_right)
                m_right = i->column();
        }
        if((m_is_rect = (m_right - m_left + 1) * (m_bottom - m_top + 1) == m_selection.count())) {
            m_is_valid = true;
            m_validity_set = true;
        }
        m_corners_found = true;
    }
}

std::list<std::pair<int,int>> table_model::selected_range::row_bloc(){
    std::list<std::pair<int,int>> list;
    if(is_rect())
        list.push_back({m_top, m_bottom - m_top + 1});
    else {
        set_validity();
        list.push_back({m_row_list.front(),1});
        for (auto read = std::next(m_row_list.cbegin(),1); read != m_row_list.cend(); ++read) {
            if(*read == list.back().first + list.back().second)
                ++list.back().second;
            else
                list.push_back({*read,1});
        }
    }
    return list;
}

const std::vector<int> &table_model::selected_range::rows() {
    if(is_rect() &&m_row_list.empty()){
        m_row_list.resize(static_cast<szt>(m_bottom - m_top + 1));
        std::iota(m_row_list.begin(),m_row_list.end(),m_top);
    }
    set_validity();
    return m_row_list;
}

void table_model::selected_range::set_validity() {
    if(!m_validity_set) {
        if(!(m_is_valid = is_rect())){
            // Réorganise la sélection ligne par ligne.
            std::map<int,std::list<int>> list;
            for(auto i = m_selection.cbegin(); i != m_selection.cend(); ++i)
                list[i->row()].push_back(i->column());
            // Cherche les éléments de la première ligne et les lignes.
            list.begin()->second.sort();
            m_column_list.assign(list.cbegin()->second.cbegin(),list.cbegin()->second.cend());
            m_row_list.resize(list.size());
            auto j = m_row_list.begin();
            for (auto i = list.cbegin(); i != list.cend(); ++i, ++j)
                *j = i->first;
            // Vérifie si tout les lignes contiennent la même sélection.
            m_is_valid = true;
            if(list.size() != 1) {
                auto i = ++(list.begin());
                while(m_is_valid &&i != list.end()) {
                    i->second.sort();
                    m_is_valid = list.cbegin()->second == i->second;
                    ++i;
                }
            }
        }
        m_validity_set = true;
    }
}
