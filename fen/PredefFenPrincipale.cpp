#include "PredefFenPrincipale.h"

using namespace fenMPS;

PredefFenPrincipale::PredefFenPrincipale(AbstractNoyau * noyau, std::unique_ptr<bddMPS::Bdd> &&bdd, PredefZoneCentrale * centralZone,
                                         const QString & bddPathXML, const QString & configPath, QWidget *parent)
    :   FenPrincipale (noyau,std::move(bdd),centralZone,bddPathXML,configPath,parent)
{
    addMenuBdd();
}

void PredefFenPrincipale::addMenuBdd(){
    m_bddMenu = menuBar()->addMenu(tr("Bdd"));

    // Table
    m_bddTableMenu = m_bddMenu->addMenu("Tables");
    std::vector<std::pair<int,QString>> vec;
    vec.reserve(m_noyau->bdd().nbrEntity());
    for(szt i = 0; i < m_noyau->bdd().nbrEntity(); ++i) {
        if(m_noyau->bdd().managers().valide(i)) {
            vec.emplace_back(i,m_noyau->bdd().managers().info(i).name());
            if(!m_noyau->bdd().managers().get(i).infoArbre().name().isEmpty())
                vec.emplace_back(-i,m_noyau->bdd().managers().get(i).infoArbre().name());
        }
    }
    std::sort(vec.begin(),vec.end(),
              [](const std::pair<int, QString> & p1, const std::pair<int, QString> & p2)->bool {return p1.second < p2.second;});

    for(auto iter = vec.cbegin(); iter != vec.cend(); ++iter) {
        auto action = m_bddTableMenu->addAction(iter->second);
        auto i =iter->first;
        connect(action,&QAction::triggered,[this,i](){centraleZone()->openTab({PredefTab::GestionBddTabId,i});});
    }

    // XML
    m_bddXmlMenu = m_bddMenu->addMenu("XML");
    auto iXml = m_bddXmlMenu->addAction("Importer");
    //auto schXml = m_bddXmlMenu->addAction("Schema Xml");
    connect(iXml,&QAction::triggered,this,&PredefFenPrincipale::importXml);
    //connect(schXml,&QAction::triggered,this,&PredefFenPrincipale::schemaXml);
}

void PredefFenPrincipale::connectActionToOpenTab(QAction * action, const std::pair<int,int>& pairIndex,
                                           const std::vector<QVariant> & vec)
    {connect(action,&QAction::triggered,this,[this,pairIndex,vec](){centraleZone()->openTab(pairIndex,vec);});}

void PredefFenPrincipale::importXml() {
    auto fileName = QFileDialog::getOpenFileName(this,
                                                 tr("Importer des données à partir d'un fichier XML"),
                                                 m_noyau->config().defaultDirectory(),
                                                 tr("Fichier XML (*.xml)"));
    if(!fileName.isEmpty())
        m_noyau->importXml(fileName);
}

void PredefFenPrincipale::schemaXml() {
    auto fileName = QFileDialog::getSaveFileName(this,
                                                 tr("Écriture du schema xml des fichiers d'importation de données"),
                                                 m_noyau->config().defaultDirectory(),
                                                 tr("Schema XML (*.xsd)"));
    if(!fileName.isEmpty())
        m_noyau->schemaXmlForimport(fileName);
}
