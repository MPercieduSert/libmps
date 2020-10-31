/*Auteur: PERCIE DU SERT Maxime
 *Date: 29/04/2019
 */
#ifndef ABSTRACTCOLONNESMODEL_H
#define ABSTRACTCOLONNESMODEL_H

#include <memory>
#include <QAbstractItemModel>
#include <QBrush>
#include "FindModel.h"
#include "Bdd.h"
#include "ConteneurPtr.h"
#include "Entity.h"
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
 * \brief Classe mère des models de colonnes homogènes.
 */
class AbstractColonnesModel : public AbstractModel {
    Q_OBJECT
public:
    //! Type des colonnes du modèle.
    enum colonneType {NoType = -1,
                      BoolColonne = findNodeModel::BoolNodeType,
                      ConstanteColonne = findNodeModel::ConstanteNodeType,
                      DateColonne = findNodeModel::DateNodeType,
                      DateTimeColonne = findNodeModel::DateTimeNodeType,
                      DoubleColonne = findNodeModel::DoubleNodeType,
                      IntColonne = findNodeModel::IntNodeType,
                      TexteColonne = findNodeModel::TexteNodeType,
                      UIntColonne = findNodeModel::UIntNodeType};
                      //NbrTypeColonne};

    //! Role des donnés du model.
    enum itemDataRole {IdRole = 0x0111};

    //! État des lignes.auto & e1 =static_cast<const Eleve &>(m_data.front()[row1]);
    enum etat {Sauver,
               Valide,
               Invalide,
               Double,
               DoubleSauver,
               NbrEtat};

    //! Identifiant de fonctions statistiques à appliquer à une colonne.
    enum fonctionStatistique {Max,
                              Min,
                              Etendue,
                              Moyenne,
                              Mediane,
                              Q1,Q3,
                              D1,D9};

    //////////////////////////////// AbstractColonne //////////////////////
    //! Classe abstraite d'information et d'intéraction avec les colonnes.
    class AbstractColonne {
    protected:
        Qt::ItemFlags m_flags;      //!< Drapeaux associés à la colonne.
        QString m_header;           //!< Entête de la colonne.
        const int m_type;           //!< Type de la colonne.
    public:
        //! Constructeur.
        AbstractColonne(const QString & name, Qt::ItemFlags flags, int type = NoType)
            : m_flags(flags), m_header(name), m_type(type) {}

        //! Destructeur.
        virtual ~AbstractColonne();

        //! Compare entre deux lignes ( ligne1 < ligne2).
        virtual bool compare(szt ligne1, szt ligne2) const
            {return data(ligne1,Qt::DisplayRole) < data(ligne2,Qt::DisplayRole);}

        //! Accesseur de la donnée d'indice id dans la colonne.
        virtual QVariant data(szt /*id*/, int /*role*/ = Qt::DisplayRole) const {return QVariant();}

        //! Accesseut de la donnée pour le test de recherche.
        virtual QVariant dataTest(szt /*id*/) const {return QVariant();}

        //! Accesseur des drapeaux associés à la donnée.
        virtual Qt::ItemFlags flags(szt /*id*/) const {return m_flags;}

        //! Accesseur de l'entête de la colonne.
        const QString & header() const noexcept {return m_header;}

        //! Mutateur de la donnée d'indice id dans la colonne.
        virtual bool setData(szt /*id*/, const QVariant & /*value*/, int /*role*/ = Qt::EditRole) {return true;}

        //! Accesseur drapeaux associés à la colonne.
        void setFlags(const Qt::ItemFlags & flags)
            {m_flags = flags;}

        //! Mutateur de l'entête de la colonne.
        void setHeader(const QString & name) {m_header = name;}

        //! Acceseur du type de la colonne.
        int type() const {return m_type;}
    };

    //! Ensemble des colonnes.
    using Colonnes = std::vector<std::unique_ptr<AbstractColonne>>;

    //! Information de création d'une nouvelle colonne.
    struct NewColonneInfo {
        Qt::ItemFlags flags;
        int id = 0;
        QString name = QString();
        int type = 0;
        szt tableau = 0;
        std::vector<QVariant> args = std::vector<QVariant>();
    };

    /////////////////////////////////////// AbstractTableau ///////////////////////////////////
    //! Classe Abstraite contenant les donnée du model.
    class AbstractTableau {
    public:
        //! Constructeur.
        AbstractTableau() = default;

        //!Destructeur.
        virtual ~AbstractTableau() = default;

        //! Ajoute count lignes au tableau.
        virtual void add(szt /*count*/) {}

        //! Efface les données du tableau.
        virtual void clear() = 0;

        //! Comparaison d'égalité de deux ligne.
        virtual bool egal(szt /*ligne1*/, szt /*ligne2*/) const {return false;}

        //! Supprime une ligne des données du model.
        virtual void erase(szt /*ligne*/) {}

        //! Supprime les lignes [first,last) des données du model.
        virtual void erase(szt /*first*/, szt /*last*/);

        //! Teste si la ligne correspond à une donnée interne.
        virtual bool existsInternalData(szt /*ligne*/) const {return false;}

        //! Hydrate la donnée d'une ligne du tableau et retourne le succés de l'opération.
        virtual void hydrate(szt /*ligne*/) {}

        //! Fabrique une colonne lié aux données du tableau.
        virtual std::unique_ptr<AbstractColonne> makeColonne(const NewColonneInfo & info) = 0;

        //! Teste si la ligne est nouvelle ou modifiée.
        virtual bool newOrModif(szt /*ligne*/) const {return true;}

        //! Supprime les données correspondant à la ligne dans la base de donnée.
        //! Ne doit pas supprimer de ligne de donnée du model.
        virtual bool removeInternalData(szt /*ligne*/)  {return true;}

        //! Sauve la ligne dans la base de donnée.
        virtual void save(szt /*ligne*/) {}

        //! Taille (nombre de lignes).
        virtual szt size() const = 0;

        //! Teste si la ligne de donnée est valide.
        virtual bool valide(szt /*ligne*/) const {return true;}
    };

protected:
    int m_colonneSorted = -1;       //! Colonne trié par défaut si positive.
    Qt::SortOrder m_orderSort = Qt::AscendingOrder;
    // Colonnes
    Colonnes m_colonnes;     //!< Contient les informations d'une colonne.
    // ByPass
    bool m_uniqueLigne;                //!< Court-circuite l'unicité si false.
    bool m_valideLigne;                //!< Court-circuite la validité si false.

    // Etats des lignes
    std::vector<szt> m_etats;           //!< Vecteur des états de chaque ligne.
    std::vector<QBrush> m_brush;        //!< Correspondance états <-> QBrush.
    std::map<szt,std::list<szt>> m_doubles;     //! Liste des identifiants des doubles.

    // Donnée
    std::unique_ptr<AbstractTableau> m_data;    //! Donnée du model.

public:
    //! Constructeur.
    explicit AbstractColonnesModel(bool uniqueLigne = true, bool valideLigne = true, QObject * parent = nullptr);

    //! Destructeur par defaut.
    ~AbstractColonnesModel() override = default;

    //! Accesseur du vecteur des aspect des états.
    const std::vector<QBrush> & brushEtat() const noexcept
        {return m_brush;}

    //! Efface les données du model.
    void clear() {
        beginResetModel();
            m_data->clear();
            m_etats.clear();
            resetRowToLigne();
        endResetModel();
    }

    //! Accesseur de la colonne en position pos.
    const AbstractColonne & colonne(szt pos) const
        {return *m_colonnes.at(pos);}

    //! Accesseur de la colonne en position pos.
    AbstractColonne & colonne(szt pos)
        {return *m_colonnes.at(pos);}

    //! Accesseur de la colonne trié lors de réinitialisation (-1 pour aucune).
    int colonneSorted() const
        {return m_colonneSorted;}

    //! Renvoie le nombre de colonnes.
    int columnCount(const QModelIndex & /*parent*/ = QModelIndex()) const override {return static_cast<int>(m_colonnes.size());}

    //! Renvoie la donnée d'index et de role spécifié.
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //! Recherche les lignes de données vérifiant les conditions d'un modéle de recherche donné.
    virtual void find(FindModel * findModel) = 0;

    //! Renvoie les drapeaux associés à un index.
    Qt::ItemFlags flags(const QModelIndex & index) const override;

    //! Renvoie le label des lignes et des colonnes.
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    //! Hydrate la row.
    void hydrate(int row, const QModelIndex & parent = QModelIndex());

    //! Insert une nouvelle colonne à la postion pos.
    virtual bool insertColonne(int pos, const NewColonneInfo & info, bool allParent = false);

    //! Ajoute une nouvelle colonne en dernière position.
    virtual bool push_backColonne(const NewColonneInfo & info, bool allParent = false);

    //! Réinitialise l'affichage des lignes du model (pas les donnée).
    virtual void resetRow() {
        beginResetModel();
            resetRowToLigne();
        endResetModel();
    }

    //! Mutateur du vecteur des QBrush des états.
    void setBrushEtat(const std::vector<QBrush> & vecBrush)
        {m_brush = vecBrush;}

    //! Modifie la donnée d'index et de role spécifié.
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;

    //! Mutateur de la colonne trié lors de réinitialisation (-1 pour aucune).
    void setColonneSorted(int col, Qt::SortOrder order = Qt::AscendingOrder) {
        m_colonneSorted = col;
        m_orderSort = order;
    }

    //! Mutateur du tableau de donnée.
    void setTableau(std::unique_ptr<AbstractTableau> && tableau);

    //! Fonctions statistiques appliquées à une colonnes.
    QVariant statistique(int pos, szt fonction, bool lignesVisibles = true) const;

signals:
    //! Signals du changement d'états d'une ligne.
    void etatLigneChanged(szt ligne);

    //! Signals du changement d'états de groupe de row.
    void etatRowsChanged(const QModelIndex & parent, int first, int last);

    //! Signals du changement d'états de toute les lignes.
    void etatsChanged();

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
    virtual bool removeRowsSelected(const QModelIndexList &/*selection*/) {return false;}

    //! Enregistre les données du model dans la base de donnée.
    virtual void save();

    //! Mets à jour l'état de toutes les lignes.
    virtual void updateAllEtats();

    //! Mets à jour l'état d'une ligne.
    virtual void updateEtats(int first, int last, const QModelIndex & parent = QModelIndex());
protected:
    //! Accesseur de la column.
    const AbstractColonne & colonne(const QModelIndex & index) const
        {return *m_colonnes[static_cast<szt>(index.column())];}

    //! Accesseur de la column.
    AbstractColonne & colonne(const QModelIndex & index)
        {return *m_colonnes[static_cast<szt>(index.column())];}

    //! Fait la correspondance entre l'index et la ligne de donnée.
    virtual szt ligne(const QModelIndex & index) const = 0;

    //! Nombre de ligne de données en szt.
    szt nbrLignes() const {return m_data->size();}

    //! Nombre de ligne afficher en szt.
    virtual szt nbrRow() const = 0;

    //! Met le vecteur des correspondance de ligne à l'identitée.
    virtual void resetRowToLigne() = 0;

    //! Fait la correspondance entre la ligne d'index et la ligne de donnée.
    virtual szt rowToLigne(int row, const QModelIndex & parent = QModelIndex()) const = 0;

    //! Renvoie un vecteur contenant l'ensemble des identifiants des lignes visibles ou de toutes les lignes.
    virtual std::vector<szt> statOnLignes(bool /*lignesVisibles*/ = true) const {return std::vector<szt>();}

    //! Met à jour la validité d'une ligne.
    void updateValideLigne(szt ligne);

    //! Met à jour l'unicité d'une ligne.
    void updateUniqueLigne(szt ligne, bool update = true);

protected slots:
    //! Mets à jour la tailles du vecteur d'états.
    void resizeEtats()
        {m_etats.resize(nbrLignes(),Sauver);}
};
}//end modelmps
#endif // ABSTRACTCOLONNESMODEL_H
