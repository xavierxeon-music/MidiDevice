#ifndef MidiVirtualInputH
#define MidiVirtualInputH

#include "../../Private/MidiRtMidiInput.h"

namespace Midi
{
   namespace Virtual
   {
      // creates a virtual midi input
      class Input : public RtMidi::Input
      {
      public:
         Input(const QString& portName);
         virtual ~Input();

      public:
         void open() override;
         void close() override;

      private:
         bool isOpen;
      };
   } // namespace Virtual
} // namespace Midi

#endif // NOT MidiVirtualInputH
