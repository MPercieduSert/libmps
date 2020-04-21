#include "AbstractTableModel.h"

using namespace modelMPS;

void AbstractTableModel::coller(const QModelIndexList & selection) {
    SelectedRange range(selection);
    if(!range.isValid())
        QMessageBox::critical(nullptr,tr("Erreur de séléction"),
                              tr("Ce type de séléction multiple n'est pas valide pour éffectuer un collage"));
    else {
        auto str = QApplication::clipboard()->text();
        auto rows = str.splitRef('\n');
        if(!rows.isEmpty() && rows.last().isEmpty())
            {rows.removeLast();}
        auto numRows = rows.count();
        auto numColumns = rows.first().count('\t') + 1;
        DialogSelection n = DialogSelection::NoDialog;
        if(range.rowCount() * range.columnCount() != 1
            && (range.rowCount() < numRows || range.columnCount() < numColumns))
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
                        if (row < rowCount() && column < columnCount())
                            setData(index(row, column), columns.at(j).toString());
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
                        if (row < rowCount() && column < columnCount())
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

void AbstractTableModel::copier(const QModelIndexList &selection) {
    SelectedRange range(selection);
    if(!range.isValid())
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
                    str.append(data(index(range.top() + i, range.left() + j)).toString());
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
                    str += data(index(range.row(i), range.column(j))).toString();
                }
            }
        }
        QApplication::clipboard()->setText(str);
    }
}

void AbstractTableModel::couper(const QModelIndexList &selection) {
    copier(selection);
    effacer(selection);
}

AbstractTableModel::DialogSelection AbstractTableModel::dialogColler() const {
    QMessageBox dial;
    dial.setText(tr("Erreur collage"));
    dial.setInformativeText(tr("La taille de la selection est plus petite que la taille du contenu du presse papier."));
    dial.addButton(QMessageBox::Cancel);
    QPushButton * collerInBouton = dial.addButton(tr("Coller dans la selection"),QMessageBox::YesRole);
    QPushButton * collerOutBouton = dial.addButton(tr("Coller tout"),QMessageBox::NoRole);
    dial.exec();

    if(dial.clickedButton() == collerInBouton)
        return DialogSelection::InsertIn;
    if(dial.clickedButton() == collerOutBouton)
        return DialogSelection::InsertOut;
    return DialogSelection::Annuler;
}

void AbstractTableModel::effacer(const QModelIndexList &selection) {
    for(QModelIndexList::const_iterator i = selection.cbegin(); i != selection.cend(); ++i)
        setData(*i,QVariant());
}

AbstractTableModel::SelectedRange::SelectedRange(const QModelIndexList &range)
    : m_selection(range) {
    if(range.isEmpty()) {
        m_bottom = -1;
        m_left = -1;
        m_right = -1;
        m_top = -1;
        m_isRect = false;
        m_isValid = false;
        m_cornersFound = true;
        m_validitySet = true;
    }
    else {
        m_cornersFound = false;
        m_validitySet = false;
    }
}

const std::vector<int> & AbstractTableModel::SelectedRange::columns() {
    if(isRect() && m_columnList.empty()){
        m_columnList.resize(static_cast<szt>(m_right - m_left + 1));
        std::iota(m_columnList.begin(),m_columnList.end(),m_left);
    }
    setValidity();
    return m_columnList;
}

std::list<std::pair<int,int>> AbstractTableModel::SelectedRange::columnBloc() {
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

void AbstractTableModel::SelectedRange::findCorners() {
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
        if((m_isRect = (m_right - m_left + 1) *  (m_bottom - m_top + 1) == m_selection.count())) {
            m_isValid = true;
            m_validitySet = true;
        }
        m_cornersFound = true;
    }
}

std::list<std::pair<int,int>> AbstractTableModel::SelectedRange::rowBloc(){
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

const std::vector<int> & AbstractTableModel::SelectedRange::rows() {
    if(isRect() && m_rowList.empty()){
        m_rowList.resize(static_cast<szt>(m_bottom - m_top + 1));
        std::iota(m_rowList.begin(),m_rowList.end(),m_top);
    }
    setValidity();
    return m_rowList;
}

void AbstractTableModel::SelectedRange::setValidity() {
    if(!m_validitySet) {
        if(!(m_isValid = isRect())){
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
            m_isValid = true;
            if(list.size() != 1) {
                auto i = ++(list.begin());
                while(m_isValid && i != list.end()) {
                    i->second.sort();
                    m_isValid = list.cbegin()->second == i->second;
                    ++i;
                }
            }
        }
        m_validitySet = true;
    }
}
