#include "Private/MidiRtMidiInput.h"

#include <QDebug>
#include <QMetaMethod>

#include <JSONTools.h>
#include <Midi/MidiInterfaceOutput.h>
#include <Tools/SevenBit.h>

// helper

Midi::RtMidi::Relay::Relay(Input* input)
   : QObject(nullptr)
   , input(input)
{
}

void Midi::RtMidi::Relay::processMessage(const Bytes& message)
{
   input->processMessage(message);
}

// input

Midi::RtMidi::Input::Input(const QString& portName)
   : Base(portName)
   , Interface::Input()
   , input()
   , relay(this)
   , docBufferMap()
   , documentFunctionList()
{
   input.setErrorCallback(&RtMidi::Input::midiError);
   input.setCallback(&RtMidi::Input::midiReceive, this);
   input.ignoreTypes(false, false, false); // do not ignore anything
}

Midi::RtMidi::Input::~Input()
{
}

QStringList Midi::RtMidi::Input::getAvailable()
{
   QStringList deviceList;

   RtMidiIn dummy;
   for (uint index = 0; index < dummy.getPortCount(); index++)
   {
      const std::string testPortName = dummy.getPortName(index);
      const QString rawPortName = QString::fromStdString(testPortName);
      const QString sequencerPortName = PortName::makeNice(rawPortName);
      deviceList << sequencerPortName;
   }

   return deviceList;
}

void Midi::RtMidi::Input::controllerChange(const Channel& channel, const ControllerMessage& controllerMessage, const uint8_t& value)
{
   Interface::Input::controllerChange(channel, controllerMessage, value);

   if (controllerMessage == Midi::ControllerMessage::DataInit)
   {
      docBufferMap[channel].clear();
   }
   else if (controllerMessage == Midi::ControllerMessage::DataBlock)
   {
      docBufferMap[channel].push_back(value);
   }
   else if (controllerMessage == Midi::ControllerMessage::DataApply)
   {
      const Bytes bytes = SevenBit::decode(docBufferMap[channel]);
      QByteArray content(bytes.size(), '\0');
      std::memcpy(content.data(), bytes.data(), bytes.size());

      const QJsonObject doc = JSONTools::Helper::convert(content);
      document(channel, doc, value);
   }
}

void Midi::RtMidi::Input::document(const Channel& channel, const QJsonObject& object, const uint8_t docIndex)
{
   for (const DocumentFunction& documentFunction : documentFunctionList)
      documentFunction(channel, object, docIndex);
}

void Midi::RtMidi::Input::midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData)
{
   Q_UNUSED(timeStamp)

   if (!message || !userData)
      return;

   Input* me = reinterpret_cast<Input*>(userData);
   if (!me)
      return;

   static int once = qRegisterMetaType<Bytes>("Bytes");
   Q_UNUSED(once);

   static QMetaMethod processMessage = [&]()
   {
      const int processMessageIndex = Relay::staticMetaObject.indexOfMethod("processMessage(Bytes)");
      Q_ASSERT(processMessageIndex != -1);

      QMetaMethod processMessage = Relay::staticMetaObject.method(processMessageIndex);
      return processMessage;
   }();

   static Bytes buffer;
   auto maybeProcessBuffer = [&]()
   {
      if (0 == buffer.size())
         return;

      if (processMessage.isConst())
         processMessage.invoke(&me->relay, Qt::QueuedConnection, Q_ARG(Bytes, buffer));

      buffer.clear();
   };

   static const uint8_t mask = 0x80;
   for (const uint8_t byte : *message)
   {
      const uint8_t test = byte & mask;
      if (test == mask) // new message start
         maybeProcessBuffer();

      buffer << byte;
   }
   maybeProcessBuffer();
}

void Midi::RtMidi::Input::processMessage(const Bytes& message)
{
   for (Interface::Output* passthrough : passthroughList)
      passthrough->sendBuffer(message);

   Interface::Input::processMessage(message);
}
