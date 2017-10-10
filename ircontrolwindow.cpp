#include "ircontrolwindow.h"
#include "ui_ircontrolwindow.h"

#include <QDebug>
#include <QProcess>

ircontrolwindow::ircontrolwindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ircontrolwindow)
{	
	ui->setupUi(this);
	/* first steps */
	ser = new QSerialPort(this);
	connect(ser, &QSerialPort::readyRead, this,
			&ircontrolwindow::serial_read);
	ser_setup();
	bind_table_setup();
	binddb.getbinds();
	add_bind("16745085", "cmus-remote", "--pause");
}

ircontrolwindow::~ircontrolwindow()
{
	/* free bind table items */
	qDeleteAll(bind_table_items.begin(),
			bind_table_items.end());
	delete ser;
	delete ui;
}

void ircontrolwindow::ser_setup()
{
	/*
	 * setup serial items
	 */
	bool hasitem;
	hasitem = gen_serial_list();
	if (hasitem)
	{
		ui->ser_con_button->setEnabled(true);
	} else
	{
		ui->ser_con_button->setEnabled(false);
	}
}

void ircontrolwindow::bind_table_setup()
{
	int i;
	for (i = 0; i < 3; i++)
		ui->bind_table->insertColumn(i);
	ui->bind_table->setHorizontalHeaderLabels(
			QStringList() << "bind" 
			<< "command" << "arguments");

}

bool ircontrolwindow::gen_serial_list()
{
	/*
	 * generates our serial port list in
	 * ui->ser_dropdown
	 */
	ui->ser_dropdown->clear();

	int i, psize;
	psize = QSerialPortInfo::availablePorts().size();
	if (psize > 0)
	{
		for (i = 0; i < psize; i++)
		{
			if (!QSerialPortInfo::availablePorts()
				.at(i).description().isEmpty())
				ui->ser_dropdown->addItem(
				QSerialPortInfo::availablePorts()
				.at(i).portName());
			return true;
		}
	}
	return false;
}

void ircontrolwindow::serial_read()
{
	/*
	 * read serial data and send things to parse
	 */
	if (serdata[serdata.size() - 1] == '\n')
	{
		QString str;
		str = QString(serdata).remove(
				QRegExp("[\\n\\t\\r]"));
		/* this ir code is the repeat code on my
		 * current controller, i have to check if it is
		 * universal */
		if (str != "4294967295")
		{
			parse_bind(str);

		}

		serdata = "";
	} else
		serdata = serdata + ser->readAll();
}

bool ircontrolwindow::serial_connect(QString port)
{
	/*
	 * serial connect
	 */
	if (!ser->isOpen())
	{
		ser->setPortName(port);
		ser->setBaudRate(QSerialPort::Baud9600);
		ser->setDataBits(QSerialPort::Data8);
		ser->setParity(QSerialPort::NoParity);
		ser->setStopBits(QSerialPort::OneStop);
		ser->setFlowControl(
				QSerialPort::NoFlowControl);
		ser->open(QIODevice::ReadWrite);
		ser->waitForBytesWritten(9000); 
	}
	return ((ser->isWritable()) ? true : false);
}

bool ircontrolwindow::serial_disconnect()
{
	/*
	 * serial disconnect
	 */
	if (ser->isOpen())
		ser->close();
	return ((ser->isOpen()) ? false : true);
}

void ircontrolwindow::pexecute(QString com, QStringList args)
{
	/*
	 * execute a non-blocking process
	 */
	QProcess *process = new QProcess(this);
	connect(process, SIGNAL(finished(int)), process,
			SLOT(deleteLater()));
	/* this is terrible, find a better way */
	((args[0] != "") ? process->start(com, args)
	 			: process->start(com));
}

void ircontrolwindow::parse_bind(QString val)
{
	/*
	 * parse bind with database values
	 */
	qDebug() << "parsing" << val;
	ui->bind_input->setText(val);
	int i, rowsize;
	QString rowbind, com;
	QStringList args;
	rowsize = ui->bind_table->rowCount();
	for (i = 0; i < rowsize; i++)
	{
		rowbind = ui->bind_table->item(i, 0)->text();
		if (val == rowbind)
		{
			com = ui->bind_table->item(
					i, 1)->text();
			/* args needs to be split */
			args << ui->bind_table->item(
					i, 2)->
					text().split(" ");
			pexecute(com, args);
			qDebug() << args;
		}
	}
}

void ircontrolwindow::add_bind(QString bind,
		QString com, QString args)
{
	/*
	 * add a bind to our list and to the database
	 */
	qDebug() << bind << com << args;
	int row;
	row = ui->bind_table->rowCount();
	ui->bind_table->insertRow(row);

	QTableWidgetItem *nbind = new QTableWidgetItem(
			tr("%1").arg(bind));
	QTableWidgetItem *ncom = new QTableWidgetItem(
			tr("%1").arg(com));
	QTableWidgetItem *nargs = new QTableWidgetItem(
			tr("%1").arg(args));
	/* 
	 * store to delete later 
	 * TODO remove items once user deletes them
	 * */
	bind_table_items.append(nbind);
	bind_table_items.append(ncom);
	bind_table_items.append(nargs);

	ui->bind_table->setItem(row, 0, nbind);
	ui->bind_table->setItem(row, 1, ncom);
	ui->bind_table->setItem(row, 2, nargs);
}

void ircontrolwindow::on_ser_refresh_button_clicked()
{
	/*
	 * refresh serial port
	 */
	ser_setup();
}

void ircontrolwindow::on_ser_con_button_clicked()
{
	/*
	 * serial connect button
	 */
	bool ret;
	ret = serial_connect(serportname);
	if (ret)
	{
		ui->ser_con_button->setEnabled(false);
		ui->ser_refresh_button->setEnabled(
				false);
		ui->ser_discon_button->setEnabled(true);
	}
}

void ircontrolwindow::on_ser_dropdown_currentIndexChanged(
		const QString &arg1)
{
	/*
	 * serial drop down item selected
	 */
	serportname = arg1;
}

void ircontrolwindow::on_ser_discon_button_clicked()
{
	/*
	 * serial disconnect button
	 */
	bool ret;
	ret = serial_disconnect();
	if (ret)
	{
		ui->ser_con_button->setEnabled(true);
		ui->ser_refresh_button->setEnabled(
				true);
		ui->ser_discon_button->setEnabled(false);
	}
}

void ircontrolwindow::on_bind_add_button_clicked()
{
	/*
	 * bind add button clicked
	 */
	add_bind(ui->bind_input->text(),
			ui->command_input->text(),
			ui->argument_input->text());	
}




