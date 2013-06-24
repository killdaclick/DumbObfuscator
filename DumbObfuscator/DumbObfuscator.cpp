#include "stdafx.h"
#include "DumbObfuscator.h"

#include <boost/algorithm/hex.hpp>

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

}

void DumbObfuscator::connectSignal( void )
{
	connect( ui.obfuscateButton, SIGNAL(clicked()), this, SLOT(obfuscateButtonClicked) );
}

void DumbObfuscator::disconnectSignals( void )
{
	disconnect( ui.obfuscateButton, SIGNAL(clicked()), this, SLOT(obfuscateButtonClicked) );
}



