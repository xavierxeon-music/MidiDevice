#include <Midi/MidiVirtualInput.h>

#include <QDebug>

Midi::Virtual::Input::Input(const QString& portName)
   : RtMidi::Input(portName)
   , isOpened(false)
{
}

Midi::Virtual::Input::~Input()
{
   Virtual::Input::close(); // need full class name in destructor
}

void Midi::Virtual::Input::open()
{
   if (isOpened)
   {
      qDebug() << "virtual midi input " << portName << "already open";
      return;
   }

   input.openVirtualPort(portName.toStdString());

   qInfo() << "opened virtual midi input " << portName;
   isOpened = true;
}

void Midi::Virtual::Input::close()
{
   if (!isOpened)
      return;

   input.closePort();
   qInfo() << "closed virtual midi input" << portName;

   isOpened = false;
}

bool Midi::Virtual::Input::isOpen() const
{
   return isOpened;
}
