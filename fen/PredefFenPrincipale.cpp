#include "PredefFenPrincipale.h"

using namespace fenMPS;

PredefFenPrincipale::PredefFenPrincipale(AbstractNoyau * noyau, std::unique_ptr<bddMPS::Bdd> &&bdd, PredefZoneCentrale * centralZone,
                                         const QString & bddPathXML, const QString & configPath, QWidget *parent)
    :   FenPrincipale (noyau,std::move(bdd),centralZone,bddPathXML,configPath,parent)
{
    addMenuBdd();
}

void PredefFenPrincipale::addMenuBdd(){
    m_bdd_menu = menuBar()->addMenu(tr("Bdd"));

    // Table
    m_bdd_table_menu = m_bdd_menu->addMenu("Tables");
    auto vec = noyau()->bdd().table_entity_names();
    for(auto iter = vec.cbegin(); iter != vec.cend(); ++iter) {
        auto action = m_bdd_table_menu->addAction(iter->second);
        auto i =iter->first;
        connect(action,&QAction::triggered,[this,i](){centraleZone()->openTab({PredefTab::GestionBddTabId,i});});
    }

    // XML
    m_bdd_xml_menu = m_bdd_menu->addMenu("XML");
    auto i_xml = m_bdd_xml_menu->addAction("Importer");
    auto e_xml = m_bdd_xml_menu->addAction("Exporter");
    //auto schXml = m_bddXmlMenu->addAction("Schema Xml");
    connect(i_xml,&QAction::triggered,this,&PredefFenPrincipale::import_xml);
    connect(e_xml,&QAction::triggered,this,&PredefFenPrincipale::export_xml);
    //connect(schXml,&QAction::triggered,this,&PredefFenPrincipale::schemaXml);
}

void PredefFenPrincipale::connectActionToOpenTab(QAction * action, const std::pair<int,int>& pairIndex,
                                           const std::vector<QVariant> & vec)
    {connect(action,&QAction::triggered,this,[this,pairIndex,vec](){centraleZone()->openTab(pairIndex,vec);});}

void PredefFenPrincipale::export_xml() {
    widgetMPS::SelectTableDialog dialog(noyau()->bdd().table_entity_names(),this);
    if(dialog.exec()){
        auto fileName = QFileDialog::getSaveFileName(this,
                                                     tr("Exporter des données dans un fichier XML"),
                                                     m_noyau->config().defaultDirectory(),
                                                     tr("Fichier XML (*.xml)"));
        if(!fileName.isEmpty()){
            conteneurMPS::VectorPtr<entityMPS::Entity> vec;
            for (auto iter = dialog.set().cbegin(); iter != dialog.set().cend(); ++iter)
                vec<<noyau()->bdd().getList(*iter);
            m_noyau->exportXml(fileName,std::move(vec),dialog.option());
        }
    }
}

void PredefFenPrincipale::import_xml() {
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
