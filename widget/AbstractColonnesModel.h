/*Auteur: PERCIE DU SERT Maxime
 *Date: 29/04/2019
 */
#ifndef ABSTRACTCOLONNESMODEL_H
#define ABSTRACTCOLONNESMODEL_H

#include <memory>
#include <QAbstractItemModel>
#include <QBrush>
#include "AbstractFindModel.h"
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
class AbstractColonnesModel : public QAbstractItemModel {
    Q_OBJECT
public:
    enum {NoType = -1};

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

    //! Information sur une colonne.
    struct NewColonneInfo {
        Qt::ItemFlags flags;
        int id;
        QString name;
        int type;
    };

    //////////////////////////////// AbstractColonne //////////////////////
    //! Classe abstraite d'information et d'intéraction avec les colonnes.
    class AbstractColonne {
    protected:
        Qt::ItemFlags m_flags;      //!< Drapeaux associés à la colonne.
        QString m_header;           //!< Entête de la colonne.
        const int m_type;           //!< Type de la colonne.
    public:
        //! Constructeur.
        AbstractColonne(const QString & name, Qt::ItemFlags flags, int type)
            : m_flags(flags), m_header(name), m_type(type) {}

        //! Destructeur.
        virtual ~AbstractColonne();

        //! Compare entre deux lignes ( ligne1 < ligne2).
        virtual bool compare(szt ligne1, szt ligne2) const
            {return data(ligne1,Qt::DisplayRole) < data(ligne2,Qt::DisplayRole);}

        //! Accesseur de la donnée d'indice id dans la colonne.
        virtual QVariant data(szt /*id*/, int /*role*/ = Qt::DisplayRole) const {return QVariant();}

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

    /////////////////////////////////////// AbstractTableau ///////////////////////////////////
    //! Classe Abstraite contenant les donnée du model.
    class AbstractTableau {
    public:
        //! Constructeur.
        AbstractTableau() = default;

        //!Destructeur.
        virtual ~AbstractTableau() = default;

        // Ajoute count lignes au tableau.
        virtual void add(szt /*count*/) {}

        //! Comparaison d'égalité de deux ligne.
        virtual bool egal(szt /*ligne1*/, szt /*ligne2*/) const {return false;}

        //! Supprime une ligne des données du model.
        virtual void erase(szt /*ligne*/) {}

        //! Supprime les lignes [first,last] des données du model.
        virtual void erase(szt /*first*/, szt /*last*/);

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
    // Colonnes
    std::vector<std::unique_ptr<AbstractColonne>> m_colonnes;     //!< Contient les informations d'une colonne.
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

    //! Accesseur de la colonne trié lors de réinitialisation (-1 pour aucune).
    int colonneSorted() const
        {return m_colonneSorted;}

    //! Renvoie le nombre de colonnes.
    int columnCount(const QModelIndex & /*parent*/ = QModelIndex()) const override {return static_cast<int>(m_colonnes.size());}

    //! Renvoie la donnée d'index et de role spécifié.
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //! Renvoie les drapeaux associés à un index.
    Qt::ItemFlags flags(const QModelIndex & index) const override;

    //! Renvoie le label des lignes et des colonnes.
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    //! Insert une nouvelle colonne à la postion pos.
    virtual bool insertColonne(int pos, const NewColonneInfo & info, bool allParent = false);

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
    void setColonneSorted(int col)
        {m_colonneSorted = col;}

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

//    //! Renvoie le informations de recherche de la colonne d'indice pos.
//    AbstractFindModel::Colonne colonneForFindModel(szt pos) const {
//        AbstractFindModel::Colonne colonne;
//        if(pos < m_columns.size()) {
//            colonne.type = m_columns[pos]->type();
//            colonne.nom = m_columns[pos]->header();
//        }
//        return colonne;
//    }

    //! Fait la correspondance entre l'index et la ligne de donnée.
    virtual szt ligne(const QModelIndex & index) const = 0;

    //! Nombre de ligne de données en szt.
    szt nbrLignes() const {return m_data->size();}

    //! Nombre de ligne afficher en szt.
    virtual szt nbrRow() const = 0;

    //! Met le vecteur des correspondance de ligne à l'identitée.
    virtual void resetRowToLigne() = 0;

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

////////////////////////////////////// Colonne /////////////////////////////////////////////
/*! \ingroup groupeModel
 * \brief Classe template générique d'une colonne.
 */
template<class T, class Vec> class BaseColonne : public AbstractColonnesModel::AbstractColonne {
protected:
    QVariant (*m_get)(const T &, int);                  //!< Fonction lisant la donnée du model.
    bool (*m_set)(const QVariant &, T &, int);          //!< Fonction modifiant la donnée du model.
    Vec & m_vec;                                        //!< Référence sur le vecteur de donnée.
public:
    //! Constructeur.
    BaseColonne(const QString & name, Qt::ItemFlags flags, int type,
                 Vec & vec,
                 QVariant (*get)(const T &, int),
                 bool (*set)(const QVariant &, T &, int))
        : AbstractColonne(name,flags,type), m_get(get), m_set(set), m_vec(vec) {}
};

/*! \ingroup groupeModel
 * \brief Classe template générique d'une colonne associée à un vecteur de valeur.
 */
template<class T, class Vec> class ValueColonne : public BaseColonne<T,Vec> {
protected:
    using BaseColonne<T,Vec>::m_get;
    using BaseColonne<T,Vec>::m_set;
    using BaseColonne<T,Vec>::m_vec;
public:
    //! Constructeur.
    using BaseColonne<T,Vec>::BaseColonne;

    //! Accesseur de la donnée d'indice id dans la colonne.
    QVariant data(szt id, int role) const override
        {return m_get(m_vec[id], role);}

    //! Mutateur de la donnée d'indice id dans la colonne.
    bool setData(szt id, const QVariant & value,  int role) override
        {return m_set(value,m_vec[id],role);}
};

template<class T> using VectorColonne = ValueColonne<T, std::vector<T>>;
template<class T> using VectorPtrColonne = ValueColonne<T, conteneurMPS::VectorPtr<T>>;

/*! \ingroup groupeModel
 * \brief Classe template générique d'une colonne associée à un vecteur de pointeur type U ancêtre de T.
 */
template<class T, class Vec> class PtrColonne : public BaseColonne<T,Vec> {
protected:
    using BaseColonne<T,Vec>::m_vec;
public:
    //! Constructeur.
    using BaseColonne<T,Vec>::BaseColonne;

    //! Accesseur de la donnée d'indice id dans la colonne.
    QVariant data(szt id, int role) const override
        {return m_get(static_cast<const T &>(*(m_vec[id])), role);}

    //! Mutateur de la donnée d'indice id dans la colonne.
    bool setData(szt id, const QVariant & value,  int role) override
        {return m_set(value,static_cast<T &>(*(m_vec[id])),role);}
};

/*! \ingroup groupeModel
 * \brief Classe template générique d'une colonne associée à un vecteur de pointeur type U ancêtre de T.
 */
template<class T, class Vec> class ConvertColonne : public BaseColonne<T,Vec> {
protected:
    using BaseColonne<T,Vec>::m_vec;
public:
    //! Constructeur.
    using BaseColonne<T,Vec>::BaseColonne;

    //! Accesseur de la donnée d'indice id dans la colonne.
    QVariant data(szt id, int role) const override
        {return m_get(static_cast<const T &>(m_vec[id]), role);}

    //! Mutateur de la donnée d'indice id dans la colonne.
    bool setData(szt id, const QVariant & value,  int role) override
        {return m_set(value,static_cast<T &>(m_vec[id]),role);}
};

template<class Ent> using EntityColonne = ConvertColonne<Ent,conteneurMPS::VectorPtr<entityMPS::Entity>>;

///////////////////////////////////////// Tableau ///////////////////////////////////////////////////////////
/*! \ingroup groupeModel
 * \brief Classe template générique d'un tableau composer d'une colonne de donné.
 */
template<class T, class Vec> class AbstractVecTableau : public AbstractColonnesModel::AbstractTableau {
protected:
    Vec m_vec;       //! Vecteur de donnée.
public:
    //! Constructeur d'une colonne vide.
    AbstractVecTableau() = default;
    //! Constructeur d'un tableau d'une colonne de size value.
    AbstractVecTableau(szt size, const T & value = T()) : m_vec(size,value) {}
    //! Constructeur d'un tableau constitué de vec.
    AbstractVecTableau(const Vec & vec) : m_vec(vec) {}
    //! Constructeur d'un tableau constitué de vec.
    AbstractVecTableau(Vec & vec) : m_vec(std::move(vec)) {}
    //! Constructeur d'un tableau d'une colonne de size valeur fabriquée.(Factory: T(*factory)(szt)).
    template<class Factory> AbstractVecTableau(szt size, Factory factory);

    //! Comparaison d'égalité de deux ligne.
    bool egal(szt ligne1, szt ligne2) const override {return m_vec[ligne1] == m_vec[ligne2];}

    //! Supprime une ligne des données du model.
    void erase(szt ligne) override {m_vec.erase(std::next(m_vec.cbegin(),ligne));}

    //! Supprime les lignes [first,last] des données du model.
    void erase(szt first, szt last) override;

    //! Taille (nombre de lignes).
    szt size() const override {return m_vec.size();}
};

template<class T> class VectorTableau : public AbstractVecTableau<T,std::vector<T>>{
protected:
    using AbstractVecTableau<T,std::vector<T>>::m_vec;
public:
    //! Constructeur.
    using AbstractVecTableau<T,std::vector<T>>::AbstractVecTableau;

    // Ajoute count lignes au tableau.
    void add(szt count) override {m_vec.resize(m_vec.size() + count);}

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<AbstractColonnesModel::AbstractColonne>
        makeColonne(const AbstractColonnesModel::NewColonneInfo &info) override;
};


/*! \ingroup groupeModel
 * \brief Classe template générique d'un tableau composer d'une colonne de type vectorPtr<Ent>.
 */
template<class Ent> class AbstractVectorEntTableau : public AbstractVecTableau<Ent,conteneurMPS::VectorPtr<Ent>> {
protected:
    using AbstractVecTableau<Ent,conteneurMPS::VectorPtr<Ent>>::m_vec;
public:
    //! Constructeur.
    using AbstractVecTableau<Ent,conteneurMPS::VectorPtr<Ent>>::AbstractVecTableau;

    // Ajoute count lignes au tableau.
    void add(szt count) override {m_vec.resize(m_vec.size() + count, [](){return new Ent();});}

    //! Teste si la ligne de donnée est valide.
    bool valide(szt ligne) const {return m_vec[ligne].isValid();}
};

/*! \ingroup groupeModel
 * \brief Classe template générique d'un tableau composer d'une colonne de type vectorPtr<Ent> lier à la base de donnée.
 */
template<class Ent> class AbstractBddVectorEntTableau : public AbstractVectorEntTableau<Ent> {
protected:
    using AbstractVectorEntTableau<Ent>::m_vec;
    bddMPS::Bdd & m_bdd;        //! Lien avec la base de données.
public:
    //! Constructeur d'une colonne vide
    AbstractBddVectorEntTableau(bddMPS::Bdd & bdd) : m_bdd(bdd) {}
    //! Constructeur d'un tableau d'une colonne de size value.
    AbstractBddVectorEntTableau(bddMPS::Bdd & bdd, szt size, const Ent & value = Ent())
        : AbstractVectorEntTableau<Ent>(size,value), m_bdd(bdd) {}
    //! Constructeur d'un tableau constitué de vec.
    AbstractBddVectorEntTableau(bddMPS::Bdd & bdd, const conteneurMPS::VectorPtr<Ent> & vec)
        : AbstractVectorEntTableau<Ent>(vec), m_bdd(bdd) {}
    //! Constructeur d'un tableau constitué de vec.
    AbstractBddVectorEntTableau(bddMPS::Bdd & bdd, conteneurMPS::VectorPtr<Ent> & vec)
        : AbstractVectorEntTableau<Ent>(std::move(vec)), m_bdd(bdd) {}
    //! Constructeur d'un tableau d'une colonne de size valeur fabriquée.(Factory: T(*factory)(szt)).
    template<class Factory> AbstractBddVectorEntTableau(bddMPS::Bdd & bdd, szt size, Factory factory)
        : AbstractVectorEntTableau<Ent>(size,factory), m_bdd(bdd) {}

    //! Teste si la ligne est nouvelle ou modifiée.
    bool newOrModif(szt ligne) const {return m_vec[ligne].isNew() || !m_bdd.sameInBdd(m_vec[ligne]);}

    //! Supprime les données correspondant à la ligne dans la base de donnée.
    //! Ne doit pas supprimer de ligne de donnée du model.
    bool removeInternalData(szt ligne)  {return m_bdd.del(m_vec[ligne]);}

    //! Sauve la ligne dans la base de donnée.
    void save(szt ligne) {m_bdd.save(m_vec[ligne]);}
};

template<class T, class Vec> template<class Factory> AbstractVecTableau<T,Vec>::AbstractVecTableau(szt size, Factory factory) {
    m_vec.reserve(size);
    for(szt i = 0; i != size; ++i)
        m_vec.push_back(factory(i));
}

template<class T, class Vec> void AbstractVecTableau<T,Vec>::erase(szt first, szt last)
    {m_vec.erase(std::next(m_vec.cbegin(),first),std::next(m_vec.cbegin(),last + 1));}

template <class T> std::unique_ptr<AbstractColonnesModel::AbstractColonne>
                        VectorTableau<T>::makeColonne(const AbstractColonnesModel::NewColonneInfo & info) {
    return std::make_unique<VectorColonne>(info.name,info.flags,info.type,m_vec,
                [](const T & element, int role){
                    if(role == Qt::DisplayRole || role == Qt::EditRole)
                        return element;
                    return QVariant();},
                [](const QVariant & value, T & element, int role){
                    if(role == Qt::EditRole)
                        element = value.value<T>();
                    return QVariant();});
}
}
#endif // ABSTRACTCOLONNESMODEL_H
