#include "new_modif_dialog.h"

using namespace mps;
using namespace dialogue;

DESTR_VIDE_DEF(abstract_new_modif_form)
DESTR_VIDE_DEF(new_modif_dialog)

//AbstractNomForm
abstract_nom_new_modif_form::abstract_nom_new_modif_form(b2d::bdd &bdd, const QString &label, bool new_ent, QWidget *parent)
    : abstract_new_modif_form (bdd,new_ent,parent) {
    m_main_layout = new QVBoxLayout(this);
    m_name_label = new QLabel(label);
    m_name_layout = new QHBoxLayout;
    m_name_layout->addWidget(m_name_label);
    m_main_layout->addLayout(m_name_layout);
    if(m_new) {
        m_name_line = new QLineEdit;
        m_name_line->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        m_name_label->setBuddy(m_name_line);
        m_name_layout->addWidget(m_name_line);
    }
    else {
        m_name_cb = new widget::id_combo_box();
        m_name_cb->setInsertPolicy(QComboBox::NoInsert);
        m_name_cb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        m_name_label->setBuddy(m_name_cb);
        m_name_layout->addWidget(m_name_cb);
    }
}

void abstract_nom_new_modif_form::connexion() {
    if(m_new)
        connect(m_name_line,&QLineEdit::textChanged,this,[this](){emit save_permis(valide());});
    else
        connect(m_name_cb,qOverload<int>(&QComboBox::currentIndexChanged),this,[this](){
            update_data();
            emit save_permis(valide());
        });
}

//Abstractnc_nomForm
abstract_nc_nom_new_modif_form::abstract_nc_nom_new_modif_form(b2d::bdd &bdd, const QString &labelNc, const QString &labelNom,
                                                     bool new_ent, QWidget *parent)
    : abstract_nom_new_modif_form(bdd,labelNom,new_ent,parent) {
    m_nc_label = new QLabel(labelNc);
    m_nc_line = new QLineEdit;
    m_nc_line->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_nc_label->setBuddy(m_nc_line);
    m_nc_layout = new QHBoxLayout;
    m_nc_layout->addWidget(m_nc_label);
    m_nc_layout->addWidget(m_nc_line);
    m_main_layout->addLayout(m_nc_layout);
}

void abstract_nc_nom_new_modif_form::connexion() {
    abstract_nom_new_modif_form::connexion();
    connect(m_nc_line,&QLineEdit::textChanged,this,[this](){emit save_permis(valide());});
}

//AbstractTypenc_nomForm
abstract_type_nc_nom_new_modif_form::abstract_type_nc_nom_new_modif_form(b2d::bdd_predef &bdd, const QString &ref_root, idt id_entity,
                                                             const QString &labelType, const QString &labelNc, const QString &labelNom,
                                                             bool new_ent, QWidget *parent)
    : abstract_nc_nom_new_modif_form(bdd,labelNc,labelNom,new_ent,parent), m_cible(bdd.cible(id_entity)), m_valide(!m_new)
{
    m_type_label = new QLabel(labelType);
    m_main_layout->addWidget(m_type_label);

    m_type_tree = new widget::tree_widget;
    m_main_layout->addWidget(m_type_tree);

    m_type_tree->setColumnCount(nbrColumn);
    m_type_tree->setHeaderLabels(QStringList({"nom","nom abrégé"}));
    m_type_tree->setSelectionModel(new QItemSelectionModel(m_type_tree->model()));
    m_type_tree->set_tree_ref(m_bdd.get_arbre<type>(m_bdd.ref_to_id<type>(ref_root)),
                        [this](const type &tp)->QTreeWidgetItem *{
        auto item = new QTreeWidgetItem({tp.nom(),tp.nc()});
        item->setData(widget::tree_widget::Id_Colonne,widget::tree_widget::Id_Role,tp.id());
        Permission perm(item->data(nom_type,widget::tree_widget::Id_Role).toUInt(),m_cible);
        m_bdd.get_unique(perm);
        if(perm.test(b2d::code::Attribuable))
            item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        else
            item->setFlags(Qt::ItemIsEnabled);
        return item;
    });
    m_type_tree->setSelectionMode(QAbstractItemView::SingleSelection);
    m_type_tree->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void abstract_type_nc_nom_new_modif_form::connexion() {
    abstract_nc_nom_new_modif_form::connexion();
    connect(m_type_tree->selectionModel(),&QItemSelectionModel::selectionChanged,this,[this](){
        Permission perm(id_type(),m_cible);
        m_bdd.get_unique(perm);
        m_valide = perm.test(b2d::code::Attribuable);
        emit save_permis(valide());
    });
}

//AbstractParentnc_nomForm
abstract_parent_nc_nom_new_modif_form::abstract_parent_nc_nom_new_modif_form(b2d::bdd &bdd, const QStringList &header,
                                                             const QString &labelParent, const QString &labelNc, const QString &labelNom,
                                                             bool new_ent, QWidget *parent)
    : abstract_nc_nom_new_modif_form(bdd,labelNc,labelNom,new_ent,parent)
{
    m_parent_label = new QLabel(labelParent);
    m_main_layout->addWidget(m_parent_label);

    m_parent_tree = new widget::tree_widget;
    m_main_layout->addWidget(m_parent_tree);

    m_parent_tree->setColumnCount(nbrColumn);
    m_parent_tree->setHeaderLabels(header);
    m_parent_tree->setSelectionModel(new QItemSelectionModel(m_parent_tree->model()));
}

//new_modif_dialog

new_modif_dialog::new_modif_dialog(abstract_new_modif_form *form, QWidget *parent)
    : QDialog (parent),
      m_form(form)
{
    setWindowTitle(m_form->title());
    // boutons
    m_annul_bouton = new QPushButton(tr("Annuler"));
    connect(m_annul_bouton,&QPushButton::clicked,this,&new_modif_dialog::reject);
    m_ok_bouton = new QPushButton(tr("Enregistrer"));
    m_ok_bouton->setEnabled(m_form->valide());
    connect(m_ok_bouton,&QPushButton::clicked,this,&new_modif_dialog::accept);
    connect(m_form,SIGNAL(save_permis(bool)),m_ok_bouton,SLOT(setEnabled(bool)));
    if(!m_form->del_disable()) {
        m_suppr_bouton = new QPushButton(tr("Supprimer"));
        connect(m_form,SIGNAL(effacer_permis(bool)),m_suppr_bouton,SLOT(setEnabled(bool)));
        connect(m_suppr_bouton,&QPushButton::clicked,this,[this](){
                                                             if(m_form->del())
                                                                QDialog::accept();
        });
    }
    m_form->connexion();
    m_form->update_data();

    // Calque bouton
    m_bouton_layout = new QHBoxLayout;
    m_bouton_layout->addWidget(m_ok_bouton);
    if(!m_form->del_disable())
        m_bouton_layout->addWidget(m_suppr_bouton);
    m_bouton_layout->addWidget(m_annul_bouton);

    m_main_layout = new QVBoxLayout(this);
    m_main_layout->addWidget(m_form);
    m_main_layout->addLayout(m_bouton_layout);
}
