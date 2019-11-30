#include "AbstractTableModel.h"

using namespace modelMPS;

void AbstractTableModel::coller(const QModelIndexList & selection) {
    SelectedRange range(selection);
    if(!range.isValid())
        QMessageBox::critical(nullptr,tr("Erreur de séléction"),tr("Ce type de séléction multiple n'est pas valide pour éffectuer un collage"));
    else {
        auto str = QApplication::clipboard()->text();
        auto rows = str.split('\n');
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
        [[fallthrough]];
        case DialogSelection::NoDialog:
        case DialogSelection::InsertOut:
            if(range.isRect()) {
                for (int i = 0; i < numRows; ++i) {
                    auto columns = rows[i].split('\t');
                    for (int j = 0; j < numColumns; ++j) {
                        int row = range.top() + i;
                        int column = range.left() + j;
                        if (row < rowCount() && column < columnCount())
                            setData(index(row, column), columns[j]);
                    }
                }
            }
            else {
                for (int i = 0; i < numRows; ++i) {
                    auto columns = rows[i].split('\t');
                    for (int j = 0; j < numColumns; ++j) {
                        auto row = i < range.rowCount() ? range.rows().at(i) : range.rows().back() + i - range.rowCount() + 1;
                        auto column =  j < range.columnCount() ? range.columns().at(j) : range.columns().back() + j - range.columnCount() + 1;
                        if (row < rowCount() && column < columnCount())
                            setData(index(row, column), columns[j]);
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
        QMessageBox::critical(nullptr,tr("Erreur de séléction"),tr("Ce type de séléction multiple n'est pas valide pour éffectuer un collage"));
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
                    str += data(index(range.rows().at(i), range.columns().at(j))).toString();
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
    }
    else {
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

        if(!(m_isValid = m_isRect = (m_right - m_left +1 ) *  (m_bottom - m_top +1) == m_selection.count())) {
            std::map<int,std::list<int>> list;
            for(auto i = m_selection.cbegin(); i != m_selection.cend(); ++i)
                list[i->row()].push_back(i->column());

            list.begin()->second.sort();
            m_columnList.assign(list.cbegin()->second.cbegin(),list.cbegin()->second.cend());
            m_rowList.resize(list.size());
            auto j = m_rowList.begin();
            for (auto i = list.cbegin(); i != list.cend(); ++i)
                *j = i->first;

            m_isValid = true;
            if(list.size() != 1) {
                auto i = ++(list.begin());
                while(m_isValid && i != list.end())
                {
                    i->second.sort();
                    m_isValid = list.cbegin()->second == i->second;
                    ++i;
                }
            }
        }
    }
}
