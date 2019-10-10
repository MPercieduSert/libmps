/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/04/2019
 */
#ifndef MOTCLEDELEGATE_H
#define MOTCLEDELEGATE_H

#include <QComboBox>
#include <QContextMenuEvent>
#include <QEvent>
#include <QMenu>
#include <QPainter>
#include <QStyledItemDelegate>
#include "TreeModelMotCle.h"


/*! \ingroup groupeDelegate
 * \brief Délégué pour le model TreeModelMotCle.
 */
class MotCleDelegate: public QStyledItemDelegate
{
    Q_OBJECT
protected:
    const int m_nbrEntity;            //!< Nombre d'entités associé aux mot-clés.

public:
    //! Constructeur.
    MotCleDelegate(int nbrEntity, QObject * parent = nullptr)
        : QStyledItemDelegate(parent), m_nbrEntity(nbrEntity) {}

    //! Destructeur par defaut.
    ~MotCleDelegate() override = default;
    //! Creer un item pour édité une donnée.
    QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const override;
    //! Affiche la donnée.
    /*void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;*/
    //! Modifie l'éditeur avec la donnée du modèle.
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    //! Modifie la donnée dans le modèle.
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    //! Gestionnaire des événement;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
protected:
    //! Affiche un menu contextuel
    void showContextMenu(QAbstractItemModel *model, const QModelIndex &index, const QPoint& globalPos);
};

#endif // MOTCLEDELEGATE_H
