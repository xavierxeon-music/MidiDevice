#include "Private/MidiRtMidiOutput.h"

#include <QDebug>
#include <QThread>

#include <JSONTools.h>
#include <Tools/SevenBit.h>

Midi::RtMidi::Output::Output(const QString& portName)
   : Base(portName)
   , Interface::Output()
   , output()
{
}

Midi::RtMidi::Output::~Output()
{
}

QStringList Midi::RtMidi::Output::getAvailable()
{
   QStringList deviceList;

   RtMidiOut dummy;
   for (uint index = 0; index < dummy.getPortCount(); index++)
   {
      const std::string testPortName = dummy.getPortName(index);
      const QString rawPortName = QString::fromStdString(testPortName);
      const QString sequencerPortName = PortName::makeNice(rawPortName);
      deviceList << sequencerPortName;
   }

   return deviceList;
}

void Midi::RtMidi::Output::sendDocument(const QJsonObject& object, const Channel& channel, const uint8_t docIndex)
{
   sendControllerChange(channel, Midi::ControllerMessage::DataInit, docIndex);

   const QByteArray content = JSONTools::Helper::convert(object);
   Bytes bytes(content.size());
   std::memcpy(bytes.data(), content.constData(), content.size());

   const std::string data = SevenBit::encode(bytes);
   for (const char& byte : data)
      sendControllerChange(channel, Midi::ControllerMessage::DataBlock, byte);

   sendControllerChange(channel, Midi::ControllerMessage::DataApply, docIndex);
}

void Midi::RtMidi::Output::sendBuffer(const Bytes& buffer)
{
   output.sendMessage(&buffer);
   QThread::usleep(1000); // estimate is 1 ms per 3 bytes
}
