#include "TableModel.h"

using namespace modelMPS;

///////////////////////////////////////// TableModel //////////////////////////
TableModel::TableModel(bool uniqueLigne, bool valideLigne, QObject *parent)
    : AbstractColonnesModel (uniqueLigne,valideLigne,parent){
    connect(this,&TableModel::etatLigneChanged,this,[this](szt ligne){
        auto row = ligneToRow(ligne);
        if(row != -1)
            emit headerDataChanged(Qt::Vertical,row,row);});
    connect(this,&TableModel::etatRowsChanged,this,[this](const QModelIndex &parent, int first, int last){
        if(!parent.is_valid())
            emit headerDataChanged(Qt::Vertical,first,last);});
    connect(this,&TableModel::etatsChanged,this,[this](){emit headerDataChanged(Qt::Vertical,0,rowCount());});
}

void TableModel::coller(const QModelIndexList &selection) {
    SelectedRange range(selection);
    if(!range.is_valid())
        QMessageBox::critical(nullptr,tr("Erreur de séléction"),
                              tr("Ce type de séléction multiple n'est pas valide pour éffectuer un collage"));
    else {
        auto str = QApplication::clipboard()->text();
        auto rows = str.splitRef('\n');
        if(!rows.isEmpty() &&rows.last().isEmpty())
            {rows.removeLast();}
        auto numRows = rows.count();
        auto numColumns = rows.first().count('\t') + 1;
        DialogSelection n = DialogSelection::NoDialog;
        if(range.rowCount() *range.columnCount() != 1
            &&(range.rowCount() < numRows || range.columnCount() < numColumns))
            n = dialogColler();

        switch (n) {
        case DialogSelection::InsertIn:
            if(numRows > range.rowCount())
                numRows = range.rowCount();
            if(numColumns > range.columnCount())
                numColumns = range.columnCount();
        [[clang::fallthrough]];
        case DialogSelection::NoDialog:
        case DialogSelection::InsertOut:
            if(range.isRect()) {
                for (int i = 0; i < numRows; ++i) {
                    auto columns = rows.at(i).split('\t');
                    for (auto j = 0; j < numColumns; ++j) {
                        auto row = range.top() + i;
                        auto column = range.left() + j;
                        if (row < rowCount() &&column < columnCount())
                            set_data(index(row, column), columns.at(j).to_string());
                    }
                }
            }
            else {
                for (int i = 0; i < numRows; ++i) {
                    auto columns = rows.at(i).split('\t');
                    for (int j = 0; j < numColumns; ++j) {
                        auto row = i < range.rowCount() ?
                                    range.row(i) : range.rows().back() + i - range.rowCount() + 1;
                        auto column =  j < range.columnCount() ?
                                    range.column(j) : range.columns().back() + j - range.columnCount() + 1;
                        if (row < rowCount() &&column < columnCount())
                            set_data(index(row, column), columns.at(j).to_string());
                    }
                }
            }
            break;
        case Annuler:
            break;
        }
    }
}

void TableModel::copier(const QModelIndexList &selection) {
    SelectedRange range(selection);
    if(!range.is_valid())
        QMessageBox::critical(nullptr,tr("Erreur de séléction"),
                              tr("Ce type de séléction multiple n'est pas valide pour éffectuer un collage"));
    else {
        QString str;
        if(range.isRect()) {
            for (int i = 0; i < range.rowCount(); ++i) {
                if (i > 0)
                    str.append("\n");
                for (int j = 0; j < range.columnCount(); ++j) {
                    if (j > 0)
                        str.append("\t");
                    str.append(data(index(range.top() + i, range.left() + j)).to_string());
                }
            }
        }
        else {
            for (int i = 0; i < range.rowCount(); ++i) {
                if (i > 0)
                    str += "\n";
                for (int j = 0; j < range.columnCount(); ++j)  {
                    if (j > 0)
                        str += "\t";
                    str += data(index(range.row(i), range.column(j))).to_string();
                }
            }
        }
        QApplication::clipboard()->set_text(str);
    }
}

void TableModel::couper(const QModelIndexList &selection) {
    copier(selection);
    effacer(selection);
}

TableModel::DialogSelection TableModel::dialogColler() const {
    QMessageBox dial;
    dial.set_text(tr("Erreur collage"));
    dial.setInformativeText(tr("La taille de la selection est plus petite que la taille du contenu du presse papier."));
    dial.addButton(QMessageBox::Cancel);
    QPushButton *collerInBouton = dial.addButton(tr("Coller dans la selection"),QMessageBox::YesRole);
    QPushButton *collerOutBouton = dial.addButton(tr("Coller tout"),QMessageBox::NoRole);
    dial.exec();

    if(dial.clickedButton() == collerInBouton)
        return DialogSelection::InsertIn;
    if(dial.clickedButton() == collerOutBouton)
        return DialogSelection::InsertOut;
    return DialogSelection::Annuler;
}

void TableModel::effacer(const QModelIndexList &selection) {
    for(QModelIndexList::const_iterator i = selection.cbegin(); i != selection.cend(); ++i)
        set_data(*i,QVariant());
}

void TableModel::find(AbstractFindModel *findModel) {
    if(findModel){
        beginResetModel();
            m_rowToLigne.clear();
            if(findModel->root_leaf()){
                for (szt id = 0; id != nbrLignes(); ++id)
                    if(findModel->testRoot(id))
                        m_rowToLigne.push_back(id);
            }
            else {
                for (szt id = 0; id != nbrLignes(); ++id)
                    if(findModel->testTree(id))
                        m_rowToLigne.push_back(id);
            }
            if(m_colonneSorted >=0 &&m_colonneSorted < columnCount())
                sort(m_colonneSorted,m_orderSort);
        endResetModel();
    }
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags f = AbstractColonnesModel::flags(index);
    if (index.is_valid())
        f |= Qt::ItemNeverHasChildren;
    return f;
}

bool TableModel::hasChildren(const QModelIndex &parent) const {
    if (!parent.is_valid())
        return rowCount(parent) > 0 &&columnCount(parent) > 0;
    return false;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(orientation == Qt::Vertical){
        if(role == Qt::DisplayRole)
            return section + 1;
        if((m_valideLigne || m_uniqueLigne) &&role == Qt::BackgroundRole)
            return m_brush[m_etats[m_rowToLigne[static_cast<szt>(section)]]];
    }
    return AbstractColonnesModel::headerData(section,orientation,role);
}

QModelIndex TableModel::index(int row, int column, const QModelIndex &parent) const
    {return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();}


bool TableModel::insertRows(int row, int count, const QModelIndex &parent) {
    if (count <= 0 || row < 0 || row > rowCount())
        return false;
    auto nbrLigneOld = nbrLignes();
    m_data->add(static_cast<szt>(count));
    // Insertion des nouvelles lignes dans le vecteur des indices.
    beginInsertRows(parent, row, row + count - 1);
        auto iterNew = m_rowToLigne.insert(std::next(m_rowToLigne.cbegin(),row),static_cast<szt>(count),0);
        for(szt ligne = nbrLigneOld; ligne != nbrLignes(); ++ligne, ++iterNew)
            *iterNew = ligne;
    endInsertRows();
    return true;
}

int TableModel::ligneToRow(szt ligne) const {
    int row = 0;
    auto iter = m_rowToLigne.cbegin();
    while (iter != m_rowToLigne.end() &&*iter != ligne){
        ++row;
        ++iter;
    }
    return iter != m_rowToLigne.end() ? row : -1;
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent) {
    count = std::min(count, rowCount()-row);
    if(count <= 0 || row < 0)
        return false;
    beginRemoveRows(parent,row,row + count - 1);
        // Suppression d'une ligne.
        if (count == 1) {
            auto ligne = rowToLigne(row);
            if(!m_data->existsInternalData(ligne) || m_data->removeInternalData(ligne)){
                m_rowToLigne.erase(std::next(m_rowToLigne.cbegin(),row));
                for (auto write = m_rowToLigne.begin(); write != m_rowToLigne.end(); ++write) {
                    if(*write > ligne)
                        --(*write);
                }
                m_data->erase(ligne);
            }
        }
        //Suppression de plusieur lignes.
        else {
            // Récupération des indices des lignes à supprimer et suppression des lignes du vecteur des lignes.
            auto read = std::next(m_rowToLigne.cbegin(),row);
            auto vecId = std::vector<szt>();
            vecId.reserve(static_cast<szt>(count));
            auto first = read;
            auto last = read;
            auto newPlage = false;
            for(auto i = 0; i != count; ++i, ++read){
                if(!m_data->existsInternalData(*read) ||m_data->removeInternalData(*read)){
                    if(newPlage)
                        first = last = read;
                    vecId.push_back(*read);
                    ++last;
                }
                else {
                    m_rowToLigne.erase(first,last);
                    newPlage = true;
                }
            }
            if(!newPlage)
                m_rowToLigne.erase(first,last);
            // Mise à jour du vecteur des lignes.
            std::sort(vecId.begin(),vecId.end());
            for (auto write = m_rowToLigne.begin(); write != m_rowToLigne.end(); ++write) {
                szt pos = 0;
                read = vecId.begin();
                while (read != vecId.end() &&*write > *read) {
                    ++pos;
                    ++read;
                }
                *write -= pos;
            }
            szt firstLigne = vecId.back();
            szt lastLigne = firstLigne;
            newPlage = true;
            // Suppréssion des lignes de données par numéro décroissant.
            for(auto iter = std::next(vecId.crbegin(),1); iter != vecId.crend(); ++iter){
                if (*iter == firstLigne - 1)
                    --firstLigne;
                else {
                    if(firstLigne == lastLigne)
                        m_data->erase(firstLigne);
                    else
                        m_data->erase(firstLigne, lastLigne + 1);
                    firstLigne = lastLigne = *iter;
                }
            }
            if(firstLigne == lastLigne)
                m_data->erase(firstLigne);
            else
                m_data->erase(firstLigne, lastLigne + 1);
        }
    endRemoveRows();
    return true;
}

bool TableModel::removeRowsSelected(const QModelIndexList &selection) {
    SelectedRange range(selection);
    if(range.is_valid() &&range.left() == 0 &&range.right() == columnCount() - 1){
        auto bloc = range.rowBloc();
        for (auto read = bloc.crbegin(); read != bloc.crend(); ++read)
            removeRows(read->first,read->second);
        return true;
    }
    else
        return false;
}

void TableModel::resetRowToLigne() {
    m_rowToLigne.resize(nbrLignes());
    std::iota(m_rowToLigne.begin(),m_rowToLigne.end(),0);
    if(m_colonneSorted >=0 &&m_colonneSorted < columnCount())
        sort(m_colonneSorted);
}

void TableModel::save() {
    AbstractColonnesModel::save();
    if(m_valideLigne || m_uniqueLigne)
        headerDataChanged(Qt::Vertical,0,rowCount()-1);
}

void TableModel::sort(int column, Qt::SortOrder order){
    setColonneSorted(column,order);
    beginResetModel();
        auto col = static_cast<szt>(column);
        if(order == Qt::AscendingOrder)
            std::stable_sort(m_rowToLigne.begin(),m_rowToLigne.end(),[this,col](szt a, szt b) {return m_colonnes[col]->compare(a,b);});
        else
            std::stable_sort(m_rowToLigne.begin(),m_rowToLigne.end(),[this,col](szt a, szt b) {return m_colonnes[col]->compare(b,a);});
    endResetModel();
}

std::vector<szt> TableModel::statOnLignes(bool lignesVisibles) const {
    if(lignesVisibles)
        return m_rowToLigne;
    auto vec = std::vector<szt>(nbrLignes());
    std::iota(vec.begin(),vec.end(),0);
    return vec;

}

void TableModel::updateAllEtats() {
    AbstractColonnesModel::updateAllEtats();
    headerDataChanged(Qt::Vertical,0,rowCount()-1);
}

void TableModel::updateEtats(int first, int last, const QModelIndex &parent) {
    if(!parent.is_valid()) {
        AbstractColonnesModel::updateEtats(first,last,parent);
        headerDataChanged(Qt::Vertical,first,last);
    }
}

/////////////////////////////////// SelectedRange///////////////////////////////////////

TableModel::SelectedRange::SelectedRange(const QModelIndexList &range)
    : m_selection(range) {
    if(range.isEmpty()) {
        m_bottom = -1;
        m_left = -1;
        m_right = -1;
        m_top = -1;
        m_isRect = false;
        m_is_valid = false;
        m_cornersFound = true;
        m_validitySet = true;
    }
    else {
        m_cornersFound = false;
        m_validitySet = false;
    }
}

const std::vector<int> &TableModel::SelectedRange::columns() {
    if(isRect() &&m_columnList.empty()){
        m_columnList.resize(static_cast<szt>(m_right - m_left + 1));
        std::iota(m_columnList.begin(),m_columnList.end(),m_left);
    }
    setValidity();
    return m_columnList;
}

std::list<std::pair<int,int>> TableModel::SelectedRange::columnBloc() {
    std::list<std::pair<int,int>> list;
    if(isRect())
        list.push_back({m_left, m_right - m_left + 1});
    else {
        setValidity();
        list.push_back({m_columnList.front(),1});
        for (auto read = std::next(m_columnList.cbegin(),1); read != m_columnList.cend(); ++read) {
            if(*read == list.back().first + list.back().second)
                ++list.back().second;
            else
                list.push_back({*read,1});
        }
    }
    return list;
}

void TableModel::SelectedRange::findCorners() {
    if(!m_cornersFound){
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
        if((m_isRect = (m_right - m_left + 1) * (m_bottom - m_top + 1) == m_selection.count())) {
            m_is_valid = true;
            m_validitySet = true;
        }
        m_cornersFound = true;
    }
}

std::list<std::pair<int,int>> TableModel::SelectedRange::rowBloc(){
    std::list<std::pair<int,int>> list;
    if(isRect())
        list.push_back({m_top, m_bottom - m_top + 1});
    else {
        setValidity();
        list.push_back({m_rowList.front(),1});
        for (auto read = std::next(m_rowList.cbegin(),1); read != m_rowList.cend(); ++read) {
            if(*read == list.back().first + list.back().second)
                ++list.back().second;
            else
                list.push_back({*read,1});
        }
    }
    return list;
}

const std::vector<int> &TableModel::SelectedRange::rows() {
    if(isRect() &&m_rowList.empty()){
        m_rowList.resize(static_cast<szt>(m_bottom - m_top + 1));
        std::iota(m_rowList.begin(),m_rowList.end(),m_top);
    }
    setValidity();
    return m_rowList;
}

void TableModel::SelectedRange::setValidity() {
    if(!m_validitySet) {
        if(!(m_is_valid = isRect())){
            // Réorganise la sélection ligne par ligne.
            std::map<int,std::list<int>> list;
            for(auto i = m_selection.cbegin(); i != m_selection.cend(); ++i)
                list[i->row()].push_back(i->column());
            // Cherche les éléments de la première ligne et les lignes.
            list.begin()->second.sort();
            m_columnList.assign(list.cbegin()->second.cbegin(),list.cbegin()->second.cend());
            m_rowList.resize(list.size());
            auto j = m_rowList.begin();
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
        m_validitySet = true;
    }
}
