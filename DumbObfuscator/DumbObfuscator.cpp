#include "stdafx.h"
#include "DumbObfuscator.h"

#include <boost/algorithm/hex.hpp>

#define HEX_NUM_PREFIX			"0x"
#define HEX_NUM_NEXT_VAL		", "
#define HEX_LANGC_TABLE_START	"{ "
#define HEX_LANGC_TABLE_STOP	" }"

DumbObfuscator::DumbObfuscator(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	connectSignal();
}

DumbObfuscator::~DumbObfuscator()
{
	disconnectSignals();
}

void DumbObfuscator::obfuscateButtonClicked( void )
{
	ui.statusLabel->setText("...");
	
	if( ui.rotationGroupBox->isChecked() )
	{
		QString outputText = "";
		QString outputNum = "";
		QString outputC = HEX_LANGC_TABLE_START;
		unsigned char rot = ui.rotateValue->text().toUInt();

		if( ui.decrSwitch->isChecked() )
			rot *= (-1);
		else if( ui.incrSwitch->isChecked() )
			rot;
		else
		{
			ui.statusLabel->setText("Rotation parameters ERROR!");
			return;
		}

		QString input = ui.inputEdit->toPlainText();
		auto outputStd = boost::algorithm::hex( input.toStdString() );
		auto output = QString::fromStdString( outputStd );

		unsigned int digit = 1;
		unsigned char hexNum;
		QString hexStr = "";
		for( QString::iterator itr = output.begin();
			itr != output.end(); )
		{
			QString::iterator itrNext = itr;
			itrNext++;
			
			if( digit == 1 )
			{
				hexStr.push_back( *itr );
				itr++;
				digit++;
				continue;
			}

			if( digit == 2 )
			{
				hexStr.push_back( *itr );
				bool ok;
				hexNum = hexStr.toUInt( &ok, 16 );
				if( !ok )
				{
					ui.statusLabel->setText("Conversion error!");
					return;
				}
				hexNum += rot;
				hexStr = QString::number( hexNum, 16 );
				outputNum.push_back( hexStr );
				outputText.push_back( QString::fromStdString(boost::algorithm::unhex( hexStr.toStdString() )) );
				if( itrNext == output.end() )
					outputC.push_back( HEX_NUM_PREFIX + hexStr + HEX_LANGC_TABLE_STOP );
				else
					outputC.push_back( HEX_NUM_PREFIX + hexStr + HEX_NUM_NEXT_VAL );

				digit = 1;
				hexStr = "";
				itr++;
				continue;
			}
		
			//auto itrNext = itr;
			//std::advance( itrNext, 2 );
			//if( itrNext != output.end() 
		}

		ui.outputText->setPlainText( outputText );
		ui.outputC->setPlainText( outputC );
		ui.outputNum->setPlainText( outputNum );
		ui.statusLabel->setText("Obfuscation completed!");
	}
	else
	{
		ui.statusLabel->setText("No parameters selected!");
		return;
	}
}

void DumbObfuscator::connectSignal( void )
{
	connect( ui.obfuscateButton, SIGNAL(clicked()), this, SLOT(obfuscateButtonClicked()) );
}

void DumbObfuscator::disconnectSignals( void )
{
	disconnect( ui.obfuscateButton, SIGNAL(clicked()), this, SLOT(obfuscateButtonClicked()) );
}



