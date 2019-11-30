#include "MotCleDelegate.h"

using namespace modelMPS;

QWidget * MotCleDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    if(TreeModelMotCle::NbrColumnMotCle <= index.column() && index.column() < TreeModelMotCle::NbrColumnMotCle + m_nbrEntity)
    {
        QComboBox * editor = new QComboBox(parent);
        editor->addItem("Interdit",bmps::motClePermissionNum::InterditMCNum);
        editor->addItem("Permis",bmps::motClePermissionNum::PermisMCNum);
        editor->setAutoFillBackground(true);
        return editor;
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

bool MotCleDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent * mouseEvent = static_cast<QMouseEvent *>(event);
        if(mouseEvent->button() == Qt::RightButton)
        {
            showContextMenu(model,index,mouseEvent->globalPos());
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event,model,option,index);
}

/*void MotCleDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(TreeModelMotCle::NbrColumnMotCle <= index.column() && index.column() < TreeModelMotCle::NbrColumnMotCle + TreeModelMotCle::NbrIndex)
    {
        painter->save();
        switch (index.data().toInt()) {
        case bmps::motClePermissionNum::InterditMCNum:
            painter->fillRect(option.rect, QColor(Qt::red));
            break;
        case bmps::motClePermissionNum::PermisMCNum:
            painter->fillRect(option.rect, QColor(Qt::green));
        default:
            break;
        }
        painter->restore();
    }
    else
        QStyledItemDelegate::paint(painter,option,index);
}*/

void MotCleDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    /*if(index.column() == TreeModelMotCle::NomMotCleIndex index
            && static_cast<TreeItem<MotCle> *>(index.internalPointer())->root()->data().id() == m_progRootId)*/

    if(!index.isValid())
    {
        if(TreeModelMotCle::NbrColumnMotCle <= index.column() && index.column() < TreeModelMotCle::NbrColumnMotCle + m_nbrEntity)
        {
            QComboBox * comboBox = static_cast<QComboBox *>(editor);
            comboBox->setCurrentIndex(comboBox->findData(index.data(Qt::EditRole)));
        }
        else
            QStyledItemDelegate::setEditorData(editor,index);
    }
}

void MotCleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(TreeModelMotCle::NbrColumnMotCle <= index.column() && index.column() < TreeModelMotCle::NbrColumnMotCle + m_nbrEntity)
        model->setData(index,static_cast<QComboBox *>(editor)->currentData());
    else
        QStyledItemDelegate::setModelData(editor,model,index);
}

void MotCleDelegate::showContextMenu(QAbstractItemModel *model, const QModelIndex &index, const QPoint& globalPos)
{
    QMenu menu;
    QAction * addSon = menu.addAction(tr("Ajouter un fils"));
    QAction * addBro = menu.addAction(tr("Ajouter un frère"));

    connect(addSon,&QAction::triggered,[model,&index]{static_cast<TreeModelMotCle *>(model)->insertRow(0,index);});
    connect(addBro,&QAction::triggered,[model,&index]{static_cast<TreeModelMotCle *>(model)->insertRow(index.row()+1,index.parent());});
    if(static_cast<TreeModelMotCle *>(model)->autorisation(index,bmps::autorisation::Suppr))
    {
        menu.addSeparator();
        QAction * delMotCle = menu.addAction(QString(tr("Supprimer le mot clé "))
                                             .append(model->data(model->index(index.row(),TreeModelMotCle::NomMotCleIndex,index.parent()))
                                                     .toString()));
        connect(delMotCle,&QAction::triggered,[model,&index]{static_cast<TreeModelMotCle *>(model)->removeRow(index.row()
                                                                                                              ,index.parent());});
    }
    menu.exec(globalPos);
}
