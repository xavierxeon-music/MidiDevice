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
         Input(const QString& portName);
         virtual ~Input() = 0;

      public:
         virtual void open() override = 0;
         virtual void close() override = 0;
         static QStringList getAvailable();
         virtual void controllerChange(const Channel& channel, const ControllerMessage& controllerMessage, const uint8_t& value) override;
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
      };

   } // namespace RtMidi
} // namespace Midi

#endif // NOT MidiRtMidiInputH
