/*Auteur: PERCIE DU SERT Maxime
 *Date: 10/04/2020
 */
#ifndef ABSTRACTTABLEENTITYMODEL_H
#define ABSTRACTTABLEENTITYMODEL_H

#include <memory>
#include "AbstractTableModel.h"
#include "Bdd.h"
#include "ConteneurPtr.h"
#include "Entity.h"


namespace modelMPS {
namespace cmps = conteneurMPS;
using Entity = entityMPS::Entity;
/*! \ingroup groupeModel
 * \brief Classe mère des model de type tabelau.
 */
class AbstractTableEntityModel : public AbstractTableModel {
    Q_OBJECT
protected:
    //! Type du tableau de d'entités.
    using Tableau = std::list<cmps::VectorPtr<Entity>>;

    //! Classe abstraite d'information et d'intéraction avec les colonnes.
    class AbstractColumn {
    protected:
        QString m_header;               //!< Entête de la colonne.
        const Tableau::iterator m_iterColData;          //!< Identitiant de la colonne de donnée.
    public:
        //! Constructeur.
        AbstractColumn(Tableau::iterator iterColData, const QString & name = QString())
            : m_header(name), m_iterColData(iterColData)  {}

        //! Destructeur.
        virtual ~AbstractColumn();

        //! Accesseur de la donnée d'indice id dans la colonne.
        virtual QVariant data(szt id, int role) const = 0;

        //! Renvoie les drapeaux associés à la donnée.
        virtual Qt::ItemFlags flags(szt id) const = 0;

        //! Accesseur de l'entête de la colonne.
        const QString & header() const noexcept {return m_header;}

        //! Mutateur de la donnée d'indice id dans la colonne.
        virtual bool setData(const QVariant & value, szt id, int role) = 0;

        //! Mutateur de l'entête de la colonne.
        void setHeader(const QString & name) {m_header = name;}

        //! Compare entre deux éléments (a < b).
        virtual bool compare(szt a, szt b) const
            {return data(a,Qt::DisplayRole) < data(b,Qt::DisplayRole);}
    };

    //! Classe template générique d'intéraction avec les colonnes.
    template<class Ent> class EntityColumn : public AbstractColumn {
    protected:
        Qt::ItemFlags m_flags;                            //! Drapeaux associés à la colonne.
        QVariant (*m_get)(const Ent &, int);              //! Fonction lisant la donnée du model.
        bool (*m_set)(const QVariant &, Ent &, int);      //! Fonction modifiant la donnée du model
    public:
        //! Constructeur.
        EntityColumn(const QString & name,
                     Tableau::iterator iterColData,
                     QVariant (*get)(const Ent &, int),
                     bool (*set)(const QVariant &, Ent &, int),
                     Qt::ItemFlags flags = Qt::ItemIsEnabled|Qt::ItemIsSelectable)
            : AbstractColumn(iterColData,name), m_flags(flags),m_get(get), m_set(set) {}

        //! Accesseur de la donnée d'indice id dans la colonne.
        QVariant data(szt id, int role) const override
            {return m_get(static_cast<const Ent &>((*m_iterColData)[id]), role);}

        //! Accesseur drapeaux associés à la colonne.
        Qt::ItemFlags flags(szt /*id*/) const override
            {return m_flags;}

        //! Mutateur de la donnée d'indice id dans la colonne.
        bool setData(const QVariant & value, szt id, int role) override
            {return m_set(value,static_cast<Ent &>((*m_iterColData)[id]),role);}

        //! Accesseur drapeaux associés à la colonne.
        void setFlags(const Qt::ItemFlags & flags)
            {m_flags = flags;}
    };

    bddMPS::Bdd & m_bdd;                                        //!< Lien avec la base donnée.
    Tableau m_data;                                             //!< Donnée du model.
    std::vector<std::unique_ptr<AbstractColumn>> m_columns;     //!< Contient les informations d'une colonne
    std::vector<szt> m_idRow;                                   //!< Contient les correspondances de ligne.
public:
    using Indice = std::pair<szt,szt>;

    //! Constructeur.
    AbstractTableEntityModel(bddMPS::Bdd & bdd, QObject * parent = nullptr)
        : AbstractTableModel(parent), m_bdd(bdd) {}

    //! Destructeur.
    ~AbstractTableEntityModel() override = default;

    //! Renvoie le nombre de colonnes.
    int columnCount() const {return static_cast<int>(m_columns.size());}

    //! Renvoie le nombre de colonnes.
    int columnCount(const QModelIndex & /*parent*/) const override {return columnCount();}

    //! Renvoie la donnée d'index et de role spécifié.
    QVariant data(const QModelIndex &index, int role) const override;

    //! Selectionne les lignes à afficher vérifiant une condition
    template<class UnaryPredicat> void find(UnaryPredicat predicat, bool reset = true);

    //! Renvoie les drapeaux associés à un index.
    Qt::ItemFlags flags(const QModelIndex & index) const override;

    //! Renvoie le label des lignes et des colonnes.
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    //! Teste la validité de l'index.
    bool indexValide(const QModelIndex & index) const
        {return index.isValid() && index.row() < rowCount() && index.column() < columnCount();}

    //! Insert une nouvelle colonne.
    virtual bool insertColumn(int column, std::unique_ptr<AbstractColumn> && col);

    //! Insert count lignes dans le model juste avant la ligne row.
    bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex()) override;

    //! Renvoie le maximum de la colonne col.
    QVariant max(int col) const;

    //! Renvoie le minimum de la colonne col.
    QVariant min(int col) const;

    //! Ajoute une colonne de données et renvoie son identifiant (Factory: unique_ptr<Entity>(*factory)()).
    template<class Factory> Tableau::iterator push_backData(Factory factory, szt count = 0);

    //! Ajoute une colonne de données et renvoie son identifiant.
    template<class Ent> Tableau::iterator push_backData(const cmps::VectorPtr<Ent> & vec);

    //! Ajoute une colonne de données et renvoie son identifiant.
    template<class Ent> Tableau::iterator push_backData(cmps::VectorPtr<Ent> && vec);

    //! Supprime count lignes en commençant par la ligne row.
    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex()) override;

    //! Supprime une selection de lignes entières.
    bool removeRowsSelected(const QModelIndexList &selection);

    //! Réinitialise l'affichage des lignes du model (pas les donnée).
    virtual void resetRow(bool /*init*/ = false) {
        beginResetModel();
            resetIdRow();
        endResetModel();
    }

    //! Renvoie le nombre de lignes.
    int rowCount() const {return static_cast<int>(nbrRow());}

    //! Renvoie le nombre de lignes.
    int rowCount(const QModelIndex & /*parent*/) const override {return rowCount();}

    //! Modifie la donnée d'index et de role spécifié.
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;

    //! Tri le model par colonne.
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

public slots:
    //! Enregistre les données du model dans la base de donnée.

    void save() override;
protected:
    //! Lien avec la base de données.
    virtual bddMPS::Bdd & bdd() const
        {return m_bdd;}

    //! Renvoie un pointeur vers une entity.
    virtual Entity * entityFactory(const Indice & ind) = 0;

    //! Fait la correspondance entre la ligne d'index et la ligne de donnée.
    szt idRow(int row) const
        {return m_idRow[static_cast<szt>(row)];}

    //! Fait la correspondance entre la ligne de donnée et la ligne d'index.
    int idToRow(szt id) const
        {return static_cast<int>(*(std::find(m_idRow.cbegin(),m_idRow.cend(),id)));}

    //! Fait la correspondance entre un index (QModelIndex) et un indice premettant l'accès au données.
    Indice indice(const QModelIndex &index) const
        {return Indice(idRow(index.row()),static_cast<szt>(index.column()));}

    //! Nombre de ligne de données en szt.
    szt nbrId() const
        {return static_cast<szt>(m_data.front().size());}

    //! Nombre de ligne afficher en szt.
    szt nbrRow() const
        {return static_cast<szt>(m_idRow.size());}

    //! Supprime les données correspondant à la ligne id dans la base de donnée.
    virtual void removeInternalData(szt /*id*/)  {}

    //! Met le vecteur des correspondance de ligne à l'identitée.
    void resetIdRow(){
        m_idRow.resize(nbrId());
        std::iota(m_idRow.begin(),m_idRow.end(),0);
    }

    //! Renvoie un numéro de ligne valide et l'itérateur sur m_idRow correspondant.
    QPair<int,std::vector<szt>::const_iterator> valideRowAndIter(int row) const;
};

class AbstractTableValideModel : public AbstractTableEntityModel {
    Q_OBJECT
protected:
    std::vector<szt> m_etats;           //!< Vecteur des états de chaque ligne.
    std::vector<QBrush> m_brush;        //!< Correspondance états <-> QBrush.

public:
    //! État des lignes.auto & e1 =static_cast<const Eleve &>(m_data.front()[row1]);
    enum etat {Sauver,
              Valide,
              Invalide,
              NbrEtatValide};

    //! Constructeur.
    AbstractTableValideModel(bddMPS::Bdd & bdd, QObject * parent = nullptr);

    //! Accesseur du vecteur des QBrush des états.
    const std::vector<QBrush> & brushEtat() const noexcept
        {return m_brush;}

    //! Change la couleur des lignes.
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    //! Teste si la ligne est nouvelle ou modifiée.
    virtual bool newOrModifRow(szt id) const = 0;

    //! Mutateur du vecteur des QBrush des états.
    void setBrushEtat(const std::vector<QBrush> & vecBrush)
        {m_brush = vecBrush;}

    //! Teste si la ligne de donnée d'identifiant id est valide.
    virtual bool valideRow(szt id) const = 0;

    //! Enregistre les données de la ligne id du model dans la base de donnée.
    virtual void saveRow(szt id) = 0;

public slots:
    //! Mets à jour la tailles du vecteur d'états.
    void resizeEtats() {
        if(!m_data.empty())
            m_etats.resize(m_data.front().size(),Sauver);
    }

    //! Enregistre les données du model dans la base de donnée.
    void save() override;

    //! Mets à jour l'état d'une ligne.
    virtual void updateHeaderData(int first, int last);//, const QVector<int> & roles = QVector<int>());
};

class AbstractTableUniqueModel : public AbstractTableValideModel {
    Q_OBJECT
protected:
    std::map<szt,std::list<szt>> m_doubles;     //! Liste des identifiants des doubles.

public:
    //! État des lignes.
    enum etat {Double = NbrEtatValide,
               DoubleSauver,
              NbrEtatUnique};

    //! Constructeur.
    AbstractTableUniqueModel(bddMPS::Bdd & bdd, QObject * parent = nullptr)
        : AbstractTableValideModel(bdd,parent) {
        m_brush.push_back(QBrush(QColor(255,125,0)));
        m_brush.push_back(QBrush(QColor(255,125,0),Qt::Dense4Pattern));
    }

    //! Comparaison d'égalité de deux ligne.
    virtual bool egalRow(szt row1, szt row2) const = 0;

public slots:
    //! Mets à jour l'état d'une ligne.
    void updateHeaderData(int first, int last) override;//, const QVector<int> & roles = QVector<int>());
};

//////////////////// AbstractTableEntityModel //////////////

template<class UnaryPredicat> void AbstractTableEntityModel::find(UnaryPredicat predicat, bool reset) {
    beginResetModel();
        if(reset && nbrRow() != nbrId())
            resetIdRow();
        szt size = 0;
        auto write = m_idRow.begin();
        for (auto read = m_idRow.cbegin(); read != m_idRow.cend(); ++read) {
            if(predicat(*read)) {
                *write = *read;
                ++write;
                ++size;
            }
        }
        m_idRow.resize(size);
    endResetModel();
}

template<class Factory> AbstractTableEntityModel::Tableau::iterator AbstractTableEntityModel::push_backData(Factory factory, szt count) {
    if(count == 0)
        count = nbrId();
    else if(m_data.size() != 0 &&  nbrId() != count)
        throw std::invalid_argument("AbstractTableEntityModel::Tableau::iterator "
                                    "AbstractTableEntityModel::push_backData(szt):"
                                    "Taille de la nouvelle donnée ne coorespond pas au nombre des ligne du model.");
    auto col = cmps::VectorPtr<Entity>(count,nullptr);
    for (szt i = 0; i < col.size(); ++i)
        col.setPtr(i,factory());
    auto iter = m_data.insert(m_data.end(),std::move(col));
    if(m_data.size() == 1 && count != 0)
        resetRow(true);
    return iter;
}

template<class Ent> AbstractTableEntityModel::Tableau::iterator AbstractTableEntityModel::push_backData(const cmps::VectorPtr<Ent> & vec) {
    if(m_data.size() != 0 && vec.size != nbrId())
        throw std::invalid_argument("AbstractTableEntityModel::Tableau::iterator "
                                    "AbstractTableEntityModel::push_backData(const cmps::VectorPtr<Ent> &):"
                                    "Taille de la nouvelle donnée ne coorespond pas au nombre des ligne du model.");
    auto iter = m_data.insert(m_data.end(),vec);
    if(m_data.size() == 1 && vec.size() != 0)
        resetRow(true);
    return iter;
}

template<class Ent> AbstractTableEntityModel::Tableau::iterator AbstractTableEntityModel::push_backData(cmps::VectorPtr<Ent> && vec) {
    if(m_data.size() != 0 && vec.size() != nbrId())
        throw std::invalid_argument("AbstractTableEntityModel::Tableau::iterator "
                                    "AbstractTableEntityModel::push_backData(cmps::VectorPtr<Ent> &&):"
                                    "Taille de la nouvelle donnée ne coorespond pas au nombre de lignes du model.");
    auto iter = m_data.insert(m_data.end(),std::move(vec));
    if(m_data.size() == 1 && m_data.front().size() != 0)
        resetRow(true);
    return iter;
}
}
#endif // ABSTRACTTABLEENTITYMODEL_H
