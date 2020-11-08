#include "FindWidget.h"

using namespace delegateMPS;
using namespace modelMPS;
using namespace findNodeModel;
using namespace widgetMPS;
using namespace findNodeWidget;


///////////////////////////// FindDelegate ////////////////////////////
NodeWidget * FindDelegate::createWidget(const NodeIndex &index, QWidget *parent) const {
    if(index.isValid()) {
        switch (index.data(NodeTypeCible,DataRole).toInt()) {
        case FindModel::BoolNodeType:
            return new BoolNodeWidget(index,parent);
        case FindModel::ChoiceNodeType:
            return new ChoiceNodeWidget(index,parent);
        case FindModel::DateNodeType:
            return new DateNodeWidget(index,parent);
        case FindModel::OperationNodeType:
            return new OperationNodeWidget(index,parent);
        case FindModel::TexteNodeType:
            return new TexteNodeWidget(index,parent);
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
        connect(m_resetButton,&QPushButton::clicked,m_model,&FindModel::reset);
        connect(m_findButton,&QPushButton::clicked,m_model,&FindModel::find);
    }
    m_model->setParent(this);
}

/////////////////////////////////////////////////// Noeud de Recherche /////////////////////////////////
BoolNodeWidget::BoolNodeWidget(const NodeIndex & index, QWidget * parent,int tp)
    : ConditionNodeWidget (index,parent,tp) {
    m_falseCheck = new QCheckBox(m_index.data(FindModel::FalseCible,LabelRole).toString());
    m_trueCheck = new QCheckBox(m_index.data(FindModel::TrueCible,LabelRole).toString());
    // Calque
    m_boolLayout = new QHBoxLayout;
    m_boolLayout->addWidget(m_trueCheck);
    m_boolLayout->addWidget(m_falseCheck);
    m_mainLayout->addLayout(m_boolLayout);
}

ChoiceNodeWidget::ChoiceNodeWidget(const NodeIndex & index, QWidget * parent, int tp)
    : FindNodeWidget (index,parent,tp) {
    // Opération.
    m_opLabel = new QLabel(tr("Opération :"));
    m_opCB = new QComboBox;
    m_opCB->setEnabled(index.flags(FindModel::OpCible).testFlag(Qt::ItemIsEnabled));
    m_opCB->setEditable(index.flags(FindModel::OpCible).testFlag(Qt::ItemIsEditable));
    m_opCB->setInsertPolicy(QComboBox::NoInsert);
    for (szt i = 0; i != FindModel::NbrOperation; ++i)
        m_opCB->addItem(OperationNode::Strings[i],i);
    m_opCB->addItem("?",-1);
    m_opCB->setCurrentIndex(FindModel::NbrOperation);
    // Colonne
    m_colonneLabel = new QLabel(tr("Colonne :"));
    m_colonneCB = new QComboBox;
    m_colonneCB->setEditable(index.flags(FindModel::ColonneCible).testFlag(Qt::ItemIsEnabled));
    m_colonneCB->setEditable(index.flags(FindModel::ColonneCible).testFlag(Qt::ItemIsEditable));
    m_colonneCB->setInsertPolicy(QComboBox::NoInsert);
    auto vec = static_cast<const modelMPS::FindModel *>(index.model())->nomColonnes();
    for (szt i = 0; i != vec.size(); ++i)
        m_colonneCB->addItem(vec[i],i);
    m_colonneCB->addItem("?",-1);
    m_colonneCB->setCurrentIndex(static_cast<int>(vec.size()));
    // Calque
    m_mainLayout = new QGridLayout(this);
    m_mainLayout->addWidget(m_opLabel,LabelRow,OpCol);
    m_mainLayout->addWidget(m_opCB,ComboBoxRow,OpCol);
    m_mainLayout->addWidget(m_colonneLabel,LabelRow,ColonneCol);
    m_mainLayout->addWidget(m_colonneCB,ComboBoxRow,ColonneCol);
}

ComparaisonNodeWidget::ComparaisonNodeWidget(const NodeIndex & index, QWidget * parent, int tp)
    : ConditionNodeWidget (index,parent,tp) {
    m_compLabel = new QLabel(tr("Comparaison :"));
    m_compCB = new QComboBox;
    m_compCB->setEditable(index.flags(FindModel::ColonneCible).testFlag(Qt::ItemIsEnabled));
    m_compCB->setInsertPolicy(QComboBox::NoInsert);
    for (szt i = 0; i != FindModel::NbrComparaison; ++i)
        m_compCB->addItem(AbstractComparaisonNode::Strings[i],i);
    // Calque
    m_compLayout = new QVBoxLayout;
    m_compLayout->addWidget(m_compLabel);
    m_compLayout->addWidget(m_compCB);
    m_mainLayout->addLayout(m_compLayout);
}

ConditionNodeWidget::ConditionNodeWidget(const NodeIndex & index, QWidget * parent, int tp)
    : NegationNodeWidget (index,parent,tp) {
    m_colonneLabel = new QLabel(tr("Colonne :"));
    m_colonneCB = new QComboBox;
    m_colonneCB->setEditable(index.flags(FindModel::ColonneCible).testFlag(Qt::ItemIsEnabled));
    m_colonneCB->setInsertPolicy(QComboBox::NoInsert);
    auto vec = static_cast<const modelMPS::FindModel *>(index.model())->nomColonnes();
    for (szt i = 0; i != vec.size(); ++i)
        m_colonneCB->addItem(vec[i],i);
    // Calque
    m_colonneLayout = new QVBoxLayout;
    m_colonneLayout->addWidget(m_colonneLabel);
    m_colonneLayout->addWidget(m_colonneCB);
    m_mainLayout->addLayout(m_colonneLayout);
}

DateNodeWidget::DateNodeWidget(const NodeIndex & index, QWidget * parent,int tp)
    : ComparaisonNodeWidget (index,parent,tp) {
    m_dateLabel = new QLabel(tr("Date :"));
    m_dateEdit = new QDateEdit;
    // Calque
    m_dateLayout = new QVBoxLayout;
    m_dateLayout->addWidget(m_dateLabel);
    m_dateLayout->addWidget(m_dateEdit);
    m_mainLayout->addLayout(m_dateLayout);
}

NegationNodeWidget::NegationNodeWidget(const NodeIndex & index, QWidget * parent, int tp)
    : FindNodeWidget (index,parent,tp) {
    m_nonCheckBox = new QCheckBox(tr("Négation"));
    m_nonCheckBox->setEnabled(index.flags(FindModel::NegCible).testFlag(Qt::ItemIsEnabled));
    // Calque
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->addWidget(m_nonCheckBox);
}

OperationNodeWidget::OperationNodeWidget(const NodeIndex & index, QWidget * parent, int tp)
    : NegationNodeWidget (index,parent,tp) {
    m_opLabel = new QLabel(tr("Opération :"));
    m_opCB = new QComboBox;
    m_opCB->setEditable(index.flags(FindModel::OpCible).testFlag(Qt::ItemIsEnabled));
    m_opCB->setInsertPolicy(QComboBox::NoInsert);
    for (szt i = 0; i != FindModel::NbrOperation; ++i)
        m_opCB->addItem(OperationNode::Strings[i],i);
    m_opLayout = new QVBoxLayout;
    m_opLayout->addWidget(m_opLabel);
    m_opLayout->addWidget(m_opCB);
    m_mainLayout->addLayout(m_opLayout);
}

TexteNodeWidget::TexteNodeWidget(const NodeIndex & index, QWidget * parent,int tp)
    : ConditionNodeWidget (index,parent,tp) {
    m_texteLabel = new QLabel(tr("Chercher :"));
    m_lineEdit = new QLineEdit;
    m_lineEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_caseCheck = new QCheckBox(tr("Sensible à la case"));
    m_regexCheck = new QCheckBox(tr("Expression régulière"));
    // Calque
    m_texteLayout = new QHBoxLayout;
    m_texteLayout->addWidget(m_texteLabel);
    m_texteLayout->addWidget(m_lineEdit);
    m_texteLayout->addWidget(m_caseCheck);
    m_texteLayout->addWidget(m_regexCheck);
    m_mainLayout->addLayout(m_texteLayout);
}

void TexteNodeWidget::updateData(){
    ConditionNodeWidget::updateData();
    m_lineEdit->setText(m_index.data(FindModel::TexteCible).toString());
    m_caseCheck->setChecked(m_index.data(FindModel::CaseCible).toBool());
    m_regexCheck->setChecked(m_index.data(FindModel::RegexCible).toBool());
}
