/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/04/2019
 */
#ifndef TABLE_MODEL_H
#define TABLE_MODEL_H

#include <QApplication>
#include <QClipboard>
#include <QMessageBox>
#include <QPushButton>
#include "abstract_colonnes_model.h"

namespace mps {
namespace model_base {
/*! \ingroup groupe_model
 *\brief Classe mère des model de type tableau.
 */
class table_model : public abstract_colonnes_model {
    Q_OBJECT
protected:
    enum dialog_selection{No_Dialog,
                         Annuler,
                         Insert_In,
                         Insert_Out};
public:
    /*! \ingroup groupe_model
     *\brief Classe d'information sur la selection d'un tableau.
     */
    class selected_range {
    protected:
        bool m_corners_found;    //!< Vraie si les bornes ont été cherchées.
        int m_bottom;           //!< Indice de ligne de la selection plus basse.
        std::vector<int> m_column_list;   //!< Liste des colonnes selectionnées dans la première ligne séléctionné, vide si la séléction est rectangulaire.
        bool m_is_rect;          //!< Vraie si la selection est un rectangle.
        bool m_is_valid;         //!< Vraie si la selection est valide c'est-à-dire si chaque ligne séléctionnée posséde les mêmes colonnes séléctionner.
        int m_left;             //!< Indice de la colonne la plus à gauche.
        int m_right;            //!< Indice de la colonne la plus à droite.
        std::vector<int> m_row_list;    //!< Liste des lignes selectionnées (même partiellement), vide si la séléction est rectangulaire.
        QModelIndexList m_selection; //! Sélection.
        int m_top;              //!< Indice de la ligne la plus haute.
        bool m_validity_set;     //!< Vraie si la validité à été testée.


    public:
        //! Constructeur.
        selected_range(const QModelIndexList &range);

        //! Accesseur de bottom.
        int bottom() {
            find_corners();
            return m_bottom;}

        //! Accesseur de la i-ème colonne de la sélection si séléction est valide.
        int column(int i)
            {return columns().at(static_cast<szt>(i));}

        //! Renvoie une liste de paire début et nombre de colonnes par bloc.
        std::list<std::pair<int,int>> column_bloc();

        //! Renvoie le nombre de colonnes de la selection.
        int column_count()
            {return m_is_rect ? m_right - m_left +1 : static_cast<int>(columns().size());}

        //! Renvoie les colonnes séléctionnées dans la première ligne de la séléction.
        const std::vector<int> &columns();

        //! Test si la selection est un rectangle.
        bool is_rect() {
            find_corners();
            return m_is_rect;}

        //! Test si la selection est valide.
        bool is_valid() {
            set_validity();
            return m_is_valid;}

        //! Accesseur de left.
        int left() {
            find_corners();
            return m_left;}

        //! Accesseur de right.
        int right() {
            find_corners();
            return m_right;}

        //! Accesseur de la i-ème ligne de la sélection.
        int row(int i)
            {return rows().at(static_cast<szt>(i));}

        //! Renvoie une liste de paire début et nombre de lignes par bloc.
        std::list<std::pair<int,int>> row_bloc();

        //! Renvoie le nombre de lignes de la selection.
        int row_count() {return m_is_rect ? m_bottom - m_top +1: static_cast<int>(rows().size());}

        //! Renvoie l'ensemble des lignes contenant au moins une case séléctionnée.
        const std::vector<int> &rows();

        //! Accesseur de tom.
        int top() {
            find_corners();
            return m_top;}

    protected:
        //! Recherche les coins du plus petit rectangle contenant la selection.
        void find_corners();

        //! Recherche les coins du plus petit rectangle contenant la selection.
        void set_validity();
    };
protected:
    std::vector<szt> m_row_to_ligne;       //!< Correspondances de index.row <-> lignes.
public:
    //! Constructeur.
    explicit table_model(bool unique_ligne = true, bool valide_ligne = true, QObject *parent = nullptr);

    //! Destructeur par defaut.
    ~table_model() override = default;

    //! Recherche les lignes de données vérifiant les conditions d'un modéle de recherche donné.
    void find(Abstract_Find_Model *find_model) override;

    //! Drapeaux d'un item
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    //! Pas d'enfants sauf pour la racine.
    bool hasChildren(const QModelIndex &parent) const override;

    //! Renvoie le label des lignes et des colonnes.
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    //! Renvoie l'index correxpondant à la ligne et colonne de parent.
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    //! Insert count lignes dans le model juste avant la ligne row.
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    //! Renvoie l'index du parent.
    QModelIndex parent(const QModelIndex &/*index*/) const override
        {return QModelIndex();}

    //! Supprime count lignes en commençant par la ligne row.
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    //! Supprime une selection de lignes entières.
    bool remove_rows_selected(const QModelIndexList &selection) override;

    //! Renvoie le nombre de lignes.
    int rowCount(const QModelIndex &parent = QModelIndex()) const override
        {return parent.isValid() ? 0 : static_cast<int>(m_row_to_ligne.size());}

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
    void update_all_etats() override;

    //! Mets à jour l'état d'une ligne.
    void update_etats(int first, int last, const QModelIndex &parent = QModelIndex()) override;

protected:
    //! Ouvre une fenêtre de dialogue pour demander si l'insertion doit être faite à l'intérieur de la selection ou non.
    dialog_selection dialog_coller() const;

    //! Fait la correspondance entre l'index et la ligne de donnée.
    szt ligne(const QModelIndex &index) const override
        {return row_to_ligne(index.row());}

    //! Fait la correspondance entre la ligne de donnée et la ligne d'index.
    int ligne_to_row(szt ligne) const;

    //! Nombre de ligne afficher en szt.
    szt nbr_row() const override
        {return static_cast<szt>(m_row_to_ligne.size());}

    //! Met le vecteur des correspondance de ligne à l'identitée.
    void reset_row_to_ligne() override;

    //! Fait la correspondance entre la ligne d'index et la ligne de donnée.
    szt row_to_ligne(int row, const QModelIndex &/*parent*/ = QModelIndex()) const override
        {return m_row_to_ligne[static_cast<szt>(row)];}

    //! Renvoie un vecteur contenant l'ensemble des identifiants des lignes visibles ou de toutes les lignes.
    std::vector<szt> stat_on_lignes(bool lignes_visibles = true) const override;
};
}}
#endif // TABLE_MODEL_H
