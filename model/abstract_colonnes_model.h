/*Auteur: PERCIE DU SERT Maxime
 *Date: 29/04/2019
 */
#ifndef ABSTRACTCOLONNESMODEL_H
#define ABSTRACTCOLONNESMODEL_H

#include <memory>
#include <QAbstractItemModel>
#include <QBrush>
#include "abstract_model.h"
#include "bdd.h"

/*! \defgroup groupe_model Model
 *\brief Ensemble de classes représentant les models.
 */

namespace mps {
/*! \ingroup groupe_model
 *\brief Espace de noms des model.
 */
namespace model_base {
//! Type des colonnes du modèle.
enum colonne_type {No_Type = -1,
                  Bool_Colonne_Type,
                  Constante_Colonne_Type,
                  Date_Colonne_Type,
                  Date_Time_Colonne_Type,
                  Double_Colonne_Type,
                  Int_Colonne_Type,
                  Texte_Colonne_Type,
                  UInt_Colonne_Type};

/*! \ingroup groupe_model
 *\brief Classe mère d'un modèle d'arbre de recherche.
 */
class Abstract_Find_Model {
public:
    //! Destructeur.
    virtual ~Abstract_Find_Model() = default;

    //! Teste si l'arbre est réduit à sa racine.
    virtual bool root_leaf() const = 0;

    //! Teste si la ligne d'indice id vérifie la condition de la racine.
    virtual bool test_root(szt id) const = 0;

    //! Teste si la ligne d'indice id vérifie l'arbre des conditions.
    virtual bool test_tree(szt id) const = 0;
};

/*! \ingroup groupe_model
 *\brief Classe mère des models de colonnes homogènes.
 */
class abstract_colonnes_model : public abstract_model {
    Q_OBJECT
public:
    //! Role des donnés du model.
    enum item_data_role {Id_Role = 0x0111};

    //! État des lignes.
    enum ligne_etat {Sauver,
               Valide,
               Invalide,
               Double,
               DoubleSauver,
               NbrEtat};

    //! Identifiant de fonctions statistiques à appliquer à une colonne.
    enum fonction_statistique {Max,
                              Min,
                              Etendue,
                              Moyenne,
                              Mediane,
                              Q1,Q3,
                              D1,D9};

    //////////////////////////////// abstract_colonne //////////////////////
    //! Classe abstraite d'information et d'intéraction avec les colonnes.
    class abstract_colonne {
    protected:
        Qt::ItemFlags m_flags;      //!< Drapeaux associés à la colonne.
        QString m_header;           //!< Entête de la colonne.
        const int m_type;           //!< Type de la colonne.
    public:
        //! Constructeur.
        abstract_colonne(const QString &name, Qt::ItemFlags flags, int type = No_Type)
            : m_flags(flags), m_header(name), m_type(type) {}

        //! Destructeur.
        virtual ~abstract_colonne();

        //! Compare entre deux lignes ( ligne_1 < ligne_2).
        virtual bool compare(szt ligne_1, szt ligne_2) const
            {return data(ligne_1,Qt::DisplayRole) < data(ligne_2,Qt::DisplayRole);}

        //! Accesseur de la donnée d'indice id dans la colonne.
        virtual QVariant data(szt /*id*/, int /*role*/ = Qt::DisplayRole) const {return QVariant();}

        //! Accesseut de la donnée pour le test de recherche.
        virtual QVariant data_test(szt /*id*/) const {return QVariant();}

        //! Accesseur des drapeaux associés à la donnée.
        virtual Qt::ItemFlags flags(szt /*id*/) const {return m_flags;}

        //! Accesseur de l'entête de la colonne.
        const QString &header() const noexcept {return m_header;}

        //! Mutateur de la donnée d'indice id dans la colonne.
        virtual bool set_data(szt /*id*/, const QVariant &/*value*/, int /*role*/ = Qt::EditRole) {return true;}

        //! Accesseur drapeaux associés à la colonne.
        void set_flags(const Qt::ItemFlags &flags)
            {m_flags = flags;}

        //! Mutateur de l'entête de la colonne.
        void set_header(const QString &name) {m_header = name;}

        //! Acceseur du type de la colonne.
        int type() const {return m_type;}
    };

    //! Ensemble des colonnes.
    using colonnes = std::vector<std::unique_ptr<abstract_colonne>>;

    //! Information de création d'une nouvelle colonne.
    struct new_colonne_info {
        Qt::ItemFlags flags;
        int id = 0;
        QString name = QString();
        int type = 0;
        numt tableau = 0;
        std::vector<QVariant> args = std::vector<QVariant>();
    };

    /////////////////////////////////////// abstract_tableau ///////////////////////////////////
    //! Classe Abstraite contenant les donnée du model.
    class abstract_tableau {
    public:
        //! Constructeur.
        abstract_tableau() = default;

        //!Destructeur.
        virtual ~abstract_tableau() = default;

        //! Ajoute count lignes au tableau.
        virtual void add(szt /*count*/) {}

        //! Efface les données du tableau.
        virtual void clear() = 0;

        //! Comparaison d'égalité de deux ligne.
        virtual bool egal(szt /*ligne_1*/, szt /*ligne_2*/) const {return false;}

        //! Supprime une ligne des données du model.
        virtual void erase(szt /*ligne*/) {}

        //! Supprime les lignes [first,last) des données du model.
        virtual void erase(szt /*first*/, szt /*last*/);

        //! Teste si la ligne correspond à une donnée interne.
        virtual bool exists_internal_data(szt /*ligne*/) const {return false;}

        //! Hydrate la donnée d'une ligne du tableau et retourne le succés de l'opération.
        virtual void hydrate(szt /*ligne*/) {}

        //! Fabrique une colonne lié aux données du tableau.
        virtual std::unique_ptr<abstract_colonne> make_colonne(const new_colonne_info &info) = 0;

        //! Teste si la ligne est nouvelle ou modifiée.
        virtual bool new_or_modif(szt /*ligne*/) const {return true;}

        //! Supprime les données correspondant à la ligne dans la base de donnée.
        //! Ne doit pas supprimer de ligne de donnée du model.
        virtual bool remove_internal_data(szt /*ligne*/)  {return true;}

        //! Sauve la ligne dans la base de donnée.
        virtual void save(szt /*ligne*/) {}

        //! Taille (nombre de lignes).
        virtual szt size() const = 0;

        //! Teste si la ligne de donnée est valide.
        virtual bool valide(szt /*ligne*/) const {return true;}
    };

protected:
    int m_colonne_sorted = -1;       //! Colonne trié par défaut si positive.
    Qt::SortOrder m_order_sort = Qt::AscendingOrder;
    // Colonnes
    colonnes m_colonnes;     //!< Contient les informations d'une colonne.
    // ByPass
    bool m_unique_ligne;                //!< Court-circuite l'unicité si false.
    bool m_valide_ligne;                //!< Court-circuite la validité si false.

    // Etats des lignes
    std::vector<enumt> m_etats;           //!< Vecteur des états de chaque ligne.
    std::vector<QBrush> m_brush;        //!< Correspondance états <-> QBrush.
    std::map<szt,std::list<szt>> m_doubles;     //! Liste des identifiants des doubles.

    // Donnée
    std::unique_ptr<abstract_tableau> m_data;    //! Donnée du model.

public:
    //! Constructeur.
    explicit abstract_colonnes_model(bool unique_ligne = true, bool valide_ligne = true, QObject *parent = nullptr);

    //! Destructeur par defaut.
    ~abstract_colonnes_model() override = default;

    //! Accesseur du vecteur des aspect des états.
    const std::vector<QBrush> &brush_etat() const noexcept
        {return m_brush;}

    //! Efface les données du model.
    void clear() {
        beginResetModel();
            m_data->clear();
            m_etats.clear();
            reset_row_to_ligne();
        endResetModel();
    }

    //! Accesseur de la colonne en position pos.
    const abstract_colonne &colonne(szt pos) const
        {return *m_colonnes.at(pos);}

    //! Accesseur de la colonne en position pos.
    abstract_colonne &colonne(szt pos)
        {return *m_colonnes.at(pos);}

    //! Accesseur de la colonne trié lors de réinitialisation (-1 pour aucune).
    int colonne_sorted() const
        {return m_colonne_sorted;}

    //! Renvoie le nombre de colonnes.
    int columnCount(const QModelIndex &/*parent*/ = QModelIndex()) const override {return static_cast<int>(m_colonnes.size());}

    //! Renvoie la donnée d'index et de role spécifié.
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //! Recherche les lignes de données vérifiant les conditions d'un modéle de recherche donné.
    virtual void find(Abstract_Find_Model *find_model) = 0;

    //! Renvoie les drapeaux associés à un index.
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    //! Renvoie le label des lignes et des colonnes.
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    //! Hydrate la row.
    void hydrate(int row, const QModelIndex &parent = QModelIndex());

    //! Insert une nouvelle colonne à la postion pos.
    virtual bool insert_colonne(int pos, const new_colonne_info &info, bool all_parent = false);

    //! Nombre de colonne.
    szt nbr_colonne() const noexcept
        {return m_colonnes.size();}

    //! Ajoute une nouvelle colonne en dernière position.
    virtual bool push_back_colonne(const new_colonne_info &info, bool all_parent = false);

    //! Réinitialise l'affichage des lignes du model (pas les donnée).
    virtual void reset_row() {
        beginResetModel();
            reset_row_to_ligne();
        endResetModel();
    }

    //! Mutateur du vecteur des QBrush des états.
    void set_brush_etat(const std::vector<QBrush> &vecBrush)
        {m_brush = vecBrush;}

    //! Modifie la donnée d'index et de role spécifié.
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    //! Mutateur de la colonne trié lors de réinitialisation (-1 pour aucune).
    void set_colonne_sorted(int col, Qt::SortOrder order = Qt::AscendingOrder) {
        m_colonne_sorted = col;
        m_order_sort = order;
    }

    //! Mutateur du tableau de donnée.
    void set_tableau(std::unique_ptr<abstract_tableau> &&tableau);

    //! Fonctions statistiques appliquées à une colonnes.
    QVariant statistique(int pos, enumt fonction, bool lignes_visibles = true) const;

signals:
    //! Signals du changement d'états d'une ligne.
    void etat_ligne_changed(szt ligne);

    //! Signals du changement d'états de groupe de row.
    void etat_rows_changed(const QModelIndex &parent, int first, int last);

    //! Signals du changement d'états de toute les lignes.
    void etats_changed();

public slots:
    //! Effectue l'action coller.
    virtual void coller(const QModelIndexList &/*selection*/) {}

    //! Effectue l'action coller.
    virtual void copier(const QModelIndexList &/*selection*/) {}

    //! Effectue l'action coller.
    virtual void couper(const QModelIndexList &/*selection*/) {}

    //! Effectue l'action coller.
    virtual void effacer(const QModelIndexList &/*selection*/) {}

    //! Supprime une selection de lignes entières.
    virtual bool remove_rows_selected(const QModelIndexList &/*selection*/) {return false;}

    //! Enregistre les données du model dans la base de donnée.
    virtual void save();

    //! Mets à jour l'état de toutes les lignes.
    virtual void update_all_etats();

    //! Mets à jour l'état d'une ligne.
    virtual void update_etats(int first, int last, const QModelIndex &parent = QModelIndex());
protected:
    //! Accesseur de la column.
    const abstract_colonne &colonne(const QModelIndex &index) const
        {return *m_colonnes[static_cast<szt>(index.column())];}

    //! Accesseur de la column.
    abstract_colonne &colonne(const QModelIndex &index)
        {return *m_colonnes[static_cast<szt>(index.column())];}

    //! Fait la correspondance entre l'index et la ligne de donnée.
    virtual szt ligne(const QModelIndex &index) const = 0;

    //! Nombre de ligne de données en szt.
    szt nbr_lignes() const {return m_data->size();}

    //! Nombre de ligne afficher en szt.
    virtual szt nbr_row() const = 0;

    //! Met le vecteur des correspondance de ligne à l'identitée.
    virtual void reset_row_to_ligne() = 0;

    //! Fait la correspondance entre la ligne d'index et la ligne de donnée.
    virtual szt row_to_ligne(int row, const QModelIndex &parent = QModelIndex()) const = 0;

    //! Renvoie un vecteur contenant l'ensemble des identifiants des lignes visibles ou de toutes les lignes.
    virtual std::vector<szt> stat_on_lignes(bool /*lignes_visibles*/ = true) const {return std::vector<szt>();}

    //! Met à jour la validité d'une ligne.
    void update_valide_ligne(szt ligne);

    //! Met à jour l'unicité d'une ligne.
    void update_unique_ligne(szt ligne, bool update = true);

protected slots:
    //! Mets à jour la tailles du vecteur d'états.
    void resize_etats()
        {m_etats.resize(nbr_lignes(),Sauver);}
};
}}
#endif // ABSTRACTCOLONNESMODEL_H
