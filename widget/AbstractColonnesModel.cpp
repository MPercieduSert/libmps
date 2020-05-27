#include "AbstractColonnesModel.h"

using namespace modelMPS;

AbstractColonnesModel::AbstractColonne::~AbstractColonne() = default;
AbstractBoolColonne::~AbstractBoolColonne() = default;

AbstractColonnesModel::AbstractColonnesModel(bool uniqueLigne, bool valideLigne, QObject * parent)
    : AbstractModel(parent), m_uniqueLigne(uniqueLigne), m_valideLigne(valideLigne) {
    if(m_valideLigne || m_uniqueLigne) {
        connect(this,&AbstractColonnesModel::rowsRemoved,this,&AbstractColonnesModel::resizeEtats);
        connect(this,&AbstractColonnesModel::rowsInserted,this,
                [this](const QModelIndex & parent,int first,int last){
                    resizeEtats();
                    updateEtats(first,last,parent);});
        connect(this,&AbstractColonnesModel::dataChanged,this,
                [this](const QModelIndex & topLeft, const QModelIndex & bottomRight) {
                    if(topLeft.isValid() && bottomRight.isValid())
                        updateEtats(topLeft.row(), bottomRight.row(), topLeft.parent());});
        connect(this,&AbstractColonnesModel::modelReset,this,
                [this](){resizeEtats();
                updateAllEtats();});
    m_brush.resize(NbrEtat);
    m_brush[Sauver] = QBrush();
    m_brush[Valide] = QBrush(QColor(Qt::green));
    m_brush[Invalide] = QBrush(QColor(Qt::red));
    m_brush[Double] = QBrush(QColor(255,125,0));
    m_brush[DoubleSauver] = QBrush(QColor(155,88,0));
    }
}

QVariant AbstractColonnesModel::data(const QModelIndex &index, int role) const {
    if(checkIndex(index))
        return colonne(index).data(ligne(index),role);
    return QVariant();
}

Qt::ItemFlags AbstractColonnesModel::flags(const QModelIndex & index) const {
    if(checkIndex(index))
        return colonne(index).flags(ligne(index));
    return Qt::ItemIsEnabled;
}

QVariant AbstractColonnesModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return m_colonnes[static_cast<szt>(section)]->header();
    return QVariant();
}

void AbstractColonnesModel::hydrate(int row, const QModelIndex & parent){
    m_data->hydrate(rowToLigne(row,parent));
    emit dataChanged(index(row,0,parent),index(row,columnCount() - 1,parent));
}

bool AbstractColonnesModel::insertColonne(int pos, const NewColonneInfo & info, bool allParent){
    if(pos < 0 || pos > columnCount())
        pos = columnCount();
    auto iter = std::next(m_colonnes.cbegin(),pos);
    if(allParent) {
        beginResetModel();
            m_colonnes.insert(iter,m_data->makeColonne(info));
        endResetModel();
    }
    else {
        beginInsertColumns(QModelIndex(),pos,pos);
            m_colonnes.insert(iter,m_data->makeColonne(info));
        endInsertColumns();
    }
    return true;
}

void AbstractColonnesModel::save() {
    if(!m_valideLigne && !m_uniqueLigne)
        for (szt ligne = 0; ligne != nbrLignes(); ++ligne)
            m_data->save(ligne);
    else {
        for (szt ligne = 0; ligne != nbrLignes(); ++ligne) {
            if(m_etats[ligne] == Valide){
                m_data->save(ligne);
                m_etats[ligne] = Sauver;
                etatLigneChanged(ligne);
            }
        }
    }
}

bool AbstractColonnesModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(checkIndex(index) && colonne(index).setData(ligne(index),value,role)) {
        emit dataChanged(index,index);
        return true;
    }
    return false;
}

void AbstractColonnesModel::setTableau(std::unique_ptr<AbstractTableau> && tableau) {
    m_data = std::move(tableau);
    resetRow();
}

QVariant AbstractColonnesModel::statistique(int pos, szt fonction, bool lignesVisibles) const {
    if(pos < 0 || pos >= columnCount())
        return QVariant();
    auto col = static_cast<szt>(pos);
    auto idLignes = statOnLignes(lignesVisibles);
    switch (fonction) {
    case Max: {
        auto iter = std::max_element(idLignes.cbegin(),idLignes.cend(),
                                     [this,col](szt a, szt b) {return m_colonnes[col]->compare(a,b);});
        return m_colonnes[col]->data(*iter,Qt::DisplayRole);}
    case Min: {
        auto iter = std::min_element(idLignes.cbegin(),idLignes.cend(),
                                     [this,col](szt a, szt b) {return m_colonnes[col]->compare(a,b);});
        return m_colonnes[col]->data(*iter,Qt::DisplayRole);}
    default:
        return QVariant();
    }
}

void AbstractColonnesModel::updateAllEtats() {
    if(m_valideLigne && !m_uniqueLigne){
        for(szt ligne = 0; ligne != nbrLignes(); ++ligne)
            updateValideLigne(ligne);
        emit etatsChanged();
    }
    else if(m_uniqueLigne) {
        for(szt ligne = 0; ligne != nbrLignes(); ++ligne)
            updateUniqueLigne(ligne, false);
        emit etatsChanged();
    }
}

void AbstractColonnesModel::updateEtats(int first, int last, const QModelIndex &parent) {
    if((m_valideLigne || m_uniqueLigne) && first >= 0 && first <= last && last < rowCount(parent)) {
        if(!m_uniqueLigne)
            for(auto row = first; row != last + 1; ++row)
                updateValideLigne(ligne(index(row,0,parent)));
        else
            for(auto row = first; row != last + 1; ++row)
                updateUniqueLigne(ligne(index(row,0,parent)));
        emit etatRowsChanged(parent,first,last);
    }
}

void AbstractColonnesModel::updateValideLigne(szt ligne){
    if(!m_data->valide(ligne))
        m_etats[ligne] = Invalide;
    else if (m_data->newOrModif(ligne))
        m_etats[ligne] = Valide;
    else
        m_etats[ligne] = Sauver;

}

void AbstractColonnesModel::updateUniqueLigne(szt ligne, bool update){
    if(!m_data->valide(ligne))
        m_etats[ligne] = Invalide;
    else if (m_data->newOrModif(ligne)) {
        auto iter = m_doubles.find(ligne);
        auto find = true;
        if(iter != m_doubles.end()) {
            if(!m_data->egal(ligne,iter->second.front())){
                if(iter->second.size() == 1) {
                    auto ligneD = iter->second.front();
                    m_doubles.erase(ligne);
                    m_doubles.erase(ligneD);
                    if(m_data->newOrModif(ligneD)){
                        m_etats[ligneD] = Valide;
                        if(update)
                            emit etatLigneChanged(ligneD);
                    }
                    else{
                        m_etats[ligneD] = Sauver;
                        if(update)
                            emit etatLigneChanged(ligneD);
                    }
                }
                else {
                    for (auto iterList = iter->second.cbegin(); iterList != iter->second.cend(); ++iterList) {
                        auto & listDouble = m_doubles[*iterList];
                        listDouble.erase(std::find(listDouble.cbegin(),listDouble.cend(),ligne));
                    }
                    m_doubles.erase(ligne);
                }
            }
            else
                find = false;
        }
        if(find) {
            szt ligne2 = 0;
            while(ligne2 != nbrLignes() && (ligne2 == ligne || !m_data->egal(ligne2,ligne)))
                ++ligne2;
            if(ligne2 == nbrLignes())
                m_etats[ligne] = Valide;
            else {
                m_etats[ligne] = Double;
                if(m_doubles[ligne2].empty()){
                    m_doubles[ligne].push_back(ligne2);
                    m_doubles[ligne2].push_back(ligne);
                    if(m_data->newOrModif(ligne2)) {
                        m_etats[ligne2] = Double;
                        if(update)
                            emit etatLigneChanged(ligne2);
                    }
                    else {
                        m_etats[ligne2] = DoubleSauver;
                        if(update)
                            emit etatLigneChanged(ligne2);
                    }
                }
                else {
                    m_doubles[ligne] = m_doubles[ligne2];
                    m_doubles[ligne].push_back(ligne2);
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
void AbstractColonnesModel::AbstractTableau::erase(szt first, szt last) {
    for (auto ligne = first; ligne != last; ++ligne)
        erase(ligne);
}

/////////////////////////////////////// CompositionTableau /////////////////////
void CompositionTableau::add(szt count) {
    for(auto iter = m_tableaux.begin(); iter != m_tableaux.end(); ++iter)
        (*iter)->add(count);
}

bool CompositionTableau::egal(szt ligne1, szt ligne2) const
    TEST_COMPOSITION_TABLEAU(EgalModule,egal(ligne1,ligne2))

void CompositionTableau::erase(szt ligne) {
    for(auto iter = m_tableaux.begin(); iter != m_tableaux.end(); ++iter)
        (*iter)->erase(ligne);
}

void CompositionTableau::erase(szt first, szt last) {
    for(auto iter = m_tableaux.begin(); iter != m_tableaux.end(); ++iter)
        (*iter)->erase(first, last);
}

bool CompositionTableau::existsInternalData(szt ligne) const
    TEST_COMPOSITION_TABLEAU(ExistsModule,existsInternalData(ligne))

void CompositionTableau::hydrate(szt ligne)
    OPERATION_COMPOSITION_TABLEAU(HydrateModule,hydrate(ligne));

bool CompositionTableau::newOrModif(szt ligne) const
    TEST_COMPOSITION_TABLEAU(NewOrModifModule,newOrModif(ligne))

void CompositionTableau::push_back(std::unique_ptr<AbstractTableau> && tableau, bool actif){
    if(!m_tableaux.empty() && tableau->size() != size())
        throw std::runtime_error(QString("void CompositionTableau::push_back(std::unique_ptr<AbstractTableau> && )\n"
                                 "La taille du nouveau tableau (").append(QString::number(tableau->size()))
                                 .append(") ne correspond pas à ceux déjà présents (").append(QString::number(size())).toStdString());
    m_tableaux.push_back(std::move(tableau));
    for (auto iter = m_module.begin(); iter != m_module.cend(); ++iter)
        iter->push_back(actif);
}

bool CompositionTableau::removeInternalData(szt ligne)
    TEST_COMPOSITION_TABLEAU(RemoveModule,removeInternalData(ligne))

void CompositionTableau::save(szt ligne)
    OPERATION_COMPOSITION_TABLEAU(SaveModule,save(ligne))

bool CompositionTableau::valide(szt ligne) const
    TEST_COMPOSITION_TABLEAU(ValideModule,valide(ligne))

