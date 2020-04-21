/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/04/2019
 */
#ifndef MABSTRACTTABLEMODEL_H
#define MABSTRACTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QApplication>
#include <QClipboard>
//#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidgetSelectionRange>
//#include <QVBoxLayout>
//#include <stdlib.h>
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
        bool m_cornersFound;    //!< Vraie si les bornes ont été cherchées.
        int m_bottom;           //!< Indice de ligne de la selection plus basse.
        std::vector<int> m_columnList;   //!< Liste des colonnes selectionnées dans la première ligne séléctionné, vide si la séléction est rectangulaire.
        bool m_isRect;          //!< Vraie si la selection est un rectangle.
        bool m_isValid;         //!< Vraie si la selection est valide c'est-à-dire si chaque ligne séléctionnée posséde les mêmes colonnes séléctionner.
        int m_left;             //!< Indice de la colonne la plus à gauche.
        int m_right;            //!< Indice de la colonne la plus à droite.
        std::vector<int> m_rowList;    //!< Liste des lignes selectionnées (même partiellement), vide si la séléction est rectangulaire.
        QModelIndexList m_selection; //! Sélection.
        int m_top;              //!< Indice de la ligne la plus haute.
        bool m_validitySet;     //!< Vraie si la validité à été testée.


    public:
        //! Constructeur.
        SelectedRange(const QModelIndexList &range);

        //! Accesseur de bottom.
        int bottom() {
            findCorners();
            return m_bottom;}

        //! Accesseur de la i-ème colonne de la sélection si séléction est valide.
        int column(int i)
            {return columns().at(static_cast<szt>(i));}

        //! Renvoie une liste de paire début et nombre de colonnes par bloc.
        std::list<std::pair<int,int>> columnBloc();

        //! Renvoie le nombre de colonnes de la selection.
        int columnCount() {return m_isRect ? m_right - m_left +1 : static_cast<int>(columns().size());}

        //! Renvoie les colonnes séléctionnées dans la première ligne de la séléction.
        const std::vector<int> & columns();

        //! Test si la selection est un rectangle.
        bool isRect() {
            findCorners();
            return m_isRect;}

        //! Test si la selection est valide.
        bool isValid() {
            setValidity();
            return m_isValid;}

        //! Accesseur de left.
        int left() {
            findCorners();
            return m_left;}

        //! Accesseur de right.
        int right() {
            findCorners();
            return m_right;}

        //! Accesseur de la i-ème ligne de la sélection.
        int row(int i)
            {return rows().at(static_cast<szt>(i));}

        //! Renvoie une liste de paire début et nombre de lignes par bloc.
        std::list<std::pair<int,int>> rowBloc();

        //! Renvoie le nombre de lignes de la selection.
        int rowCount() {return m_isRect ? m_bottom - m_top +1: static_cast<int>(rows().size());}

        //! Renvoie l'ensemble des lignes contenant au moins une case séléctionnée.
        const std::vector<int> & rows();

        //! Accesseur de tom.
        int top() {
            findCorners();
            return m_top;}

    protected:
        //! Recherche les coins du plus petit rectangle contenant la selection.
        void findCorners();

        //! Recherche les coins du plus petit rectangle contenant la selection.
        void setValidity();
    };

    //! Constructeur.
    explicit AbstractTableModel(QObject * parent = nullptr) : QAbstractTableModel(parent) {}

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
    virtual void save() {}

protected:
    //! Ouvre une fenêtre de dialogue pour demander si l'insertion doit être faite à l'intérieur de la selection ou non.
    DialogSelection dialogColler() const;

signals:

public slots:
};
}
#endif // MABSTRACTTABLEMODEL_H
