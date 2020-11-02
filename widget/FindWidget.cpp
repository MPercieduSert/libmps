#include "FindWidget.h"

using namespace delegateMPS;
using namespace modelMPS;
using namespace findNodeModel;
using namespace widgetMPS;
using namespace findNodeWidget;


///////////////////////////// FindDelegate ////////////////////////////
NodeWidget * FindDelegate::createWidget(const NodeIndex &index) const {
    if(index.isValid()) {
        switch (index.data(modelMPS::NodeType,modelMPS::progRole).toInt()) {
        case BoolNodeType:
        case ChoiceNodeType:
        case DateNodeType:
        case OperationNodeType:
        case TexteNodeType:
            break;
        }

    }
    return new NodeWidget(index);
}
//QWidget * FindDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
//    if(index.isValid()) {
//        if(index.column() == OpColumn) {
//            auto * comboBox = new QComboBox(parent);
//            for (szt i = 0; i != NbrOperation; ++i)
//                comboBox->addItem(OperationNode::Strings[i],i);
//            return comboBox;
//        }
//        if(index.column() == ComparaisonColumn && index.model()->data(index,Qt::UserRole).toUInt() & ComparaisonSet) {
//            auto * comboBox = new QComboBox(parent);
//            for (szt i = 0; i != NbrComparaison; ++i)
//                comboBox->addItem(AbstractComparaisonNode::Strings[i],i);
//            return comboBox;
//        }
//    }
//    return QStyledItemDelegate::createEditor(parent,option,index);
//}

//bool FindDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
//                                        const QStyleOptionViewItem &option, const QModelIndex &index) {
//    if(index.isValid() && index.column() == NegColumn
//            && event->type() == QEvent::MouseButtonPress
//            && model->flags(index).testFlag(Qt::ItemIsEnabled)) {
//        auto eventMouse = static_cast<QMouseEvent *>(event);
//        if(eventMouse->button() == Qt::LeftButton) {
//            model->setData(index,!model->data(index,Qt::EditRole).toBool());
//            return true;
//        }
//    }
//    return QStyledItemDelegate::editorEvent(event,model,option,index);
//}

//void FindDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
//    if(index.isValid() && (index.column() == OpColumn
//                           || index.column() == ColonneColumn
//                           || index.model()->data(index,Qt::UserRole).toUInt() & ComparaisonSet)) {
//        auto * comboBox = static_cast<QComboBox *>(editor);
//        comboBox->setCurrentIndex(index.model()->data(index,Qt::EditRole).toInt());
//    }
//    else
//        QStyledItemDelegate::setEditorData(editor,index);
//}

//void FindDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
//    if(index.isValid() && (index.column() == OpColumn
//                           || index.column() == ColonneColumn
//                           || index.model()->data(index,Qt::UserRole).toUInt() & ComparaisonSet)) {
//        auto * comboBox = static_cast<QComboBox *>(editor);
//        model->setData(index,comboBox->currentIndex());
//    }
//    else
//        QStyledItemDelegate::setModelData(editor,model,index);
//}

/////////////////////////////////////////// FindWidget //////////////////////////////////////
FindWidget::FindWidget(FindModel *model, QWidget *parent)
 : QWidget (parent){
    // Widget
    m_addButton = new QPushButton("+");
    m_delButton = new QPushButton("-");
    m_findButton = new QPushButton(tr("Chercher"));
    m_resetButton = new QPushButton(tr("Réinitialiser"));
    m_view = new NodeView;
    setFindModel(model);
    m_view->setDelegate(new FindDelegate);

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
//        connect(m_addButton,&QPushButton::clicked,this,
//                [this](){if(m_view->selectionModel()->hasSelection())
//                            m_model->insertNode(0,m_view->selectionModel()->currentIndex().siblingAtColumn(0));});
//        connect(m_delButton,&QPushButton::clicked,this,
//                [this](){if(m_view->selectionModel()->hasSelection())
//                            m_model->removeNode(m_view->selectionModel()->currentIndex().row(),
//                                                m_view->selectionModel()->currentIndex().parent().siblingAtColumn(0));});
        connect(m_findButton,&QPushButton::clicked,m_model,&FindModel::find);
    }
    m_model->setParent(this);
}

/////////////////////////////////////////////////// Noeud de Recherche /////////////////////////////////
NegationNodeWidget::NegationNodeWidget(const NodeIndex & index, QWidget * parent)
    : FindNodeWidget (index, parent) {
    m_nonCheckBox = new QCheckBox(tr("Négation"));
    m_nonCheckBox->setEnabled(index.flags(NegType).testFlag(Qt::ItemIsEnabled));
    connect(m_nonCheckBox,&QCheckBox::stateChanged,this,[this](){
       m_index.model()->setData(m_index,NegType,m_nonCheckBox->isChecked());
    });
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->addWidget(m_nonCheckBox);
}

ConditionNodeWidget::ConditionNodeWidget(const NodeIndex & index, QWidget * parent)
    : NegationNodeWidget (index,parent) {
    m_colonneLabel = new QLabel(tr("Colonne :"));
    m_colonneCB = new QComboBox;
    m_colonneCB->setEditable(index.flags(ColonneType).testFlag(Qt::ItemIsEnabled));
    m_colonneCB->setInsertPolicy(QComboBox::NoInsert);
    auto vec = static_cast<const modelMPS::FindModel *>(index.model())->nomColonnes();
    for (szt i = 0; i != vec.size(); ++i)
        m_colonneCB->addItem(vec[i],i);
    connect(m_colonneCB,qOverload<int>(&QComboBox::currentIndexChanged),this,[this]()
    {m_index.model()->setData(m_index,ColonneType,m_colonneCB->currentData());});
    m_colonneLayout = new QVBoxLayout;
    m_colonneLayout->addWidget(m_colonneLabel);
    m_colonneLayout->addWidget(m_colonneCB);
    m_mainLayout->addLayout(m_colonneLayout);
}

ComparaisonNodeWidget::ComparaisonNodeWidget(const NodeIndex & index, QWidget * parent)
    : ConditionNodeWidget (index,parent) {
    m_compLabel = new QLabel(tr("Comparaison :"));
    m_compCB = new QComboBox;
    m_compCB->setEditable(index.flags(ColonneType).testFlag(Qt::ItemIsEnabled));
    m_compCB->setInsertPolicy(QComboBox::NoInsert);
    for (szt i = 0; i != NbrComparaison; ++i)
        m_compCB->addItem(AbstractComparaisonNode::Strings[i],i);
    connect(m_compCB,qOverload<int>(&QComboBox::currentIndexChanged),this,[this]()
    {m_index.model()->setData(m_index,ColonneType,m_compCB->currentData());});
    m_compLayout = new QVBoxLayout;
    m_compLayout->addWidget(m_compLabel);
    m_compLayout->addWidget(m_compCB);
    m_mainLayout->addLayout(m_compLayout);
}
