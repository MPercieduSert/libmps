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
        int id;
        QString name;
        int type;
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
template<class Read, class Write, class Vec> class BaseColonne : public AbstractColonnesModel::AbstractColonne {
protected:
    QVariant (*m_get)(Read, int);                   //!< Fonction lisant la donnée du model.
    bool (*m_set)(Write,const QVariant &,int);    //!< Fonction modifiant la donnée du model.
    QVariant (*m_test)(Read);                       //!< Fonction lisant la donnée du model pour les tests de recherche.
    Vec & m_vec;                                    //!< Référence sur le vecteur de donnée.
public:
    //! Constructeur.
    BaseColonne(const QString & name, Qt::ItemFlags flags, int type,
                 Vec & vec,
                 QVariant (*get)(Read,int),
                 QVariant (*test)(Read),
                 bool (*set)(Write,const QVariant &,int))
        : AbstractColonne(name,flags,type), m_get(get), m_set(set), m_test(test), m_vec(vec) {}

    //! Accesseur de la donnée d'indice id dans la colonne.
    QVariant data(szt id, int role) const override
        {return m_get(m_vec[id], role);}

    //! Accesseut de la donnée pour le test de recherche.
    QVariant dataTest(szt id) const
        {return m_test(m_vec[id]);}

    //! Mutateur de la donnée d'indice id dans la colonne.
    bool setData(szt id, const QVariant & value,  int role) override
        {return m_set(m_vec[id],value,role);}
};

template<class T> using VectorRefColonne = BaseColonne<const T&,T&,std::vector<T>>;
template<class T> using VectorValColonne = BaseColonne<T,T,std::vector<T>>;
template<class T> using VectorPtrColonne = BaseColonne<const T&,T&, conteneurMPS::VectorPtr<T>>;

/*! \ingroup groupeModel
 * \brief Classe template générique d'une colonne avec identifiant.
 */
template<class Read, class Write, class Vec> class IdBaseColonne : public BaseColonne<Read,Write,Vec> {
protected:
    using Base = BaseColonne<Read,Write,Vec>;
    using Base::m_vec;
public:
    //! Constructeur.
    //using BaseColonne<Read,Write,Vec>::BaseColonne;
    using Base::BaseColonne;

    //! Accesseur de la donnée d'indice id dans la colonne.
    QVariant data(szt id, int role) const override {
        if(role == AbstractColonnesModel::IdRole)
            return m_vec[id].id();
        return Base::data(id,role);
    }


    //! Mutateur de la donnée d'indice id dans la colonne.
    bool setData(szt id, const QVariant & value,  int role) override {
        if(role == AbstractColonnesModel::IdRole) {
            m_vec[id].setId(value.toUInt());
            return true;
        }
        return Base::setData(id,value,role);}
};

template<class T> using IdVectorRefColonne = IdBaseColonne<const T&,T&,std::vector<T>>;
template<class T> using IdVectorValColonne = IdBaseColonne<T,T,std::vector<T>>;
template<class T> using IdVectorPtrColonne = IdBaseColonne<const T&,T&, conteneurMPS::VectorPtr<T>>;

/*! \ingroup groupeModel
 * \brief Classe Abstraite mère des colonnes de type booléen.
 */
class AbstractBoolColonne : public AbstractColonnesModel::AbstractColonne {
protected:
    QString m_trueLabel;                //!< Label du booléen vraie.
    QString m_falseLabel;               //!< Label du booléen faux.
public:
    //! Constructeur.
    AbstractBoolColonne(const QString & name, Qt::ItemFlags flags, int type,
                const QString & trueLabel = QString(), const QString & falseLabel = QString())
        : AbstractColonne(name,flags,type),
          m_trueLabel(trueLabel), m_falseLabel(falseLabel) {}

    //! Destructeur.
    ~AbstractBoolColonne() override;

    //! Accesseur du label pour faux.
    const QString & falseLabel() const
        {return m_falseLabel;}

    //! Mutateur du label pour faux.
    void setFalseLabel(const QString & falseLabel)
        {m_falseLabel = falseLabel;}

    //! Mutateur du label pour vraie.
    void setTrueLabel(const QString & trueLabel)
        {m_trueLabel = trueLabel;}

    //! Accesseur du label pour vraie.
    const QString & trueLabel() const
        {return m_trueLabel;}
};

/*! \ingroup groupeModel
 * \brief Classe mère des colonnes de type booléen.
 */
template<class Read, class Write, class Vec> class BoolColonne : public AbstractBoolColonne  {
protected:
    bool (*m_get)(Read);           //!< Fonction lisant la donnée du model.
    void (*m_set)(Write,bool);     //!< Fonction modifiant la donnée du model.
    Vec & m_vec;                        //!< Référence sur le vecteur de donnée.

public:
    //! Constructeur.
    BoolColonne(const QString & name, Qt::ItemFlags flags, int type,
                Vec & vec,
                bool (*get)(Read),
                void (*set)(Write,bool),
                const QString & trueLabel = QString(), const QString & falseLabel = QString())
        : AbstractBoolColonne(name,flags,type,trueLabel,falseLabel),
          m_get(get), m_set(set), m_vec(vec) {}

    //! Accesseur de la donnée d'indice id dans la colonne.
    QVariant data(szt id, int role) const override {
        if(role == Qt::CheckStateRole)
            return m_get(m_vec[id]) ? Qt::Checked : Qt::Unchecked;
        if(role == Qt::DisplayRole)
            return m_get(m_vec[id]) ? m_trueLabel : m_falseLabel;
        return QVariant();
    }

    //! Accesseut de la donnée pour le test de recherche.
    QVariant dataTest(szt id) const
        {return m_get(m_vec[id]);}

    //! Mutateur de la donnée d'indice id dans la colonne.
    bool setData(szt id, const QVariant & value,  int role) override {
        if(role == Qt::CheckStateRole) {
            m_set(m_vec[id],value.toBool());
            return true;}
        return false;
    }
};

template<class T> using VectorRefBoolColonne = BoolColonne<const T&,T&,std::vector<T>>;
template<class T> using VectorValBoolColonne = BoolColonne<T,T,std::vector<T>>;
template<class T> using VectorPtrBoolColonne = BoolColonne<const T&,T&, conteneurMPS::VectorPtr<T>>;

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

    //! Supprime les lignes [first,last) des données du model.
    void erase(szt first, szt last) override;

    //! Accesseur de la donnée d'une ligne.
    const T & internalData(szt ligne) const
        {return m_vec.at(ligne);}

    //! Accesseur de la donnée d'une ligne.
    T & internalData(szt ligne)
        {return m_vec.at(ligne);}

    //! Mutateur de la donnée d'une ligne.
    void setInternalData(szt ligne, const T & data)
        {m_vec.at(ligne) = data;}

    //! Mutateur du vecteur de donné.
    void setVecData(const Vec & data)
        {m_vec = data;}

    //! Mutateur du vecteur de donné.
    void setVecData(Vec && data)
        {m_vec = std::move(data);}

    //! Taille (nombre de lignes).
    szt size() const override {return m_vec.size();}
};

template<class T> class VectorTableau : public AbstractVecTableau<T,std::vector<T>>{
protected:
    using AbstractVecTableau<T,std::vector<T>>::m_vec;
public:
    //! Constructeur.
    using AbstractVecTableau<T,std::vector<T>>::AbstractVecTableau;

    //! Ajoute count lignes au tableau.
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

    //! Ajoute count lignes au tableau.
    void add(szt count) override {m_vec.resize(m_vec.size() + count, [](){return new Ent();});}

    //! Teste si la ligne correspond à une donnée interne.
    bool existsInternalData(szt ligne) const override {return !m_vec[ligne].isNew();}

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

    //! Hydrate la donnée d'une ligne du tableau et retourne le succés de l'opération.
    void hydrate(szt ligne) override {m_bdd.get(m_vec[ligne]);}

    //! Teste si la ligne est nouvelle ou modifiée.
    bool newOrModif(szt ligne) const override {return m_vec[ligne].isNew() || !m_bdd.sameInBdd(m_vec[ligne]);}

    //! Supprime les données correspondant à la ligne dans la base de donnée.
    //! Ne doit pas supprimer de ligne de donnée du model.
    bool removeInternalData(szt ligne) override {return m_bdd.del(m_vec[ligne]);}

    //! Sauve la ligne dans la base de donnée.
    void save(szt ligne) override {m_bdd.save(m_vec[ligne]);}
};

////////////////////////////////////////////////////////// CompositionTableau /////////////////////////////////////
//! Macro de définition des membres test de CompositionTableau.
#define TEST_COMPOSITION_TABLEAU(MODULE,MEMBRE){auto iterTableau = m_tableaux.cbegin(); \
    auto iterTest = m_module[MODULE].cbegin(); \
    while(iterTableau != m_tableaux.cend() && (!*iterTest || (*iterTableau)->MEMBRE)){ \
        ++iterTableau; ++iterTest;} \
    return iterTableau == m_tableaux.cend();}

//! Macro de définition des membres modulaires de CompotionTableau.
#define OPERATION_COMPOSITION_TABLEAU(MODULE,MEMBRE){auto iterActif = m_module[MODULE].cbegin(); \
    for(auto iter = m_tableaux.begin(); iter != m_tableaux.end(); ++iter, ++iterActif)  \
        if(*iterActif) (*iter)->MEMBRE;}

/*! \ingroup groupeModel
 * \brief Classe de composition paramétrée de plusieurs tableaux en un.
 */
class CompositionTableau : public AbstractColonnesModel::AbstractTableau {
protected:
    //! Type du tableau des modules actifs par tableaux.
    using ModuleActif = std::vector<std::vector<bool>>;
    std::vector<std::unique_ptr<AbstractTableau>> m_tableaux;   //!< Vecteur des pointeurs de tableau.
    ModuleActif m_module;                                       //!< Tableau des modules actif.
public:
    //! Module
    enum module {EgalModule,
                 ExistsModule,
                 HydrateModule,
                 NewOrModifModule,
                 RemoveModule,
                 SaveModule,
                 ValideModule,
                 NbrModule};

    //! Constructeur.
    CompositionTableau() : m_module(NbrModule) {}

    //!Destructeur.
    ~CompositionTableau() override = default;

    //! Acceseur des modules actif.
    bool actif(szt module, szt tableau) const
        {return m_module.at(module).at(tableau);}

    //! Ajoute count lignes au tableau.
    void add(szt count) override;

    //! Comparaison d'égalité de deux ligne.
    bool egal(szt ligne1, szt ligne2) const override;

    //! Supprime une ligne des données du model.
    void erase(szt ligne) override;

    //! Supprime les lignes [first,last) des données du model.
    void erase(szt first, szt last) override;

    //! Teste si la ligne correspond à une donnée interne.
    bool existsInternalData(szt ligne) const override;

    //! Hydrate la donnée d'une ligne du tableau et retourne le succés de l'opération.
    void hydrate(szt ligne) override;

    //! Fabrique une colonne lié aux données du tableau.
    std::unique_ptr<AbstractColonnesModel::AbstractColonne> makeColonne(const AbstractColonnesModel::NewColonneInfo & info) override
        {return m_tableaux.at(info.tableau)->makeColonne(info);}

    //! Teste si la ligne est nouvelle ou modifiée.
    bool newOrModif(szt ligne) const override;

    //! Ajoute un tableau à la composition. Le nouveau tableau doit avoir la même taille que ceux auquel il est associé.
    void push_back(std::unique_ptr<AbstractTableau> && tableau, bool actif = true);

    //! Supprime les données correspondant à la ligne dans la base de donnée.
    //! Ne doit pas supprimer de ligne de donnée du model.
    bool removeInternalData(szt ligne) override;

    //! Sauve la ligne dans la base de donnée.
    void save(szt ligne) override;

    //! Mutateurs des modules actifs.
    void setActif(szt module, szt tableau, bool actif = true)
        {m_module.at(module).at(tableau) = actif;}

    //! Taille (nombre de lignes).
    szt size() const override
        {return m_tableaux.empty() ? 0 : m_tableaux.front()->size();}

    //! Accesseur des tableaux.
    AbstractTableau & tableau(szt pos)
        {return *m_tableaux.at(pos);}

    //! Accesseur des tableaux.
    const AbstractTableau & tableau(szt pos) const
        {return *m_tableaux.at(pos);}

    //! Teste si la ligne de donnée est valide.
    bool valide(szt ligne) const override;
};
template<class T, class Vec> template<class Factory> AbstractVecTableau<T,Vec>::AbstractVecTableau(szt size, Factory factory) {
    m_vec.reserve(size);
    for(szt i = 0; i != size; ++i)
        m_vec.push_back(factory(i));
}

template<class T, class Vec> void AbstractVecTableau<T,Vec>::erase(szt first, szt last)
    {m_vec.erase(std::next(m_vec.cbegin(),first),std::next(m_vec.cbegin(),last));}

template <class T> std::unique_ptr<AbstractColonnesModel::AbstractColonne>
                        VectorTableau<T>::makeColonne(const AbstractColonnesModel::NewColonneInfo & info) {
    switch (info.type) {
    case AbstractColonnesModel::BoolColonne:
        if(info.args.size() == 0)
            return std::make_unique<VectorValColonne<T>>(info.name,info.flags,info.type,m_vec,
                                                         [](T element)->bool{return element;},
                                                         [](bool value, bool element){element = value;});
        if(info.args.size() == 2)
            return std::make_unique<VectorValColonne<T>>(info.name,info.flags,info.type,m_vec,
                                                         [](T element)->bool{return element;},
                                                         [](bool value, bool element){element = value;},
                                                         info.args[0].toString(),info.args[1].toString());
    case AbstractColonnesModel::DoubleColonne:
    case AbstractColonnesModel::IntColonne:
    case AbstractColonnesModel::UIntColonne:
        return std::make_unique<VectorValColonne<T>>(info.name,info.flags,info.type,m_vec,
                    [](T element, int role){
                        if(role == Qt::DisplayRole || role == Qt::EditRole)
                            return element;
                        return QVariant();},
                    [](const QVariant & value, T element, int role){
                        if(role == Qt::EditRole)
                            element = value.value<T>();
                        return QVariant();});
    default:
        return std::make_unique<VectorRefColonne<T>>(info.name,info.flags,info.type,m_vec,
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
}
#endif // ABSTRACTCOLONNESMODEL_H
