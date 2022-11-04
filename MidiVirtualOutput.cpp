#include <Midi/MidiVirtualOutput.h>

#include <QDebug>

Midi::Virtual::Output::Output(const QString& portName)
   : RtMidi::Output(portName)
   , isOpened(false)
{
}

Midi::Virtual::Output::~Output()
{
   Virtual::Output::close(); // need full name space in destructor
}

void Midi::Virtual::Output::open()
{
   if (isOpened)
   {
      qDebug() << "virtual midi output " << portName << "already open";
      return;
   }

   output.openVirtualPort(portName.toStdString());
   output.setErrorCallback(&Virtual::Output::midiError);

   qInfo() << "opened virtual midi output " << portName;
   isOpened = true;
}

void Midi::Virtual::Output::close()
{
   if (!isOpened)
      return;

   output.closePort();
   qInfo() << "closed virtual midi output" << portName;

   isOpened = false;
}

bool Midi::Virtual::Output::isOpen() const
{
   return isOpened;
}
