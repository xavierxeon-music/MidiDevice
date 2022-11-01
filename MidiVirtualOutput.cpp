#include <Midi/MidiVirtualOutput.h>

#include <QDebug>

Midi::Virtual::Output::Output(const QString& portName)
   : RtMidi::Output(portName)
   , isOpen(false)
{
}

Midi::Virtual::Output::~Output()
{
   Virtual::Output::close();
}

void Midi::Virtual::Output::open()
{
   if (isOpen)
   {
      qDebug() << "virtual midi output " << portName << "already open";
      return;
   }

   output.openVirtualPort(portName.toStdString());
   output.setErrorCallback(&Virtual::Output::midiError);

   qInfo() << "opened virtual midi output " << portName;
   isOpen = true;
}

void Midi::Virtual::Output::close()
{
   if (!isOpen)
      return;

   output.closePort();
   qInfo() << "closed virtual midi output" << portName;

   isOpen = false;
}

