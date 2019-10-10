#include "NewMotCleDialog.h"

NewMotCleDialog::NewMotCleDialog(const QString & nomparent, int position, QWidget * parent): QDialog(parent)
{
    setWindowTitle(QString(tr("Créer un mot clé dans\"")).append(nomparent)
                   .append("\" à la position ").append(QString::number(position).append(".")));

    m_nomLabel = new QLabel(tr("Nom du mot clé:"));
    m_ncLabel = new QLabel(tr("Nom abrégé du mot clé:"));
    m_nomLine = new QLineEdit();
    m_ncLine = new QLineEdit();
    m_cancelBouton = new QPushButton(tr("Annuler"));
    m_creerBouton = new QPushButton(tr("Créer"));

    connect(m_creerBouton,&QPushButton::clicked,this,&NewMotCleDialog::accept);
    connect(m_cancelBouton,&QPushButton::clicked,this,&NewMotCleDialog::reject);

    m_hLayout = new QHBoxLayout();
    m_hLayout->addWidget(m_creerBouton);
    m_hLayout->addWidget(m_cancelBouton);
    m_vLayout = new QVBoxLayout(this);
    m_vLayout->addWidget(m_nomLabel);
    m_vLayout->addWidget(m_nomLine);
    m_vLayout->addWidget(m_ncLabel);
    m_vLayout->addWidget(m_ncLine);
    m_vLayout->addLayout(m_hLayout);
}

void NewMotCleDialog::accept()
{
    if(!m_nomLine->text().isEmpty() && !m_ncLine->text().isEmpty())
        QDialog::accept();
    else
        QMessageBox::critical(this,tr("Entrée invalide"),tr("Le nom et le nom abrégé du mot clé doivent être remplis"));
}

NewMotCleDialog::dialogResult NewMotCleDialog::value() const
{
    dialogResult value;
    value.nom = m_nomLine->text();
    value.nc = m_ncLine->text();
    return value;
}
