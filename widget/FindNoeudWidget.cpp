#include "FindNoeudWidget.h"

using namespace delegateMPS;
using namespace model_base;
using namespace findNoeudModel;
using namespace widgetMPS;

///////////////////////////// FindNoeudDelegate ////////////////////////////
QWidget *FindNoeudDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if(index.is_valid()) {
        if(index.column() == OpColumn) {
            auto *comboBox = new QComboBox(parent);
            for (uint i = 0; i != Nbr_Operation; ++i)
                comboBox->addItem(OperationNoeud::Strings[i],i);
            return comboBox;
        }
        if(index.column() == ColonneColumn) {
            auto *comboBox = new QComboBox(parent);
            auto vec = static_cast<const model_base::FindNoeudModel *>(index.model())->nom_colonnes();
            for (uint i = 0; i != vec.size(); ++i)
                comboBox->addItem(vec[i],i);
            return comboBox;
        }
        if(index.column() == ComparaisonColumn &&index.model()->data(index,Qt::UserRole).toUInt() &ComparaisonSet) {
            auto *comboBox = new QComboBox(parent);
            for (uint i = 0; i != Nbr_Comparaison; ++i)
                comboBox->addItem(AbstractComparaisonNoeud::Strings[i],i);
            return comboBox;
        }
    }
    return QStyledItemDelegate::createEditor(parent,option,index);
}

bool FindNoeudDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                        const QStyleOptionViewItem &option, const QModelIndex &index) {
    if(index.is_valid() &&index.column() == NegColumn
            &&event->type() == QEvent::MouseButtonPress
            &&model->flags(index).testFlag(Qt::ItemIsEnabled)) {
        auto eventMouse = static_cast<QMouseEvent *>(event);
        if(eventMouse->button() == Qt::LeftButton) {
            model->set_data(index,!model->data(index,Qt::EditRole).toBool());
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event,model,option,index);
}

void FindNoeudDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    if(index.is_valid() &&(index.column() == OpColumn
                           || index.column() == ColonneColumn
                           || index.model()->data(index,Qt::UserRole).toUInt() &ComparaisonSet)) {
        auto *comboBox = static_cast<QComboBox *>(editor);
        comboBox->setCurrentIndex(index.model()->data(index,Qt::EditRole).toInt());
    }
    else
        QStyledItemDelegate::setEditorData(editor,index);
}

void FindNoeudDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    if(index.is_valid() &&(index.column() == OpColumn
                           || index.column() == ColonneColumn
                           || index.model()->data(index,Qt::UserRole).toUInt() &ComparaisonSet)) {
        auto *comboBox = static_cast<QComboBox *>(editor);
        model->set_data(index,comboBox->currentIndex());
    }
    else
        QStyledItemDelegate::setModelData(editor,model,index);
}

/////////////////////////////////////////// FindWidget //////////////////////////////////////
FindNoeudWidget::FindNoeudWidget(FindNoeudModel *model, QWidget *parent)
 : QWidget (parent){
    // Widget
    m_addButton = new QPushButton("+");
    m_delButton = new QPushButton("-");
    m_findButton = new QPushButton(tr("Chercher"));
    m_resetButton = new QPushButton(tr("Réinitialiser"));
    m_view = new QTreeView;
    set_find_model(model);
    auto *delegateOld = m_view->itemDelegate();
    m_view->setItemDelegate(new Delegate(this));
    delete delegateOld;
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);
    m_view->setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::DoubleClicked);
    connect(m_model,&model_base::FindNoeudModel::rowsInserted,m_view,&QTreeView::expand);

    // Calque
    m_buttonsLayout = new QHBoxLayout;
    m_buttonsLayout->addWidget(m_findButton);
    m_buttonsLayout->addWidget(m_addButton);
    m_buttonsLayout->addWidget(m_delButton);
    m_buttonsLayout->addWidget(m_resetButton);
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addWidget(m_view);
    m_mainLayout->addLayout(m_buttonsLayout);
}

void FindNoeudWidget::set_find_model(FindNoeudModel *model) {
    m_view->setModel(model);
    if(m_model) {
        m_addButton->disconnect(this);
        m_delButton->disconnect(this);
        m_findButton->disconnect(this);
        m_resetButton->disconnect(this);
        delete m_model;
    }
    m_model = model;
    if(m_model) {
        connect(m_addButton,&QPushButton::clicked,this,
                [this](){if(m_view->selectionModel()->hasSelection())
                            m_model->insertNoeud(0,m_view->selectionModel()->currentIndex().siblingAtColumn(0));});
        connect(m_delButton,&QPushButton::clicked,this,
                [this](){if(m_view->selectionModel()->hasSelection())
                            m_model->removeNoeud(m_view->selectionModel()->currentIndex().row(),
                                                m_view->selectionModel()->currentIndex().parent().siblingAtColumn(0));});
        connect(m_findButton,&QPushButton::clicked,m_model,&FindNoeudModel::find);
    }
    m_model->set_parent(this);
}
