#ifndef MidiRtMidiInputH
#define MidiRtMidiInputH

#include "MidiRtMidiBase.h"
#include <Midi/MidiInterfaceInput.h>

#include <QJsonObject>

namespace Midi
{
   namespace RtMidi
   {
      class Input;

      class Relay : public QObject
      {
         Q_OBJECT
      public:
         Relay(Input* input);

      public:
         Q_INVOKABLE void dataFromInput(const Bytes& message);

      private:
         Input* input;
      };

      class Input : public Base, public Interface::Input
      {
      public:
         using DocumentFunction = std::function<void(const Channel& channel, const QJsonObject& object, const uint8_t docIndex)>;

      public:
         Input(const QString& portName);
         virtual ~Input() = 0;

      public:
         virtual void open() override = 0;
         virtual void close() override = 0;
         static QStringList getAvailable();
         virtual void controllerChange(const Channel& channel, const ControllerMessage& controllerMessage, const uint8_t& value) override;

         template <typename ClassType>
         void onDocument(ClassType* instance, void (ClassType::*functionPointer)(const Channel&, const QJsonObject&, const uint8_t));
         virtual void document(const Channel& channel, const QJsonObject& object, const uint8_t docIndex);

      protected:
         static void midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData);

      protected:
         RtMidiIn input;

      private:
         friend class Relay;
         using BufferMap = std::map<Channel, std::string>;

      private:
         void dataFromInput(const Bytes& message) override;

      private:
         Relay relay;
         BufferMap docBufferMap;
         std::vector<DocumentFunction> documentFunctionList;
      };

   } // namespace RtMidi
} // namespace Midi

template <typename ClassType>
void Midi::RtMidi::Input::onDocument(ClassType* instance, void (ClassType::*functionPointer)(const Channel&, const QJsonObject&, const uint8_t))
{
   DocumentFunction documentFunction = std::bind(functionPointer, instance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
   documentFunctionList.push_back(documentFunction);
}

#endif // NOT MidiRtMidiInputH
