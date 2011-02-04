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

#ifndef LERNUWIDGET_H
#define LERNUWIDGET_H

#include <QtGui/QMainWindow>
#include <QHttp>
#include "ui_lernuwidget.h"

class LernuWidget : public QMainWindow
{
	Q_OBJECT

public:
	LernuWidget(QWidget *parent = 0, Qt::WFlags flags = 0);
	~LernuWidget();
private slots:
	void translate();
	void switchLanguage();
	void doneHttp(bool error);
private:
	Ui::LernuWidgetClass ui;
	QHttp * http;
};

#endif // LERNUWIDGET_H
