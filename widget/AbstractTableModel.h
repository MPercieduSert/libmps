/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/04/2019
 */
#ifndef MABSTRACTTABLEMODEL_H
#define MABSTRACTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QApplication>
#include <QClipboard>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidgetSelectionRange>
#include <QVBoxLayout>
#include <stdlib.h>
#include "typemps.h"

/*! \defgroup groupeModel Model
 * \brief Ensemble de classes représentant les models.
 */

/*! \ingroup groupeModel
 * \brief Espace de noms des model.
 */
namespace modelMPS {
using namespace typeMPS;
/*! \ingroup groupeModel
 * \brief Classe mère des model de type tabelau.
 */
class AbstractTableModel : public QAbstractTableModel {
    Q_OBJECT
protected:
    enum DialogSelection{NoDialog,
                         Annuler,
                         InsertIn,
                         InsertOut};
public:
    /*! \ingroup groupeModel
     * \brief Classe d'information sur la selection d'un tableau.
     */
    class SelectedRange {
    protected:
        szt m_bottom;           //!< Indice de ligne de la selection plus basse.
        std::vector<szt> m_columnList;   //!< Liste des colonnes selectionnées.
        bool m_isRect;          //!< Vraie si la selection est un rectangle.
        bool m_isValid;         //!< Vraie si la selection est valide c'est-à-dire si chaque ligne séléctionnée posséde les mêmes colonnes séléctionner.
        szt m_left;             //!< Indice de la colonne la plus à gauche.
        szt m_right;            //!< Indice de la colonne la plus à droite.
        std::vector<szt> m_rowList;    //!< Liste des lignes selectionnées
        QModelIndexList m_selection; //! Sélection.
        szt m_top;              //!< Indice de la ligne la plus haute.

    public:
        //! Constructeur.
        SelectedRange(const QModelIndexList &range);

        //! Accesseur de bottom.
        szt bottom() const  {return m_bottom;}

        //! Test si la selection est un rectangle.
        bool isRect() const {return m_isRect;}

        //! Test si la selection est valide.
        bool isValid() const {return m_isValid;}

        //! Accesseur de left.
        szt left() const    {return m_left;}

        //! Renvoie les colonnes séléctionnées dans la première ligne de la séléction.
        const std::vector<szt> & columns() const {return m_columnList;}

        //! Renvoie le nombre de colonnes de la selection.
        szt columnCount() const {return m_isRect ? m_right - m_left +1 : m_columnList.size();}

        //! Renvoie le nombre de lignes de la selection.
        szt rowCount() const {return m_isRect ? m_bottom - m_top +1: m_rowList.size();}

        //! Accesseur de right.
        szt right() const   {return m_right;}

        //! Renvoie l'ensemble des lignes contenant au moins une case séléctionnée.
        const std::vector<szt> & rows() const {return m_rowList;}

        //! Accesseur de tom.
        szt top() const     {return m_top;}
    };

    //! Constructeur.
    explicit AbstractTableModel(QObject * /*parent*/ = nullptr) {}

    //! Destructeur par defaut.
    ~AbstractTableModel() override = default;

    //! Effectue l'action coller.
    virtual void coller(const QModelIndexList &selection);

    //! Effectue l'action coller.
    virtual void copier(const QModelIndexList &selection);

    //! Effectue l'action coller.
    virtual void couper(const QModelIndexList &selection);

    //! Effectue l'action coller.
    virtual void effacer(const QModelIndexList &selection);

    //! Enregistre les données du model dans la base de donnée.
    virtual void save() = 0;

protected:
    //! Ouvre une fenêtre de dialogue pour demander si l'insertion doit être faite à l'intérieur de la selection ou non.
    DialogSelection dialogColler() const;

signals:

public slots:
};
}
#endif // MABSTRACTTABLEMODEL_H
