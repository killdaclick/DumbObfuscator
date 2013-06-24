#ifndef DUMBOBFUSCATOR_H
#define DUMBOBFUSCATOR_H

#include <QtGui/QMainWindow>
#include "ui_DumbObfuscator.h"

class DumbObfuscator : public QMainWindow
{
	Q_OBJECT

	public:
		DumbObfuscator(QWidget *parent = 0, Qt::WFlags flags = 0);
		~DumbObfuscator();

	private:
		Ui::DumbObfuscatorClass ui;

		void connectSignal( void );
		void disconnectSignals( void );

	public slots:
		void obfuscateButtonClicked( void );
};

#endif // DUMBOBFUSCATOR_H
