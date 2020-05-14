/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/04/2019
 */
#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QApplication>
#include <QClipboard>
#include <QMessageBox>
#include <QPushButton>
#include "AbstractColonnesModel.h"
#include "typemps.h"

namespace modelMPS {
using namespace typeMPS;
/*! \ingroup groupeModel
 * \brief Classe mère des model de type tableau.
 */
class TableModel : public AbstractColonnesModel {
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
protected:
    std::vector<szt> m_rowToLigne;       //!< Correspondances de index.row <-> lignes.
public:
    //! Constructeur.
    explicit TableModel(bool uniqueLigne = true, bool valideLigne = true, QObject * parent = nullptr);

    //! Destructeur par defaut.
    ~TableModel() override = default;

    //! Recherche les lignes de données vérifiant les conditions d'un modéle de recherche donné.
    void find(AbstractFindModel * findModel) override;

    //! Drapeaux d'un item
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    //! Pas d'enfants sauf pour la racine.
    bool hasChildren(const QModelIndex &parent) const override;

    //! Renvoie le label des lignes et des colonnes.
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    //! Renvoie l'index correxpondant à la ligne et colonne de parent.
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    //! Insert count lignes dans le model juste avant la ligne row.
    bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex()) override;

    //! Renvoie l'index du parent.
    QModelIndex parent(const QModelIndex &/*index*/) const override
        {return QModelIndex();}

    //! Supprime count lignes en commençant par la ligne row.
    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex()) override;

    //! Supprime une selection de lignes entières.
    bool removeRowsSelected(const QModelIndexList &selection) override;

    //! Renvoie le nombre de lignes.
    int rowCount(const QModelIndex & parent = QModelIndex()) const override
        {return parent.isValid() ? 0 : static_cast<int>(m_rowToLigne.size());}

    QModelIndex sibling(int row, int column, const QModelIndex &) const override
        {return index(row, column);}

    //! Tri le model par colonne.
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

public slots:
    //! Effectue l'action coller.
    void coller(const QModelIndexList &selection) override;

    //! Effectue l'action coller.
    void copier(const QModelIndexList &selection) override;

    //! Effectue l'action coller.
    void couper(const QModelIndexList &selection) override;

    //! Effectue l'action coller.
    void effacer(const QModelIndexList &selection) override;

    //! Enregistre les données du model dans la base de donnée.
    void save() override;

    //! Mets à jour l'état de toutes les lignes.
    void updateAllEtats() override;

    //! Mets à jour l'état d'une ligne.
    void updateEtats(int first, int last, const QModelIndex & parent = QModelIndex()) override;

protected:
    //! Ouvre une fenêtre de dialogue pour demander si l'insertion doit être faite à l'intérieur de la selection ou non.
    DialogSelection dialogColler() const;

    //! Fait la correspondance entre l'index et la ligne de donnée.
    szt ligne(const QModelIndex & index) const override
        {return rowToLigne(index.row());}

    //! Fait la correspondance entre la ligne de donnée et la ligne d'index.
    int ligneToRow(szt ligne) const;

    //! Nombre de ligne afficher en szt.
    szt nbrRow() const override
        {return static_cast<szt>(m_rowToLigne.size());}

    //! Met le vecteur des correspondance de ligne à l'identitée.
    void resetRowToLigne() override;

    //! Fait la correspondance entre la ligne d'index et la ligne de donnée.
    szt rowToLigne(int row) const
        {return m_rowToLigne[static_cast<szt>(row)];}

    //! Renvoie un vecteur contenant l'ensemble des identifiants des lignes visibles ou de toutes les lignes.
    std::vector<szt> statOnLignes(bool lignesVisibles = true) const override;
};
}
#endif // TABLEMODEL_H
