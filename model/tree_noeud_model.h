/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/05/2020
 */
#ifndef TREENODEMODEL_H
#define TREENODEMODEL_H

#include <memory>
#include "abstract_model.h"

namespace mps {
namespace model_base {

/*! \ingroup groupe_model
 *\brief Classe mère des model de type arbre à colonnes hétérogènes.
 */
class tree_noeud_model : public abstract_model {
    Q_OBJECT
public:
    enum {No_Type = -1};

    //! Noeud abstré de l'arbre de recherche.
    class abstract_noeud {
    protected:
        int m_type;     //! type du noeud.
    public:
        //! Constructeur.
        abstract_noeud(int type = No_Type) : m_type(type){}

        virtual ~abstract_noeud();

        //! Nombre de colonne des descendants.
        virtual int child_column_count() const {return 0;}

        //! Accesseur de la donnée associé à column.
        virtual QVariant data(int /*column*/, int /*role*/ = Qt::DisplayRole) const {return QVariant();}

        //! Accesseur des drapeaux associés à column.
        virtual Qt::ItemFlags flags(int /*column*/) const {return  Qt::NoItemFlags;}

        //! Mutateur de la donnée associé à column.
        virtual bool set_data(int /*column*/, const QVariant &/*value*/, int /*role*/ = Qt::EditRole) {return true;}

        //! Accesseur du type du noeud.
        int type() const {return m_type;}
    };
    //! Classe des noeud de l'arbre de données
    using noeud_ptr = std::unique_ptr<abstract_noeud>;
    //! Classe du protomodel.
    using Data = tree_for_model<noeud_ptr>;
protected:
    Data m_data;                            //!< arbre de données du model.
    std::vector<QString> m_header;          //!< Entête des colonnes.      
public:
    TREE_FOR_MODEL_INDEX_PARENT_ROWCOUNT(m_data)    // Implémentation des méthodes virtuelles index, parent, row_count.

    //!Constructeur.
    tree_noeud_model(bool racine, QObject *parent);

    //! Accesseur du nombre de colonne des descendant.
    int columnCount(const QModelIndex &parent) const override;

    //! Accesseur la donnée associé à un couple (index,role).
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //! Renvoie les drapeaux associé à un index.
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    const abstract_noeud &get_data(const QModelIndex &index) const
        {return **m_data.get_iter(index);}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    abstract_noeud &get_data(const QModelIndex &index)
        {return **m_data.get_iter(index);}

    //! Accesseur des entêtes des colonnes.
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if(orientation == Qt::Horizontal &&role == Qt::DisplayRole
                &&0 <= section  &&section < static_cast<int>(m_header.size()))
            return m_header[static_cast<szt>(section)];
        return QVariant();
    }

    //! Insert count noeuds de nature type avant la ligne row de parent.
    bool insert_noeuds(int type, int row, int count, const QModelIndex &parent = QModelIndex());

    //! Supprimer count ligne en commençant par la ligne row.
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    //! Mutateur la donnée associé à un couple (index,role).
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

protected:
    //! Fabrique des noeuds.
    virtual noeud_ptr node_factory(int /*type*/, int /*row*/, const QModelIndex &/*parent*/) {return std::make_unique<abstract_noeud>();}
};
}}
#endif // TREE_NODE_MODEL_H
