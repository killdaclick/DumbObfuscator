#include "stdafx.h"
#include "DumbObfuscator.h"

#include <boost/algorithm/hex.hpp>

#define HEX_NUM_PREFIX			"0x"
#define HEX_NUM_NEXT_VAL		", "
#define HEX_LANGC_TABLE_START	"{ "
#define HEX_LANGC_TABLE_STOP	" };"
#define SHADOW_VAR				"variableShadow"
#define REAL_VAR				"variable"

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
		QString shadowVar, realVar;
		QString varType = "";

		if( ui.varQStringType->isChecked() && !ui.varStdStringType->isChecked() )
			varType = "QString";
		else if( !ui.varQStringType->isChecked() && ui.varStdStringType->isChecked() )
			varType = "std::string";
		else
		{
			ui.statusLabel->setText("Variable type ERROR!");
			return;
		}

		if( ui.shadowVar->text() == "" )
			shadowVar = SHADOW_VAR;
		else
			shadowVar = ui.shadowVar->text();

		if( ui.realVar->text() == "" )
			realVar = REAL_VAR;
		else
			realVar = ui.realVar->text();

		QString outputC = "unsigned char " + shadowVar + "[] = " + QString(HEX_LANGC_TABLE_START);
		signed char rot = ui.rotateValue->text().toInt();

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
				auto tmpHex = hexStr.toStdString();
				if( tmpHex.size() == 1 )
					tmpHex.insert(0,"0");
				auto tmpUnhex = QString::fromStdString(boost::algorithm::unhex( tmpHex ));
				outputText.push_back( tmpUnhex );
				if( hexStr.size() == 1 )
					hexStr.push_front("0");
				outputNum.push_back( hexStr );
				if( itrNext == output.end() )
					outputC.push_back( HEX_NUM_PREFIX + hexStr.toUpper() + HEX_LANGC_TABLE_STOP );
				else
					outputC.push_back( HEX_NUM_PREFIX + hexStr.toUpper() + HEX_NUM_NEXT_VAL );

				digit = 1;
				hexStr = "";
				itr++;
				continue;
			}
		}

		outputC.push_back("\n" + varType + " " + realVar + " = \"\";\nfor( int i=0; i<sizeof(" + shadowVar + "); i++ ) { " + realVar + ".push_back(" + shadowVar + "[i]+(" + QString::number(-rot) + ")); }");

		ui.outputText->setPlainText( outputText );
		ui.outputC->setPlainText( outputC );
		ui.outputNum->setPlainText( outputNum.toUpper() );
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
	connect( ui.actionExit, SIGNAL(triggered()), this, SLOT(close()) );
}

void DumbObfuscator::disconnectSignals( void )
{
	disconnect( ui.obfuscateButton, SIGNAL(clicked()), this, SLOT(obfuscateButtonClicked()) );
	disconnect( ui.actionExit, SIGNAL(triggered()), this, SLOT(close()) );
}



