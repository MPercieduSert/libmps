#include "NewModifDialog.h"

using namespace dialogMPS;

DESTR_VIDE_DEF(AbstractNewModifForm)
DESTR_VIDE_DEF(NewModifDialog)

//AbstractNomForm
AbstractNomNewModifForm::AbstractNomNewModifForm(bddMPS::Bdd & bdd, const QString & label, bool newEnt, QWidget * parent)
    : AbstractNewModifForm (bdd,newEnt,parent)
{
    m_mainLayout = new QVBoxLayout(this);
    m_nameLabel = new QLabel(label);
    m_mainLayout->addWidget(m_nameLabel);
    if(m_new) {
        m_nameLine = new QLineEdit;
        m_nameLabel->setBuddy(m_nameLine);
        m_mainLayout->addWidget(m_nameLine);
    }
    else {
        m_nameCB = new widgetMPS::IdComboBox();
        m_nameCB->setInsertPolicy(QComboBox::NoInsert);
        m_nameLabel->setBuddy(m_nameCB);
        m_mainLayout->addWidget(m_nameCB);
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

//AbstractNcNomForm
AbstractNcNomNewModifForm::AbstractNcNomNewModifForm(bddMPS::Bdd &bdd, const QString &labelNc, const QString &labelNom,
                                                     bool newEnt, QWidget * parent)
    : AbstractNomNewModifForm(bdd,labelNom,newEnt,parent) {
    m_ncLabel = new QLabel(labelNc);
    m_ncLine = new QLineEdit;
    m_ncLabel->setBuddy(m_ncLine);   
    m_mainLayout->addWidget(m_ncLabel);
    m_mainLayout->addWidget(m_ncLine);
}

void AbstractNcNomNewModifForm::connexion() {
    AbstractNomNewModifForm::connexion();
    connect(m_ncLine,&QLineEdit::textChanged,this,[this](){emit savePermis(valide());});
}

//AbstractTypeNcNomForm
AbstractTypeNcNomNewModifForm::AbstractTypeNcNomNewModifForm(bddMPS::BddPredef & bdd, const QString & refRoot, idt idEntity,
                                                             const QString & labelType, const QString &labelNc, const QString &labelNom,
                                                             bool newEnt, QWidget * parent)
    : AbstractNcNomNewModifForm(bdd,labelNc,labelNom,newEnt,parent), m_cible(bdd.cible(idEntity)), m_valide(!m_new)
{
    m_typeLabel = new QLabel(labelType);
    m_mainLayout->addWidget(m_typeLabel);

    m_typeTree = new QTreeWidget;
    m_mainLayout->addWidget(m_typeTree);

    m_typeTree->setColumnCount(nbrColumn);
    m_typeTree->setHeaderLabels(QStringList({"nom","nom court"}));
    m_typeTree->setSelectionModel(new QItemSelectionModel(m_typeTree->model()));
    auto tree = m_bdd.getArbre<Type>(m_bdd.refToId<Type>(refRoot));
    auto iter = tree.beginBrother();
    auto * item = new QTreeWidgetItem(QStringList({iter->nom(),iter->nc()}));
    item->setData(nomType,Qt::UserRole,iter->id());
    setFlags(item);
    m_typeTree->addTopLevelItem(item);
    addChilds(iter,item);
}

void AbstractTypeNcNomNewModifForm::addChilds(conteneurMPS::tree<Type>::brother_iterator iter, QTreeWidgetItem * item) {
    if(!iter.leaf()){
        for (auto iterChild = iter.beginChild(); iterChild; ++iterChild) {
            auto * itemChild = new QTreeWidgetItem(QStringList({iterChild->nom(),iterChild->nc()}));
            itemChild->setData(nomType,Qt::UserRole,iterChild->id());
            item->addChild(itemChild);
            setFlags(item);
            addChilds(iterChild,itemChild);
        }
    }
}

void AbstractTypeNcNomNewModifForm::connexion() {
    AbstractNcNomNewModifForm::connexion();
    connect(m_typeTree->selectionModel(),&QItemSelectionModel::selectionChanged,this,[this](){
        Permission perm(idType(),m_cible);
        m_bdd.getUnique(perm);
        m_valide = perm.test(bddMPS::code::Attribuable);
        emit savePermis(valide());
    });
}

void AbstractTypeNcNomNewModifForm::setFlags(QTreeWidgetItem * item) {
    Permission perm(item->data(nomType,Qt::UserRole).toUInt(),m_cible);
    m_bdd.getUnique(perm);
    if(perm.test(bddMPS::code::Attribuable))
        item->setFlags(Qt::ItemIsSelectable);
    else
        item->setFlags(Qt::ItemIsEnabled);
}

void AbstractTypeNcNomNewModifForm::setType(idt idType){
    m_typeTree->collapseAll();
    QTreeWidgetItemIterator iter(m_typeTree);
    while (*iter && (*iter)->data(nomType,Qt::UserRole).toUInt() != idType)
        ++iter;
    if(*iter)
        m_typeTree->setCurrentItem(*iter);
}

//NewModifDialog

NewModifDialog::NewModifDialog(AbstractNewModifForm * form, QWidget * parent)
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
