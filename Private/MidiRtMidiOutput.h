#ifndef MidiRtMidiOutputH
#define MidiRtMidiOutputH

#include "MidiRtMidiBase.h"
#include <Midi/MidiInterfaceOutput.h>

#include <QJsonObject>

namespace Midi
{
   namespace RtMidi
   {
      class Output : public Base, public Interface::Output
      {
      public:
         Output(const QString& portName);
         virtual ~Output() = 0;

      public:
         virtual void open() override = 0;
         virtual void close() override = 0;
         static QStringList getAvailable();
         void sendDocument(const QJsonObject& object, const Channel& channel = 1, const uint8_t docIndex = 0);
         void sendBuffer(const Bytes& buffer) override;

      protected:
         RtMidiOut output;
      };
   } // namespace RtMidi
} // namespace Midi

#endif // NOT MidiRtMidiOutputH
