#ifndef IRCONTROLWINDOW_H
#define IRCONTROLWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTableWidgetItem>
#include <QProcess>
#include "database.h"

namespace Ui {
	class ircontrolwindow;
}

class ircontrolwindow : public QMainWindow
{
	Q_OBJECT

	public:
		explicit ircontrolwindow(QWidget *parent = 0);
		~ircontrolwindow();

	private slots:
		void on_ser_refresh_button_clicked();

		void on_ser_con_button_clicked();

		void on_ser_dropdown_currentIndexChanged(const QString &arg1);

		void on_ser_discon_button_clicked();

		void on_bind_add_button_clicked();

	private:
		Ui::ircontrolwindow *ui;
		/* serial */
		void ser_setup();
		bool gen_serial_list();
		bool serial_connect(QString port);
		bool serial_disconnect();
		void serial_read();
		QString serportname;
		QSerialPort *ser;
		QByteArray serdata;
		/* execute */
		void pexecute(QString com, QStringList args);
		/* binds */
		void parse_bind(QString val);
		void bind_table_setup();
		void add_bind(QString bind,
				QString com,
				QString args);
		QList<QTableWidgetItem*> bind_table_items;
		/* database */
		database binddb;
};

#endif // IRCONTROLWINDOW_H
