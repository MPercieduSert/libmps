#include "FindWidget.h"

using namespace delegateMPS;
using namespace modelMPS;
using namespace findNodeModel;
using namespace widgetMPS;

///////////////////////////// FindDelegate ////////////////////////////
QWidget * FindDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if(index.isValid()) {
        if(index.column() == OpColumn) {
            auto * comboBox = new QComboBox(parent);
            for (szt i = 0; i != NbrOperation; ++i)
                comboBox->addItem(OperationNode::Strings[i],i);
            return comboBox;
        }
        if(index.column() == ColonneColumn) {
            auto * comboBox = new QComboBox(parent);
            auto vec = static_cast<const modelMPS::FindModel *>(index.model())->nomColonnes();
            for (szt i = 0; i != vec.size(); ++i)
                comboBox->addItem(vec[i],i);
            return comboBox;
        }
        if(index.column() == ComparaisonColumn && index.model()->data(index,Qt::UserRole).toUInt() & ComparaisonSet) {
            auto * comboBox = new QComboBox(parent);
            for (szt i = 0; i != NbrComparaison; ++i)
                comboBox->addItem(AbstractComparaisonNode::Strings[i],i);
            return comboBox;
        }
    }
    return QStyledItemDelegate::createEditor(parent,option,index);
}

bool FindDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                        const QStyleOptionViewItem &option, const QModelIndex &index) {
    if(index.isValid() && index.column() == NegColumn
            && event->type() == QEvent::MouseButtonPress
            && model->flags(index).testFlag(Qt::ItemIsEnabled)) {
        auto eventMouse = static_cast<QMouseEvent *>(event);
        if(eventMouse->button() == Qt::LeftButton) {
            model->setData(index,!model->data(index,Qt::EditRole).toBool());
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event,model,option,index);
}

void FindDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    if(index.isValid() && (index.column() == OpColumn
                           || index.column() == ColonneColumn
                           || index.model()->data(index,Qt::UserRole).toUInt() & ComparaisonSet)) {
        auto * comboBox = static_cast<QComboBox *>(editor);
        comboBox->setCurrentIndex(index.model()->data(index,Qt::EditRole).toInt());
    }
    else
        QStyledItemDelegate::setEditorData(editor,index);
}

void FindDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    if(index.isValid() && (index.column() == OpColumn
                           || index.column() == ColonneColumn
                           || index.model()->data(index,Qt::UserRole).toUInt() & ComparaisonSet)) {
        auto * comboBox = static_cast<QComboBox *>(editor);
        model->setData(index,comboBox->currentIndex());
    }
    else
        QStyledItemDelegate::setModelData(editor,model,index);
}

/////////////////////////////////////////// FindWidget //////////////////////////////////////
FindWidget::FindWidget(FindModel *model, QWidget *parent)
 : QWidget (parent){
    // Widget
    m_addButton = new QPushButton("+");
    m_delButton = new QPushButton("-");
    m_findButton = new QPushButton(tr("Chercher"));
    m_resetButton = new QPushButton(tr("Réinitialiser"));
    m_view = new QTreeView;
    setFindModel(model);
    auto * delegateOld = m_view->itemDelegate();
    m_view->setItemDelegate(new Delegate(this));
    delete delegateOld;
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);
    m_view->setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::DoubleClicked);
    connect(m_model,&modelMPS::FindModel::rowsInserted,m_view,&QTreeView::expand);

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

void FindWidget::setFindModel(FindModel * model) {
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
                            m_model->insertNode(0,m_view->selectionModel()->currentIndex().siblingAtColumn(0));});
        connect(m_delButton,&QPushButton::clicked,this,
                [this](){if(m_view->selectionModel()->hasSelection())
                            m_model->removeNode(m_view->selectionModel()->currentIndex().row(),
                                                m_view->selectionModel()->currentIndex().parent().siblingAtColumn(0));});
        connect(m_findButton,&QPushButton::clicked,m_model,&FindModel::find);
    }
    m_model->setParent(this);
}
