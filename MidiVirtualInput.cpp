#include <Midi/MidiVirtualInput.h>

#include <QDebug>

Midi::Virtual::Input::Input(const QString& portName)
   : RtMidi::Input(portName)
   , isOpen(false)
{
}

Midi::Virtual::Input::~Input()
{
   Virtual::Input::close();
}

void Midi::Virtual::Input::open()
{
   if (isOpen)
      return;

   input.openVirtualPort(portName.toStdString());

   qInfo() << "opened virtual midi input " << portName;
   isOpen = true;
}

void Midi::Virtual::Input::close()
{
   if (!isOpen)
      return;

   input.closePort();
   qInfo() << "closed virtual midi input" << portName;

   isOpen = false;
}

