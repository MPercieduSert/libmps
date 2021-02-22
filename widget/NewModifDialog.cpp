#include "NewModifDialog.h"

using namespace dialogMPS;

DESTR_VIDE_DEF(AbstractNewModifForm)
DESTR_VIDE_DEF(NewModifDialog)

//AbstractNomForm
AbstractNomNewModifForm::AbstractNomNewModifForm(b2d::Bdd &bdd, const QString &label, bool newEnt, QWidget *parent)
    : AbstractNewModifForm (bdd,newEnt,parent)
{
    m_mainLayout = new QVBoxLayout(this);
    m_nameLabel = new QLabel(label);
    m_nameLayout = new QHBoxLayout;
    m_nameLayout->addWidget(m_nameLabel);
    m_mainLayout->addLayout(m_nameLayout);
    if(m_new) {
        m_nameLine = new QLineEdit;
        m_nameLine->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        m_nameLabel->setBuddy(m_nameLine);
        m_nameLayout->addWidget(m_nameLine);
    }
    else {
        m_nameCB = new widgetMPS::IdComboBox();
        m_nameCB->setInsertPolicy(QComboBox::NoInsert);
        m_nameCB->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        m_nameLabel->setBuddy(m_nameCB);
        m_nameLayout->addWidget(m_nameCB);
    }
}

void AbstractNomNewModifForm::connexion() {
    if(m_new)
        connect(m_nameLine,&QLineEdit::textChanged,this,[this](){emit savePermis(valide());});
    else
        connect(m_nameCB,qOverload<int>(&QComboBox::currentIndexChanged),this,[this](){
            updateData();
            emit savePermis(valide());
        });
}

//Abstractnc_nomForm
Abstractnc_nomNewModifForm::Abstractnc_nomNewModifForm(b2d::Bdd &bdd, const QString &labelNc, const QString &labelNom,
                                                     bool newEnt, QWidget *parent)
    : AbstractNomNewModifForm(bdd,labelNom,newEnt,parent) {
    m_ncLabel = new QLabel(labelNc);
    m_ncLine = new QLineEdit;
    m_ncLine->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_ncLabel->setBuddy(m_ncLine);
    m_ncLayout = new QHBoxLayout;
    m_ncLayout->addWidget(m_ncLabel);
    m_ncLayout->addWidget(m_ncLine);
    m_mainLayout->addLayout(m_ncLayout);
}

void Abstractnc_nomNewModifForm::connexion() {
    AbstractNomNewModifForm::connexion();
    connect(m_ncLine,&QLineEdit::textChanged,this,[this](){emit savePermis(valide());});
}

//AbstractTypenc_nomForm
AbstractTypenc_nomNewModifForm::AbstractTypenc_nomNewModifForm(b2d::bdd_predef &bdd, const QString &refRoot, idt id_entity,
                                                             const QString &labelType, const QString &labelNc, const QString &labelNom,
                                                             bool newEnt, QWidget *parent)
    : Abstractnc_nomNewModifForm(bdd,labelNc,labelNom,newEnt,parent), m_cible(bdd.cible(id_entity)), m_valide(!m_new)
{
    m_typeLabel = new QLabel(labelType);
    m_mainLayout->addWidget(m_typeLabel);

    m_typeTree = new widgetMPS::TreeWidget;
    m_mainLayout->addWidget(m_typeTree);

    m_typeTree->setColumnCount(nbrColumn);
    m_typeTree->setHeaderLabels(QStringList({"nom","nom abrégé"}));
    m_typeTree->setSelectionModel(new QItemSelectionModel(m_typeTree->model()));
    m_typeTree->setTreeRef(m_bdd.get_arbre<Type>(m_bdd.ref_to_id<Type>(refRoot)),
                        [this](const Type &tp)->QTreeWidgetItem *{
        auto item = new QTreeWidgetItem({tp.nom(),tp.nc()});
        item->set_data(widgetMPS::TreeWidget::IdColonne,widgetMPS::TreeWidget::IdRole,tp.id());
        Permission perm(item->data(nom_type,widgetMPS::TreeWidget::IdRole).toUInt(),m_cible);
        m_bdd.get_unique(perm);
        if(perm.test(b2d::code::Attribuable))
            item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        else
            item->setFlags(Qt::ItemIsEnabled);
        return item;
    });
    m_typeTree->setSelectionMode(QAbstractItemView::SingleSelection);
    m_typeTree->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void AbstractTypenc_nomNewModifForm::connexion() {
    Abstractnc_nomNewModifForm::connexion();
    connect(m_typeTree->selectionModel(),&QItemSelectionModel::selectionChanged,this,[this](){
        Permission perm(id_type(),m_cible);
        m_bdd.get_unique(perm);
        m_valide = perm.test(b2d::code::Attribuable);
        emit savePermis(valide());
    });
}

//AbstractParentnc_nomForm
AbstractParentnc_nomNewModifForm::AbstractParentnc_nomNewModifForm(b2d::Bdd &bdd, const QStringList &header,
                                                             const QString &labelParent, const QString &labelNc, const QString &labelNom,
                                                             bool newEnt, QWidget *parent)
    : Abstractnc_nomNewModifForm(bdd,labelNc,labelNom,newEnt,parent)
{
    m_parentLabel = new QLabel(labelParent);
    m_mainLayout->addWidget(m_parentLabel);

    m_parentTree = new widgetMPS::TreeWidget;
    m_mainLayout->addWidget(m_parentTree);

    m_parentTree->setColumnCount(nbrColumn);
    m_parentTree->setHeaderLabels(header);
    m_parentTree->setSelectionModel(new QItemSelectionModel(m_parentTree->model()));
}

//NewModifDialog

NewModifDialog::NewModifDialog(AbstractNewModifForm *form, QWidget *parent)
    : QDialog (parent),
      m_form(form)
{
    setWindowTitle(m_form->title());
    // boutons
    m_annulButton = new QPushButton(tr("Annuler"));
    connect(m_annulButton,&QPushButton::clicked,this,&NewModifDialog::reject);
    m_okButton = new QPushButton(tr("Enregistrer"));
    m_okButton->setEnabled(m_form->valide());
    connect(m_okButton,&QPushButton::clicked,this,&NewModifDialog::accept);
    connect(m_form,SIGNAL(savePermis(bool)),m_okButton,SLOT(setEnabled(bool)));
    if(!m_form->delDisable()) {
        m_supprButton = new QPushButton(tr("Supprimer"));
        connect(m_form,SIGNAL(effacerPermis(bool)),m_supprButton,SLOT(setEnabled(bool)));
        connect(m_supprButton,&QPushButton::clicked,this,[this](){
                                                             if(m_form->del())
                                                                QDialog::accept();
        });
    }
    m_form->connexion();
    m_form->updateData();

    // Calque bouton
    m_buttonLayout = new QHBoxLayout;
    m_buttonLayout->addWidget(m_okButton);
    if(!m_form->delDisable())
        m_buttonLayout->addWidget(m_supprButton);
    m_buttonLayout->addWidget(m_annulButton);

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addWidget(m_form);
    m_mainLayout->addLayout(m_buttonLayout);
}
