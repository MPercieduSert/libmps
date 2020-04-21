#include "AbstractTableEntityModel.h"

using namespace modelMPS;

AbstractTableEntityModel::AbstractColumn::~AbstractColumn() = default;

QVariant AbstractTableEntityModel::data(const QModelIndex &index, int role) const {
    if(indexValide(index)) {
        auto ind = indice(index);
        return m_columns[ind.second]->data(ind.first,role);
    }
    return QVariant();
}

Qt::ItemFlags AbstractTableEntityModel::flags(const QModelIndex & index) const {
    if(!indexValide(index))
        return Qt::ItemIsEnabled;
    auto ind = indice(index);
    return m_columns[ind.second]->flags(ind.first);
}

QVariant AbstractTableEntityModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if(orientation == Qt::Vertical)
            return section + 1;
        else
            return m_columns[static_cast<szt>(section)]->header();
    }
    return QVariant();
}

bool AbstractTableEntityModel::insertColumn(int column, std::unique_ptr<AbstractColumn> && col){
    if(column < 0 || column >= columnCount())
        column = columnCount();
    auto pos = m_columns.cbegin() + column;
    beginInsertColumns(QModelIndex(),column,column);
        m_columns.insert(pos,std::move(col));
    endInsertColumns();
    return true;
}

bool AbstractTableEntityModel::insertRows(int row, int count, const QModelIndex & parent) {
    if (count <= 0)
        return false;
    auto zcount = static_cast<szt>(count);
    auto sizeRow = nbrId();
    szt j = 0;
    // Ajoute des nouvelles lignes à la fin du tableau de donnée.
    for (auto iter = m_data.begin(); iter != m_data.end(); ++iter,++j)
        iter->resize(sizeRow + zcount,[this,j,i = sizeRow]()mutable{return entityFactory({i++,j});});
    // Insertion des nouvelles lignes dans le vecteur des indices.
    auto pair = valideRowAndIter(row);
    beginInsertRows(parent,pair.first,pair.first+count-1);
        auto iterNew = m_idRow.insert(pair.second,zcount,0);
        for(szt id = static_cast<szt>(sizeRow); id != nbrId(); ++id, ++iterNew)
            *iterNew = id;
    endInsertRows();
    return true;
}

QVariant AbstractTableEntityModel::max(int column) const {
    auto col = static_cast<szt>(column);
    auto iter = std::max_element(m_idRow.cbegin(),m_idRow.cend(),[this,col](szt a, szt b) {return m_columns[col]->compare(a,b);});
    return m_columns[col]->data(*iter,Qt::DisplayRole);
}

QVariant AbstractTableEntityModel::min(int column) const {
    auto col = static_cast<szt>(column);
    auto iter = std::min_element(m_idRow.cbegin(),m_idRow.cend(),[this,col](szt a, szt b) {return m_columns[col]->compare(a,b);});
    return m_columns[col]->data(*iter,Qt::DisplayRole);
}

bool AbstractTableEntityModel::removeRows(int row, int count, const QModelIndex & parent) {
    count = std::min(count, rowCount()-row);
    if(count <= 0)
        return false;
    auto pair = valideRowAndIter(row);
    beginRemoveRows(parent,pair.first,pair.first + count - 1);
        // Suppression d'une ligne.
        if (count == 1) {
            auto id = idRow(pair.first);
            m_idRow.erase(pair.second);
            for (auto write = m_idRow.begin(); write != m_idRow.end(); ++write) {
                if(*write > id)
                    --(*write);
            }
            removeInternalData(id);
            for (auto colonne = m_data.begin(); colonne != m_data.end(); ++colonne)
                colonne->erase(colonne->cbegin() + id);
        }
        //Suppression de plusieur lignes.
        else {
            // Récupération des indices des lignes à supprimer et suppression des lignes du vecteur des indices.
            auto read = pair.second;
            auto vecId = std::vector<szt>(static_cast<szt>(count));
            for (auto write = vecId.begin(); write != vecId.end(); ++write, ++read)
                *write = *read;
            m_idRow.erase(pair.second,pair.second + count);
            // Mise à jour du vecteur des indices.
            std::sort(vecId.begin(),vecId.end());
            for (auto write = m_idRow.begin(); write != m_idRow.end(); ++write) {
                szt pos = 0;
                read = vecId.begin();
                while (read != vecId.end() && *write > *read) {
                    ++pos;
                    ++read;
                }
                *write -= pos;
            }
            // Suppréssion des lignes de données par numéro décoissant.
            for(auto ligne = vecId.crbegin(); ligne != vecId.crend(); ++ligne){
                removeInternalData(*ligne);
                for (auto colonne = m_data.begin(); colonne != m_data.end(); ++colonne)
                    colonne->erase(colonne->cbegin()+*ligne);
            }
        }
    endRemoveRows();
    return true;
}

bool AbstractTableEntityModel::removeRowsSelected(const QModelIndexList &selection) {
    SelectedRange range(selection);
    if(range.isValid() && range.left() == 0 && range.right() == columnCount() - 1){
        auto bloc = range.rowBloc();
        for (auto read = bloc.crbegin(); read != bloc.crend(); ++read)
            removeRows(read->first,read->second);
        return true;
    }
    else
        return false;
}

void AbstractTableEntityModel::save() {
    for(auto iter = m_data.begin(); iter != m_data.end(); ++iter)
        bdd().save(*iter);
}

bool AbstractTableEntityModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(indexValide(index)) {
        auto ind = indice(index);
        if(m_columns[ind.second]->setData(value,ind.first,role)) {
            emit dataChanged(index,index);
            return true;
        }
    }
    return false;
}

void AbstractTableEntityModel::sort(int column, Qt::SortOrder order){
    beginResetModel();
        auto col = static_cast<szt>(column);
        if(order == Qt::AscendingOrder)
            std::stable_sort(m_idRow.begin(),m_idRow.end(),[this,col](szt a, szt b) {return m_columns[col]->compare(a,b);});
        else
            std::stable_sort(m_idRow.begin(),m_idRow.end(),[this,col](szt a, szt b) {return m_columns[col]->compare(b,a);});
    endResetModel();
}

QPair<int,std::vector<szt>::const_iterator> AbstractTableEntityModel::valideRowAndIter(int row) const {
    QPair<int,std::vector<szt>::const_iterator> pair;
    if(row >= 0) {
        if(row < rowCount()) {
            pair.first = row;
            pair.second = m_idRow.cbegin() + row;
        }
        else {
            pair.first = rowCount();
            pair.second = m_idRow.cend();
        }
    }
    else {
        if(-row <= rowCount()) {
            pair.first = rowCount() + row + 1;
            pair.second = std::prev(m_idRow.cend(),-row-1);
        }
        else {
            pair.first = 0;
            pair.second = m_idRow.begin();
        }
    }
    return pair;
}

//////////////////////////////////// Valide /////////////
AbstractTableValideModel::AbstractTableValideModel(bddMPS::Bdd & bdd, QObject * parent)
    : AbstractTableEntityModel(bdd,parent) {
    connect(this,&AbstractTableValideModel::rowsRemoved,this,&AbstractTableValideModel::resizeEtats);
    connect(this,&AbstractTableValideModel::rowsInserted,this,
            [this](const QModelIndex &,int first,int last){
                resizeEtats();
                updateHeaderData(first,last);});
    connect(this,&AbstractTableValideModel::dataChanged,this,
            [this](const QModelIndex & topLeft, const QModelIndex & bottomRight) {
                if(topLeft.isValid() && bottomRight.isValid())
                    updateHeaderData(topLeft.row(),bottomRight.row());});
    connect(this,&AbstractTableValideModel::modelReset,this,
            [this](){resizeEtats();
            updateHeaderData(0,rowCount()-1);});
    m_brush.push_back(QBrush());
    m_brush.push_back(QBrush(QColor(Qt::green)));
    m_brush.push_back(QBrush(QColor(Qt::red)));
}

QVariant AbstractTableValideModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(orientation == Qt::Vertical && role == Qt::BackgroundRole)
        return m_brush[m_etats[idRow(section)]];
    return AbstractTableEntityModel::headerData(section,orientation,role);
}

void AbstractTableValideModel::save() {
    for (szt id = 0; id != nbrRow(); ++id) {
        if(m_etats[id] == Valide){
            saveRow(id);
            m_etats[id] = Sauver;
        }
    }
    emit headerDataChanged(Qt::Vertical,0,rowCount());
}

void AbstractTableValideModel::updateHeaderData(int first, int last) {//, const QVector<int> & /*roles*/) {
    if(first <= last) {
        for (auto row = first; row != last + 1; ++row) {
            auto id = idRow(row);
            if(!valideRow(id))
                m_etats[id] = Invalide;
            else if (newOrModifRow(id))
                m_etats[id] = Valide;
            else
                m_etats[id] = Sauver;
        }
        emit headerDataChanged(Qt::Vertical,first,last);
    }
}

////////////////////////////////////////Unique //////////////

void AbstractTableUniqueModel::updateHeaderData(int first, int last) {//, const QVector<int> & /*roles*/) {
    if(first <= last) {
        for (auto row = first; row != last + 1; ++row) {
            auto id = idRow(row);
            if(!valideRow(id))
                m_etats[id] = Invalide;
            else if (newOrModifRow(id)) {
                auto iter = m_doubles.find(id);
                auto find = true;
                if(iter != m_doubles.end()) {
                    if(!egalRow(id,iter->second.front())){
                        if(iter->second.size() == 1) {
                            auto idD = iter->second.front();
                            m_doubles.erase(idD);
                            if(newOrModifRow(idD))
                                m_etats[idD] = Valide;
                            else
                                m_etats[idD] = Sauver;
                            auto rowD = idToRow(idD);
                            emit headerDataChanged(Qt::Vertical,rowD,rowD);
                        }
                        else
                            for (auto iterList = iter->second.cbegin(); iterList != iter->second.cend(); ++iterList) {
                                auto & listDouble = m_doubles[*iterList];
                                listDouble.erase(std::find(listDouble.cbegin(),listDouble.cend(),id));
                            }
                    }
                    else
                        find = false;
                }
                if(find) {
                    szt id2 = 0;
                    while(id2 != nbrId() && (id2 == id || !egalRow(id2,id)))
                        ++id2;
                    if(id2 == nbrId())
                        m_etats[id] = Valide;
                    else {
                        m_etats[id] = Double;
                        if(m_doubles[id2].empty()){
                            m_doubles[id].push_back(id2);
                            m_doubles[id2].push_back(id);
                            if(newOrModifRow(id2))
                                m_etats[id2] = Double;
                            else
                                m_etats[id2] = DoubleSauver;
                            auto row2 = idToRow(id2);
                            emit headerDataChanged(Qt::Vertical,row2,row2);
                        }
                        else {
                            m_doubles[id] = m_doubles[id2];
                            m_doubles[id].push_back(id2);
                            for (auto iter = m_doubles[id].cbegin(); iter != m_doubles[id].cend(); ++iter)
                                m_doubles[*iter].push_back(id);
                        }
                    }
                }
            }
            else
                m_etats[id] = Sauver;
        }
        emit headerDataChanged(Qt::Vertical,first,last);
    }
}
