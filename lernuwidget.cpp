//    This file is part of LernuWidget.
//    LernuWidget is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program. If not, see <http://www.gnu.org/licenses/>.
// 
//    2011, Andrew Semenov (semenoff.andrew@gmail.com). 

#include "lernuwidget.h"
#include <QUrl>
#include <QFile>
#include <QTextStream>

LernuWidget::LernuWidget(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	ui.listLang->addItem("Esperanto -> Esperanto");
	ui.listLang->setItemData(0,"eo|eo");
	QFile file(":/LernuWidget/Resources/out.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream stream(&file);
	while (!stream.atEnd()){
		QString str=stream.readLine();
		QStringList L=str.split('	');
		ui.listLang->addItem(L[0]+" -> Esperanto");
		ui.listLang->setItemData(ui.listLang->count()-1,L[1]+"|eo");
		ui.listLang->setItemData(ui.listLang->count()-1,this->palette().color(QPalette::Window),Qt::BackgroundColorRole);
		ui.listLang->setItemData(ui.listLang->count()-1,this->palette().color(QPalette::WindowText),Qt::ForegroundRole);
		ui.listLang->addItem("Esperanto -> "+L[0]);
		ui.listLang->setItemData(ui.listLang->count()-1,"eo|"+L[1]);
	}
	file.close();

	http=new QHttp();
	connect(ui.btnTranslate,SIGNAL(clicked()),this,SLOT(translate()));
	connect(ui.lineWord,SIGNAL(returnPressed()),this,SLOT(translate()));
	connect(ui.listLang,SIGNAL(currentIndexChanged(int)),this,SLOT(translate()));
	connect(ui.btnSwitch,SIGNAL(clicked()),this,SLOT(switchLanguage()));	
	connect(http, SIGNAL(done(bool)), this, SLOT(doneHttp(bool)));

}

LernuWidget::~LernuWidget(){
	delete http;
}

void LernuWidget::translate(){	
	QString word=ui.lineWord->text();

	QUrl url;
    url.setPath("/cgi-bin/serchi.pl");
    url.setQueryDelimiters('=', ';');
	QStringList lst=ui.listLang->itemData(ui.listLang->currentIndex()).toString().split('|');

    url.addQueryItem("delingvo",QUrl::toPercentEncoding(lst[0]));
	url.addQueryItem("allingvo",QUrl::toPercentEncoding(lst[1]));	
	url.addQueryItem("modelo", QUrl::toPercentEncoding(ui.lineWord->text()));
    
    http->setHost("lernu.net");
    ui.textResult->setText("...");
    http->get(url.toString());
}

void LernuWidget::switchLanguage(){
	QStringList L=ui.listLang->itemData(ui.listLang->currentIndex()).toString().split('|');
	QString result=L[1]+"|"+L[0];
	ui.listLang->setCurrentIndex(ui.listLang->findData(result));
}

void LernuWidget::doneHttp(bool error){	
  ui.textResult->clear();	
	if (!error){
		QStringList result=QString::fromUtf8((http->readAll().data())).split('\n');

		int type=0;
		for (QStringList::iterator i=result.begin();i!=result.end();i++){
			if (i->startsWith("[[")) break;
			QStringList word=i->split('	');
			switch(type){
				case 0:
					if ((word.count()>1) && (word[1]!="")){
						QString newline="<b>"+word[1]+"</b>";
						if ((word.count()>2) && (word[2]!="")){
							newline+=" ("+word[2].replace("/",tr("·"));
							if ((word.count()>3) && (word[3]!=""))
								newline+=" &lt;- "+word[3];
							newline+=")";
						}
						ui.textResult->append(newline);
					}	
					break;
				case 1:
					if (word.count()>3)
						ui.textResult->append("	"+word[3]+"<br />");
					i++;
					break;
			}
			type=1-type;		
		}
		
	}
}
